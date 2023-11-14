#ifndef OTC_INTERNET_HTTPDAEM_HH
#define OTC_INTERNET_HTTPDAEM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/httpdaem.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/internet/httpsssn.hh>
#include <OTC/internet/httplstn.hh>
#include <OTC/internet/httpsrvr.hh>
#include <OTC/internet/httpsrvl.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/collctn/hindex.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/httpdaem.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

typedef OTC_HttpServlet* (*OTC_HttpServletFactory)(
 char const*,char const*,OTC_HttpSession*);

class OSE_EXPORT OTC_HttpDaemon : public OTC_EVAgent
    // = TITLE
    //     Monitors port and spawns object to handle HTTP session.
    //
    // = DESCRIPTION
    //     The <OTC_HttpDaemon> class listens on a defined port and for each
    //     connection received will create a separate HTTP session object to
    //     handle any requests received via that connection. The HTTP session
    //     object will perform a lookup against this daemon object for any
    //     HTTP server object able to handle the request for the URL which
    //     is specified in a request. A HTTP servlet is then created by
    //     the HTTP server object to process the request.
    //     
    //     To handle requests, you should load into this HTTP daemon object a
    //     HTTP server object which is able to create HTTP servlets for
    //     processing any requests falling under a specified URL path. Where
    //     a request path matches with two or more HTTP servers, the HTTP
    //     server corresponding to the longest URL is used.
    //     
    //     If you wish to block specific client hosts from accessing any part
    //     of the URL namespace, you can derive from this class and override
    //     the <authorise()> method. Note that this blocks the connection
    //     before any request is even parsed.
    //     
    // = EXAMPLE
    // = BEGIN<CODE>
    //     main()
    //     {
    //       OTC_Disptacher::initialise();
    //       OTC_HttpDaemon theDaemon(8080);
    //       OTC_HttpServer theServer;
    //       theDaemon.attach("/",&theServer);
    //       theDaemon.start();
    //       OTC_Dispatcher::run();
    //       return 0;
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_HttpSession>, <OTC_HttpServlet>
{
    friend class	OTC_HttpListener;
    friend class	OTC_HttpSession;

  public:

    // = DESTRUCTION

			~OTC_HttpDaemon();
				// Destroys all active session objects and
				// subsequently and servlet objects.

    // = INITIALISATION

			OTC_HttpDaemon(int thePort);
				// Create a listener on <thePort>. The daemon
				// has to be explicitly started by calling
				// <start()> before it will start listening
				// for requests.

    // = PORT

    int			port() const
      				{ return listener_.port(); }
    				// Returns the port on which the daemon
				// is listening for connections.

    // = EXECUTION

    void		start()
      				{ listener_.start(); }
    				// Directs the HTTP dameon to start listening
				// for connection requests.

    void		stop()
      				{ listener_.stop(); }
				// Directs the HTTP dameon to stop listening
				// for connection requests.

    // = SERVER OBJECTS

    void		attach(
			 char const* theServerPath,
			 OTC_HttpServer* theServer
			);
				// Loads into this HTTP daemon <theServer>
				// as a way of creating a HTTP servlet which
				// can handle requests which fall below
				// <theServerPath>. The instance of a HTTP
				// servlet returned by the callback must be
				// created on the free store. The HTTP server
				// object will delete the servlet when it is
				// no longer required. Note that a path
				// described by <theServerPath> must start
				// with "/" and should be normalised. Ie.,
				// it shouldn't contain directory components
				// matching "." or "..".

  protected:

    // = CLIENT AUTHORISATION

    virtual bool	authorise(OTC_String const& theClientHost);
				// By default returns <true>. Can be
				// overridden in a derived class to return
				// <false>, if <theClientHost> should be
				// refused a connection. <theClientHost> will
				// be an IP address. Note that in using this
				// method there is no chance of sending back
				// a response indicating why a client may be
				// refused, the connection will simply be
				// dropped.

    // = EVENT CALLBACKS

    void		process(OTC_Event* theEvent);
				// For each new connection creates a new HTTP
				// session object. When the connection goes
				// away it will also destroy the session
				// object.

    void		handle(OTC_Event* theEvent);
				// Calls <process()> to handle <theEvent> and
				// then destroys the event. If you override
				// this member function in a derived class
				// and redefine <handle()> you must call
				// <process()> at the start of your version
				// of the <handle()> method and subsequently
				// destroy the event.

  private:

			OTC_HttpDaemon(OTC_HttpDaemon const&);
				// Do not define an implementation for this.

    OTC_HttpDaemon&	operator=(OTC_HttpDaemon const&);
				// Do not define an implementation for this.

    OTC_HttpServer*	server_(OTC_String const& theServerPath) const;
    				// Returns any HTTP server object which is
				// registered against <theServerPath>.

    OTC_NRMutex		mutex_;
    				// Locks for threads.

    OTC_HttpListener	listener_;
				// Listens for new connections.

    OTC_HIndex<OTC_String>	servers_;
    				// Set of HTTP server objects.

    OTC_HIndex<OTC_String>	sessions_;
				// Set of active HTTP session objects.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_HTTPDAEM_HH */
