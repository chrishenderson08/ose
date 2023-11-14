/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcsvlink.cc
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
#pragma implementation "OTC/text/svlink.hh"
#endif
#endif

#include <OTC/text/svlink.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/kicursor.hh>

/* ------------------------------------------------------------------------- */
OTC_SVLink::~OTC_SVLink()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SVLink::OTC_SVLink(OTC_String const& theKey, void* theItem)
  : key_(theKey), item_(theItem)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_SVLink::OTC_SVLink(OTC_SVLink const& theLink)
  : key_(theLink.key_), item_(theLink.item_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
