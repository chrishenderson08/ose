/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otchttplstn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/httplstn.hh"
#endif
#endif

#include <OTC/internet/httplstn.hh>
#include <OTC/internet/httpdaem.hh>

/* ------------------------------------------------------------------------- */
OTC_HttpListener::~OTC_HttpListener()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_HttpListener::OTC_HttpListener(OTC_HttpDaemon* theDaemon, int thePort)
  : OTC_InetListener("STREAM",thePort), daemon_(theDaemon)
{
  OTCLIB_ENSURE_FN((theDaemon != 0),
   "OTC_HttpListener::OTC_HttpListener(OTC_HttpDaemon*,int)",
   "invalid daemon object");
}

/* ------------------------------------------------------------------------- */
bool OTC_HttpListener::authorise(OTC_String const& theClientHost)
{
  return daemon_->authorise(theClientHost);
}

/* ------------------------------------------------------------------------- */
