/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcepclient.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/epclient.hh"
#endif
#endif

#include <OTC/message/epclient.hh>

/* ------------------------------------------------------------------------- */
OTC_EPClient::~OTC_EPClient()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_EPClient::OTC_EPClient(
 char const* theInterface,
 char const* theProtocol
)
  : OTC_EndPoint(OTCLIB_ENDPOINT_CLIENT,theInterface,theProtocol)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
