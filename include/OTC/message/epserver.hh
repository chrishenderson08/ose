#ifndef OTC_MESSAGE_EPSERVER_HH
#define OTC_MESSAGE_EPSERVER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/epserver.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/endpoint.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/epserver.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_EPListener;

class OSE_EXPORT OTC_EPServer : public OTC_EndPoint
    // = TITLE
    //     Base class for server endpoint.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_EPServer> class is a base class for classes which
    //     manage the server end of a connection.
    //
    // = SEE ALSO
    //     <OTC_EPListener>
{
  public:

			~OTC_EPServer();

    // = LISTENER

    OTC_EPListener*	listener()
				{ return listener_; }
				// Returns the listener object which
				// spawned this server object. If the
				// server object has been stopped and
				// disassociated from its listener,
				// this function will return <0>.

    // = SHUTDOWN

    void		stop(int theDelay=0);
				// Requests of a derived class that this
				// server object be stopped. The server
				// object will be disassociated from its
				// listener object. If this function is
				// called more than once, an exception will
				// be raised. If an attempt should be made to
				// flush out any pending messages, <theDelay>
				// can be set to a time in milliseconds as
				// the maxmimum amount of time to wait in
				// trying to write out the messages. A value
				// of <-1> for <theDelay> indicates that all
				// messages must be written out before
				// closing the connection.

  protected:

    // = INITIALISATION

			OTC_EPServer(
			 char const* theInterface,
			 char const* theProtocol,
			 OTC_EPListener* theListener
			);
				// <theProtocol> should identify the type of
				// transport layer being used. <theInterface>
				// the type of interface used to communicate
				// with that transport layer. <theListener>
				// should be the listener which spawned this
				// server object. This class will register
				// itself with the listener object.

    // = CALLBACKS

    virtual void	close() = 0;
				// Must be redefined in a derived class to
				// close down connection. This function
				// will be called by <stop()> when the
				// maxmimum permissable delay for flushing
				// messages is <0>. This server object will
				// be disassociated from its server object
				// prior to this call being made.

    // = EVENT CALLBACK

    void                handle(OTC_Event* theEvent);
                                // Handles events to stop connection.
 
  private:

			OTC_EPServer(OTC_EPServer const&);
				// Do not define an implementation for this.

    OTC_EPServer&	operator=(OTC_EPServer const&);
				// Do not define an implementation for this.

    OTC_EPListener*	listener_;
				// The listener which spawned this server
				// object.

    int                 stopDelay_;
                                // How long to wait before stopping.

    int                 stopTimer_;
                                // Stop timer id.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_BaseActions<OTC_EPServer*>
{
  public:

    static OTC_EPServer*	add(OTC_EPServer* theServer)
				{
				  theServer->reference();
				  return theServer;
				}

    static void		remove(OTC_EPServer* theServer)
				{ theServer->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPSERVER_HH */
