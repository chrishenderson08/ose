/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcslink.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/slink.hh"
#endif
#endif

#include <OTC/text/slink.hh>

/* ------------------------------------------------------------------------- */
OTC_SLink::~OTC_SLink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SLink::OTC_SLink(OTC_String const& theItem)
  : item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SLink::OTC_SLink(OTC_SLink const& theLink)
  : item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
