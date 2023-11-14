#ifndef OTC_MESSAGE_INETLSTN_HH
#define OTC_MESSAGE_INETLSTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/inetlstn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/eplisten.hh>
#include <OTC/message/inetsrvr.hh>
#include <OTC/message/inetaddr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/inetlstn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_InetListener : public OTC_EPListener
    // = TITLE
    //     Listens on a socket, creating server object for each connection.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_InetListener> class listens for connect requests on a
    //     socket. For each connection request, a new socket will be created
    //     to service that request and a server object attached to it.
    //
    // = NOTES
    //     Implementation uses default listen queue length of <5>. Also,
    //     <SO_REUSEADDR> is applied to the socket so that existing port
    //     can be used as soon as is possible.
    //
    // = SEE ALSO
    //     <OTC_InetServer>, <OTC_EVAgent>
{
    friend class	OTC_InetServer;

  public:

    // = DESTRUCTION

			~OTC_InetListener();
				// Any server objects created from this class
				// will be shutdown and destroyed at this
				// point.

    // = CONSTRUCTION
    //     Note that the constructor doesn't open the port and start
    //     listening, it is necessary to call <start()> to do this.

			OTC_InetListener(
			 char const* theInterface,
			 int thePort
			);
				// Creates the listener object. The port
				// used will be <thePort>. The interface
				// object which controls how data is written
				// over the socket when a server object is
				// created is specified by <theInterface>.
				// The two inbuilt types are "MESSAGE" and
				// "STREAM". The "MESSAGE" type implements a
				// structured messaging abstraction including
				// message routing etc. The "STREAM" type
				// provides the ability for direct control
				// over what is sent over the socket. That
				// is, all that will be sent is the contents
				// of your message.

			OTC_InetListener(
			 char const* theInterface,
			 char const* theHostname,
			 int thePort
			);
				// Creates the listener object. The port
				// used will be <thePort>. Connections will
				// only be accepted which come via the host
				// interface with <theHostname>. The interface
				// object which controls how data is written
				// over the socket when a server object is
				// created is specified by <theInterface>.
				// The two inbuilt types are "MESSAGE" and
				// "STREAM". The "MESSAGE" type implements a
				// structured messaging abstraction including
				// message routing etc. The "STREAM" type
				// provides the ability for direct control
				// over what is sent over the socket. That
				// is, all that will be sent is the contents
				// of your message.

  public:

    // = QUERY

    OTC_String const&	interface() const
				{ return interface_; }
				// Returns the type of interface.

    int			port() const
				{ return address_.port(); }
				// Returns the port number on which the
				// listener is waiting. The listener will
				// accept connections on any hostname
				// interface.

    int			fd() const
				{ return fd_; }
				// Returns the file descriptor bound to
				// the port on which this class is listening.
				// Returns <-1> if not listening.

    // = LISTENING

    void		start();
				// If not already listening for requests,
				// starts listening.

    void		stop();
				// If listening to requests, stops listening.
				// Does not cause existing servers to be
				// destroyed.

    void                suspendListening();
                                // Will suspend accepting of new connections.

    void                resumeListening();
                                // Will resume accepting of new connections.

  protected:

    // = EVENT CALLBACK

    void		handle(OTC_Event* theEvent);
				// If a connection request is pending,
				// creates a new socket for dealing with that
				// connection and creates a server object to
				// deal with the request on the newly created
				// socket.

    // = CONNECTION

    void		attach(int theFd);
				// Attaches the listener to <theFd>. Raises
				// an exception if this class is already
				// attached to a file descriptor.

    int			detach();
				// Unsubscribes from the file descriptor with
				// the result that this class will no longer
				// listen for connection requests. The old
				// file descriptor is returned. This function
				// does not call <close()> on the file
				// descriptor. Returns <-1> if this class
				// was not attached to a file descriptor.

    virtual OTC_EPServer*	create(
			 OTC_EPListener* theListener,
			 int theFd
			);
				// Creates a server object to handle messages
				// on <theFd>. The server object is to be
				// attached to <theListener>. By default this
				// this creates an instance of
				// <OTC_InetServer>. This function could be
				// overridden in a derived class to create a
				// different class, allowing the
				// implementation of this class to be reused.

    // = AUTHORISATION

    virtual bool	authorise(OTC_String const& theClientHost);
				// By default returns <true>. Can be
				// overridden in a derived class to return
				// <false>, if <theClientHost> should be
				// refused a connection. <theClientHost>
				// will be an IP address.

  private:

			OTC_InetListener(OTC_InetListener const&);
				// Do not provide an implementation for this.

    OTC_InetListener&	operator=(OTC_InetListener const&);
				// Do not provide an implementation for this.

    OTC_String		interface_;
				// The type of interface used on the socket.

    OTC_InetAddress	address_;
				// Address used to create listening port.

    int			fd_;
				// File descriptor which this class is
				// listening on for connection requests.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_INETLSTN_HH */
