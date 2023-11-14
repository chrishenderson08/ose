#ifndef OTC_DISPATCH_SVBROKER_HH
#define OTC_DISPATCH_SVBROKER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/svbroker.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/servbind.hh>
#include <OTC/dispatch/svrgstry.hh>
#include <OTC/dispatch/svpyload.hh>
#include <OTC/collctn/tdualmap.hh>
#include <OTC/collctn/tmanymap.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/svbroker.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

class OTCEV_Request;

enum OTC_ServiceVisibility
{
  OTCLIB_SERVICE_HIDDEN=0,
  OTCLIB_SERVICE_VISIBLE=1
};

class OSE_EXPORT OTC_SVBroker : private OTC_EVAgent
    // = TITLE
    //     Adds request/reply and publish/subscribe features to an agent.
    //
    // = DESCRIPTION
    //     The <OTC_SVBroker> class is used as a service broker to extend the
    //     <OTC_EVAgent> class to provide distributed request/reply and
    //     publish/subscribe styles of interaction between agents. When the
    //     <OTC_SVBroker> class is used in conjunction with an agent, the
    //     agent has a unique receiver name associated with it. This allows
    //     the agent to send requests to and subscribe to service agents. By
    //     binding a service name, the agent becomes identifiable and can be
    //     a recipient of requests and make publications.
    //     
    //     Information about service agents is determined by querying the
    //     service registry. It is also possible to subscribe to the service
    //     registry in order to find out when services become available. The
    //     service registry is accessable through the <registry()> static
    //     member function.
    //     
    // = SEE ALSO
    //     <OTC_EVAgent>, <OTC_SVRegistry>, <OTC_ServiceBinding>,
    //     <OTCEV_Request>, <OTCEV_Response>, OTCEV_Subscription>,
    //     <OTCEV_Report>, <OTCEV_Failure>, <OTC_SVPayload>
{
  public:

    			~OTC_SVBroker();

    // = INITIALISATION

			OTC_SVBroker(
			 OTC_EVAgent* theAgent,
			 char const* theServiceName=0,
			 char const* theServiceAudience="*"
			);
				// Creates a service broker for <theAgent>
				// where the name which the agent will be
				// known by is given by <theServiceName>.
				// If <theServiceAudience> is empty or
				// a null string, details of the agent will
				// only be made available within the current
				// process. If <theServiceAudience> is "*",
				// details of the agent will be distributed
				// as widely as possible, ie., to other
				// processes if possible. Other values for
				// <theServiceAudience> will restrict the
				// audience. This may be a select group
				// within a process or across processes. How
				// far is dependent on mechanisms used to
				// broadcast knowledge about the agent.
				// When this constructor is used, the agent
				// will be made visible immediately.

			OTC_SVBroker(
			 OTC_EVAgent* theAgent,
			 char const* theServiceName,
			 char const* theServiceAudience,
			 OTC_ServiceVisibility theServiceVisibility
			);
				// Creates a service broker for <theAgent>
				// where the name which the agent will be
				// known by is given by <theServiceName>.
				// If <theServiceAudience> is empty or
				// a null string, details of the agent will
				// only be made available within the current
				// process. If <theServiceAudience> is "*",
				// details of the agent will be distributed
				// as widely as possible, ie., to other
				// processes if possible. Other values for
				// <theServiceAudience> will restrict the
				// audience. This may be a select group
				// within a process or across processes. How
				// far is dependent on mechanisms used to
				// broadcast knowledge about the agent.
				// When this constructor is used, the agent
				// will be made visible immediately if
				// <theServiceVisibility> has the value
				// <OTCLIB_SERVICE_VISIBLE>. If the value
				// <OTCLIB_SERVICE_HIDDEN> is supplied, the
				// agent will only subsequently be made
				// visible when <announceAvailability()>
				// is called.

    // = NAMING

    OTC_ServiceBinding*	serviceBinding() const
    				{ return serviceBinding_; }
				// Returns the service binding object.

    // = SERVICE VISIBILITY

    OTC_ServiceVisibility	serviceVisibility() const;
				// Returns whether or not the agent is
				// currently visible. In other words,
				// whether or not it is currently registered
				// with the service registry.

    void		announceAvailability();
    				// Where the service broker was initially
				// created as being hidden, needs to be
				// called to subsequently make the agent
				// visible. This has the affect of
				// registering the agent with the service
				// registry. If the agent is already visible,
				// a call to this method has not affect.

    // = SERVICE GROUPS
    //     Interested parties can know when an agent joins and leaves a group
    //     by subscribing to specific groups with the service registry.

    void		joinGroup(char const* theServiceGroup);
				// Adds the agent to the <theServiceGroup>.
				// An empty service group is not valid and
				// such a request will be ignored.

    void		leaveGroup(char const* theServiceGroup);
				// Removes the agent from the <theServiceGroup>.

    // = REQUEST/REPLY

    OTC_String          sendRequest(
                         OTC_ServiceBinding* theServiceAgent,
    			 char const* theMethodName,
			 OTC_ROPayload const& theContent
			);
				// Sends a message with <theMethodName> to
				// <theServiceAgent>. The body of the request
				// is given by <theContent>. When received by
				// the agent the event will be contained
				// within an <OTCEV_Request> event. The
				// conversation ID associated with the request
				// is returned. You should not include a new
				// line in <theMethodName>.

    OTC_String          sendRequest(
                         char const* theServiceAddress,
    			 char const* theMethodName,
			 OTC_ROPayload const& theContent
                        );
				// Sends a message with <theMethodName> to
				// <theServiceAddress>. The body of the
				// request is given by <theContent>. When
				// received by the agent the event will be
				// contained within an <OTCEV_Request> event.
				// The conversation ID associated with the
				// request is returned. You should not
				// include a new line in <theMethodName>.

    // = OPAQUE MESSAGES
    //     The following methods are specifically provided for sending opaque
    //     data messages to the target service agent. This is useful where it
    //     is an inconvenience or inefficient to translate data into an
    //     abstract data model for conversion into the underlying XML based
    //     message format. When sent in this way, the data message will be
    //     sent as is and not converted in anyway while being sent over the
    //     messaging system. When received by the agent, the event will be
    //     contained with an <OTCEV_Opaque> event.

    OTC_String          sendMessage(
                         OTC_ServiceBinding* theServiceAgent,
    			 char const* theMessageId,
    			 char const* theDescription,
			 OTC_String const& theContent,
			 char const* theContentType=0,
			 int theSequenceNumber=0
			);
				// Sends an opaque data message containing
				// <theContent> and with <theContentType>,
				// <theMessageId> and <theDescription> to
				// <theServiceAgent>. The conversation ID
				// associated with the request is returned.
				// You should not include a new line in
				// <theMessageId> or <theDescription>.
				// <theSequenceNumber> can be used to
				// indentify the message as one of a ordered
				// batch of related messages.

    OTC_String          sendMessage(
                         OTC_ServiceBinding* theServiceAgent,
    			 char const* theMessageId,
    			 char const* theDescription,
			 char const* theContent,
			 size_t theContentLength,
			 char const* theContentType=0,
			 int theSequenceNumber=0
			);
				// Sends an opaque data message containing
				// <theContent> of <theContentLength> and
				// with <theContentType>, <theMessageId> and
				// <theDescription> to <theServiceAgent>. The
				// conversation ID associated with the
				// request is returned. You should not
				// include a new line in <theMessageId> or
				// <theDescription>. <theSequenceNumber> can
				// be used to indentify the message as one of
				// a ordered batch of related messages.

    OTC_String          sendMessage(
                         char const* theServiceAddress,
    			 char const* theMessageId,
    			 char const* theDescription,
			 OTC_String const& theContent,
			 char const* theContentType=0,
			 int theSequenceNumber=0
			);
				// Sends an opaque data message containing
				// <theContent> and with <theContentType>,
				// <theMessageId> and <theDescription> to
				// <theServiceAddress>. The conversation ID
				// associated with the request is returned.
				// You should not include a new line in
				// <theMessageId> or <theDescription>.
				// <theSequenceNumber> can be used to
				// indentify the message as one of a ordered
				// batch of related messages.

    OTC_String          sendMessage(
                         char const* theServiceAddress,
    			 char const* theMessageId,
    			 char const* theDescription,
			 char const* theContent,
			 size_t theContentLength,
			 char const* theContentType=0,
			 int theSequenceNumber=0
			);
				// Sends an opaque data message containing
				// <theContent> of <theContentLength> and
				// with <theContentType>, <theMessageId> and
				// <theDescription> to <theServiceAddress>.
				// The conversation ID associated with the
				// request is returned. You should not
				// include a new line in <theMessageId> or
				// <theDescription>. <theSequenceNumber> can
				// be used to indentify the message as one of
				// a ordered batch of related messages.

    // = SUBSCRIPTIONS
    //     The service agent being subscribed to will receive notification
    //     of subscriptions or unsubscriptions by way of an instance of
    //     the <OTCEV_Subscription> event class. Note though that if there
    //     was a cached entry against a matching subject, that cached
    //     entry will have already been sent by the time the publisher
    //     gets the subscription notification.

    void		subscribeReports(
			 char const* theServiceName,
			 char const* theSubjectPattern
			);
				// Subscribes the agent to any reports
				// published by any service agents with
				// <theServiceName> where the subject of the
				// report matches <theSubjectPattern>.

    void		unsubscribeReports(
			 char const* theServiceName,
			 char const* theSubjectPattern
			);
				// Unsubscribes the agent from any reports
				// published by any service agents with
				// <theServiceName> where the subject of the
				// report matches <theSubjectPattern>.
				// <theSubjectPattern> must match the pattern
				// that was used when the subscription was
				// made.

    void		unsubscribeReports(char const* theServiceName);
				// Unsubscribes the agent from any reports
				// published by any service agents with
				// <theServiceName>.

    void		subscribeReports(
			 OTC_ServiceBinding* theServiceAgent,
			 char const* theSubjectPattern
			);
				// Subscribes the agent to any reports
				// published by <theServiceAgent> where the
				// subject of the report matches
				// <theSubjectPattern>.

    void		unsubscribeReports(
			 OTC_ServiceBinding* theServiceAgent,
			 char const* theSubjectPattern
			);
				// Unsubscribes the agent from any reports
				// published by <theServiceAgent> where the
				// subject of the report matches
				// <theSubjectPattern>. <theSubjectPattern>
				// must match the pattern that was used when
				// the subscription was made.

    void		unsubscribeReports(OTC_ServiceBinding* theServiceAgent);
				// Unsubscribes the agent from any reports
				// published by <theServiceAgent>.

    // = PUBLICATIONS

    void		publishReport(
    			 char const* theSubjectName,
			 OTC_ROPayload const& theContent,
			 int theLifeTime=0
                        );
				// Publishes a report under <theSubjectName>
				// to any agents who have subscribed using a
				// subject pattern which matches
				// <theSubjectName>. The body of the report
				// is given by <theContent>. If <theLifeTime>
				// is <-1> the report will be held in a cache
				// such that if another agent subscribes to
				// the same subject at some time in the
				// future it will at that point receive the
				// report. If some report was cached for the
				// subject already, the existing report will
				// be destroyed and replaced with the new
				// report. If <0> is provided for
				// <theLifeTime>, any cached report will be
				// discarded but the new report will not be
				// cached. If <theLifeTime> is some other
				// positive value, the new report will become
				// the new cache entry but will only be valid
				// for the number of seconds determined by the
				// argument. That is, if the argument were
				// <5>, any new subscriptions in the next <5>
				// seconds would receive the report but after
				// that the event will be expired and deleted
				// from the cache. You should not include a
				// newline in the subject name.

    void		sendReport(
                         OTC_ServiceBinding* theServiceAgent,
    			 char const* theSubjectName,
			 OTC_ROPayload const& theContent
                        );
				// Similar to <publishReport()> in that
				// <theServiceAgent> will receive an instance
				// of <OTCEV_Report>, with <theSubjectName>
				// and <theContent>. The difference is that
				// this is a directed report and only goes to
				// the specified service agent. The recipient
				// of this report will not know the
				// difference between this and any other
				// published report except that it is
				// possible to send a report on a subject it
				// isn't subscribed too. Further, this
				// function bypasses the internal cache. The
				// report is not held in the cache nor does
				// it replace any existing value or cause any
				// existing value to be removed or expired.
				// The intent of this function is to allow
				// you to send an explicit report to a
				// subscriber based on seeing its
				// subscription request rather than it
				// getting some value from the cache. Note
				// that <theServiceAgent> still needs to
				// have subscriptions against this service
				// broker else nothing will be sent. Also
				// note that if the service agent has
				// overlapping subscriptions which match
				// the subject name, it will still receive
				// the report twice.

    void                expireReports(
			 char const* theSubjectPattern,
			 u_int theAge
			);
				// Forcibly expires any cached reports with
				// subject name matching <theSubjectPattern>.
				// The reports will be expired and removed
				// from the cache if it is older than
				// <theAge> seconds.

    void                expireReports(u_int theAge);
				// Used to forcibly expire all cached reports
				// before they are due to be expired. Reports
                                // will be expired and removed from the cache
                                // if it is older than <theAge> seconds. To
                                // delete all cached reports, use a value of
                                // <0>.

    // Note that reports are kept in the cache and if they expire will only
    // be deleted when that subject is next accessed. This may mean that
    // dead entries accumulate in the cache. If this is an issue, you should
    // periodically purge the cache of any expired entries that haven't
    // been deleted.

    void                purgeReports();
                                // Removes from the cache any expired reports.

    // = REGISTRY

    static OTC_SVRegistry*	registry();
				// Returns the service registry.

    // = MATCHING
    //     The default subject matching algorithm honours two wildcard
    //     characters. These are '*' and '?'. The '?' character in a
    //     subscription will match a single character in the subject. The '*'
    //     character in a subscription will match zero or more characters. No
    //     hierarchy mechanism is imposed, thus '*' will match '.' or '/' if
    //     you are using them to create a subject hierarchy within your
    //     subject names. To match all subjects use '*'. This approach to
    //     wildcards in subscriptions is the same as IBM MQSeries.

    virtual bool	 matchSubject(
                         char const* theSubjectPattern,
                         char const* theSubjectName
                        ) const;
				// If <theSubjectPattern> matches
				// <theSubjectName> returns <true>.

  protected:

    // = CALLBACKS

    void		handle(OTC_Event* theEvent);
				// Interprets <theEvent> and passes on events
				// to the associated agent as required.

  private:

			OTC_SVBroker(OTC_SVBroker const&);
				// Do not define an implementation for this.

    OTC_SVBroker&	operator=(OTC_SVBroker const&);
				// Do not define an implementation for this.

    bool		builtinRequest_(OTCEV_Request* theEnvelope);
				// Responds to builtin requests methods,
				// returning <true> if the request was
				// handled and <false> otherwise. If the
				// request wasn't handled, it should be
				// passed onto the agent.

    OTC_EVAgent*	agent_;
				// The agent to which the service broker is
				// associated.

    static OTC_NRMutex	mutex_;
                                // Lock for threads.

    static OTC_NRMutex	sdmutex_;
                                // Lock for threads when creating the
				// registry.


    static OTC_SVRegistry*      gRegistry_;
                                // The service registry.

    OTC_Receiver        inBox_;
				// Receiver for events. Used to bind
				// anonymous name to the associated agent.

    OTC_ServiceBinding*	serviceBinding_;
    				// Current service binding.

    OTC_ServiceVisibility	serviceVisibility_;
    				// Records whether or not the agent has
				// be recorded with the service registry.

    OTC_TManyMap<OTC_String,OTC_String>	subscriptions_;
				// Map of services we wish to be subscribed
				// to and what subjects. Map is indexed by
				// the service name.

    OTC_TDualMap<OTC_String,OTC_ServiceBinding*>	publishers_;
				// Map of who we are actually subscribed to
				// for subscriptions by service name.

    OTC_TManyMap<OTC_String,OTC_String>	agentSubscriptions_;
				// Map of specific service agents we wish to
				// be subscribed to and what subjects. Map
				// is indexed by the service address.

    OTC_TDualMap<OTC_String,OTC_ServiceBinding*>	agentPublishers_;
				// Map of who we are actually subscribed to
				// for subscriptions by service address.

    OTC_TDualMap<OTC_String,OTC_ServiceBinding*>	subscribers_;
				// Map of subject patterns to service agents,
				// for service agents which have subscribed
				// to us.

    OTC_HMap<OTC_String,OTC_ServiceBinding*>	serviceAgents_;
				// Map onto service agents which we are
				// interested in either as publisher or
				// subscriber by virtue of their service
				// address. This is populated by the service
				// registry subscription based on service
				// address.

    OTC_HIndex<OTC_String>	subjects_;
				// The subject event cache.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SVBROKER_HH */
