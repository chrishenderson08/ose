/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otclinkcurs.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/linkcurs.hh"
#endif
#endif

#include <OTC/collctn/linkcurs.hh>
#include <OTC/collctn/linklist.hh>

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::~OTC_LinkCursor()
{
  if (protection_ == OTCLIB_SAFE)
    list_->unReference();

  delete iter_;
}

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::OTC_LinkCursor(
 OTC_LinkList* theList,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : list_(theList), iter_(0), direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ASSERT(list_ != 0);

  OTC_LinkIterator const& tmpIter = list_->items(theProtection);

  iter_ = new OTC_LinkIterator(tmpIter);
  OTCLIB_ASSERT_M(iter_ != 0);

  if (protection_ == OTCLIB_SAFE)
    list_->reference();

  reset();
}

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::OTC_LinkCursor(
 OTC_LinkList* theList,
 OTC_Linkable* theStart,
 OTC_Linkable* theEnd,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : list_(theList), iter_(0), direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ASSERT(list_ != 0);

  iter_ = new OTC_LinkIterator(theStart,theEnd,theProtection);
  OTCLIB_ASSERT_M(iter_ != 0);

  if (protection_ == OTCLIB_SAFE)
    list_->reference();

  reset();
}

/* ------------------------------------------------------------------------- */
OTC_LinkCursor::OTC_LinkCursor(
 OTC_LinkCursor const& theIter
)
  : list_(theIter.list_), iter_(0), direction_(theIter.direction_),
    protection_(theIter.protection_)
{
  OTCLIB_ASSERT(list_ != 0);

  iter_ = new OTC_LinkIterator(*theIter.iter_);
  OTCLIB_ASSERT_M(iter_ != 0);

  if (protection_ == OTCLIB_SAFE)
    list_->reference();
}

/* ------------------------------------------------------------------------- */
