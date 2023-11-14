#ifndef OTC_INTERNET_RPCSRVLT_HH
#define OTC_INTERNET_RPCSRVLT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/rpcsrvlt.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvl.hh>
#include <OTC/dispatch/svbroker.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/rpcsrvlt.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_RpcServlet : public OTC_HttpServlet, public OTC_EVAgent
    // = TITLE
    //      Servlet translating RPC requests into service requests.
    //
    // = CLASS TYPE
    //      Concrete
    //
    // = DESCRIPTION
    //     The <OTC_RpcServlet> class is a HTTP servlet which acts as a
    //     gateway, translating RPC requests into service requests which
    //     are then delivered to a service. Any response form the service is
    //     subsequently translated back into the appropriate response for
    //     the RPC and returned to the HTTP client.
{
  public:

    			~OTC_RpcServlet();

    // = INITIALISATION

    			OTC_RpcServlet(
			 OTC_HttpSession* theSession,
			 OTC_ServiceBinding* theBinding=0
			);
				// Creates a HTTP servlet to handle an
				// RPC request. <theSession> is the HTTP
				// session object which has parsed the HTTP
				// request. This must be supplied by the
				// factory method contained in the HTTP
				// server object. <theBinding> is the service
				// binding of the service to which the
				// RPC request should be passed. If
				// <theBinding> is <0>, a HTTP response will
				// instead be sent back indicating the
				// target URL was invalid.

  protected:

    // = REQUEST PROCESSING

    void		processRequest();
    				// Decodes the request and either flags an
				// error.

    void		processContent(OTC_String const& theContent);
    				// Accumulates the content of the request
				// and when complete forwards request
				// onto the appropriate service.

    // = TARGET SERVICE

    virtual OTC_ServiceBinding*	service(char const* theName);
				// This function is be called by the method
				// <processRequest()> with <theName> equal to
				// the servlet path supplied to the
				// constructor. This method should analyse
				// <theName> to determine what service the
				// request is being targeted at. By default
				// the method simply looks up the service
				// registry and uses the first service having
				// <theName> that it finds. You can overload
				// this method in a derived class to use
				// some other means to identify which service
				// to use. This would be appropriate given
				// that not all services may be able to
				// respond to requests conforming to what
				// can be represented in an RPC. The instance
				// of <OTC_ServiceBinding> returned by a
				// derived class must have no owner, or if it
				// has, the derived method must call <clone()>
				// on it. This is because <processRequest()>
				// will call <destroy()> on the returned
				// object when it has finished with it.

    // = EVENT HANDLING

    void		process(OTC_Event* theEvent);
    				// Deals with the response received from a
				// service, translating it back into the
				// appropriate RPC format and returning it.

    void		handle(OTC_Event* theEvent);
    				// Calls <process()> then destroys <theEvent>.

  private:

    			OTC_RpcServlet(OTC_RpcServlet const&);
				// Do not define an implementation for this.

    OTC_RpcServlet&	operator=(OTC_RpcServlet const&);
				// Do not define an implementation for this.

    void		cleanup_();
				// Removes any subscriptions on service
				// registry and destroys the service broker.

    OTC_ServiceBinding*	binding_;
    				// Service binding of target service.

    OTC_SVBroker*	broker_;
    				// Broker through which requests are sent
				// and subsequently received.

    OTC_String		requestMethod_;
    				// The name of the request method.

    OTC_String		content_;
    				// Accumulated content of the request.

    OTC_String		conversationId_;
    				// Conversation ID of request.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_RPCSRVLT_HH */
