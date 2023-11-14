/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otciilink.cc
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
#pragma implementation "OTC/collctn/iilink.hh"
#endif
#endif

#include <OTC/collctn/iilink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

/* ------------------------------------------------------------------------- */
OTC_IILink::~OTC_IILink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IILink::OTC_IILink(int theKey, int theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IILink::OTC_IILink(OTC_IILink const& theLink)
  : key_(theLink.key_), item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
