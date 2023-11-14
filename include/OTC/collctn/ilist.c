#ifndef OTC_COLLCTN_ILIST_C
#define OTC_COLLCTN_ILIST_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ilist.c
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

#include <OTC/collctn/bucket.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IList<T>::OTC_IList()
{
  iter_ = new OTC_LinkIterator(list_.list()->items());
  OTCLIB_ASSERT_M(iter_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IList<T>::OTC_IList(OTC_IList<T> const& theList)
{
  iter_ = new OTC_LinkIterator(list_.list()->items());
  OTCLIB_ASSERT_M(iter_ != 0);

  OTC_LinkIterator iter = theList.list_.list()->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    addLast(((OTC_Bucket<T>*)iter.link())->item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IList<T>::OTC_IList(OTC_IList<T> const& theList, OTC_ShallowCopy)
  : list_(theList.list_)
{
  iter_ = new OTC_LinkIterator(list_.list()->items());
  OTCLIB_ASSERT_M(iter_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IList<T>::~OTC_IList()
{
  list_.removeAll();
  delete iter_;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IList<T>& OTC_IList<T>::operator=(OTC_IList<T> const& theList)
{
  if (&theList == this)
    return *this;

  list_.removeAll();

  OTC_LinkIterator iter = theList.list_.list()->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    addLast(((OTC_Bucket<T>*)iter.link())->item());

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IList<T>::addBefore(T const& theItem)
{
  OTC_Link* aBucket = link_(theItem);

  if (iter_->isStart())
  {
    list_.addFirst(aBucket);
    iter_->next();
  }
  else if (iter_->isEnd())
  {
    list_.addLast(aBucket);
    iter_->prev();
  }
  else
  {
    list_.addBeforeLink(aBucket,iter_->link());
    iter_->prev();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IList<T>::addAfter(T const& theItem)
{
  OTC_Link* aBucket = link_(theItem);

  if (iter_->isStart())
  {
    list_.addFirst(aBucket);
    iter_->next();
  }
  else if (iter_->isEnd())
  {
    list_.addLast(aBucket);
    iter_->prev();
  }
  else
  {
    list_.addAfterLink(aBucket,iter_->link());
    iter_->next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IList<T>::remove()
{
  OTCLIB_ENSURE_FN((iter_->isLink()),
   "OTC_IList<T>::remove()",
   "no item under iterator");

  if (!iter_->link()->isDead())
    list_.remove(iter_->link());
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_IList<T>::item_() const
{
  OTCLIB_ENSURE_FN((iter_->isLink() && !iter_->link()->isDead()),
   "OTC_IList<T>::item_()",
   "no item under iterator");

  OTC_Bucket<T>* aBucket = (OTC_Bucket<T>*)iter_->link();
  return aBucket->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_IList<T>::first_() const
{
  OTCLIB_ENSURE_FN((!list_.isEmpty()),
   "OTC_IList<T>::first_()",
   "list empty");

  OTC_Bucket<T>* aBucket = (OTC_Bucket<T>*)list_.first();
  return aBucket->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_IList<T>::last_() const
{
  OTCLIB_ENSURE_FN((!list_.isEmpty()),
   "OTC_IList::last_()",
   "list empty");

  OTC_Bucket<T>* aBucket = (OTC_Bucket<T>*)list_.last();
  return aBucket->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IList<T>::apply(
 OTC_Visitor<T>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_LinkIterator theIterator = list_.list()->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T& theItem = ((OTC_Bucket<T>*)theIterator.link())->item();
    progress = theApplicator.action(theItem);

    if (progress == OTCLIB_RESTART)
    {
      if (theDirection == OTCLIB_FORWARD)
	theIterator.resetFirst();
      else
	theIterator.resetLast();
    }
    else if (progress == OTCLIB_CONTINUE)
    {
      if (theDirection == OTCLIB_FORWARD)
	theIterator.next();
      else
	theIterator.prev();
    }
    else
      break;
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IList<T>::apply(
 OTC_Worker<T>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
{
  OTC_LinkIterator theIterator = list_.list()->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T& theItem = ((OTC_Bucket<T>*)theIterator.link())->item();
    progress = theApplicator.action(theItem);

    if (progress == OTCLIB_RESTART)
    {
      if (theDirection == OTCLIB_FORWARD)
        theIterator.resetFirst();
      else
        theIterator.resetLast();
    }
    else if (progress == OTCLIB_CONTINUE)
    {
      if (theDirection == OTCLIB_FORWARD)
        theIterator.next();
      else
        theIterator.prev();
    }
    else
      break;
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Link* OTC_IList<T>::link_(T const& theItem)
{
  OTC_Bucket<T>* theBucket;
  theBucket = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(theBucket != 0);

  return theBucket;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ILIST_C */
