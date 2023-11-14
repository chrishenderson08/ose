/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcivlink.cc
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
#pragma implementation "OTC/collctn/ivlink.hh"
#endif
#endif

#include <OTC/collctn/ivlink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

/* ------------------------------------------------------------------------- */
OTC_IVLink::~OTC_IVLink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IVLink::OTC_IVLink(int theKey, void* theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_IVLink::OTC_IVLink(OTC_IVLink const& theLink)
  : key_(theLink.key_), item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
