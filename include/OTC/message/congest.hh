#ifndef OTC_MESSAGE_CONGEST_HH
#define OTC_MESSAGE_CONGEST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/congest.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>
#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/congest.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_CongestionStatus
{
  OTCLIB_CONNECTION_BLOCKED = 0,
  OTCLIB_CONNECTION_CLEARED = 1,
  OTCLIB_CONNECTION_BACKLOG = 2
};

class OSE_EXPORT OTCEV_Congestion : public OTC_Event
    // = TITLE
    //	   Event for notifying of congestion in endpoint interface.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     The <OTCEV_Congestion> class is used for notifying when an
    //     endpoint interface is becoming congested or has recovered from
    //     such congestion. Congestion is when the endpoint interface
    //     becomes blocked due to the remote end not receiving data quick
    //     enough or when the interface accumulates a backlog of messages to
    //     send.
    //
    // = SEE ALSO
    //	   <OTC_EndPoint>
{
  public:

			~OTCEV_Congestion();

    // = CONSTRUCTION

			OTCEV_Congestion(
			 OTC_String const& theLocalAddress,
			 OTC_String const& theRemoteAddress,
			 OTC_CongestionStatus theStatus,
			 u_int thePending
			);
				// <theStatus> indicates the type of
				// congestion event being notified about for
				// the connection between endpoints with
				// <theLocalAddress> and <theRemoteAddress>.
				// The valid values for <theStatus> are
				// <OTCLIB_CONNECTION_BLOCKED> which indicates
				// that the interface is unable to send
				// further messages at this time, the value
				// <OTCLIB_CONNECTION_CLEARED> which indicates
				// that the interface is no longer blocked
				// and <OTCLIB_CONNECTION_BACKLOG> which
				// indicates that the interface is getting
				// overloaded with messages and is not
				// able to keep up. <thePending> indicates
				// how many messages are waiting to be sent.

    // = QUERY

    OTC_String const&	localAddress() const
				{ return localAddress_; }
				// Returns the local address of the
				// connection.

    OTC_String const&	remoteAddress() const
				{ return remoteAddress_; }
				// Returns the remote address of the
				// connection.

    OTC_CongestionStatus	status() const
				{ return status_; }
				// Returns the type of congestion event.

    u_int		pending() const
      				{ return pending_; }
				// Returns the number of messages which are
				// waiting to be sent.

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

			OTCEV_Congestion(OTCEV_Congestion const&);
				// Do not define an implementation for this.

    OTCEV_Congestion&	operator=(OTCEV_Congestion const&);
				// Do not define an implementation for this.

    OTC_String		localAddress_;
				// The local address of the connection

    OTC_String		remoteAddress_;
				// The remote address of the connection

    OTC_CongestionStatus	status_;
				// The type fo congestion event.

    u_int		pending_;
    				// Number of messages waiting to be sent.

    static int		gTypeId_;
				// Global object, the address of which
				// identifies this type of event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_CONGEST_HH */
