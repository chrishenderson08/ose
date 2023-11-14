#ifndef OTC_MESSAGE_EXCHANGE_HH
#define OTC_MESSAGE_EXCHANGE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/exchange.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/eplisten.hh>
#include <OTC/message/epclient.hh>
#include <OTC/message/exchannc.hh>
#include <OTC/message/eprgstry.hh>
#include <OTC/message/exrgstry.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/servbind.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>
#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/hset.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/exchange.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Exchange : public OTC_EVAgent
    // = TITLE
    //     A message exchange service.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Exchange> class implements a service for the exchange of
    //     messages (events) between agents. The agents can be located in the
    //     same process, or can be in a directly or indirectly connected
    //     process. If you wish to be notified about connections relevant to
    //     this exchange becoming active or inactive, you should subscribe
    //     to the exchange registry obtainable by using the <registry()>
    //     static member function. Note that the initial active connection
    //     notification only comes after exchanges at each end have exchanged
    //     initial configuration information.
    //     
    // = SEE ALSO
    //     <OTC_EPClient>, <OTC_EPListener>, <OTCEV_ExchangeAnnouncement>
{
  public:

			~OTC_Exchange();

    // = INITIALISATION

			OTC_Exchange(
			 OTC_ExchangeType theType,
			 char const* theGroup=0
			);
				// Creates the message exchange. The name of
				// the network group is specified by
				// <theGroup>. If <theGroup> is <0> or an
				// empty string, the default group is used.
				// If the exchange with the given group
				// already exists in this process an
				// exception will be raised. <theType>
				// indicates whether the exchange should
				// behave like a server or a client. The two
				// possible values for <theType> are
				// <OTCLIB_EXCHANGE_SERVER> and
				// <OTCLIB_EXCHANGE_CLIENT>. The default is
				// <OTCLIB_EXCHANGE_SERVER>. Other values for
				// <theType> are for for internal use only
				// and its use here is invalid and will
				// result in an exception being raised.

    // = QUERY

    OTC_String const&	name() const
				{ return inBox_.name(); }
				// Returns the full name of this exchange.
				// This includes the name of group if the
				// exchange is in one.

    OTC_String const&	group() const
				{ return group_; }
				// Returns the network group if this exchange
				// is in one. Result will be empty if it is
				// in the default group.

    OTC_ExchangeType	type() const
				{ return type_; }
				// Returns whether this exchange is
				// operating as a client or server.

    // = NAMING

    static OTC_String	fullName(char const* theGroup=0);
				// Returns what the full name of an exchange
				// would be if it was in <theGroup>. If
				// <theGroup> is <0> or an empty string,
				// the name will be for when the exchange is
				// in the default group.

    static OTC_String	groupName(OTC_String const& theFullName);
				// Returns what group the exchange with
				// <theFullName> is in. Returns an empty
				// string if it is in the default group.
				// It is assumed that <theFullName> is
				// a valid name and that it doesn't
				// contain embedded null characters, ie.,
				// no checks are made for validity.

    // = CONNECTIONS
    //     Note that its is assumed that the client endpoint or listeners
    //     implement connections which map onto a message based protocol
    //     across the connection.

    void		connect(OTC_EPClient* theClient);
				// Attempts to start the connection to
				// <theClient>. The connection of which
				// <theClient> is one end, should not
				// already be in an active state. Note that
				// this class will never give up <theClient>.
				// That is, it is assumed that on failure to
				// start, or due to a shutdown, that
				// <theClient> will automatically try to
				// restart the connection. If <theClient>
				// isn't set for automatic retries,
				// <theClient> will not be thrown away, but
				// this is based on the assumption that the
				// process is likely to be shutdown.

    OTC_EPClient*	connect(
                         char const* theHostname,
                         int thePort,
                         double theRetryDelay=5.0
                        );
                                // Initiates a connection to <theHostname> on
                                // <thePort> using INET socket protocol. If
                                // the connection fails, the connection will
                                // be automatically retried after period of
                                // seconds defined by <theRetryDelay> where
                                // the default is <5> seconds. A pointer to
                                // the client endpoint is returned.

    void		listen(OTC_EPListener* theListener);
				// Attempts to use <theListener> to listen
				// for new connections.

    OTC_EPListener*	listen(int thePort);
                                // Creates a listener socket for INET protocol
                                // on <thePort>. A pointer to the listener
                                // object is returned.

    void		shutdown(OTC_EPClient* theClient);
				// If there is an active connection for
				// <theClient> it will be stopped. When
				// the client has been totally shutdown
				// it will be dropped from the exchange.

    void		shutdown(OTC_EPListener* theListener);
				// The listener will be stopped and dropped
				// from the exchange. Provided you haven't
				// kept a reference to the listener which
				// prevents it from being destroyed, it will
				// be destroyed and any active servers
				// stopped and also dropped from the
				// exchange.

    // = REGISTRY

    static OTC_EXRegistry*	registry();
				// Returns the exchange registry.

  protected:

    // = EVENTS

    void		handle(OTC_Event* theEvent);
				// All normal interaction with the
				// service is done by messages (events).
				// This function will respond to <theEvent>
				// as appropriate.

    void                handleConnection(OTCEV_Connection* theConnection);
                                // Handles connection status event from
                                // endpoints.

    void                handleServiceAnnouncement(
			 OTCEV_ServiceAnnouncement* theAnnouncement
			);
                                // Handles service announcements from
				// local service registry.

    void                handleGroupAnnouncement(
			 OTCEV_GroupAnnouncement* theAnnouncement
			);
                                // Handles group announcements from local
				// service registry.

    void                handleBroadcastMessage(OTCEV_Envelope* theEnvelope);
                                // Tries to deliver <theEnvelope> locally
                                // to next recipient and also broadcasts
                                // <theEnvelope> to any exchanges which
                                // haven't seen it yet.

    void                handleExchangeMessage(OTCEV_Envelope* theEnvelope);
                                // Interprets <theEnvelope> as being sent to
                                // this instance from another exchange.
				// Handles various setup commands.

    // = MESSAGES

    void                messageExchangeAnnounce(OTCEV_Envelope* theEnvelope);
                                // Handles those messages which are initial
                                // setup messages from remote exchanges.

    void                messageServiceAvailable(OTCEV_Envelope* theEnvelope);
                                // Handles remote message indicating a service
				// is available.

    void                messageServiceWithdrawn(OTCEV_Envelope* theEnvelope);
                                // Handles remote message indicating a service
				// has been withdrawn.

    void                messageGroupAnnouncement(OTCEV_Envelope* theEnvelope);
                                // Handles remote message indicating a service
				// has joined or left a group.

    // = BROADCASTS

    void                broadcastMessage(
                         OTC_EPRegistry* theEPRegistry,
                         OTC_String const& theFromAddress,
                         OTC_String const& theSubject,
                         OTC_String const& theHeader,
                         OTC_Event* theEvent
                        );
                                // Broadcasts an event messages out to
                                // exchanges who haven't already received it.
				// Ie., don't send out via the endpoint
				// corresponding to <theEPRegistry> if not
				// <0>, as that corresponds to source of the
				// event message in the first place. The
				// message is sent with <theSubject>,
				// <theHeader> and contains <theEvent>.
				// Message should be seen to originate from
				// <theFromAddress>.

  private:

			OTC_Exchange(OTC_Exchange const&);
				// Do not define an implementation for this.

    OTC_Exchange&	operator=(OTC_Exchange const&);
				// Do not define an implementation for this.

    static OTC_NRMutex	mutex_;
				// Lock for main event handling.

    static OTC_NRMutex	rmutex_;
				// Lock for exchange registry.

    static OTC_EXRegistry*	gRegistry_;
				// Exchange registry.

    OTC_Receiver	inBox_;
				// Inbox to which messages are delivered.

    OTC_Receiver	remoteReceiverInBox_;
				// Special inbox for messages sent to
				// "$REMOTE-RECEIVER". Messages are broadcast
				// out to other processes to be delivered to
				// next receiver.

    OTC_Receiver	receiverInBox_;
				// Special inbox for messages sent to
				// "$RECEIVER". Messages are broadcast
				// out to other processes to be delivered to
				// next receiver. This is the same as above,
				// except that those sent to this receiver
				// name might also be delivered locally.

    OTC_String		group_;
				// The group the exchange is in.

    OTC_ExchangeType	type_;
				// Client or server.

    OTC_HSet<OTC_EPListener*>	listeners_;
				// Set of listeners.

    OTC_HMap<OTC_String,OTC_EPRegistry*>	endPoints_;
				// The set of endpoints monitored by this
				// exchange. This includes client endpoints
				// provided explicitly to this class and
				// server endpoints created by the listeners
				// explicitly provided to this class. The
				// endpoints are indexed by their local
				// address.

    OTC_HMap<OTC_String,OTC_ServiceBinding*>	services_;
				// Index onto services located within this
				// process or bridged from exchange groups
				// having a different name but which can
				// ride over this exchange group.

    OTC_TDualMap<OTC_String,OTC_String>	mapServiceGroup_;
    				// Mapping between service group names and
				// service addresses where a service has
				// joined a service group.

    OTC_String		localProcess_;
    				// The name of the local process.

    OTC_HSet<OTC_String>	processes_;
    				// Set of processes we are currently talking
				// to.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EXCHANGE_HH */
