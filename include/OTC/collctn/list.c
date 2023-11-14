#ifndef OTC_COLLCTN_LIST_C
#define OTC_COLLCTN_LIST_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/list.c
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

#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/avllnode.hh>
#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_List<T>::OTC_List(OTC_List<T> const& theList)
{
  OTC_Iterator<T> iter = 0;
  iter = theList.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    addLast(iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_List<T>::OTC_List(OTC_List<T>& theList, OTC_ShallowCopy)
  : list_(theList.list_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_List<T>& OTC_List<T>::operator=(OTC_List<T> const& theList)
{
  if (&theList != this)
  {
    removeAll();
    OTC_Iterator<T> iter = 0;
    iter = theList.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      addLast(iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_List<T>::addFirst(OTC_Iterator<T> theItems)
{
  int theIndex = 0;

  while (theItems.isValid())
  {
    list_.addBeforeItem(link_(theItems.item()),theIndex++);

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_List<T>::addLast(OTC_Iterator<T> theItems)
{
  while (theItems.isValid())
  {
    list_.addLast(link_(theItems.item()));

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_List<T>::addBeforeItem(OTC_Iterator<T> theItems, u_int theIndex)
{
  while (theItems.isValid())
  {
    list_.addBeforeItem(link_(theItems.item()),theIndex++);

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>* OTC_List<T>::items_(
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
OTC_Cursor<T>* OTC_List<T>::items_(
 int theStart,
 u_int theLength,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  if (theLength == 0)
    return 0;

  OTCLIB_ENSURE_FN((theStart >= 0 && theStart+theLength <= population()),
   "OTC_List<T>::items_(int, u_int, OTC_Direction, OTC_Protection)",
   "invalid range");

  OTC_Anchor* theStartAnchor = 0;
  OTC_Anchor* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
  }

  OTC_Linkable* theStartLink = 0;
  OTC_Linkable* theEndLink = 0;

  theStartLink = ((OTC_AVLLinkNode*)
   list_.tree()->node(theStart))->link();
  theEndLink = ((OTC_AVLLinkNode*)
   list_.tree()->node(theStart+theLength-1))->link();

  if (theStartAnchor != 0)
  {
    theStartLink->addBefore(theStartAnchor);
    theStartLink = theStartAnchor;
  }
  else
  {
    theStartLink = theStartLink->prev();
  }

  if (theEndAnchor != 0)
  {
    theEndLink->addAfter(theEndAnchor);
    theEndLink = theEndAnchor;
  }
  else
  {
    theEndLink = theEndLink->next();
  }

  OTC_Cursor<T>* theIter;
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_.list(),theStartLink,
   theEndLink,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Link* OTC_List<T>::link_(T const& theItem)
{
  OTC_Bucket<T>* theBucket;
  theBucket = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(theBucket != 0);

  return theBucket;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_List<T>::replaceItem_(T const& theItem, int theIndex)
{
  OTCLIB_ENSURE_FN((theIndex >= 0 && theIndex < (int)population()),
   "OTC_List<T>::replaceItem_(T const&, int)",
   "index out of range");

  OTC_Link* theLink;
  theLink = link_(theItem);

  list_.replaceItem(theLink,theIndex);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LIST_C */
