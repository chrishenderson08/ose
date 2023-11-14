/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcsilink.cc
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
#pragma implementation "OTC/text/silink.hh"
#endif
#endif

#include <OTC/text/silink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

/* ------------------------------------------------------------------------- */
OTC_SILink::~OTC_SILink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SILink::OTC_SILink(OTC_String const& theKey, int theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SILink::OTC_SILink(OTC_SILink const& theLink)
  : key_(theLink.key_), item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
