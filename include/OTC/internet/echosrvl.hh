#ifndef OTC_INTERNET_ECHOSRVL_HH
#define OTC_INTERNET_ECHOSRVL_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/echosrvl.hh
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

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/echosrvl.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EchoServlet : public OTC_HttpServlet
    // = TITLE
    //     HTTP servlet which echos back details of request.
    //
    // = DESCRIPTION
    //    The <OTC_EchoServlet> is a simple HTTP servlet object which
    //    echos back information about the request it receives.
    //
    // = SEE ALSO
    //     <OTC_HttpSession>, <OTC_HttpDaemon>, <OTC_HttpServer>
{
  public:

    virtual		~OTC_EchoServlet();

    // = INITIALISATION

			OTC_EchoServlet(OTC_HttpSession* theSession)
			  : OTC_HttpServlet(theSession),
			    contentLength_(0)
			    	{}
				// <theSession> is the HTTP session object
				// which has parsed the HTTP request. These
				// must be initialised with the values
				// supplied to the HTTP servlet factory
				// method contained in the HTTP server object.

    // = REQUEST HANDLING

    void		processRequest();
				// Processes the HTTP request, sending back
				// to the HTTP client details about the
				// request.

    void		processContent(OTC_String const& theContent);
				// Presuming a content length was specified,
				// sends back the content to the client until
				// all the specified content has been
				// received.

  private:

			OTC_EchoServlet(OTC_EchoServlet const&);
				// Do not define an implementation for this.

    OTC_EchoServlet&	operator=(OTC_EchoServlet const&);
				// Do not define an implementation for this.

    int			contentLength_;
    				// The amount of content received.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_ECHOSRVL_HH */
