/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcilink.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/ilink.hh"
#endif
#endif

#include <OTC/collctn/ilink.hh>

/* ------------------------------------------------------------------------- */
OTC_ILink::~OTC_ILink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_ILink::OTC_ILink(int theItem)
  : item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_ILink::OTC_ILink(OTC_ILink const& theLink)
  : item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
