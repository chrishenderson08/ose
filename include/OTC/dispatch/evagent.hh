#ifndef OTC_DISPATCH_EVAGENT_HH
#define OTC_DISPATCH_EVAGENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/evagent.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/hashtble.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/memory/cmmnpool.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/evagent.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Event;

class OSE_EXPORT OTC_EVAgent
    // = TITLE
    //	   Base class for objects capable of receiving events.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   <OTC_EVAgent> is a base class for any active object capable of
    //	   receiving events. In essence this class represent a locus
    //	   of control within a process.
    //
    // = SEE ALSO
    //	   <OTC_Event>, <OTC_Dispatcher>
{
    friend		class OTC_Event;
    friend		class OTC_EVHandler;

  public:

    // = DESTRUCTION

    virtual		~OTC_EVAgent();
				// Removes this agent from the table
				// of active agents.

    // = IDENTIFICATION

    int			id() const
				{ return id_; }
				// Returns the ID number of this agent.

    // = LOOKUP

    static OTC_EVAgent* lookup(int theAgentId);
				// Looks in the table of active agents for
				// the agent with ID number <theAgentId>. If
				// found, a pointer to that agent is
				// returned, otherwise <0> is returned.

  protected:

    // = INITIALISATION

			OTC_EVAgent();
				// Allocates a new agent ID and adds this
				// object to the table of active agents.

    // = EVENT CALLBACK

    virtual void	accept(OTC_Event* theEvent);
				// This is the function where events are
				// initially delivered. By default calls
				// <execute()> passing <theEvent> as
				// argument. Would only be redefined if it
				// was necessary to detect certain events and
				// process them in some special way before
				// they are filtered out to a specific
				// callback or the generic handle function.
				// If the derived class didn't do anything
				// with the event, it must call this base
				// class version of the function. If the
				// derived class did do something with the
				// event, it should ensure the event is
				// destroyed in some way.

    virtual void	execute(OTC_Event* theEvent);
    				// By default calls <filter()> passing
				// <theEvent> as argument. Can be redefined
				// in a derived class in order to have the
				// event be handled within the context of
				// a separate thread for this agent by
				// using the <OTC_EVHandler> class.

    virtual void	filter(OTC_Event* theEvent);
				// By default calls <handle()> passing
				// <theEvent> as argument. Can be redefined
				// in a derived class, if you want to filter
				// out certain events and convert them to
				// specific function callbacks. Derived class
				// versions of the functions should always
				// call immediate base class version of the
				// function if the event is not handled in a
				// special way. If the derived class did do
				// something with the event, it should ensure
				// the event is destroyed in some way.

    virtual void	handle(OTC_Event* theEvent) = 0;
				// Must be redefined in a derived class to
				// handle <theEvent>. This is where the
				// event should ultimately be handled by
				// a derived class. The derived class is
				// responsible for destroying <theEvent>.

  private:

			OTC_EVAgent(OTC_EVAgent const&);
				// Do not define an implementation for this.

    OTC_EVAgent&	operator=(OTC_EVAgent const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_HashTable*	globAgents_;
				// Table of active agents.

    static int		globId_;
				// Count for allocating agent ID's.

    int			id_;
				// The id of this agent.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVAGENT_HH */
