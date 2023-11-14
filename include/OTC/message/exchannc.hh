#ifndef	OTC_MESSAGE_EXCHANNC_HH
#define	OTC_MESSAGE_EXCHANNC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/exchannc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/connect.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/exchannc.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTCEV_ExchangeAnnouncement : public OTC_Event
    // = TITLE
    //	   Event for notifying about exchange connections.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     The <OTCEV_ExchangeAnnouncement> class is used for notifying about
    //     the state of a logical connection existing between two exchanges
    //     in the same network group. One exchange will be in this process,
    //     the other in the process at the other end of a locally connected
    //     endpoint.
    //     
    // = SEE ALSO
    //	   <OTC_EndPoint>, <OTC_Exchange>
{
  public:

			~OTCEV_ExchangeAnnouncement();

    // = CONSTRUCTION

			OTCEV_ExchangeAnnouncement(
			 OTC_String const& theGroup,
			 OTC_String const& theLocalAddress,
			 OTC_String const& theRemoteAddress,
			 OTC_String const& theRemoteProcess,
			 OTC_ConnectionState theState
			);
				// <theState> indicates the state of any
				// logical connections with another exchange
				// for the exchange in this process in
				// network group given by <theGroup>.
				// <theLocalAddress> gives the address of the
				// local endpoint with that of the remote
				// endpoint in <theRemoteAddress>.
				// <theRemoteProcess> is that name provided
				// for the remote process. Note that
				// unlike <OTCEV_Connection> and the
				// <OTC_EndPoint> class you will not see any
				// event with empty remote address and state
				// of inactive. For endpoints this indicated
				// a failure to connect. The exchange
				// anouncement only notifies of success of a
				// connection and then loss of that
				// connection. Ie., no notification of failed
				// attempts are generated.

    // = QUERY

    OTC_String const&	group() const
				{ return group_; }
				// Returns the network group the exchange
				// is in.

    OTC_String const&	localAddress() const
				{ return localAddress_;	}
				// Returns the local address of	the
				// connection.

    OTC_String const&	remoteAddress()	const
				{ return remoteAddress_; }
				// Returns the remote address of the
				// connection.

    OTC_String const&	remoteProcess()	const
				{ return remoteProcess_; }
				// Returns the name of the remote process.

    OTC_ConnectionState	state() const
				{ return state_; }
				// Returns the state of the connection.

    // = IDENTIFICATION

    void*		type() const;
				// Returns a value uniquely identifying
				// this	type of	event.

    static void*	typeId()
				{ return (void*)&gTypeId_; }
				// Returns a value uniquely identifying
				// this	type of	event.

    // = DEBUGGING

    void		dump(ostream& outs) const;
				// Dumps out message type and buffer
				// contents.

  protected:

    // = NON DELIVERY

    void                cancelSource(int theAgentId);
				// Unsubscribes the agent with ID
				// <theAgentId> to which this event was to be
				// delivered, from all exchange announcements.
				// This function will be called when
                                // the agent to which this event was to be
                                // delivered does not exist.

  private:

			OTCEV_ExchangeAnnouncement(
			 OTCEV_ExchangeAnnouncement const&
			);
				// Do not define an implementation for this.

    OTCEV_ExchangeAnnouncement&	operator=(OTCEV_ExchangeAnnouncement const&);
				// Do not define an implementation for this.

    OTC_String		group_;
				// The network group the exchange is in.

    OTC_String		localAddress_;
				// The local address of	the connection

    OTC_String		remoteAddress_;
				// The remote address of the connection

    OTC_String		remoteProcess_;
				// The remote process.

    OTC_ConnectionState	state_;
				// The state of the connection.

    static int		gTypeId_;
				// Global object, the address of which
				// identifies this type	of event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EXCHANNC_HH */
