#ifndef OTC_COLLCTN_DEQUE_C
#define OTC_COLLCTN_DEQUE_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/deque.c
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Deque<T>::OTC_Deque(OTC_Deque<T> const& theDeque)
{
  OTC_Iterator<T> iter = 0;
  iter = theDeque.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    addLast(iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Deque<T>::OTC_Deque(OTC_Deque<T>& theDeque, OTC_ShallowCopy)
  : list_(theDeque.list_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Deque<T>& OTC_Deque<T>::operator=(OTC_Deque<T> const& theDeque)
{
  if (&theDeque != this)
  {
    removeAll();
    OTC_Iterator<T> iter = 0;
    iter = theDeque.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      addLast(iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Deque<T>::addFirst(OTC_Iterator<T> theItems)
{
  OTC_Link* theLink = 0;

  if (theItems.isValid())
  {
    theLink = link_(theItems.item());
    list_.addFirst(theLink);

    theItems.next();
  }

  while (theItems.isValid())
  {
    OTC_Link* tmpLink;
    tmpLink = link_(theItems.item());

    list_.addAfterLink(tmpLink,theLink);

    theLink = tmpLink;

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Deque<T>::addLast(OTC_Iterator<T> theItems)
{
  while (theItems.isValid())
  {
    list_.addLast(link_(theItems.item()));

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>* OTC_Deque<T>::items_(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T>* theIter;
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_.list(),theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Link* OTC_Deque<T>::link_(T const& theItem)
{
  OTC_Bucket<T>* theBucket;
  theBucket = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(theBucket != 0);

  return theBucket;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_DEQUE_C */
