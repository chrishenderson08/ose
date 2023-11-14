#ifndef ONS_AGENT_MONITOR_HH
#define ONS_AGENT_MONITOR_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/monitor.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/agent.hh>
#include <ONS/agent/rgstrycb.hh>

#include <OTC/dispatch/svbroker.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/monitor.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_Monitor : public ONS_Agent
    // = TITLE
    //     Agent implementing callbacks for service registry monitoring.
    //
    // = DESCRIPTION
    //     The <ONS_Monitor> class extends the functionality of the base
    //     class <ONS_Agent> to add means of triggering callbacks when
    //     events of interest occur within the service registry. Direct calls
    //     can also be made to a subset of the service registry lookup
    //     functions.
    //     
    // = SEE ALSO
    //     <OTC_EVAgent>, <ONS_Agent>, <ONS_Service>, <OTC_SVRegistry>,
    //     <OTC_ServiceAnnouncement>, <OTC_GroupAnnouncement>,
    //     <ONS_MonitorExtension>
{
    friend class	ONS_MonitorExtension;

    typedef ONS_Monitor	THIS;

  public:

    // = DESTRUCTION

    			~ONS_Monitor();
				// Unsubscribes from all event sources.

    // = INITIALISATION

			ONS_Monitor() {}
				// Performs initialisation of the agent.

    // = SERVICE REGISTRY

    OTC_SVRegistry*	serviceRegistry() const
				{ return OTC_SVBroker::registry(); }
				// Returns a pointer to the service registry.

    // = SERVICE LOOKUP
    //     Where the following methods return an iterator, it is still safe
    //     to call them within a threaded application as the iterator will be
    //     operating on a copy of the appropriate data. This does mean
    //     however that there can be a significant overhead due to having to
    //     create the copy of the data. If you want to track service agents
    //     in a specific category, you are better of adding a subscription
    //     and accumulating the results in your own data structure which can
    //     then be under control of your own thread mutex locks if necessary.

    OTC_Iterator<OTC_ServiceBinding*>   serviceAgents() const
				{
				  return OTC_SVBroker::registry()
				   ->serviceAgents();
				}
                                // Returns an iterator over all service
                                // agents.

    OTC_Iterator<OTC_String>    serviceGroups(
                                 char const* theServiceAddress=0
                                ) const
				{
				  return OTC_SVBroker::registry()
				   ->serviceGroups(theServiceAddress);
				}
                                // Returns an iterator over service groups
                                // which the service agent identified by
                                // <theServiceAddress> is a member of. If
                                // <theServiceAddress> is <0>, an iterator
                                // over all active service groups is
                                // returned.

    OTC_Iterator<OTC_ServiceBinding*>   lookupServiceName(
                                 char const* theServiceName
                                ) const
				{
				  return OTC_SVBroker::registry()
				   ->lookupServiceName(theServiceName);
				}
                                // Returns an iterator over services with
                                // <theServiceName>.

    OTC_Iterator<OTC_ServiceBinding*>   lookupServiceAddress(
                                 char const* theServiceAddress
                                ) const
				{
				  return OTC_SVBroker::registry()
				   ->lookupServiceAddress(theServiceAddress);
				}
                                // Returns an iterator over services with
                                // <theServiceAddress>. This will only ever
                                // return at most one agent. An iterator is
                                // still used however, to make memory
                                // management easier and consistant with
                                // other lookup functions.

    OTC_Iterator<OTC_ServiceBinding*>   lookupServiceGroup(
                                 char const* theServiceGroup
                                ) const
				{
				  return OTC_SVBroker::registry()
				   ->lookupServiceGroup(theServiceGroup);
				}
                                // Returns an iterator over services in
                                // <theServiceGroup>.

    // = SERVICE NAMES

    template<class T>
    void		subscribeServiceName(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_ServiceStatus) const,
			 OTC_String const& theServiceName
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    template<class T>
    void		subscribeServiceName(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_ServiceStatus),
			 OTC_String const& theServiceName
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    template<class T>
    void		subscribeServiceName(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus) const,
			 OTC_String const& theServiceName
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    template<class T>
    void		subscribeServiceName(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*, OTC_ServiceStatus),
			 OTC_String const& theServiceName
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    template<class T>
    void		subscribeServiceName(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus) const,
			 OTC_String const& theServiceName
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    template<class T>
    void		subscribeServiceName(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&, OTC_ServiceStatus),
			 OTC_String const& theServiceName
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    void		unsubscribeServiceName(
    			 OTC_String const& theServiceName
			);
				// Cancels any interest by this agent in
				// notifications about service with
				// <theServiceName>.

    // = SERVICE GROUPS

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_ServiceStatus) const,
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_ServiceStatus),
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus) const,
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus),
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus) const,
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus),
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    void		unsubscribeServiceGroup(
    			 OTC_String const& theServiceGroup
			);
				// Cancels any interest by this agent in
				// notifications about <theServiceGroup>.

    // = SERVICE ADDRESSES
    //     Note that in subscribing to a service address, you will receive
    //     both service and group announcements for that address.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus) const,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus),
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus,char const*) const,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription=0
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus,char const*),
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus) const,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus),
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus,
			  OTC_String const&) const,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus,
			  OTC_String const&),
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,theSubscription);
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. Normally there can only be
				// one callback registered at a time against
				// each unique service address. This can be
				// avoided if a unique name is provided for
				// <theSubscription>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus) const,
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus),
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus,char const*) const,
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus,char const*),
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus) const,
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus),
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus,
			  OTC_String const&) const,
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    template<class T>
    OTC_ServiceStatus	subscribeServiceAddress(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus,
			  OTC_String const&),
			 OTC_String const& theServiceAddress
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return subscribeServiceAddress_(theCallback,
				   theServiceAddress,OTC_String::nullString());
				}
				// Registers this agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    void		unsubscribeServiceAddress(
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			);
				// Cancels any interest by this agent in
				// notifications about <theServiceAddress>
				// under <theSubscription> name.

    void		unsubscribeServiceAddress(
			 OTC_String const& theServiceAddress
			)
				{
				  unsubscribeServiceAddress(
				   theServiceAddress,OTC_String::nullString());
				}
				// Cancels any interest by this agent in
				// notifications about <theServiceAddress>
				// where no subscription name was provided.

    // = SERVICE REGISTRY

    template<class T>
    void		subscribeRegistry(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus) const
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeRegistry_(theCallback);
				}
				// Registers this agent as being interested
                                // in announcements regarding the addition or
                                // removal of services, or of services
				// joining or leaving groups. Notifications
				// will be made by executing <theMethod>.

    template<class T>
    void		subscribeRegistry(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus)
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeRegistry_(theCallback);
				}
				// Registers this agent as being interested
                                // in announcements regarding the addition or
                                // removal of services, or of services
				// joining or leaving groups. Notifications
				// will be made by executing <theMethod>.

    template<class T>
    void		subscribeRegistry(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus) const
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeRegistry_(theCallback);
				}
				// Registers this agent as being interested
                                // in announcements regarding the addition or
                                // removal of services, or of services
				// joining or leaving groups. Notifications
				// will be made by executing <theMethod>.

    template<class T>
    void		subscribeRegistry(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  OTC_String const&,OTC_ServiceStatus)
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  subscribeRegistry_(theCallback);
				}
				// Registers this agent as being interested
                                // in announcements regarding the addition or
                                // removal of services, or of services
				// joining or leaving groups. Notifications
				// will be made by executing <theMethod>.

    void		unsubscribeRegistry();
				// Cancels any interest by this agent in
				// receiving all notifications from the
				// service registry.

  protected:

    // = THREAD CONTEXTS

    void		allocateContexts(u_int theNumThreads);
    				// Allocates any data structures which need
				// to be specific to each thread, there
				// being <theNumThreads>.

    // = EVENT HANDLING

    void		filter(OTC_Event* theEvent);
				// Determines if <theEvent> corresponds to
				// a registered callback and if so triggers
				// the callback instead. Other events are
				// passed onto the base class <filter()>
				// member function.

    void		handle(OTC_Event* theEvent)
    				{ ONS_Agent::handle(theEvent); }
				// Destroys <theEvent>. Should be overridden
				// in a derived class to handle any event
				// not associated with a callback.

    // = CALLBACK TRIGGERS

    virtual void	handleServiceAnnouncement(
			 OTCEV_ServiceAnnouncement* theAnnouncement
			);
				// Called when <theAnnouncement> event is
				// received. If a service name or service
				// address subscription callback is
				// registered, it will be called as
				// appropriate.

    virtual void	handleGroupAnnouncement(
			 OTCEV_GroupAnnouncement* theAnnouncement
			);
				// Called when <theAnnouncement> event is
				// received. If a service group subscription
				// callback is registered, it will be called.

  private:

    // = CALLBACK REGISTRATION

    void		subscribeServiceName_(
			 ONS_EventCallback* theCallback,
			 OTC_String const& theServiceName
			);
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theCallback>.

    void		subscribeServiceGroup_(
			 ONS_EventCallback* theCallback,
			 OTC_String const& theServiceGroup
			);
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theCallback>.

    OTC_ServiceStatus	subscribeServiceAddress_(
			 ONS_EventCallback* theCallback,
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			);
				// Registers this agent as being interested
				// in availability and withdrawal of service
				// agent within <theServiceAddress>.
				// Notifications will be made by executing
				// <theCallback>. <theSubscription> can
				// be set to distinguish a specific
				// subscription and thus avoid limitation of
				// there being only one subscription against
				// a specific service address. Method returns
				// the current status of the service agent
				// with <theServiceAddress>.

    void		subscribeRegistry_(
    			 ONS_EventCallback* theCallback
			);
				// Registers this agent as being interested
                                // in announcements regarding the addition or
                                // removal of services, or of services
				// joining or leaving groups. Notifications
				// will be made by executing <theCallback>.

  private:

			ONS_Monitor(ONS_Monitor const&);
				// Do not define an implementation for this.

    ONS_Monitor&	operator=(ONS_Monitor const&);
				// Do not define an implementation for this.

    OTC_NRMutex		mutex_;
				// Mutex to protect thread access.

    OTC_HMap<OTC_String,ONS_EventCallback*>	registryCallbacks_;
				// Mapping from the subscription key to the
				// registered callback for that action.
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_MONITOR_HH */
