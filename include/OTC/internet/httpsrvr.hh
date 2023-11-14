#ifndef OTC_INTERNET_HTTPSRVR_HH
#define OTC_INTERNET_HTTPSRVR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/httpsrvr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsrvl.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/httpsrvr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_HttpServer
    // = TITLE
    //     Factory for creation of HTTP servlets to handle HTTP requests.
    //
    // = DESCRIPTION
    //     The <OTC_HttpServer> is a base class for a factory object which
    //     creates an instance of a HTTP servlet to process a specific HTTP
    //     request. The default implementation creates an instance of a
    //     echo HTTP servlet to honour all requests. You should create your
    //     own derived class to process requests in other ways. An instance
    //     of your derived class needs to be registered with the HTTP
    //     daemon which is accepting the HTTP client connections.
    //     
    // = SEE ALSO
    //     <OTC_HttpDaemon>, <OTC_HttpServlet>
{
    friend class	OTC_HttpSession;

  public:

    virtual		~OTC_HttpServer();

			OTC_HttpServer() {}

  protected:

    // = AUTHORISATION

    virtual bool	authorise(
			 char const* theLogin,
			 char const* thePassword
			);
				// By default returns <true>. Can be
				// overridden in a derived class to return
				// <false> if the user identified by
				// <theLogin> and <thePassword> is not
				// permitted to make a request against this
				// HTTP server object. If <false> is
				// returned, a response indicating that
				// authorisation is required will
				// automatically be sent back.

    // = SERVLET CREATION

    virtual OTC_HttpServlet*	servlet(OTC_HttpSession* theSession);
				// Should be overridden to return an instance
				// of a HTTP servlet to process the current
				// request being handled by <theSession>. By
				// default this method returns an instance of
				// the echo HTTP servlet. Details of the path
				// to be used to select the servlet can be
				// obtained by using the member function
				// <servletPath()> on <theSession>.

  private:

			OTC_HttpServer(OTC_HttpServer const&);
				// Do not define an implementation for this.

    OTC_HttpServer&	operator=(OTC_HttpServer const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_HTTPSRVR_HH */
