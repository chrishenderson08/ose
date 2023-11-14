#ifndef ONS_AGENT_SERVICE_HH
#define ONS_AGENT_SERVICE_HH
/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/service.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/monitor.hh>
#include <ONS/agent/svexcept.hh>
#include <ONS/agent/rquestcb.hh>
#include <ONS/agent/rspnsecb.hh>
#include <ONS/agent/failrecb.hh>
#include <ONS/agent/delaydcb.hh>
#include <ONS/agent/actvrqst.hh>
#include <ONS/agent/pndgcncl.hh>
#include <ONS/agent/pndgrspn.hh>
#include <ONS/agent/opaquecb.hh>
#include <ONS/agent/reportcb.hh>
#include <ONS/agent/binary.hh>
#include <OTC/dispatch/sbscrptn.hh>
#include <OTC/dispatch/pbntfctn.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "ONS/agent/service.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class ONS_ServiceContext;

class OSE_EXPORT ONS_Service : public ONS_Monitor
    // = TITLE
    //     Service implementation utilising member function callbacks.
    //
    // = DESCRIPTION
    //     The <ONS_Service> class implements a service agent whereby the
    //     service requests are mapped to exported member functions of the
    //     class, with automatic marshalling and unmarshalling of method
    //     arguments and results being automatically carried out. Methods are
    //     also provided for initiating remote requests, dealing with the
    //     results from those request and also for publishing reports.
    //     
    // = SEE ALSO
    //     <OTC_SVBroker>, <OTCEV_Request>, <OTCEV_Response>,
    //     <OTCEV_Failure>, <OTCEV_Report>, <OTC_SVRegistry>,
    //     <OTC_SVPayload>, <OTC_ROPayload>, <OTC_RWPayload>,
    //     <ONS_ArgumentWrapper>, <ONS_ServiceExtension>
{
    friend class	ONS_ServiceExtension;

    typedef ONS_Service	THIS;

  public:

			~ONS_Service();

    // = INITIALISATION
    //     Where possible any derived class should be created in the main
    //     dispatcher thread. If this is not done and the mode of construction
    //     is used which makes the service visible immediately is used,
    //     service notifications relating to the construction of the service
    //     may be received by interested parties within the same process
    //     before this service has been fully constructed. If the third party
    //     is quick enough, it might even initiate a request or subscription
    //     against this service before this service is ready to respond to it
    //     in a meaningful way. For a similar reason, a derived class
    //     constructor should never activate threading. Threading should
    //     instead be activated by whatever creates the most derived class.
    //     Alternatively, the creator of the derived class could schedule
    //     a startup callback with the registered startup method activating
    //     threading. Note that problems which can occur as a result of the
    //     service agent being immediately visible can be avoiding by using
    //     the mode of construction which keeps it hidden until an explicit
    //     call to <announceAvailability()>.

			ONS_Service(
			 char const* theServiceName="",
			 char const* theServiceAudience="*"
			);
				// Initialises the service agent to have
				// <theServiceName>. Notifications about the
				// existance of the service agent will be
				// propogated to the extent given by
				// <theServiceAudience>. Default arguments
				// create an anonymous agent with an empty
				// service name and maximum service audience.
				// It is recommended that anonymous agents
				// not be overly used. When this constructor
				// is used, the agent will be made visible
				// immediately.

			ONS_Service(
			 char const* theServiceName,
			 char const* theServiceAudience,
			 OTC_ServiceVisibility theServiceVisibility
			);
				// Initialises the service agent to have
				// <theServiceName>. Notifications about the
				// existance of the service agent will be
				// propogated to the extent given by
				// <theServiceAudience>. When this
				// constructor is used, the service agent
				// will be made visible immediately if
				// <theServiceVisibility> has the value
				// <OTCLIB_SERVICE_VISIBLE>. If the value
				// <OTCLIB_SERVICE_HIDDEN> is supplied, the
				// service agent will only subsequently be
				// made visible when <announceAvailability()>
				// is called.

    // = SERVICE BINDING
    //     A method for obtaining the network group of the service agent is
    //     not explicitly provided as it will always be empty when querying
    //     on itself. It is only in another process that the network group
    //     is ever seen as different as it relates to how information about
    //     the service is propogated and is not strictly an attribute of the
    //     service itself.

    OTC_ServiceBinding* serviceBinding() const
                                { return svbroker_->serviceBinding(); }
                                // Returns the service binding.

    OTC_String const&   serviceName() const
                                { return serviceBinding()->serviceName(); }
                                // Returns the name of the service agent.

    OTC_String const&   serviceAudience() const
                                { return serviceBinding()->serviceAudience(); }
                                // Returns the service audience.

    OTC_String const&   serviceAddress() const
                                { return serviceBinding()->serviceAddress(); }
                                // Returns the full address of the service.

    OTC_String const&   agentIdentity() const
                                { return serviceBinding()->agentIdentity(); }
                                // Returns the services unique agent identity.

    OTC_String const&   processIdentity() const
                                { return serviceBinding()->processIdentity(); }
                                // Returns the corresponding process identity.

    // = SERVICE VISIBILITY

    OTC_ServiceVisibility	serviceVisibility() const
    				{ return svbroker_->serviceVisibility(); }
				// Returns whether or not the service agent
				// is currently visible. In other words,
				// whether or not it is currently registered
				// with the service registry.

    void		announceAvailability()
    				{ svbroker_->announceAvailability(); }
    				// Where the service agent was initially
				// created as being hidden, needs to be
				// called to subsequently make the service
				// agent visible. This has the affect of
				// registering the service agent with the
				// service registry. If the service agent is
				// already visible, a call to this method has
				// no affect.

    // = SERVICE GROUPS

    void                joinGroup(char const* theServiceGroup)
                                { svbroker_->joinGroup(theServiceGroup); }
                                // Adds this service agent to the
                                // <theServiceGroup>. An empty service group   
                                // is not valid and such a request will be
                                // ignored.

    void                leaveGroup(char const* theServiceGroup)
                                { svbroker_->leaveGroup(theServiceGroup); }
                                // Removes this service agent from the
                                // <theServiceGroup>.

    // = EXPORTED METHODS

    OTC_Iterator<OTC_String>	exportedMethods() const;
				// Returns an iterator over the list of
				// exported methods for this service agent.

    // The many overloadings of the <exportMethod()> which follow provide the
    // means of making member functions with up to nine arguments callable
    // using the request/reply functionality of the messaging system. The
    // member functions can be const or non const and can return void or a
    // value. All of the arguments and the return type of the member function
    // being exported must be able to be marshalled and unmarshalled against
    // the payload classes. Arguments cannot be non const references. Use
    // of pointers as arguments is limited, with special argument wrappers
    // needing to be defined for the type so it is known how to create the
    // object which the pointer refers to. A predefined argument wrapper is
    // provided such that a pointer to const character string can be accepted
    // as an argument.

    template<class T>
    void		exportMethod(
			 void (T::*theMethod)() const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// no arguments and returning void.

    template<class T>
    void		exportMethod(
			 void (T::*theMethod)(),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking no arguments and returning void.

    template<class T, class RT>
    void		exportMethod(
			 RT (T::*theMethod)() const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// no arguments and returning a value.

    template<class T, class RT>
    void		exportMethod(
			 RT (T::*theMethod)(),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking no arguments and returning a value.

    template<class T, class A1>
    void		exportMethod(
			 void (T::*theMethod)(A1) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// one argument and returning void.

    template<class T, class A1>
    void		exportMethod(
			 void (T::*theMethod)(A1),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking one argument and returning void.

    template<class T, class RT, class A1>
    void		exportMethod(
			 RT (T::*theMethod)(A1) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// one argument and returning a value.

    template<class T, class RT, class A1>
    void		exportMethod(
			 RT (T::*theMethod)(A1),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking one argument and returning a value.

    template<class T, class A1, class A2>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking two arguments and returning void.

    template<class T, class A1, class A2>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking two arguments and returning void.

    template<class T, class RT, class A1, class A2>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// two arguments and returning a value.

    template<class T, class RT, class A1, class A2>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking two arguments and returning a value.

    template<class T, class A1, class A2, class A3>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// three arguments and returning void.

    template<class T, class A1, class A2, class A3>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking three arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// three arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking three arguments and returning a
				// value.

    template<class T, class A1, class A2, class A3, class A4>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking four arguments and returning void.

    template<class T, class A1, class A2, class A3, class A4>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking four arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3, class A4>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// four arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3, class A4>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking four arguments and returning a
				// value.

    template<class T, class A1, class A2, class A3, class A4, class A5>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking five arguments and returning void.

    template<class T, class A1, class A2, class A3, class A4, class A5>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking five arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// five arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking five arguments and returning a
				// value.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking six arguments and returning void.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking six arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// six arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking six arguments and returning a
				// value.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking seven arguments and returning void.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking seven arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6, class A7>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// seven arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6, class A7>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking seven arguments and returning a
				// value.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking eight arguments and returning void.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking eight arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6, class A7, class A8>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// eight arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6, class A7, class A8>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking eight arguments and returning a
				// value.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8, class A9>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method
				// taking nine arguments and returning void.

    template<class T, class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8, class A9>
    void		exportMethod(
			 void (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking nine arguments and returning void.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6, class A7, class A8, class A9>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const method taking
				// nine arguments and returning a value.

    template<class T, class RT, class A1, class A2, class A3, class A4,
     class A5, class A6, class A7, class A8, class A9>
    void		exportMethod(
			 RT (T::*theMethod)(A1,A2,A3,A4,A5,A6,A7,A8,A9),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const method
				// taking nine arguments and returning a
				// value.

    // = CURRENT OPERATION

    OTC_String		conversationId() const;
				// Returns the conversation ID of the
				// request, response, failure or message
				// currently being handled or an empty string
				// if none of these are currently being
				// handled.

    // Following should only be called from a member function which has been
    // called as a result of some event having occurred. If a copy of
    // the object returned is required, the <clone()> method will need to be
    // executed with the result being downcast back to the appropriate type.
    //
    // Note that when multiple threads have been activated against an agent,
    // the following methods will return the object which is specific to
    // that thread.

    OTCEV_Request*	currentRequest() const;
				// If the executing member function is
				// triggered within the scope of handling a
				// request, the original request event will
				// be returned, else a null pointer will be
				// returned.

    OTCEV_Response*	currentResponse() const;
				// If the executing member function is
				// triggered within the scope of handling a
				// response, the original response event will
				// be returned, else a null pointer will be
				// returned.

    OTCEV_Failure*	currentFailure() const;
				// If the executing member function is
				// triggered within the scope of handling a
				// failure response, the original failure
				// event will be returned, else a null
				// pointer will be returned. Note that if
				// the failure resulted from being unable
				// to decode the result from a response,
				// the <currentResponse()> member function
				// will provide access to the response event
				// at the same time as the failure event.

    bool		failureOccurred() const
				{ return currentFailure() != 0; }
				// Returns <true> if a failure response is
				// currently being processed.

    OTCEV_Opaque*	currentMessage() const;
				// If the executing member function is
				// triggered within the scope of handling a
				// message, the original message event will
				// be returned, else a null pointer will be
				// returned.

    OTCEV_Report*	currentReport() const;
				// If the executing member function is
				// triggered within the scope of handling a
				// report, the original report event will
				// be returned, else a null pointer will be
				// returned.

    // Following should only be called by the initiator of a request
    // immediately after that request has been made. Typically there should
    // be no need to actually get access to this request object, but
    // access provided anyway. Once a new request is initiated, the value
    // will be replaced.

    ONS_ActiveRequest*	lastRequest() const;
				// Returns details regarding the last request
				// which was made against a remote service.

    // = MESSAGE PRIMITIVES
    //     The following are low level primitives which deal directly with
    //     the message payload objects, the message events or setup basic
    //     subscription mechanisms. Although they can be used directly in
    //     some circumstances, you should prefereably use the higher level
    //     primitives which will automatically do argument marshalling.

    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 OTC_ROPayload const& thePayload
			) const;
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the arguments defined by
				// <thePayload>. To be consistant with the
				// high level methods which do automatic
				// marshalling, the top level of the
				// payload object should be an array. The
				// conversation ID for the request is
				// returned.

    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 OTC_RWPayload const& thePayload
			) const;
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the arguments defined by
				// <thePayload>. To be consistant with the
				// high level methods which do automatic
				// marshalling, the top level of the
				// payload object should be an array. The
				// conversation ID for the request is
				// returned.

    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 OTC_SVPayload const& thePayload
			) const;
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the arguments defined by
				// <thePayload>. To be consistant with the
				// high level methods which do automatic
				// marshalling, the top level of the
				// payload object should be an array. The
				// conversation ID for the request is
				// returned.

    void		sendResponse(
			 OTCEV_Request* theRequest,
			 OTC_ROPayload const& thePayload
			) const;
				// Sends <thePayload> as the response to
				// <theRequest>. The function will return
				// control to the caller, so if returning a
				// response to the active request, it would
				// be necessary to call <delayResponse()>
				// to avoid a second response being sent when
				// the top level request function returns.

    void		sendResponse(OTCEV_Request* theRequest) const;
				// Sends a null value as the response to
				// <theRequest>. The function will return
				// control to the caller, so if returning a
				// response to the active request, it would
				// be necessary to call <delayResponse()>
				// to avoid a second response being sent when
				// the top level request function returns.

    void		sendFailure(
			 OTCEV_Request* theRequest,
			 int theError,
			 char const* theDescription,
			 char const* theOrigin="",
			 char const* theDetails=""
			) const;
				// Sends a failure response for <theRequest>.
				// The function will return control to the
				// caller, so if returning a response to the
				// active request, it would be necessary to
				// call <delayResponse()> to avoid a second
				// response being sent when the top level
				// request function returns.

    // The following methods should only be called while there is an active
    // request being processed. If the methods are called when there is no
    // active request, a runtime exception will be raised with the program
    // most likely being shutdown as a result. Note that in calling the
    // <returnResponse()> method the callback handling the response is
    // thereby terminated with no further code after the call to the
    // <returnResponse()> method being executed.

    void		returnResponse(OTC_ROPayload const& thePayload) const;
				// Sends <thePayload> as the response to
				// the active request. The method does not
				// return control to the caller and instead
				// raises an exception of type
				// <ONS_DelayedResponse> to transfer control
				// back up to the highest level where the
				// request was original triggered.

    void		returnResponse(OTC_RWPayload const& thePayload) const;
				// Sends <thePayload> as the response to
				// the active request. The method does not
				// return control to the caller and instead
				// raises an exception of type
				// <ONS_DelayedResponse> to transfer control
				// back up to the highest level where the
				// request was original triggered.

    void		returnResponse(OTC_SVPayload const& thePayload) const;
				// Sends <thePayload> as the response to
				// the active request. The method does not
				// return control to the caller and instead
				// raises an exception of type
				// <ONS_DelayedResponse> to transfer control
				// back up to the highest level where the
				// request was original triggered.

    // The following methods are similar to <returnResponse()> except that
    // they allow you to return a response for a suspended request. If the
    // supplied conversation ID isn't valid, the methods silently return
    // without doing anything. Where the conversation ID is valid, as well
    // as sending the response, the record of the suspended request is
    // thrown away and the request will not actually be resumed.

    void		deliverResponse(
    			 OTC_String const& theConversationId,
			 OTC_ROPayload const& thePayload
			);
				// Sends <thePayload> as the response to
				// the suspended request identified by
				// <theConversationId>. Knowledge of the
				// suspended request is them discarded.

    void		deliverResponse(
    			 OTC_String const& theConversationId,
    			 OTC_RWPayload const& thePayload
			);
				// Sends <thePayload> as the response to
				// the suspended request identified by
				// <theConversationId>. Knowledge of the
				// suspended request is them discarded.

    void		deliverResponse(
    			 OTC_String const& theConversationId,
    			 OTC_SVPayload const& thePayload
			);
				// Sends <thePayload> as the response to
				// the suspended request identified by
				// <theConversationId>. Knowledge of the
				// suspended request is them discarded.

    // The purpose of the following method has largely been superseded by
    // the <suspendResponse()> and <resumeResponse()> methods.

    void                delayResponse() const;
				// Should be called by a method being
				// executed as a result of a remote request
				// if the method doesn't want to send a
				// response at this time. In order to be able
				// to send a response later on, a clone of
				// the request object would need to be cached
				// and later used to send a response or
				// failure at that time. Note that this
				// method throws an exception of type
				// <ONS_DelayedResponse>, so methods should
				// deallocate resources correctly in the face
				// of exceptions.

    // Unlike the high level methods which do automatic marshalling of the
    // result of a remote method, the following methods operate directly on
    // the response events. Other than that, the methods behave in a similar
    // way and the rules regarding their use as detailed elsewhere should
    // be adhered to.

    template<class T>
    void		processResponse(
			 void (T::*theMethod)(OTCEV_Response*) const,
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>.

    template<class T>
    void		processResponse(
			 void (T::*theMethod)(OTCEV_Response*),
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>.

    // The following complement the <processResponse()> and operate directly
    // on the failure events.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(OTCEV_Failure*) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(OTCEV_Failure*),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method.

    // When registering a callback taking the request event, the callback
    // must explicitly return or send a response using the <returnResponse()>
    // or <sendResponse()> member functions. This is because the normal
    // result of the function is ignored and no automatic marshalling of the
    // result is done. Since the result is <void> there wouldn't be a chance
    // of returning a result anyway.
 
    template<class T>
    void		exportMethod(
			 void (T::*theMethod)(OTCEV_Request*) const,
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a const member
				// function accepting a request event.

    template<class T>
    void		exportMethod(
			 void (T::*theMethod)(OTCEV_Request*),
			 char const* theMethodName
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_RequestCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  exportMethod_(theCallback,theMethodName);
				}
				// Export <theMethod> under <theMethodName>
				// where <theMethod> is a non const member
				// function accepting a request event.

    // The following is a very special case and is a bit of a hack just so
    // the interfaces are similar to the Python wrappers.

    void		exportMethod(void*, char const* theMethodName)
    				{ exportMethod_(0,theMethodName); }
				// Will remove a method previously exported
				// under <theMethodName>. Although the first
				// argument technically accepts any "void*",
				// a fixed value of <0> should preferably be
				// used and best conveys the intended purpose
				// of indicating that any prior registration
				// of an exported method is being nulled out.

    // The following primitives relate to the publication of service reports.
    // For all versions of the <publishReport()> member function if
    // <theLifeTime> is <-1> the report will be held in a cache such that if
    // another agent subscribes to the same subject at some time in the
    // future it will at that point receive the report. If some report was
    // cached for the subject already, the existing report will be destroyed
    // and replaced with the new report. If <0> is provided for
    // <theLifeTime>, any cached report will be discarded but the new report
    // will not be cached. If <theLifeTime> is some other positive value, the
    // new report will become the new cache entry but will only be valid for
    // the number of seconds determined by the argument. That is, if the
    // argument were <5>, any new subscriptions in the next <5> seconds would
    // receive the report but after that the event will be expired and
    // deleted from the cache. You should not include a newline in the
    // subject name.

    void		publishReport(
			 char const* theSubjectName,
			 OTC_ROPayload const& thePayload,
			 int theLifeTime=0
			) const
				{
				  svbroker_->publishReport(theSubjectName,
				   thePayload,theLifeTime);
				}
				// Publishes a report under <theSubjectName>
				// to any agents who have subscribed using a
				// subject pattern which matches
				// <theSubjectName>. The body of the report
				// is given by <thePayload>.

    void		publishReport(
			 char const* theSubjectName,
			 OTC_RWPayload const& thePayload,
			 int theLifeTime=0
			) const
				{
				  svbroker_->publishReport(theSubjectName,
				   thePayload,theLifeTime);
				}
				// Publishes a report under <theSubjectName>
				// to any agents who have subscribed using a
				// subject pattern which matches
				// <theSubjectName>. The body of the report
				// is given by <thePayload>.

    void		publishReport(
			 char const* theSubjectName,
			 OTC_SVPayload const& thePayload,
			 int theLifeTime=0
			) const
				{
				  svbroker_->publishReport(theSubjectName,
				   thePayload,theLifeTime);
				}
				// Publishes a report under <theSubjectName>
				// to any agents who have subscribed using a
				// subject pattern which matches
				// <theSubjectName>. The body of the report
				// is given by <thePayload>.

    // The following primitives allow sending of reports to specific service
    // agents.

    void                sendReport(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theSubjectName,
                         OTC_ROPayload const& theContent
                        )
				{
				  svbroker_->sendReport(theServiceBinding,
				   theSubjectName,theContent);
				}
				// Similar to the <publishReport()> in that
				// the service agent with <theServiceBinding>
				// will receive a report with <theSubjectName>
				// and <theContent>. The difference is that
				// this is a directed report and only goes to
				// the specified service agent. The recipient
				// of this report will not know the
				// difference between this and any other
				// published report except that it is
				// possible to send a report on a subject it
				// isn't subscribed to. Further, this
				// function bypasses the internal cache. The
				// report is not held in the cache nor does
				// it replace any existing value or cause any
				// existing value to be removed or expired.
				// The intent of this function is to allow
				// you to send an unsolicitated report to a
				// subscriber based on seeing its
				// subscription request rather than it
				// getting some value from the cache. Note
				// that the service agent with
				// <theServiceBinding> still needs to have
				// at least one subscription against this
				// service broker else nothing will be sent.
				// Also note that if the service agent has
				// overlapping subscriptions which match the
				// subject name, it will receive the report
				// twice.

    void                sendReport(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theSubjectName,
                         OTC_RWPayload const& theContent
                        )
				{
				  svbroker_->sendReport(theServiceBinding,
				   theSubjectName,theContent);
				}
				// Overloading of <sendReport()> taking
				// <OTC_RWPayload> instead of <OTC_ROPayload>.

    void                sendReport(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theSubjectName,
                         OTC_SVPayload const& theContent
                        )
				{
				  svbroker_->sendReport(theServiceBinding,
				   theSubjectName,theContent);
				}
				// Overloading of <sendReport()> taking
				// <OTC_SVPayload> instead of <OTC_ROPayload>.

    // The following are low level primitives used by <monitorReports()>
    // and <ignoreReports()>. You shouldn't use the low level primitives
    // unless you want to take full control and management of dealing
    // with the reports that are received by overriding the <handleReport()>
    // member function.

    void                subscribeReports(
                         char const* theServiceName,
                         char const* theSubjectPattern
                        )
				{
				  svbroker_->subscribeReports(
				   theServiceName,theSubjectPattern);
				}
				// Subscribes the service agent to any
				// reports published by any service agents
				// with <theServiceName> where the subject of
				// the report matches <theSubjectPattern>.

    void                unsubscribeReports(
                         char const* theServiceName,
                         char const* theSubjectPattern
                        )
				{
				  svbroker_->unsubscribeReports(
				   theServiceName,theSubjectPattern);
				}
				// Unsubscribes the service agent from any
				// reports published by any service agents
				// with <theServiceName> where the subject of
				// the report matches <theSubjectPattern>.
				// <theSubjectPattern> must match the pattern
                                // that was used when the subscription was
                                // made.

    void                unsubscribeReports(char const* theServiceName)
				{
				  svbroker_->unsubscribeReports(
				   theServiceName);
				}
                                // Unsubscribes the agent from any reports
                                // published by any service agents with
                                // <theServiceName>.

    void                subscribeReports(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theSubjectPattern
                        )
				{
				  svbroker_->subscribeReports(
				   theServiceBinding,theSubjectPattern);
				}
				// Subscribes the service agent to any
				// reports published by <theServiceBinding>
				// where the subject of the report matches
				// <theSubjectPattern>.

    void                unsubscribeReports(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theSubjectPattern
                        )
				{
				  svbroker_->unsubscribeReports(
				   theServiceBinding,theSubjectPattern);
				}
				// Unsubscribes the service agent from any
				// reports published by <theServiceBinding>
				// where the subject of the report matches
				// <theSubjectPattern>. <theSubjectPattern>
				// must match the pattern that was used when
				// the subscription was made.

    void                unsubscribeReports(
			 OTC_ServiceBinding* theServiceBinding
			)
				{
				  svbroker_->unsubscribeReports(
				   theServiceBinding);
				}
				// Unsubscribes the service agent from any
				// reports published by <theServiceBinding>.

    // Unlike the high level methods which do automatic marshalling of the
    // contents of a report, the following methods operate directly on
    // the report events. Other than that, the methods behave in a similar
    // way and the rules regarding their use as detailed elsewhere should
    // be adhered to.

    template<class T>
    void		monitorReports(
			 void (T::*theMethod)(OTCEV_Report*) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T>
    void		monitorReports(
			 void (T::*theMethod)(OTCEV_Report*),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T>
    void		monitorReports(
			 void (T::*theMethod)(OTCEV_Report*) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    template<class T>
    void		monitorReports(
			 void (T::*theMethod)(OTCEV_Report*),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    template<class T>
    void		acceptReports(
			 void (T::*theMethod)(OTCEV_Report*) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received are
				// received as an unsolitated report from any
				// service agents with <theServiceName>.

    template<class T>
    void		acceptReports(
			 void (T::*theMethod)(OTCEV_Report*),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received are
				// received as an unsolitated report from any
				// service agents with <theServiceName>.

    template<class T>
    void		acceptReports(
			 void (T::*theMethod)(OTCEV_Report*) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theCallback> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received are
				// received as an unsolitated report from the
				// service agent with <theServiceBinding>.

    template<class T>
    void		acceptReports(
			 void (T::*theMethod)(OTCEV_Report*),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theCallback> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received are
				// received as an unsolitated report from the
				// service agent with <theServiceBinding>.

    // = SENDING REQUESTS
    //     The <sendRequest()> method is the means by which a method on a
    //     remote service agent is executed. The conversation ID returned
    //     should be captured if the result from the remote method or any
    //     failure response needs to be processed.

    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName
			) const;
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service where the arguments will be a
				// null array, representing that no
				// arguments are actually being supplied.
				// The conversation ID for the request is
				// returned.

    // Following are template versions of <sendRequest()> accepting up
    // to a maximum of 9 arguments. The result returned is always the
    // conversation ID of the initiated request.

    template<class A1>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 theArg1
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied argument.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied two arguments.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2, class A3>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied three arguments.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2, class A3, class A4>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3, A4 const& theArg4
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  thePayload[3] <<= theArg4;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied four arguments.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2, class A3, class A4, class A5>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3, A4 const& theArg4,
			 A5 const& theArg5
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  thePayload[3] <<= theArg4;
				  thePayload[4] <<= theArg5;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied five arguments.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2, class A3, class A4, class A5, class A6>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3, A4 const& theArg4,
			 A5 const& theArg5, A6 const& theArg6
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  thePayload[3] <<= theArg4;
				  thePayload[4] <<= theArg5;
				  thePayload[5] <<= theArg6;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied six arguments.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2, class A3, class A4, class A5,
     class A6, class A7>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3, A4 const& theArg4,
			 A5 const& theArg5, A6 const& theArg6,
			 A7 const& theArg7
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  thePayload[3] <<= theArg4;
				  thePayload[4] <<= theArg5;
				  thePayload[5] <<= theArg6;
				  thePayload[6] <<= theArg7;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied seven arguments.
				// The conversation ID for the request is
				// returned.

    template<class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3, A4 const& theArg4,
			 A5 const& theArg5, A6 const& theArg6,
			 A7 const& theArg7, A8 const& theArg8
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  thePayload[3] <<= theArg4;
				  thePayload[4] <<= theArg5;
				  thePayload[5] <<= theArg6;
				  thePayload[6] <<= theArg7;
				  thePayload[7] <<= theArg8;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied eight arguments.
				// The conversation ID for the request is
				// returned.


    template<class A1, class A2, class A3, class A4, class A5,
     class A6, class A7, class A8, class A9>
    OTC_String		sendRequest(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theMethodName,
			 A1 const& theArg1, A2 const& theArg2,
			 A3 const& theArg3, A4 const& theArg4,
			 A5 const& theArg5, A6 const& theArg6,
			 A7 const& theArg7, A8 const& theArg8,
			 A9 const& theArg9
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload[0] <<= theArg1;
				  thePayload[1] <<= theArg2;
				  thePayload[2] <<= theArg3;
				  thePayload[3] <<= theArg4;
				  thePayload[4] <<= theArg5;
				  thePayload[5] <<= theArg6;
				  thePayload[6] <<= theArg7;
				  thePayload[7] <<= theArg8;
				  thePayload[8] <<= theArg9;
				  return sendRequest(theServiceBinding,
				   theMethodName,thePayload);
				}
				// Sends a request to the remote service
				// identified by <theServiceBinding>,
				// invoking <theMethodName> on the remote
				// service with the supplied nine arguments.
				// The conversation ID for the request is
				// returned.

    // = RESPONSE HANDLING
    //     Following methods are used by a client initiating a request to
    //     indicate interest in the result or any failure which might have
    //     occurred. The conversation ID returned from <sendRequest()> is
    //     used to bind a specific request to the callbacks registered to
    //     handle any response.
    //
    //     Note that the <processResponse()> and <processFailure() methods
    //     must be called immediately after the corresponding <sendRequest()>
    //     invokation and specifically before any subsequent call to the
    //     <sendRequest()> method. This is necessary as information pertinent
    //     to the call is cached internally and that information will be lost
    //     on the next call.

    template<class T>
    void		processResponse(
			 void (T::*theMethod)() const,
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// remote method must not have returned a
				// result, ie., had a <void> return type.
				// Registering of a callback is merely used
				// to determine if the remote method actually
				// executed.

    template<class T>
    void		processResponse(
			 void (T::*theMethod)(),
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// remote method must not have returned a
				// result, ie., had a <void> return type.
				// Registering of a callback is merely used
				// to determine if the remote method actually
				// executed.

    template<class T, class RT>
    void		processResponse(
			 void (T::*theMethod)(RT) const,
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// callback should accept a single argument
				// of type which the result of the remote
				// method can be converted.

    template<class T, class RT>
    void		processResponse(
			 void (T::*theMethod)(RT),
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// callback should accept a single argument
				// of type which the result of the remote
				// method can be converted.

    template<class T, class RT>
    void		processResponse(
			 void (T::*theMethod)(char const*,RT) const,
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// callback should accept two arguments
				// where the first will be supplied with the
				// conversation ID of the original request
				// and a second argument of type which the
				// result of the remote method can be
				// converted.

    template<class T, class RT>
    void		processResponse(
			 void (T::*theMethod)(char const*,RT),
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// callback should accept two arguments
				// where the first will be supplied with the
				// conversation ID of the original request
				// and a second argument of type which the
				// result of the remote method can be
				// converted.

    template<class T, class RT>
    void		processResponse(
			 void (T::*theMethod)(OTC_String const&,RT) const,
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// callback should accept two arguments
				// where the first will be supplied with the
				// conversation ID of the original request
				// and a second argument of type which the
				// result of the remote method can be
				// converted.

    template<class T, class RT>
    void		processResponse(
			 void (T::*theMethod)(OTC_String const&,RT),
			 OTC_String const& theConversationId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ResponseCallbackFactory<THIS,
                                   T>::create(derivedObject<THIS,
                                   T>(this),theMethod);
				  processResponse_(theCallback,
				   theConversationId);
				}
				// Registers that <theMethod> should be
				// called when a response is received for the
				// request with <theConversationId>. The
				// callback should accept two arguments
				// where the first will be supplied with the
				// conversation ID of the original request
				// and a second argument of type which the
				// result of the remote method can be
				// converted.

    // In addition to registering a callback to handle a valid response
    // from a remote method, a distinct callback can be registered to
    // handle any failure response. When registering a failure callback,
    // a response timeout can optionally be specificed. If supplied,
    // the response timeout indicates after how long a timeout failure
    // will be triggered if a normal response or other failure response
    // hasn't otherwise been received. The default for the response timeout
    // is <0.0> which indicates no timeout at all. The response timeout
    // is expressed in seconds.
    //
    // In using the <processFailure()> method, an automatic subscription is
    // created on the existance of the service to which the remote request
    // has been sent. If the remote service doesn't actually exist or is
    // withdrawn before a response of any type is received, a failure
    // response is triggered to notify of this occurring.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)() const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(char const*) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take a single argument which will be
				// passed the conversation ID of the original
				// request.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(char const*),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take a single argument which will be
				// passed the conversation ID of the original
				// request.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(OTC_String const&) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take a single argument which will be
				// passed the conversation ID of the original
				// request.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(OTC_String const&),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take a single argument which will be
				// passed the conversation ID of the original
				// request.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(int,char const*,
			  char const*,char const*) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take four arguments which will be the
				// error, the description of the error, the
				// origin of the error and any additional
				// details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(int,char const*,
			  char const*,char const*),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take four arguments which will be the
				// error, the description of the error, the
				// origin of the error and any additional
				// details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(int,OTC_String const&,
			  OTC_String const&,OTC_String const&) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take four arguments which will be the
				// error, the description of the error, the
				// origin of the error and any additional
				// details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(int,OTC_String const&,
			  OTC_String const&,OTC_String const&),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take four arguments which will be the
				// error, the description of the error, the
				// origin of the error and any additional
				// details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(char const*,int,
			  char const*,char const*,char const*) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take five arguments which will be
				// passed the conversation ID of the original
				// request, the error, the description of the
				// error, the origin of the error and any
				// additional details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(char const*,int,
			  char const*,char const*,char const*),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take five arguments which will be
				// passed the conversation ID of the original
				// request, the error, the description of the
				// error, the origin of the error and any
				// additional details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(OTC_String const&,int,
			  OTC_String const&,OTC_String const&,
			  OTC_String const&) const,
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take five arguments which will be
				// passed the conversation ID of the original
				// request, the error, the description of the
				// error, the origin of the error and any
				// additional details.

    template<class T>
    void		processFailure(
			 void (T::*theMethod)(OTC_String const&,int,
			  OTC_String const&,OTC_String const&,
			  OTC_String const&),
			 OTC_String const& theConversationId,
			 double theResponseTimeout=0.0
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_FailureCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processFailure_(theCallback,
				   theConversationId,theResponseTimeout);
				}
				// Registers that <theMethod> should be
				// called when a failure response is received
				// for the request with <theConversationId>.
				// A timeout failure will be triggered and
				// <theMethod> called after a delay in seconds
				// corresponding to <theResponseTimeout>
				// if a normal response or failure response
				// is not otherwise received from the remote
				// method. <theMethod> in this instance should
				// take five arguments which will be
				// passed the conversation ID of the original
				// request, the error, the description of the
				// error, the origin of the error and any
				// additional details.

    // The following pertain to pending registrations of interest in the
    // result or failure of a remote method.

    void		ignoreResponse(OTC_String const& theConversationId);
				// Cancels any previous interest in having
				// methods called in response to the result
				// or failure of the remote request with
				// <theConversationId>.

    bool		responsePending(
    			 OTC_String const& theConversationId
			) const;
				// Returns <true> if there is a registration
				// of interest in a normal response or
				// failure response from a remote request
				// with <theConversationId> and none has yet
				// been received. Returns the value <false>
				// otherwise.

    // = COMPLETING RESPONSES
    //     Under normal circumstances the value returned from an exported
    //     method is what is returned to the remote client which initiated
    //     the request. The following methods provide a means of explicity
    //     returning a failure response as well as returning a specific
    //     response other than through the return value of the exported
    //     method. The first set of methods are only usable within the scope
    //     of the request you want to send a response to.

    void                abortResponse(
                         int theError,
                         char const* theDescription,
                         char const* theOrigin="",
                         char const* theDetails=""      
                        ) const;
				// Should be called by a method being
				// executed as a result of a remote request
				// if a failure notification needs to be
				// returned. Note that this method throws an
				// exception of type <ONS_ServiceFailure>, so
				// methods should deallocate resources
				// correctly in the face of these exceptions.
				// An alternative to using this method is
				// to explicitly throw an exception of type
				// <ONS_ServiceFailure> instead.

    void		returnResponse() const;
				// Sends a null value as the response to
				// the active request. The method does not
				// return any control to the caller and
				// instead raises an exception of type
				// <ONS_DelayedResponse> to transfer control
				// back up to the highest level where the
				// request was original triggered.

    template<class RT>
    void		returnResponse(RT theResult) const
				{
				  OTC_SVPayload thePayload;
				  thePayload <<= theResult;
				  returnResponse(thePayload);
				}
				// Sends <theResult> as the response to
				// the active request. The method does not
				// return any control to the caller and
				// instead raises an exception of type
				// <ONS_DelayedResponse> to transfer control
				// back up to the highest level where the
				// request was original triggered. As this
				// method is a template member function, the
				// result will be automatically marshalled if
				// mechanism for doing this is available.

    // The following methods take an additional first argument which is
    // the conversation ID of the suspended request for which you want to
    // send a response. Once the response has been sent, the record of the
    // suspended request is automatically discarded. Note that the equivalent
    // for the <abortResponse()> method when referring to a suspended request
    // is the <cancelResponse()> method.

    void		deliverResponse(OTC_String const& theConversationId);
				// Sends a null value as the response to the
				// suspended request with <theConversationId>.
				// Knowledge of the suspended request is them
				// discarded.

    template<class RT>
    void		deliverResponse(
    			 OTC_String const& theConversationId,
			 RT theResult
			)
				{
				  OTC_SVPayload thePayload;
				  thePayload <<= theResult;
				  deliverResponse(theConversationId,
				   thePayload);
				}
				// Sends <theResult> as the response to the
				// suspended request with <theConversationId>.
				// Knowledge of the suspended request is them
				// discarded.

    // = DELAYING RESPONSES
    //     The <suspendResponse()> and <resumeResponse()> methods are used
    //     when an exported method is not able to return a response straight
    //     away but must wait on some other event to be able to form a
    //     response. This will occur when an exported method needs to make
    //     one or more downstream requests to other remote service agents to
    //     accumulate data for a response. Alternatively it may need to wait
    //     upon data being received via a socket or some other form of
    //     interprocess communication. When a response is suspended, the
    //     conversation ID of the suspended response should be used to
    //     track any state associated with the pending response. The value
    //     returned by the registered callback when suspending processing
    //     of a request will become the value returned to the client unless
    //     it is further suspended, a failure response returned or an
    //     explicit return value is provided using <returnResponse()>.
    //
    //     The <suspendResponse()> method should only be called within the
    //     context of the execution of an exported method when servicing a
    //     remote request delivered to the service agent.

    template<class T>
    void		suspendResponse(void (T::*theMethod)() const)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request.

    template<class T>
    void		suspendResponse(void (T::*theMethod)())
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request.

    template<class T, class RT>
    void		suspendResponse(RT (T::*theMethod)() const)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request.

    template<class T, class RT>
    void		suspendResponse(RT (T::*theMethod)())
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request.

    template<class T>
    void		suspendResponse(
			 void (T::*theMethod)(char const*) const
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T>
    void		suspendResponse(
			 void (T::*theMethod)(char const*)
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T, class RT>
    void		suspendResponse(
			 RT (T::*theMethod)(char const*) const
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T, class RT>
    void		suspendResponse(
			 RT (T::*theMethod)(char const*)
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T>
    void		suspendResponse(
			 void (T::*theMethod)(OTC_String const&) const
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T>
    void		suspendResponse(
			 void (T::*theMethod)(OTC_String const&)
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T, class RT>
    void		suspendResponse(
			 RT (T::*theMethod)(OTC_String const&) const
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    template<class T, class RT>
    void		suspendResponse(
			 RT (T::*theMethod)(OTC_String const&)
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// <theMethod> is registered as a callback
				// to be executed when the processing of the
				// request is later resumed. The request is
				// resumed by calling <resumeResponse()> with
				// the conversation ID of the request. In
				// this case <theMethod> will be passed a
				// single argument corresponding to the
				// conversation ID of the request.

    void		suspendResponse()
				{
				  ONS_EventCallback* theCallback =
				   ONS_DelayedCallbackFactory<THIS,
				   ONS_Service>::create(this,
				   &ONS_Service::voidResponse_);
				  suspendResponse_(theCallback);
				}
				// Delays the returning of a response for the
				// request which is currently being handled.
				// A dummy method returning void is
				// registered as a callback to be executed
				// when the processing of the request is
				// later resumed. The request is resumed by
				// calling <resumeResponse()> with the
				// conversation ID of the request.

    void		resumeResponse(OTC_String const& theConversationId);
				// Triggers the resumption of the suspended
				// request with <theConversationId>.

    // = ABANDONING RESPONSE
    //     If you want to be able to send a failure response back for the
    //     suspended response, use <cancelResponse()> instead.

    void		abandonResponse(
			 OTC_String const& theConversationId
			);
				// Abandons the suspended response with
				// <theConversationId>. No failure response
				// of any sort is returned.

    // = CANCELLING RESPONSES
    //     The <abortResponse()> member function is used when the actual
    //     code processing a request wishes to return a failure response. If
    //     it is necessary to forcibly send a failure response for a
    //     suspended request from a separate callback then <cancelResponse()>
    //     or <cancelAllResponses()> should be used. This would be used for
    //     example when a service is being shutdown and any pending responses
    //     need to be aborted.

    void		cancelResponse(
			 OTC_String const& theConversationId,
			 int theError,
			 char const* theDescription,
			 char const* theOrigin="",
			 char const* theDetails=""
			);
				// Forces a failure response to be delivered
				// back to the client initiating the remote
				// request with <theConverstionId>. The
				// failure response will have <theError> and
				// <theDescription> and be marked as coming
				// from <theOrigin>. <theDetails> should
				// contain any additional information
				// relevant to the generated error.

    void		cancelAllResponses(
			 int theError,
			 char const* theDescription,
			 char const* theOrigin="",
			 char const* theDetails=""
			);
				// Forces a failure response to be delivered
				// back to the client initiating the remote
				// request with <theConverstionId>. The
				// failure response will have <theError> and
				// <theDescription> and be marked as coming
				// from <theOrigin>. <theDetails> should
				// contain any additional information
				// relevant to the generated error.

    // = CANCELLING REQUESTS
    //     After having initiated a request, a client can if desired force a
    //     failure response for that same request which overrides any normal
    //     response or failure response which might be received. Such a
    //     response will be notified through the failure callback which had
    //     already been registered.

    void		cancelRequest(
			 OTC_String const& theConversationId,
			 int theError,
			 char const* theDescription,
			 char const* theOrigin="",
			 char const* theDetails=""
			);
				// Forces a failure response to be delivered
				// back to this service agent for the pending
				// remote request with <theConverstionId>.
				// The failure response will have <theError>
				// and <theDescription> and be marked as
				// coming from <theOrigin>. <theDetails>
				// should contain any additional information
				// relevant to the generated error.

    void		cancelAllRequests(
			 int theError,
			 char const* theDescription,
			 char const* theOrigin="",
			 char const* theDetails=""
			);
				// Forces a failure response to be delivered
				// back to this service agent for all pending
				// remote requests. The failure response will
				// have <theError> and <theDescription> and
				// be marked as coming from <theOrigin>.
				// <theDetails> should contain any additional
				// information relevant to the generated
				// error.

    // = OPAQUE MESSAGES
    //     The following methods are specifically provided for sending opaque
    //     data messages to the target service agent. This is useful where it  
    //     is an inconvenience or inefficient to translate data into an
    //     abstract data model for conversion into the underlying XML based
    //     message format. When sent in this way, the data message will be
    //     sent as is and not converted in anyway while being sent over the
    //     messaging system. Note that since multiple messages with the same
    //     message ID but with differing sequence number can arrive from a
    //     single sender, it can't be known when to discard a callback. A
    //     callback must therefore call <discardMessages()> to drop the
    //     callback registration. User code should also possibly implement a
    //     timeout which when triggered also calls <discardMessages()> to
    //     cope with the possibility that a sender never sends the expected
    //     message.

    OTC_String		sendMessage(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theMessageId,
                         char const* theDescription,
                         OTC_String const& theContent,
                         char const* theContentType=0,
                         int theSequenceNumber=0
                        ) const;
				// Sends an opaque data message containing
				// <theContent> and with <theContentType>,
				// <theMessageId> and <theDescription> to the
				// service agent with <theServiceBinding>.
				// The conversation ID associated with the
				// request is returned. You should not
				// include a new line in <theMessageId> or
				// <theDescription>. <theSequenceNumber> can
				// be used to indentify the message as one of
				// a ordered batch of related messages.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(char const*,OTC_String const&,
			  char const*) const,
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(char const*,OTC_String const&,
			  char const*),
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(char const*,OTC_String const&,
			  char const*, int) const,
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(char const*,OTC_String const&,
			  char const*, int),
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(char const*,char const*,
			  char const*,OTC_String const&,char const*,int) const,
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(char const*,char const*,
			  char const*,OTC_String const&,char const*,int),
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,OTC_String const&) const,
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,OTC_String const&),
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,OTC_String const&, int) const,
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,OTC_String const&, int),
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,OTC_String const&,
			  OTC_String const&,OTC_String const&,int) const,
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    template<class T>
    void		processMessages(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,OTC_String const&,
			  OTC_String const&,OTC_String const&,int),
			 char const* theSender,
			 char const* theMessageId
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_OpaqueCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  processMessages_(theCallback,theSender,
				   theMessageId);
				}
				// Registers <theMethod> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    void		discardMessages(
			 char const* theSender,
			 char const* theMessageId=0
			);
				// Indicates there is no longer an interest
				// in messages originating from <theSender>
				// with <theMessageId>. If <theMessageId>
				// is <0>, then any messages from <theSender>
				// will be ignored.

    virtual void	unexpectedMessage(
			 OTC_String const& theSender,
			 OTC_String const& theMessageId,
			 OTC_String const& theDescription,
			 OTC_String const& theContent,
			 OTC_String const& theContentType,
			 int theSequenceNumber
			);
				// Called when a message is received for
				// which there is no registered callback.
				// Can be overridden in a derived class
				// in order to deal with such messages.

    // = REPORT PUBLICATION

    template<class T>
    void		publishReport(
			 char const* theSubjectName,
			 T const& theObject,
			 int theLifeTime=0
			) const
				{
				  OTC_SVPayload thePayload;
				  thePayload <<= theObject;
				  publishReport(theSubjectName,
				   thePayload,theLifeTime);
				}
				// Publishes a report under <theSubjectName>
				// to any agents who have subscribed using a
				// subject pattern which matches
				// <theSubjectName>. The body of the report
				// is given by <theObject>. If <theLifeTime>
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
				// for the number of seconds determined by
				// the argument. That is, if the argument
				// were <5>, any new subscriptions in the
				// next <5> seconds would receive the report
				// but after that the event will be expired
				// and deleted from the cache. You should not
				// include a newline in the subject name.

    // The following provides a means of sending a report to specific
    // service agent out of the set of service agents with subscriptions
    // on this service agent. One would normally use it by overloading
    // the <handleSubscription()> method and using it to send reports
    // constructed on the spot at the point of the subscription, rather
    // than a stale entry in the report cache being returned.

    template<class T>
    void                sendReport(
                         OTC_ServiceBinding* theServiceBinding,
                         char const* theSubjectName,
                         T const& theObject
                        )
				{
				  OTC_SVPayload thePayload;
				  thePayload <<= theObject;
				  sendReport(theServiceBinding,
				   theSubjectName,thePayload);
				}
				// Similar to the <publishReport()> in that
				// the service agent with <theServiceBinding>
				// will receive a report with <theSubjectName>
				// and <theObject> as content. The difference
				// is that this is a directed report and only
				// goes to the specified service agent. The
				// recipient of this report will not know the
				// difference between this and any other
				// published report except that it is
				// possible to send a report on a subject it
				// isn't subscribed to. Further, this
				// function bypasses the internal cache. The
				// report is not held in the cache nor does
				// it replace any existing value or cause any
				// existing value to be removed or expired.
				// The intent of this function is to allow
				// you to send an unsolicitated report to a
				// subscriber based on seeing its
				// subscription request rather than it
				// getting some value from the cache. Note
				// that the service agent with
				// <theServiceBinding> still needs to have
				// at least one subscription against this
				// service broker else nothing will be sent.
				// Also note that if the service agent has
				// overlapping subscriptions which match the
				// subject name, it will receive the report
				// twice.

    // = REPORT MANAGEMENT

    bool		matchSubject(
                         char const* theSubjectPattern,
                         char const* theSubjectName
			) const
				{
				  return svbroker_->matchSubject(
				   theSubjectPattern,theSubjectName);
				}
                                // If <theSubjectPattern> matches   
				// <theSubjectName> returns <true>. The
				// pattern matching algorithm which applies
				// is that which is used when the service is
				// acting as a publisher of reports.

    void                expireReports(
			 char const* theSubjectPattern,
			 u_int theAge
			)
				{
				  svbroker_->expireReports(
				   theSubjectPattern,theAge);
				}
				// Forcibly expires any cached reports with
				// subject name matching <theSubjectPattern>.
				// The reports will be expired and removed
				// from the cache if it is older than
				// <theAge> seconds.

    void                expireReports(u_int theAge)
				{ svbroker_->expireReports(theAge); }
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

    void                purgeReports()
				{ svbroker_->purgeReports(); }
                                // Removes from the cache any expired reports.

    // = REPORT SUBSCRIPTIONS
    //     When a remote service agent is being subscribed to, the remote
    //     service agent will receive notification of the subscription or
    //     unsubscription by way of an instance of the <OTCEV_Subscription>
    //     event class which in this implementation triggers a call to
    //     <handleSubscription()>. Note though that if there was a cached
    //     entry against a matching subject, that cached entry will have
    //     already been sent by the time the publisher gets the subscription
    //     notification.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,char const*,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,char const*,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,char const*,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(char const*,char const*,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		monitorReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  monitorReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    // The following don't set up an active subscription against a remote
    // service agent and the callbacks registered using them are only
    // called for unsolicitated reports which don't match an active
    // subscription pattern.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,char const*,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,char const*,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT) const,
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT),
			 char const* theServiceName,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceName,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,char const*,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(char const*,char const*,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT) const,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
                                // Registers <theMethod> to be called when
                                // reports with subject name matching
                                // <theSubjectPattern> are received from the
                                // service agent with <theServiceBinding>.

    template<class T, class RT>
    void		acceptReports(
			 void (T::*theMethod)(OTC_String const&,
			  OTC_String const&,RT),
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			)
				{
				  ONS_EventCallback* theCallback =
				   ONS_ReportCallbackFactory<THIS,
				   T>::create(derivedObject<THIS,
				   T>(this),theMethod);
				  acceptReports_(theCallback,
				   theServiceBinding,theSubjectPattern);
				}
				// Registers <theMethod> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    // The following will deregister both active and passive subscription
    // callbacks and cause the service agent to be unsubscribed from any
    // remote service agent as appropriate.

    void		ignoreReports(
			 char const* theServiceName,
			 char const* theSubjectPattern=0
			);
				// Cancels the subscription against any
				// remote service agents with <theServiceName>
				// for reports published being published with
				// <theSubjectPattern>. If <0> is passed
				// for <theSubjectPattern>, all subscriptions
				// for the remote service agents with
				// <theServiceName> are cancelled.

    void		ignoreReports(
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern=0
			);
				// Cancels the subscription against the
				// remote service agent with
				// <theServiceBinding> for reports published
				// with <theSubjectPattern>. If <0> is passed
				// for <theSubjectPattern>, all subscriptions
				// for the remote service agent with
				// <theServiceBinding> is cancelled.

    // Following can be overridden in a derived class to handle any reports
    // which don't seem to match any subscription at all.

    virtual void	unexpectedReport(
			 OTC_String const& theServiceName,
			 OTC_String const& theSubjectName,
			 OTC_ROPayload const& theContent
			);
				// Called where there is no active or passive
				// subscription matching the report from
				// <theServiceName> and with <theSubjectName>.
				// The content of the report is supplied as
				// a raw payload object in <theContent>.

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
    				{ ONS_Monitor::handle(theEvent); }
				// Destroys <theEvent>. Should be overridden
				// in a derived class to handle any event
				// not associated with a callback.

    // = CALLBACK TRIGGERS

    virtual void	handleRequest(OTCEV_Request* theRequest);
				// Called when <theRequest> event is
				// received. If a callback is registered with
				// the same name as the request method, it
				// will be called.

    virtual void	handleResponse(OTCEV_Response* theResponse);
				// Called when <theResponse> event is
				// received.

    virtual void	handleFailure(OTCEV_Failure* theFailure);
				// Called when <theFailure> event is
				// received.

    virtual void	handleMessage(OTCEV_Opaque* theMessage);
				// Called when <theMessage> event is
				// received.

    virtual void	handleReport(OTCEV_Report* theReport);
				// Called when <theReport> event is
				// received.

    // The following have no default implementation and can be overridden
    // in a derived class in order to process the event.

    virtual void	handleSubscription(
			 OTCEV_Subscription* theSubscription
			);
				// Called when <theSubscription> event is
				// received. This event notifies when a
				// remote service has made a subscription
				// against this service.

    virtual void	handlePublisherNotification(
			 OTCEV_PublisherNotification* theNotification
			);
				// Called when <theNotification> event is
				// received. This event notifies when a
				// subscription against a remote service
				// has been accepted.

    // = METHOD EXECUTION

    virtual void	executeMethod(
			 ONS_EventCallback* theCallback,
			 OTCEV_Request* theRequest,
			 OTC_SVPayload& theResult
			);
				// Executes <theCallback> with <theRequest>.
				// The callback will will out <theResult>.
				// May be overridden in a derived class to
				// catch additional exceptions raised by
				// exported methods and send specific failure
				// responses. May also be overridden as a
				// means of logging requests.

    virtual void	invalidMethod(OTCEV_Request* theRequest);
				// Called when a request with the appropriate
				// method name cannot be found. <theRequest>
				// contains the details of the request. By
				// default raised a service failure exception
				// which results in failure response
				// indicating the method is not available.
				// May be overridden in a derived class of
				// logging of such failed requests needs to
				// be done. The derived class method should
				// still call this method as the last thing
				// it does.

  private:

    // = CALLBACK REGISTRATION

    void		exportMethod_(
			 ONS_EventCallback* theCallback,
			 char const* theMethodName
			);
				// Sets up the exported method with
				// <theMethodName> which when called maps to
				// <theCallback>.

    void		processResponse_(
			 ONS_EventCallback* theCallback,
			 OTC_String const& theConversationId
			);
				// Registers <theCallback> to be called when
				// a normal response is received for the
				// request with <theConversationId>.

    void		processFailure_(
			 ONS_EventCallback* theCallback,
			 OTC_String const& theConversationId,
			 double theRequestTimeout
			);
				// Registers <theCallback> to be called when
				// a failure response is received for the
				// request with <theConversationId>. If
				// <theRequestTimeout> is greater than zero,
				// a timeout failure will be generated for
				// the request if a normal response or failure
				// response isn't received in that number of
				// seconds. A failure response is also
				// automatically generated if the service
				// agent which is the target of the request
				// is withdrawn before any response is
				// received.

    void		suspendResponse_(
			 ONS_EventCallback* theCallback
			) const;
				// Suspends the method being executed in
				// response to a remote request and registers
				// <theCallback> to be called when the
				// handling of the request is resumed.

    void		processMessages_(
			 ONS_EventCallback* theCallback,
			 char const* theSender,
			 char const* theMessageId
			);
				// Registers <theCallback> to be called when
				// messages with <theMessageId> are received
				// from <theSender>.

    void		monitorReports_(
			 ONS_EventCallback* theCallback,
			 char const* theServiceName,
			 char const* theSubjectPattern
			);
				// Registers <theCallback> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from any
				// service agents with <theServiceName>.

    void		monitorReports_(
			 ONS_EventCallback* theCallback,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			);
				// Registers <theCallback> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received from the
				// service agent with <theServiceBinding>.

    void		acceptReports_(
			 ONS_EventCallback* theCallback,
			 char const* theServiceName,
			 char const* theSubjectPattern
			);
				// Registers <theCallback> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received are
				// received as an unsolitated report from any
				// service agents with <theServiceName>.

    void		acceptReports_(
			 ONS_EventCallback* theCallback,
			 OTC_ServiceBinding* theServiceBinding,
			 char const* theSubjectPattern
			);
				// Registers <theCallback> to be called when
				// reports with subject name matching
				// <theSubjectPattern> are received are
				// received as an unsolitated report from the
				// service agent with <theServiceBinding>.

    // = RESPONSE FAILURE

    void		handleResponseTimeout_(
			 OTC_String const& theTimer
			);
				// Called when a failure callback has been
				// registered for a request and a response
				// hasn't been received within the time
				// specified by the response timeout.
				// <theTimer> identifies which conversation
				// has timed out. The failure callback which
				// was registered for the request will be
				// called.

    void		handleServiceWithdrawn_(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus,
			 OTC_String const& theSubscription
			);
				// Called when a failure callback has been
				// registered for a request and the remote
				// service is withdrawn before a response
				// is received. <theSubscription> is used
				// to identify the conversation and the
				// failure callback is called. This will
				// only occur though when <theGroup> is
				// empty and <theStatus> indicates the
				// service has been withdrawn.

    void		handleServiceUnavailable_(
			 OTC_String const& theAction
			);
				// Called when a failure callback has been
				// registered for a request and the remote
				// service didn't exist at the point the
				// request was being made. <theAction> is
				// used to identify the conversation and the
				// failure callback is called.

    // = CALLBACK TRIGGERS
    //     The following exist so that the corresponding callbacks can be
    //     executed in the context of a separate event, rather than being
    //     nested within the processing of some other callback.

    void		handleRequestCancellation_(
			 OTC_String const& theAction
			);
				// Triggers cancellation of the request
				// associated with <theAction>.

    void		handleResponseResumption_(
			 OTC_String const& theAction
			);
				// Triggers resumption of the request
				// associated with <theAction>.

  private:

			ONS_Service(ONS_Service const&);
				// Do not define an implementation for this.

    ONS_Service&	operator=(ONS_Service const&);
				// Do not define an implementation for this.

    void		voidResponse_() {}
    				// Dummy method doing nothing. Used as
				// response callback when request suspended
				// with no explicit callback function.

    void		lastRequest_(ONS_ActiveRequest* theRequest) const;
    				// Save <theRequest> against the current
				// context, where <theRequest> is the details
				// of the last outgoing request made.

    void		currentRequest_(OTCEV_Request* theRequest) const;
    				// Save <theRequest> against the current
				// context, where <theRequest> is the details
				// of the current incoming request being
				// handled.

    void		currentResponse_(OTCEV_Response* theResponse) const;
    				// Save <theResponse> against the current
				// context, where <theResponse> is the details
				// of the current incoming response being
				// handled.

    void		currentFailure_(OTCEV_Failure* theFailure) const;
    				// Save <theFailure> against the current
				// context, where <theFailure> is the details
				// of the current incoming failure being
				// handled.

    void		currentMessage_(OTCEV_Opaque* theMessage) const;
    				// Save <theMessage> against the current
				// context, where <theMessage> is the details
				// of the current incoming out of band
				// message being handled.

    void		currentReport_(OTCEV_Report* theReport) const;
    				// Save <theReport> against the current
				// context, where <theReport> is the details
				// of the current incoming report being
				// handled.

    OTC_NRMutex		mutex_;
				// Mutex to protect thread access to main
				// callback data structures.

    OTC_SVBroker*	svbroker_;
                                // Messaging system service agent broker.

    OTC_HMap<OTC_String,ONS_EventCallback*>	exportedMethods_;
				// Exported methods for this service agent.

    OTC_HMap<OTC_String,ONS_EventCallback*>	serviceCallbacks_;
				// Mapping from the subscription key to the
				// registered callback for that action.

    OTC_HMap<OTC_String,ONS_ActiveRequest*>	activeRequests_;
				// Table of active requests which are being
				// monitored for a response.

    OTC_HMap<OTC_String,ONS_PendingCancellation*>	pendingCancellations_;
				// Table of pending request cancellations.

    OTC_HMap<OTC_String,ONS_PendingResponse*>	suspendedResponses_;
				// Table of suspended responses.

    OTC_NRMutex		acmutex_;
				// Mutex to protect thread access on the
				// agent context.

    ONS_ServiceContext*	agentContext_;
    				// Cache containing details of last request
				// which was made or details of event which
				// is currently being handled.
};

/* ------------------------------------------------------------------------- */

#endif /* ONS_AGENT_SERVICE_HH */
