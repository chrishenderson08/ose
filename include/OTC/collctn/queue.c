#ifndef OTC_COLLCTN_QUEUE_C
#define OTC_COLLCTN_QUEUE_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/queue.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/holder.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Queue<T>::OTC_Queue()
  : count_(0)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Queue<T>::OTC_Queue(OTC_Queue<T> const& theQueue)
  : count_(0)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);

  OTC_LinkIterator iter = theQueue.list_->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    add(((OTC_Holder<T>*)iter.link())->item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Queue<T>& OTC_Queue<T>::operator=(OTC_Queue<T> const& theQueue)
{
  if (&theQueue == this)
    return *this;

  clear();

  OTC_LinkIterator iter = theQueue.list_->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
    add(((OTC_Holder<T>*)iter.link())->item());

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Queue<T>::add(T const& theItem)
{
  OTC_Holder<T>* theNode;
  theNode = new OTC_Holder<T>(theItem);
  OTCLIB_ASSERT_M(theNode != 0);
  list_->addLast(theNode);
  count_++;
}

/* ------------------------------------------------------------------------- */
template<class T>
T OTC_Queue<T>::remove()
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_Queue<T>::remove()",
   "queue empty");

  OTC_Holder<T>* theNode = (OTC_Holder<T>*)list_->first();
  T theItem = theNode->item();
  theNode->kill();
  count_--;
  return theItem;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Queue<T>::discard(u_int theCount)
{
  OTCLIB_ENSURE_FN((theCount <= count_),
   "OTC_Queue::discard(u_int)",
   "insufficient items");

  while (theCount > 0)
  {
    list_->first()->kill();
    theCount--;
    count_--;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_Queue<T>::head_() const
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_Queue<T>::head_()",
   "queue empty");

  OTC_Holder<T>* theNode = (OTC_Holder<T>*)list_->first();
  return theNode->item();
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_QUEUE_C */
