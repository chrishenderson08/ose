/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/otclinkable.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1996-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/linkable.hh"
#endif
#endif

#include <OTC/collctn/linkable.hh>

/* ------------------------------------------------------------------------- */
OTC_Linkable::~OTC_Linkable()
{
  OTCLIB_ASSERT(count_ == 0);

  unlink();
}

/* ------------------------------------------------------------------------- */
void OTC_Linkable::addBefore(OTC_Linkable* theObject)
{
  // Error on a null value or attempt to add this object or the object
  // already occuring before this object, before here.

  OTCLIB_ENSURE_FN((theObject != 0),
   "OTC_Linkable::addBefore(OTC_Linkable*)",
   "must have valid object to insert");
  OTCLIB_ENSURE_FN((theObject != this),
   "OTC_Linkable::addBefore(OTC_Linkable*)",
   "can't insert myself");

  // Unlink object from any other list which it may be a part of.

  if (theObject->prev_ != 0 || theObject->next_ != 0)
  {
    OTCLIB_ENSURE_FN((theObject->isLink()),
     "OTC_Linkable::addBefore(OTC_Linkable*)",
     "can't relocate an anchor point");
    theObject->unlink();
  }

  // Setup links to include object in this list.

  theObject->prev_ = prev_;
  theObject->next_ = this;
  if (prev_ != 0)
    prev_->next_ = theObject;
  prev_ = theObject;
}

/* ------------------------------------------------------------------------- */
void OTC_Linkable::addAfter(OTC_Linkable* theObject)
{
  // Error on a null value or attempt to add this object or the object
  // already occuring after this object, after here.

  OTCLIB_ENSURE_FN((theObject != 0),
   "OTC_Linkable::addAfter(OTC_Linkable*)",
   "must have valid object to insert");
  OTCLIB_ENSURE_FN((theObject != this),
   "OTC_Linkable::addAfter(OTC_Linkable*)",
   "can't insert myself");

  // Unlink object from any other list which it may be a part of.

  if (theObject->prev_ != 0 || theObject->next_ != 0)
  {
    OTCLIB_ENSURE_FN((theObject->isLink()),
     "OTC_Linkable::addAfter(OTC_Linkable*)",
     "can't relocate an anchor point");
    theObject->unlink();
  }

  // Setup links to include object in this list.

  theObject->next_ = next_;
  theObject->prev_ = this;
  if (next_ != 0)
    next_->prev_ = theObject;
  next_ = theObject;
}

/* ------------------------------------------------------------------------- */
