/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otcdequei.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/dequei.hh"
#endif
#endif

#include <OTC/collctn/dequei.hh>

/* ------------------------------------------------------------------------- */
OTC_DequeI::~OTC_DequeI()
{
  count_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_DequeI::OTC_DequeI()
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  count_ = new OTC_RCount;
  OTCLIB_ASSERT_M(count_ != 0);
  count_->reference();
}

/* ------------------------------------------------------------------------- */
OTC_DequeI::OTC_DequeI(OTC_DequeI const& theList)
{
  list_ = theList.list_;
  list_->reference();

  count_ = theList.count_;
  count_->reference();
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::remove(OTC_Link* theLink)
{
  OTCLIB_ENSURE_FN((count_->count() != 0),
   "OTC_DequeI::remove(OTC_Link*)",
   "list is empty");
  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_DequeI::remove(OTC_Link*)",
   "invalid link");
  OTCLIB_ENSURE_FN((theLink->isLink()),
   "OTC_DequeI::remove(OTC_Link*)",
   "not a link");

  if (!theLink->isDead())
  {
    count_->dec();
    theLink->kill();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::removeFirst(OTC_Protection theProtection)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (count_->count() == 0)
      return;
  }

  OTCLIB_ENSURE_FN((count_->count() != 0),
   "OTC_DequeI::removeFirst(OTC_Protection)",
   "list is empty");

  count_->dec();
  list_->first()->kill();
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::removeLast(OTC_Protection theProtection)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (count_->count() == 0)
      return;
  }

  OTCLIB_ENSURE_FN((count_->count() != 0),
   "OTC_DequeI::removeLast(OTC_Protection)",
   "list is empty");

  count_->dec();
  list_->last()->kill();
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_DequeI::first() const
{
  OTCLIB_ENSURE_FN((count_->count() != 0),
   "OTC_DequeI::first()",
   "list is empty");

  return list_->first();
}

/* ------------------------------------------------------------------------- */
OTC_Link* OTC_DequeI::last() const
{
  OTCLIB_ENSURE_FN((count_->count() != 0),
   "OTC_DequeI::last()",
   "list is empty");

  return list_->last();
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::addFirst(OTC_Link* theLink)
{
  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_DequeI::addFirst(OTC_Link*)",
   "invalid link");

  list_->addFirst(theLink);
  count_->inc();
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::addLast(OTC_Link* theLink)
{
  OTCLIB_ENSURE_FN((theLink != 0),
   "OTC_DequeI::addLast(OTC_Link*)",
   "invalid link");

  list_->addLast(theLink);
  count_->inc();
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::addBeforeLink(OTC_Link* theNewLink, OTC_Link* theOldLink)
{
  OTCLIB_ENSURE_FN((theNewLink != 0 && theOldLink != 0),
   "OTC_DequeI::addBeforeLink(OTC_Link*, OTC_Link*)",
   "invalid link");

  if (theOldLink->isLink() || theOldLink->prev() != 0)
    theOldLink->addBefore(theNewLink);
  else
    theOldLink->addAfter(theNewLink);

  count_->inc();
}

/* ------------------------------------------------------------------------- */
void OTC_DequeI::addAfterLink(OTC_Link* theNewLink, OTC_Link* theOldLink)
{
  OTCLIB_ENSURE_FN((theNewLink != 0 && theOldLink != 0),
   "OTC_DequeI::addAfterLink(OTC_Link*, OTC_Link*)",
   "invalid link");

  if (theOldLink->isLink() || theOldLink->next() != 0)
    theOldLink->addAfter(theNewLink);
  else
    theOldLink->addBefore(theNewLink);

  count_->inc();
}

/* ------------------------------------------------------------------------- */
