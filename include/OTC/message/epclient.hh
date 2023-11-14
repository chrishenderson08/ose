#ifndef OTC_MESSAGE_EPCLIENT_HH
#define OTC_MESSAGE_EPCLIENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/epclient.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/message/endpoint.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/message/epclient.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EPClient : public OTC_EndPoint
    // = TITLE
    //     Base class for a client endpoint.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_EPClient> class is a base class for classes which
    //     manage the client end of a connection.
{
  public:

			~OTC_EPClient();

    // = CONNECTION

    virtual void	restart(int theDelay=-1) = 0;
				// Should cause an active connection to be
				// dropped and the connection restarted. If
				// <theDelay> is <-1>, the connection will be
				// restarted after the retry delay specified
				// by any derived class has expired, or if
				// there isn't one, immediately. If
				// <theDelay> is not <-1>, <theDelay> will be
				// how long to wait before restarting. Note
				// that if the restart fails, successive
				// attempts to restart will only be made
				// if a retry delay is specified for the
				// derived class. If one is, retries will
				// occur afters delays specified by the retry
				// delay and not <theDelay>. Must be
				// redefined in a derived class.

  protected:

    // = INITIALISATION

			OTC_EPClient(
			 char const* theInterface,
			 char const* theProtocol
			);
				// <theProtocol> should identify the type of
				// transport layer being used. <theInterface>
				// the type of interface used to communicate
				// with that transport layer.

  private:

			OTC_EPClient(OTC_EPClient const&);
				// Do not define an implementation for this.

    OTC_EPClient&	operator=(OTC_EPClient const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_BaseActions<OTC_EPClient*>
{
  public:

    static OTC_EPClient*	add(OTC_EPClient* theServer)
				{
				  theServer->reference();
				  return theServer;
				}

    static void		remove(OTC_EPClient* theServer)
				{ theServer->unReference(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MESSAGE_EPCLIENT_HH */
