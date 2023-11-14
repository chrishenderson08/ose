#ifndef ONS_AGENT_MNTREXTN_HH
#define ONS_AGENT_MNTREXTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/mntrextn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/monitor.hh>
#include <ONS/agent/agntextn.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT ONS_MonitorExtension : public ONS_AgentExtension
    // = TITLE
    //	   Extension for callbacks objects associated with an agent.
    //
    // = DESCRIPTION
    //     The <ONS_MonitorExtension> class provides a means of separating out
    //     from an <ONS_Monitor> derived class, any exported methods, callbacks
    //     and associated functionality. This means that it is possible to
    //     create modules of reusable functionality which can be associated
    //     with any derived instance of <ONS_Monitor>.
    //
    // = NOTES
    //     Any derived instance of this class must be created using <operator
    //     new()> on the heap. Once a derived class calls any method which
    //     registers some form of callback, the associated monitor class
    //     instance will manage ownership of the object and the object will
    //     be automatically deleted when the last callback has been removed.
    //     Thus, unless you wanted to reuse an object, there isn't actually a
    //     need to keep a pointer to the object once it has been setup nor
    //     explicitly delete it. If you do keep a pointer, you must use the
    //     reference counting management functions of the base class as
    //     appropriate.
    //     
    // = SEE ALSO
    //     <ONS_AgentExtension>, <ONS_Monitor>
{
    typedef ONS_MonitorExtension	THIS;
    typedef OTC_Resource	COBJ;

  public:

    // = INITIALISATION

			ONS_MonitorExtension(ONS_Monitor* theAgent)
			 : ONS_AgentExtension(theAgent) {}
				// Associates this callback object with
				// <theAgent>.

    // = TARGET AGENT

    ONS_Monitor*	agent() const
    				{
				  ONS_Agent* theAgent;
				  theAgent = ONS_AgentExtension::agent();
				  return (ONS_Monitor*)theAgent;
				}
				// Returns a pointer to the associated
				// agent.

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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceName_(theCallback,
				   theServiceName);
				}
				// Registers the agent as being interested
				// in availability and withdrawal of service
				// with <theServiceName>. Notifications will
				// be made by executing <theMethod>. There
				// can only be one callback registered at a
				// time against each unique service name.

    void		unsubscribeServiceName(OTC_String const& theServiceName)
    				{
				  agent()->unsubscribeServiceName(
				   theServiceName);
				}
				// Cancels any interest by the agent in
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers the agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service name.

    template<class T>
    void		subscribeServiceGroup(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus),
			 OTC_String const& theServiceGroup
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeServiceGroup_(theCallback,
				   theServiceGroup);
				}
				// Registers the agent as being interested
				// in availability and withdrawal of service
				// agents within <theServiceGroup>.
				// Notifications will be made by executing
				// <theMethod>. There can only be one
				// callback registered at a time against each
				// unique service group.

    void		unsubscribeServiceGroup(
    			 OTC_String const& theServiceGroup
			)
    				{
				  agent()->unsubscribeServiceGroup(
				   theServiceGroup);
				}
				// Cancels any interest by the agent in
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
			 OTC_String const& theSubscription
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   theSubscription);
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  return agent()->subscribeServiceAddress_(
				   theCallback,theServiceAddress,
				   OTC_String::nullString());
				}
				// Registers the agent as being interested
				// in availability and withdrawal of the
				// service with <theServiceAddress>.
				// Notifications will be made by executing
				// <theMethod>. The method returns the
				// current status of the service agent with
				// <theServiceAddress>.

    void		unsubscribeServiceAddress(
			 OTC_String const& theServiceAddress,
			 OTC_String const& theSubscription
			)
				{
				  return agent()->unsubscribeServiceAddress(
				   theServiceAddress,theSubscription);
				}
				// Cancels any interest by the agent in
				// notifications about <theServiceAddress>
				// under <theSubscription> name.

    void		unsubscribeServiceAddress(
			 OTC_String const& theServiceAddress
			)
				{
				  return agent()->unsubscribeServiceAddress(
				   theServiceAddress,OTC_String::nullString());
				}
				// Cancels any interest by the agent in
				// notifications about <theServiceAddress>
				// where no subscription name was given.

    // = SERVICE REGISTRY

    template<class T>
    void		subscribeRegistry(
			 void (T::*theMethod)(OTC_ServiceBinding*,
			  char const*,OTC_ServiceStatus) const
			)
    				{
				  ONS_EventCallback* theCallback =
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeRegistry_(theCallback);
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeRegistry_(theCallback);
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeRegistry_(theCallback);
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
				   ONS_RegistryCallbackFactory<COBJ,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  agent()->subscribeRegistry_(theCallback);
				}
				// Registers this agent as being interested
                                // in announcements regarding the addition or
                                // removal of services, or of services
				// joining or leaving groups. Notifications
				// will be made by executing <theMethod>.

    void		unsubscribeRegistry()
				{ agent()->unsubscribeRegistry(); }
				// Cancels any interest by this agent in
				// receiving all notifications from the
				// service registry.

  private:

			ONS_MonitorExtension(ONS_MonitorExtension const&);
				// Do not define an implementation for this.

    ONS_MonitorExtension&	operator=(ONS_MonitorExtension const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class ONS_CallbackObject<ONS_MonitorExtension>
{
  public:

    static void		acquire(ONS_MonitorExtension const* theObject)
    				{ theObject->reference(); }

    static void		release(ONS_MonitorExtension const* theObject)
    				{ theObject->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_MNTREXTN_HH */
