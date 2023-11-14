#ifndef OTC_MESSAGE_EPLISTEN_HH
#define OTC_MESSAGE_EPLISTEN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/eplisten.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/epserver.hh>
#include <OTC/dispatch/subject.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/hset.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/vlink.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/eplisten.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EPListener
 : public OTC_Resource, public OTC_EVAgent, public OTC_Subject
    // = TITLE
    //     Base class for listener objects.
    //
    // = CLASS TYPE
    //      Abstract
    //
    // = DESCRIPTION
    //     The <OTC_EPListener> class is a base class for classes which
    //     listen for requests to connect to this process. For each request a
    //     derived class would create a corresponding derived class of
    //     <OTC_EPServer> and add that to the set of server objects handled
    //     by this listener.
    //
    //     The <OTC_EPListener> class derives from <OTC_Subject> and thus
    //     you can indicate that you want to be notified of changes in the
    //     listener object as well as successful creation or destrution of
    //     new server objects.
    //     
    // = SEE ALSO
    //     <OTC_Subject>, <OTC_EPServer>
{
    friend class	OTC_EPServer;

  public:

    // = DESTRUCTION

			~OTC_EPListener();
				// The actions of any server objects created
				// from this listener will be stopped. If
				// nothing was keeping the server object in
				// existance, the server object will also be
				// destroyed. A server object might keep
				// itself in existance due to a request to
				// stop not being able to be honoured in one
				// operation. The server object may for
				// example want to flush out message queues.
				// If the server object kept itself alive,
				// it would become its own responsibility to
				// destroy itself. Note that it is a derived
				// classes responsibility to stop itself from
				// listening for new requests.

    // = LISTENING

    virtual void	start() = 0;
				// If not already listening for requests,
				// starts listening. Must be overriden in
				// a derived class.

    virtual void	stop() = 0;
				// If listening to requests, stops listening.
				// Does not cause existing servers to be
				// shutdown or destroyed. Must be overriden
				// in a derived class.

    void		shutdown();
				// Called when a shutdown event arrives. Will
				// suspend listening for new connections.

    // The purpose of these functions is specifically to cause accepting
    // of new connections to be throttled so that pending messages can be
    // output without having to deal with any more connections starting up.
    // The intent is that message input be throttled when shutting down a
    // process so that pending messages can be handled.

    virtual void        suspendListening();
				// Will suspend accepting of connections.
				// Note that in deriving from this class, you
				// must override this if a client and
				// implement this behaviour.

    virtual void        resumeListening();
				// Will resume accepting of connections. Note
				// that in deriving from this class, you must
				// override this if a client and implement
				// this behaviour.

    // Note that only the following are truly safe to use in a multithread
    // environment. They will call the above functions on each endpoint
    // inside a mutex lock. A derived class should not cause itself to be
    // deleted in response to one of the above functions. If it does it
    // will hang the thread, trying to lock the mutex it already has but
    // which does not allow recursive locking.

    static void         suspendAllListening();
                                // Suspend accepting of connections for all
                                // listeners.

    static void         resumeAllListening();
                                // Resume accepting of connections for all
                                // listeners.

    // = SERVERS

    OTC_Iterator<OTC_EPServer*>	servers()
				{ return servers_.items(); }
				// Returns iterator over server objects
				// created by this listener.

  protected:

    // = CONSTRUCTION

			OTC_EPListener();
				// Initialises set of server objects to be
				// empty. Also subscribes to the shutdown
				// event.

    // = EVENT CALLBACK

    void		accept(OTC_Event* theEvent);
				// Looks for <OTCEV_Shutdown> event and
				// calls <shutdown()> if one arrives.
				// Other events are passed to base class
				// <accept()> function.

    void		handle(OTC_Event* theEvent);
				// Handles server connections going away.
				// Must be called by derived class <handle()>
				// function if it gets events it doesn't know
				// what to do with.

  private:

			OTC_EPListener(OTC_EPListener const&);
				// Do not provide an implementation for this.

    OTC_EPListener&	operator=(OTC_EPListener const&);
				// Do not provide an implementation for this.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static OTC_LinkList*	listeners_;
				// List of all listeners.

    OTC_HMap<OTC_String,OTC_EPServer*>	servers_;
				// Set of server objects for this listener
				// indexed by local address.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_BaseActions<OTC_EPListener*>
{
  public:

    static OTC_EPListener*	add(OTC_EPListener* theListener)
				{
				  theListener->reference();
				  return theListener;
				}

    static void		remove(OTC_EPListener* theListener)
				{ theListener->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPLISTEN_HH */
