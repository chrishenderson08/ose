/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcrange.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/range.hh"
#endif
#endif

#include <OTC/collctn/range.hh>
#include <OTC/collctn/rngecurs.hh>

/* ------------------------------------------------------------------------- */
bool OTC_Range::isSubRange(OTC_Range const& theRange) const
{
  if (&theRange == this || isNullRange())
    return true;
  
  if (theRange.isNullRange())
    return false;

  if (
   lower() >= theRange.lower()
   &&
   lower() < theRange.upper()
   &&
   upper() > theRange.lower()
   &&
   upper() <= theRange.upper()
  )
  {
    return true;
  }

  return false;
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<int> OTC_Range::indices(OTC_Direction theDirection) const
{
  OTC_RangeCursor* theCursor;
  theCursor = new OTC_RangeCursor(lower_,length_,theDirection);
  OTCLIB_ASSERT_M(theCursor != 0);
  return theCursor;
}

/* ------------------------------------------------------------------------- */
