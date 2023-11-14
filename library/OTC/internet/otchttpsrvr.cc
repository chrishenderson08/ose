/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otchttpsrvr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/httpsrvr.hh"
#endif
#endif

#include <OTC/internet/httpsrvr.hh>

#include <OTC/internet/echosrvl.hh>

/* ------------------------------------------------------------------------- */
OTC_HttpServer::~OTC_HttpServer()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
bool OTC_HttpServer::authorise(
 char const* theLogin,
 char const* thePassword
)
{
  return true;
}

/* ------------------------------------------------------------------------- */
OTC_HttpServlet* OTC_HttpServer::servlet(OTC_HttpSession* theSession)
{
  OTC_HttpServlet* theServlet;
  theServlet = new OTC_EchoServlet(theSession);
  OTCLIB_ASSERT_M(theServlet != 0);

  return theServlet;
}

/* ------------------------------------------------------------------------- */
