#ifndef OTC_DISPATCH_SVRGSTRY_HH
#define OTC_DISPATCH_SVRGSTRY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/svrgstry.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/servbind.hh>
#include <OTC/dispatch/subject.hh>
#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/svregsub.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/tmanymap.hh>
#include <OTC/collctn/tdualmap.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/collctn/hset.hh>
#include <OTC/collctn/ordlist.hh>
#include <OTC/thread/rwmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/svrgstry.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4251)
#endif

/* ------------------------------------------------------------------------- */

typedef OTC_Deque<OTC_ServiceBinding*> otc_dispatch_svrgstry_hh_typedef1;
typedef OTC_HSet<OTC_String> otc_dispatch_svrgstry_hh_typedef2;
typedef OTC_Deque<OTC_String> otc_dispatch_svrgstry_hh_typedef3;

class OSE_EXPORT OTC_SVRegistry : public OTC_EVAgent
    // = TITLE
    //	   Tracks local and remote services.
    //
    // = DESCRIPTION
    //     The <OTC_SVRegistry> class keeps track of local and remote
    //     services, allowing other agents to subscribe to the registry so
    //     that it knows when local or remote services become available.
    //     This is done by generating service announcements when services
    //     are added or removed.
    //
    // = NOTES
    //     Note that each instance of <OTC_SVRegistry> is independently
    //     thread safe. You do not need to perform external locking when
    //     using the class. In reality, there would probably only ever be
    //     one instance, that being available from <OTC_SVBroker>.
    //
    // = SEE ALSO
    //     <OTC_ServiceBinding>, <OTCEV_ServiceAnnouncement>, <OTC_SVBroker>
{
  public:

    // = DESTRUCTION

			~OTC_SVRegistry();
				// The registry is deleted. Note that no
				// announcements are sent out when the
				// registry is deleted. Normally the registry
				// wouldn't ever be deleted.

    // = CONSTRUCTION

			OTC_SVRegistry();
				// Creates an empty registry.

    // = LOOKUP

    OTC_Iterator<OTC_ServiceBinding*>	serviceAgents() const;
				// Returns an iterator over all service
				// agents.

    OTC_Iterator<OTC_String>	serviceGroups(
				 char const* theServiceAddress=0
				) const;
				// Returns an iterator over service groups
				// which service agent identified by
				// <theServiceAddress> is a member of. If
				// <theServiceAddress> is <0> or refers to
				// an empty string, an iterator over all
				// active service groups is returned.

    OTC_Iterator<OTC_ServiceBinding*>	lookupServiceName(
				 char const* theServiceName
				) const;
				// Returns an iterator over services with
				// <theServiceName>.

    OTC_Iterator<OTC_ServiceBinding*>	lookupServiceAddress(
				 char const* theServiceAddress
				) const;
				// Returns an iterator over services with
				// <theServiceAddress>. This will only ever
				// return at most one agent. An iterator is
				// still used however, to make memory
				// management easier and consistant with
				// other lookup functions.

    OTC_Iterator<OTC_ServiceBinding*>	lookupServiceGroup(
				 char const* theServiceGroup
				) const;
				// Returns an iterator over services in
				// <theServiceGroup>.

    // = SUBSCRIPTIONS
    //     When subscribing using the following functions, instances of
    //     <OTCEV_ServiceAnnouncement> and/or <OTCEV_GroupAnnouncement> will,
    //     as appropriate for the subscription type, be received as a result
    //     of the subscription. The event contains a field detailing the
    //     subscription type so you can know which subscription triggered the
    //     event.

    // For the following, subscription type contained within the announcement
    // will be <OTCLIB_SERVICE_REGISTRY>. When subscribing to the whole
    // registry, you will receive both service and group announcements.

    void		subscribeRegistry(
    			 int theAgentId,
			 int theLocation=~0
			);
				// Adds the agent identified by <theAgentId>
				// to the list of agents to receive service
				// announcements regarding the addition or
				// removal of services, or of services
				// joining or leaving groups. The default
				// value for <theLocation> ensures that
				// announcements for local and remote
				// services will be sent back. Alternatively,
				// <theLocation> can be set to the value
				// <OTCLIB_SERVICE_LOCAL> for local service
				// announcements or the value
				// <OTCLIB_SERVICE_REMOTE> for remote service
				// announcements.

    void		unsubscribeRegistry(
    			 int theAgentId,
			 int theLocation=~0
			);
				// Removes the agent identified by
				// <theAgentId> from the list of agents to
				// receive service announcements regarding
				// the addition or removal of services, or of
				// services joining or leaving groups. The
				// default value for <theLocation> ensures
				// that announcements for local and remote
				// services will no longer be sent back.
				// Alternatively, <theLocation> can be set to
                                // the value <OTCLIB_SERVICE_LOCAL> for local
                                // service announcements or the value
                                // <OTCLIB_SERVICE_REMOTE> for remote service
                                // announcements.

    // For the following, subscription type contained within the announcement
    // will be <OTCLIB_SERVICE_NAME>. When subscribing to a service name,
    // you will only receive service announcements even if a service is a
    // members of a group.

    void		subscribeServiceName(
			 int theAgentId,
			 OTC_String const& theServiceName
			);
				// Adds the agent identified by <theAgentId>
				// to the list of agents to receive
				// announcements about services with
				// <theServiceName>. If corresponding
				// services already exist, service
				// announcements describing their
				// availability will be sent out straight
				// away. If the agent is already subscribed,
				// the subscription will be unchanged, but
				// relevant service announcements will be
				// sent again.

    void		unsubscribeServiceName(
			 int theAgentId,
			 OTC_String const& theServiceName
			);
				// Removes the agent identified by
				// <theAgentId> from the list of agents to
				// receive announcements about services with
				// <theServiceName>.

    void		unsubscribeServiceName(int theAgentId);
				// Removes the agent identified by
				// <theAgentId> from the list of agents to
				// receive announcements about services based
				// on the service name. That is, if the agent
				// has expressed interest in more than one
				// name, all such subscriptions are removed.

    // For the following, subscription type contained within the announcement
    // will be <OTCLIB_SERVICE_ADDRESS>. When subscribing to a service
    // address, you will receive both service and group announcements.

    OTC_ServiceStatus	subscribeServiceAddress(
			 int theAgentId,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			);
				// Adds <theSubscription> for the agent
				// identified by <theAgentId> to the list of
				// subscriptions to receive announcements
				// about services with <theServiceAddress>.
				// If a service with <theServiceAddress>
				// already exists, a service announcement
				// describing its availability will be sent
				// out straight away. If the agent is already
				// subscribed with <theSubscription>, the
				// subscription will be unchanged, but a
				// relevant service announcement will be sent
				// again. In reality there can only ever be
				// one service with <theServiceAddress>. As
				// this is the case, the return value will
				// give an immediate indication if the
				// service currently exists. If the service
				// exists <OTCLIB_SERVICE_AVAILABLE> will be
				// returned else <OTCLIB_SERVICE_WITHDRAWN>
				// will be returned. If the later is
				// returned, it would generally indicate that
				// the agent has already dissappeared, in
				// which case you most likely should
				// unsubscribe straight away and abandon what
				// you were doing in respect of that agent.

    // Following two methods invoke above methods with a subscription name
    // which is empty.

    OTC_ServiceStatus	subscribeServiceAddress(
			 int theAgentId,
			 OTC_String const& theServiceAddress
			)
				{
				  return subscribeServiceAddress(theAgentId,
				   theServiceAddress,OTC_String::nullString());
				}
				// Adds the subscription for the agent
				// identified by <theAgentId> to the list of
				// subscriptions to receive announcements
				// about services with <theServiceAddress>.
				// If a service with <theServiceAddress>
				// already exists, a service announcement
				// describing its availability will be sent
				// out straight away. In reality there can
				// only ever be one service with
				// <theServiceAddress>. As this is the case,
				// the return value will give an immediate
				// indication if the service currently
				// exists. If the service exists
				// <OTCLIB_SERVICE_AVAILABLE> will be
				// returned else <OTCLIB_SERVICE_WITHDRAWN>
				// will be returned. If the later is
				// returned, it would generally indicate that
				// the agent has already dissappeared, in
				// which case you most likely should
				// unsubscribe straight away and abandon what
				// you were doing in respect of that agent.

    void		unsubscribeServiceAddress(
			 int theAgentId,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			);
				// Removes <theSubscription> of the agent
				// identified by <theAgentId> from the list
				// of subscriptions to receive announcements
				// about services with <theServiceAddress>.

    void		unsubscribeServiceAddress(
			 int theAgentId,
			 OTC_String const& theServiceAddress
			)
				{
				  unsubscribeServiceAddress(theAgentId,
				   theServiceAddress,OTC_String::nullString());
				}
				// Removes the subscription of the agent
				// identified by <theAgentId> from the list
				// of subscriptions to receive announcements
				// about services with <theServiceAddress>.

    void		unsubscribeServiceAddress(int theAgentId);
				// Removes the agent identified by
				// <theAgentId> from the list of agents to
				// receive announcements about services based
				// on the service address. That is, if the
				// agent has expressed interest in more than
				// one type, all such subscriptions are
				// removed.

    // For the following, subscription type contained within the announcement
    // will be <OTCLIB_SERVICE_GROUP>. When subscribing to a service group,
    // you will only receive group announcements. That is, no service
    // announcement are sent.

    void		subscribeServiceGroup(
			 int theAgentId,
			 OTC_String const& theServiceGroup
			);
				// Adds the agent identified by <theAgentId>
				// to the list of agents to receive
				// announcements about services in
				// <theServiceGroup>. If corresponding
				// services already exist, service
				// announcements describing their
				// availability will be sent out straight
				// away. If the agent is already subscribed,
				// the subscription will be unchanged, but
				// relevant service announcements will be
				// sent again.

    void		unsubscribeServiceGroup(
			 int theAgentId,
			 OTC_String const& theServiceName
			);
				// Removes the agent identified by
				// <theAgentId> from the list of agents to
				// receive announcements about services in
				// <theServiceGroup>.

    void		unsubscribeServiceGroup(int theAgentId);
				// Removes the agent identified by
				// <theAgentId> from the list of agents to
				// receive announcements about services based
				// on the service group. That is, if the
				// agent has expressed interest in more than
				// one group, all such subscriptions are
				// removed.

    // Following can be used to cancel all subscriptions regardless of type.

    void		unsubscribeAgent(int theAgentId);
				// Removes the agent identified by
				// <theAgentId> from all subscription lists.

    // = SERVICES

    OTC_ServiceBinding*	addServiceBinding(OTC_ServiceBinding* theBinding);
				// Adds the service to the registry and
				// generates a service available announcement
				// to parties interested in the registry. If
				// a service with the same service address is
				// already known of, an exception is raised.
				// The service binding object returned is the
				// same as that passed as argument.

    OTC_ServiceBinding*	addServiceBinding(
                         char const* theServiceName,
                         char const* theServiceAudience,
                         char const* theReceiverName,
                         char const* theProcessAddress=0,
                         char const* theNetworkGroup=0
			);
				// Adds a new service to the registry and
				// generates a service available announcement
				// to parties interested in the registry. If
				// a service with the same service address is
				// already known of, an exception is raised.
				// This method is similar to above but will
				// also create the service binding object for
				// you. Note though that if the caller wants
				// to maintain a reference to the returned
				// service object beyond the time it is
				// removed from the service registry, it will
				// need to clone it.

    void		removeServiceBinding(char const* theServiceAddress);
				// Removes the service from the registry with
				// <theServiceAddress> and generates a service
				// announcement indicating the service is no
				// longer available. If no such service
				// exists, no action is taken.

    // = GROUPS

    void		addToServiceGroup(
			 char const* theServiceGroup,
			 char const* theServiceAddress
			);
    				// Adds the service described by
				// <theServiceAddress> to <theServiceGroup>.
				// Note that an empty service group is
				// not valid and such a request will be
				// ignored.

    void		removeFromServiceGroup(
			 char const* theServiceGroup,
			 char const* theServiceAddress
			);
    				// Removes the service described by
				// <theServiceAddress> from <theServiceGroup>.

  protected:

    // = EVENTS

    void		handle(OTC_Event* theEvent);
				// Handles events. The events would be the
				// messages to be sent to local services with
				// the registry as proxy.

  private:

			OTC_SVRegistry(OTC_SVRegistry const&);
				// Do not define an implementation for this.

    OTC_SVRegistry&	operator=(OTC_SVRegistry const&);
				// Do not define an implementation for this.

    OTC_RWMutex		mutex_;
				// Lock for threads.

    OTC_Subject		observersLocal_;
				// Subject object for tracking observers
				// interested in service creation and
				// deletion for local services. Ie.,
				// observers of the registry as a whole.

    OTC_Subject		observersRemote_;
				// Subject object for tracking observers
				// interested in service creation and
				// deletion for remote services. Ie.,
				// observers of the registry as a whole.

    OTC_HMap<OTC_String,OTC_ServiceBinding*>	services_;
				// Set of all services indexed by
				// address.

    OTC_TManyMap<OTC_String,OTC_ServiceBinding*>	mapServiceName_;
				// Maps service name to service.

    OTC_TDualMap<OTC_String,OTC_String>	mapServiceGroup_;
				// Maps service group to address of services
				// in that group.

    OTC_TDualMap<OTC_String,int>	subServiceName_;
				// Maps service name to agents interested in
				// that service.

    OTC_HMap<OTC_String,OTC_SVRegistrySubscription*>	subServiceAddress_;
    				// Maps service address to subscriptions
				// for that service address.

    OTC_TDualMap<OTC_String,int>	mapServiceAddress_;
				// Maps service address to agents interested
				// in that address and vice versa.

    OTC_TDualMap<OTC_String,int>	subServiceGroup_;
				// Maps service group to agents interested
				// in that group.

    OTC_Receiver	localServiceInBox_;
				// Special receiver for "$LOCAL-SERVICE".
				// Messages to to this address will be
				// immediately delivered to any local
				// services with service binding matching
				// next component of the address.

    OTC_Receiver	remoteServiceInBox_;
				// Special receiver for "$REMOTE-SERVICE".
				// Messages to to this address will be
				// immediately delivered to any remote
				// services with service binding matching
				// next component of the address.

    OTC_Receiver	serviceInBox_;
				// Special receiver for "$SERVICE".
				// Messages to to this address will be
				// immediately delivered to any
				// service with service binding matching
				// next component of the address. Ie.,
				// may get sent to services outside of
				// this process.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_SVRGSTRY_HH */
