#ifndef OTC_INTERNET_HTTPLSTN_HH
#define OTC_INTERNET_HTTPLSTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/httplstn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/inetlstn.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/internet/httplstn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_HttpDaemon;

class OSE_EXPORT OTC_HttpListener : public OTC_InetListener
    // = TITLE
    //     Listens for HTTP connection requests.
    //
    // = DESCRIPTION
    //     The <OTC_HttpListener> class is a derived version of the class
    //     <OTC_InetListener>. It exists so that authorisation requests
    //     can be relayed back to the <OTC_HttpDaemon> class. The class
    //     <OTC_HttpDaemon> class isn't derived from <OTC_InetListener>
    //     as it complicates event processing for any derived classes which
    //     may want to process events.
    //
    // = SEE ALSO
    //     <OTC_HttpDaemon>, <OTC_InetListener>
{
  public:


			~OTC_HttpListener();

    // = INITIALISATION

			OTC_HttpListener(
			 OTC_HttpDaemon* theDaemon,
			 int thePort
			);
				// Create a listener on <thePort>. Any
				// authorisation requests are relayed back
				// to <theDaemon>. Raises an exception if
				// <theDaemon> is <0>.

  protected:

    // = AUTHORISATION

    bool		authorise(OTC_String const& theClientHost);
    				// Relays the authorisation request back
				// to the associated HTTP daemon.

  private:

			OTC_HttpListener(OTC_HttpListener const&);
				// Do not define an implementation for this.

    OTC_HttpListener&	operator=(OTC_HttpListener const&);
				// Do not define an implementation for this.

    OTC_HttpDaemon*	daemon_;
    				// The associated HTTP daemon.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_INTERNET_HTTPLSTN_HH */
