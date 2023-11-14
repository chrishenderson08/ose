/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otchttpsrvl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/httpsrvl.hh"
#endif
#endif

#include <OTC/internet/httpsrvl.hh>

/* ------------------------------------------------------------------------- */
OTC_HttpServlet::~OTC_HttpServlet()
{
  session_->unReference();
}

/* ------------------------------------------------------------------------- */
void OTC_HttpServlet::destroy()
{
  delete this;
}

/* ------------------------------------------------------------------------- */
OTC_HttpServlet::OTC_HttpServlet(OTC_HttpSession* theSession)
  : session_(theSession)
{
  session_->reference();
}

/* ------------------------------------------------------------------------- */
void OTC_HttpServlet::processRequest()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_HttpServlet::processContent(OTC_String const& theContent)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
