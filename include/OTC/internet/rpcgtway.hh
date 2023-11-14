#ifndef OTC_INTERNET_RPCGTWAY_HH
#define OTC_INTERNET_RPCGTWAY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/rpcgtway.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvr.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/hmap.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/rpcgtway.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_RpcGateway : public OTC_HttpServer, public OTC_EVAgent
    // = TITLE
    //     Factory for creation of HTTP servlets to handle RPC requests.
    //
    // = DESCRIPTION
    //     The <OTC_RpcGateway> class is a HTTP server object factory for
    //     creating HTTP servlets to handle NET-RPC requests. An instance
    //     of this class monitors what services are available and dependent
    //     on the selection criteria specified when the gateway is created
    //     will deem some set of services as visible. If the gateway receives
    //     an RPC request where the servlet path corresponds to the name of a
    //     visible service, it will forward the RPC request to that service
    //     and subsequently send back any response.
    //     
    // = SEE ALSO
    //     <OTC_HttpDaemon>, <OTC_RpcServlet>, <OTC_HttpServer>
{
  public:

    virtual		~OTC_RpcGateway();

    // = INITIALISATION

			OTC_RpcGateway(
			 char const* theSelection,
			 OTC_ServiceSubscription theType=OTCLIB_SERVICE_GROUP
			);
				// Creates a HTTP server object which handles
				// passing RPC requests to selected services.
				// Exactly how <theSelection> is interpreted
				// and thus which set of services is visible
				// is determined by <theType> of selection.
				// <theType> can be <OTCLIB_SERVICE_GROUP>,
				// in which case <theSelection> indicates that
				// the service group whose name is given by
				// <theSelection> will be visible. If however
				// <theSelection> is an empty string in this
				// case then all services will be exposed
				// regardless of what groups they have been
				// placed in. For the case where <theType> is
				// <OTCLIB_SERVICE_REGISTRY>, <theSelection>
				// will be interpreted similarly to how the
				// service audience attribute is used. If
				// <theSelection> is "*", all services will
				// be exposed. If <theSelection> is an empty
				// string, only those services in the local
				// process will be exposed. For any other
				// value of <theSelection> services will be
				// visible if it matches the service audience
				// of the service. For both values of
				// <theType> above, to access a service, its
				// name must be appended to the URL, separated
				// by a slash, under which the gateway is
				// registered with the HTTP daemon. Where
				// <theType> is <OTCLIB_SERVICE_NAME>,
				// <theSelection> will be taken as the actual
				// name of the service and for <theType>
				// being <OTCLIB_SERVICE_ADDRESS>, it will be
				// taken as the service address of the
				// service. For these latter two cases, the
				// gateway will only match to a single service
				// and nothing should be appended to the URL
				// under which the gateway is registered with
				// the HTTP daemon.

  protected:

    // = SERVLET CREATION

    OTC_HttpServlet*	servlet(OTC_HttpSession* theSession);
				// Returns a HTTP servlet to process the
				// current request being handled by
				// <theSession> assuming it is an RPC
				// request.

    // = EVENT HANDLING

    void		handle(OTC_Event* theEvent);
    				// Handles <theEvent>, responding to
				// announcements about servics joining
				// and leaving service groups.

  private:

			OTC_RpcGateway(OTC_RpcGateway const&);
				// Do not define an implementation for this.

    OTC_RpcGateway&	operator=(OTC_RpcGateway const&);
				// Do not define an implementation for this.

    OTC_SVBroker	broker_;
    				// The service broker.

    OTC_String		selection_;
    				// Service group to monitor.

    OTC_ServiceSubscription	type_;
    				// The type of subscription.

    OTC_HMap<OTC_String,OTC_ServiceBinding*>	visible_;
    				// Currently visible services.

    OTC_TManyMap<OTC_String,OTC_ServiceBinding*>	available_;
    				// Services available.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_RPCGTWAY_HH */
