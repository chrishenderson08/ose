#ifndef OTC_MESSAGE_INETCLNT_HH
#define OTC_MESSAGE_INETCLNT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/inetclnt.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/episockt.hh>
#include <OTC/message/inetaddr.hh>
#include <OTC/message/epclient.hh>
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/inetclnt.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_InetClient : public OTC_EPClient
    // = TITLE
    //	   Client end of internet style socket connection.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   The <OTC_InetClient> is used for initiating the client side
    //	   of an internet style socket connection.
{
  public:

    // = DESTRUCTION

			~OTC_InetClient();
				// Will close the connection if open.

    // = INITIALISATION
    //	   The constructors do not actually create the connection,
    //	   you still need to call <start()> to initiate the connection.

			OTC_InetClient(
			 char const* theInterface,
			 char const* theHostname,
			 int thePort,
			 int theRetryDelay=-1
			);
				// Initialises this client for connection
				// to <theHostname> on <thePort>. On a
				// failure to connect, if <theRetryDelay>
				// is not <-1>, <theRetryDelay> is taken
				// to be the number of milliseconds before
				// a subsequent attempt is made to connect.
				// The interface object which controls how
				// data is written over the socket is
				// specified by <theInterface>. The two
				// inbuilt types are "MESSAGE" and "STREAM".
				// The "MESSAGE" type implements a structured
				// messaging abstraction including message
				// routing etc. The "STREAM" type provides
				// the ability for direct control over
				// what is sent over the socket. That is,
				// all that will be sent is the contents
				// of your message. If you want to connect
				// to a port on the same machine use
				// "localhost" for the hostname.

  public:

    // = REMOTE PORT

    int 		port() const
				{ return port_; }
				// Returns the port number of the host to
				// which this client will try to connect.

    // = REMOTE HOST

    OTC_String const&	targetHostname() const
				{ return hostname_; }
				// Returns the name of the remote host as
				// originally provided in the constructor.

    // The following methods return addressing information which reflects
    // details about the actual remote host to which a connection is being
    // attempted. The hostname information in this instance will have been
    // massaged based on DNS lookups and may not be the same as what was
    // originally provided.

    OTC_String		hostname() const
				{ return address_.hostname(); }
				// Returns the name of the host to which
				// this client will try to connect. Any
				// numeric address for the hostname will have
				// been converted to a FQDN if possible.

    OTC_String		numericHostname() const
				{ return address_.numericHostname(); }
				// Returns the name of the host to which
				// this client will try to connect. The
				// result will be an IP address.

    OTC_String		address() const
      				{ return address_.address(); }
    				// Returns the address of the host to which
				// this client will try to connect. This
				// combines the hostname and port number
				// seperated by a colon. Any numeric address
				// for the hostname will have been converted
				// to a FQDN if possible.

    OTC_String		numericAddress() const
      				{ return address_.numericAddress(); }
    				// Returns the address of the host to which
				// this client will try to connection. This
				// combines the hostname and port number
				// seperated by a colon. The host part will
				// be an IP address.

    // = RETRIES

    int			retryDelay() const
				{ return retryDelay_; }
				// Returns delay until a failed connect
				// is retried. Will return <-1> if retries
				// aren't enabled.

    // = ADDRESSING
    //     Following return a string which combines the hostname and the
    //     port number seperated by a colon.

    OTC_String		localTransport() const;
				// Returns the local address corresponding to
				// the transport layer address.

    OTC_String		remoteTransport() const;
				// Returns the remote address corresponding
				// to the transport layer address. Note that
				// the port number will be that actually
				// used for the other end of the connection
				// and not that which the connection request
				// was made against.

    // = CONNECTION

    void		start();
				// If the client is not connected, will
				// attempt to connect.

    void		stop(int theDelay=0);
				// If the client is connected, will close the
				// connection. If an attempt should be made
				// to flush out any pending messages,
				// <theDelay> can be set to a time in
				// milliseconds as the maxmimum amount of
				// time to wait in trying to write out the
				// messages. A value of <-1> for <theDelay>
				// indicates that all messages must be
				// written out before closing the connection.
				// If a call had been made to <stop()> with
				// non zero delay and <stop()> is called
				// again with zero delay, connection will
				// then be closed immediately.

    void		suspendInput();
				// Will suspend handling of input.

    void		resumeInput();
				// Will resume handling of input.

    void		restart(int theDelay=-1);
				// Should cause an active connection to be
				// dropped and the connection restarted. If
				// <theDelay> is <-1>, the connection will be
				// restarted after the retry delay, or if
				// there isn't one, immediately. If
				// <theDelay> is not <-1>, <theDelay> will be
				// how long to wait before restarting. Note
				// that if the restart fails, successive
				// attempts to restart will only be made if a
				// retry delay was specified. If one is,
				// retries will occur afters delays specified
				// by the retry delay and not <theDelay>.

    // = SOCKET

    int			fd() const
      				{ return (handler_!=0)?handler_->fd():-1; }
				// Returns the file descriptor for the
				// socket used for the connection.

  protected:

    // = INTERFACE HANDLER

    void		attach(OTC_EPISocket* theInterface);
				// Set the interface handler for this
				// endpoint to the socket interface defined
				// by <theInterface>. This method calls the
				// version of this method in the the
				// <OTC_EndPoint> base class, but also
				// stashes away a reference to the interface
				// object.

    // = CONNECTION SHUTDOWN

    void*		shutdown(void* theHandle, int theError);
				// Overrides the <shutdown()> method for the
				// <OTC_EndPoint> class. When called the
				// file descriptor pointed to by <theHandle>
				// will be closed.

    // = EVENT CALLBACK

    void		handle(OTC_Event* theEvent);
				// Handles timeout events for reconnecting
				// on failure.

    // = EVENT DELIVERY

    virtual void	connectionFailed();
    				// Called when an initial attempt to setup
				// the connection fails. By default generates
				// an appropriate <OTCEV_Connection> event
				// to observers where the remote address will
				// be empty.

  private:

    OTC_EPISocket*	handler_;
				// Protocol handler for socket connection.

    OTC_String		hostname_;
				// Hostname provided as the target of the
				// connection.

    int			port_;
				// Port provided as the target of the
				// connection.

    OTC_InetAddress	address_;
				// Address to which connections will
				// be made.

    OTC_String		localTransport_;
    				// Local socket name.

    OTC_String		remoteTransport_;
    				// Remote socket name.

    int			retryDelay_;
				// How long to wait to retry connection.

    int			retryTimer_;
				// Retry timer id.

    int			stopDelay_;
				// How long to wait before stopping.

    int			stopTimer_;
				// Stop timer id.

    int			blockedFd_;
				// File descriptor in use when initial
				// connection would have blocked.

    int			restartFd_;
				// File descriptor when doing a restart.
				// Saved here as need to pass back pointer.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_INETCLNT_HH */
