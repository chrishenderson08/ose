/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otchashntry.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/hashntry.hh"
#endif
#endif

#include <OTC/collctn/hashntry.hh>

/* ------------------------------------------------------------------------- */
OTC_HashEntry::~OTC_HashEntry()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_HashEntry::OTC_HashEntry()
 : state_(OTCLIB_ENTRY_EMPTY),
   item_(0),
   hashValue_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
