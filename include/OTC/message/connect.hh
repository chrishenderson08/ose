#ifndef OTC_MESSAGE_CONNECT_HH
#define OTC_MESSAGE_CONNECT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/connect.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>
#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/connect.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_ConnectionState
{
  OTCLIB_CONNECTION_ACTIVE = 0,
  OTCLIB_CONNECTION_INACTIVE = 1
};

class OSE_EXPORT OTCEV_Connection : public OTC_Event
    // = TITLE
    //	   Event for notifying state of a connection between endpoints.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The <OTCEV_Connection> class is used for notifying about the
    //	   state of a connection existing between two endpoints.
    //
    // = SEE ALSO
    //	   <OTC_EndPoint>
{
  public:

			~OTCEV_Connection();

    // = CONSTRUCTION
    //     The local and remote addresses are that as used internally for
    //     event/message delivery. The local and remote transport are the
    //     addresses pertaining to the communications link being used.
    //     The local protocol describes the type of protocol which is being
    //     used as transport layer. For example "INET" or "UNIX" for socket
    //     based connections. The local interface describes how data is being
    //     packaged up for sending over the connection. For example "STREAM"
    //     for raw connection or "MESSAGE" for a message oriented type
    //     connection.

			OTCEV_Connection(
			 OTC_String const& theLocalAddress,
			 OTC_String const& theRemoteAddress,
			 OTC_String const& theLocalInterface,
			 OTC_String const& theLocalProtocol,
			 OTC_String const& theLocalTransport,
			 OTC_String const& theRemoteTransport,
			 OTC_ConnectionState theState
			);
				// <theState> indicates the state of the
				// connection between endpoints with
				// <theLocalAddress> and <theRemoteAddress>.
				// <theLocalInterface> is the type of
				// interface used on the connection.
				// <theLocalProtocol> is the transport layer
				// protocol being used. <theLocalTransport>
				// is the name relating to local end of
				// transport layer. <theRemoteTransport>
				// is the name relating to remote end of
				// transport layer.

    // The following constructors are for backward compatability only.

			OTCEV_Connection(
			 OTC_String const& theLocalAddress,
			 OTC_String const& theRemoteAddress,
			 OTC_String const& theLocalInterface,
			 OTC_ConnectionState theState
			);
				// <theState> indicates the state of the
				// connection between endpoints with
				// <theLocalAddress> and <theRemoteAddress>.
				// <theLocalInterface> is the type of
				// interface used on the connection.

			OTCEV_Connection(
			 OTC_String const& theLocalAddress,
			 OTC_String const& theRemoteAddress,
			 OTC_ConnectionState theState
			);
				// <theState> indicates the state of the
				// connection between endpoints with
				// <theLocalAddress> and <theRemoteAddress>.

    // = QUERY

    OTC_String const&	localAddress() const
				{ return localAddress_; }
				// Returns the local address of the
				// connection.

    OTC_String const&	remoteAddress() const
				{ return remoteAddress_; }
				// Returns the remote address of the
				// connection.

    OTC_String const&	localInterface() const
				{ return localInterface_; }
				// Returns the type of interface used
				// on the connection.

    OTC_String const&	localProtocol() const
				{ return localProtocol_; }
				// Returns the type of tranport layer
				// protocol being used.

    OTC_String const&	localTransport() const
				{ return localTransport_; }
				// Returns the local address at the transport
				// layer.

    OTC_String const&	remoteTransport() const
				{ return remoteTransport_; }
				// Returns the remote address at the transport
				// layer.

    OTC_ConnectionState state() const
				{ return state_; }
				// Returns the state of the connection.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value uniquely identifying
				// this type of event.

    static void*	typeId()
				{ return (void*)&gTypeId_; }
				// Returns a value uniquely identifying
				// this type of event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps out message type and buffer
				// contents.

  private:

			OTCEV_Connection(OTCEV_Connection const&);
				// Do not define an implementation for this.

    OTCEV_Connection&	operator=(OTCEV_Connection const&);
				// Do not define an implementation for this.

    OTC_String		localAddress_;
				// The local address of the connection

    OTC_String		remoteAddress_;
				// The remote address of the connection

    OTC_String		localInterface_;
				// The type of interface used on the
				// connection.

    OTC_String		localProtocol_;
				// The type of transport layer protocol.

    OTC_String		localTransport_;
				// The local transport address.

    OTC_String		remoteTransport_;
				// The remote transport address.

    OTC_ConnectionState state_;
				// The state of the connection.

    static int		gTypeId_;
				// Global object, the address of which
				// identifies this type of event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_CONNECT_HH */
