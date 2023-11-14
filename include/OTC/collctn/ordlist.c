#ifndef OTC_COLLCTN_ORDLIST_C
#define OTC_COLLCTN_ORDLIST_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ordlist.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_OrderedList<T>::~OTC_OrderedList()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_OrderedList<T>::OTC_OrderedList(int (*theRankFn)(T const&,T const&))
  : rankFn_(theRankFn)
{
  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_OrderedList<T>::OTC_OrderedList(OTC_OrderedList<T> const& theList)
  : rankFn_(theList.rankFn_)
{
  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  OTC_Iterator<T> iter = 0;
  iter = theList.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    add(iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_OrderedList<T>::OTC_OrderedList(
 OTC_OrderedList<T> const& theList,
 OTC_ShallowCopy
)
  : rankFn_(theList.rankFn_)
{
  index_ = theList.index_;
  index_->reference();

  list_ = theList.list_;
  list_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_OrderedList<T>& OTC_OrderedList<T>::operator=(
 OTC_OrderedList<T> const& theList
)
{
  if (this != &theList)
  {
    removeAll();

    rankFn_ = theList.rankFn_;
    OTC_Iterator<T> iter = 0;
    iter = theList.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      add(iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
u_int OTC_OrderedList<T>::add(T const& theItem)
{
  OTC_Bucket<T>* theBucket;
  theBucket = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(theBucket != 0);

  OTC_AVLLinkNode* theLinkNode;
  theLinkNode = new OTC_AVLLinkNode(theBucket);
  OTCLIB_ASSERT_M(theLinkNode);

  u_int theIndex = 0;
  OTC_AVLNode* theNode = index_->root();
  if (theNode == 0)
  {
    list_->addFirst(theBucket);
    index_->addRoot(theLinkNode);
  }
  else
  {
    u_int fullCount = 0;
    u_int tmpCount = theNode->count();
    OTC_AVLNode* theParent = theNode;
    while (theNode != 0)
    {
      T& tmpItem = ((OTC_Bucket<T>*)
       ((OTC_AVLLinkNode*)theNode)->link())->item();
      int theRank = rank(theItem,tmpItem);
      theParent = theNode;
      if (theRank < 0)
      {
	theNode = theNode->left();
	if (theNode == 0)
	{
	  ((OTC_AVLLinkNode*)theParent)->link()->addBefore(theBucket);
	  theParent->addBefore(theLinkNode);
	  break;
	}
	else
	  tmpCount = theNode->count();
      }
      else
      {
	theNode = theNode->right();
	if (theNode == 0)
	{
	  ((OTC_AVLLinkNode*)theParent)->link()->addAfter(theBucket);
	  theParent->addAfter(theLinkNode);
	  tmpCount += 1;
	  break;
	}
	else
	{
	  fullCount += tmpCount;
	  tmpCount = theNode->count();
	}
      }
    }
    theIndex = fullCount + tmpCount - 1;
  }

  return theIndex;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::add(OTC_Iterator<T> theItems)
{
  while (theItems.isValid())
  {
    add(theItems.item());

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::removeFirst(OTC_Protection theProtection)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (isEmpty())
      return;
  }

  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_OrderedList<T>::removeFirst(OTC_Protection)",
   "collection empty");

  if (isEmpty())
    return;

  OTC_AVLNode* theNode = index_->root();
  OTC_AVLNode* theParent = 0;
  while (theNode != 0)
  {
    theParent = theNode;
    theNode = theNode->left();
  }
  theParent->unlink();
  ((OTC_AVLLinkNode*)theParent)->link()->kill();
  delete theParent;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::removeLast(OTC_Protection theProtection)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (isEmpty())
      return;
  }

  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_OrderedList<T>::removeLast(OTC_Protection)",
   "collection empty");

  if (isEmpty())
    return;

  OTC_AVLNode* theNode = index_->root();
  OTC_AVLNode* theParent = 0;
  while (theNode != 0)
  {
    theParent = theNode;
    theNode = theNode->right();
  }
  theParent->unlink();
  ((OTC_AVLLinkNode*)theParent)->link()->kill();
  delete theParent;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::remove(T const& theItem, OTC_Protection theProtection)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (isEmpty())
      return;
  }

  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_OrderedList<T>::remove(T const&, OTC_Protection)",
   "collection empty");

  if (isEmpty())
    return;

  OTC_AVLNode* theNode = index_->root();
  while (theNode != 0)
  {
    T& tmpItem = ((OTC_Bucket<T>*)
     ((OTC_AVLLinkNode*)theNode)->link())->item();
    int theRank = rank(theItem,tmpItem);
    if (theRank < 0)
      theNode = theNode->left();
    else if (theRank > 0)
      theNode = theNode->right();
    else
    {
      theNode->unlink();
      ((OTC_AVLLinkNode*)theNode)->link()->kill();
      delete theNode;
      return;
    }
  }

  if (theProtection == OTCLIB_UNSAFE)
    return;

  OTCLIB_ENSURE_FN((theNode != 0),
   "OTC_OrderedList<T>::remove(T const&, OTC_Protection)",
   "no such item");
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::removeItem(
 u_int theIndex,
 OTC_Protection theProtection
)
{
  if (theProtection == OTCLIB_UNSAFE)
  {
    if (isEmpty() || theIndex >= index_->population())
      return;
  }

  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_OrderedList<T>::removeItem(u_int, OTC_Protection)",
   "collection empty");
  OTCLIB_ENSURE_FN((theIndex < index_->population()),
   "OTC_OrderedList<T>::removeItem(u_int, OTC_Protection)",
   "index out of range");

  OTC_AVLNode* theNode = index_->node(theIndex);
  OTCLIB_ASSERT(theNode != 0);
  theNode->unlink();
  ((OTC_AVLLinkNode*)theNode)->link()->kill();
  delete theNode;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::removeAll(T const& theItem)
{
  removeRange(range(theItem));
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_OrderedList<T>::removeRange_(
 int theStart,
 u_int theLength,
 OTC_Protection theProtection
)
{
  if (theProtection == OTCLIB_UNSAFE && theLength > 0)
  {
    if (theStart >= (int)index_->population())
    {
      theLength = 0;
      theStart = 0;
    }
    else if (theStart < 0)
    {
      theLength = theLength + theStart;
      theStart = 0;
    }

    if (theStart+theLength >= index_->population())
    {
      theLength = index_->population() - theStart;
    }
  }

  if (theLength == 0)
    return;

  OTCLIB_ENSURE_FN((theStart>=0 && theStart+theLength<=index_->population()),
    "OTC_OrderedList<T>::removeRange(int, u_int, OTC_Protection)",
    "invalid range");

  while (theLength-- > 0)
    removeItem(theStart+theLength);
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_OrderedList<T>::index(
 T const& theItem,
 OTC_Occurrence theOccurrence,
 OTC_Placement thePlacement
) const
{
  // Always fail for an empty list.

  if (isEmpty())
    return -1;

  // Start the search.

  OTC_AVLNode* theNode = index_->root();
  u_int fullCount = 0;
  u_int tmpCount = theNode->count();
  while (theNode != 0)
  {
    T& tmpItem1 = ((OTC_Bucket<T>*)
     ((OTC_AVLLinkNode*)theNode)->link())->item();
    int theRank = rank(theItem,tmpItem1);
    if (theRank < 0)
    {
      u_int theIndex = fullCount + tmpCount - 1;

      // ANY && (GT || GE) - Item being searched is before this item in
      // the list, therefore, if after any item in case of (GT || GE),
      // the current item will do.

      if (theOccurrence == OTCLIB_ANY
       && (thePlacement == OTCLIB_GE || thePlacement == OTCLIB_GT))
      {
	return theIndex;
      }

      if (theNode->left() == 0 && thePlacement != OTCLIB_EQ)
      {
	// LAST && (GE || GT) - If we have got to the last item in
	// the list, we return -1, otherwise we return the last item
	// in the list.

	if (theOccurrence == OTCLIB_LAST
	  && (thePlacement == OTCLIB_GE || thePlacement == OTCLIB_GT))
	{
	  if (theIndex == index_->population()-1)
	    return -1;

	  return index_->population()-1;
	}

	// FIRST && (GE || GT) - Return the current item.

	if (theOccurrence == OTCLIB_FIRST
	 && (thePlacement == OTCLIB_GE || thePlacement == OTCLIB_GT))
	{
	  return theIndex;
	}

	// (ANY || LAST) && (LT || LE)  - If at first item in the list,
	// return -1, else return item preceeding this item.

	if ((theOccurrence == OTCLIB_ANY || theOccurrence == OTCLIB_LAST)
	 && (thePlacement == OTCLIB_LT || thePlacement == OTCLIB_LE))
	{
	  if (theIndex == 0)
	    return -1;

	  return theIndex-1;
	}

	// FIRST && (LT || LE) - If at first item in the list, return -1,
	// else return 0.

	if (theOccurrence == OTCLIB_FIRST
	 && (thePlacement == OTCLIB_LT || thePlacement == OTCLIB_LE))
	{
	  if (theIndex == 0)
	    return -1;

	  return 0;
	}
      }

      theNode = theNode->left();
      if (theNode != 0)
	tmpCount = theNode->count();
    }
    else if (theRank > 0)
    {
      u_int theIndex = fullCount + tmpCount - 1;

      // ANY && (LT || LE) - Item being searched is after this item in
      // the list, therefore, if after any item in case of (LT || LE),
      // the current item will do.

      if (theOccurrence == OTCLIB_ANY
       && (thePlacement == OTCLIB_LE || thePlacement == OTCLIB_LT))
      {
	return theIndex;
      }

      if (theNode->right() == 0 && thePlacement != OTCLIB_EQ)
      {
	// FIRST && (LT || LE) - If we have got to the first item in
	// the list, we return -1, otherwise we return first item in the
	// list.

	if (theOccurrence == OTCLIB_FIRST
	  && (thePlacement == OTCLIB_LT || thePlacement == OTCLIB_LE))
	{
	  if (theIndex == 0)
	    return -1;

	  return 0;
	}

	// LAST && (LT && LE) - Return the current item.

	if (theOccurrence == OTCLIB_LAST
	 && (thePlacement == OTCLIB_LT || thePlacement == OTCLIB_LE))
	{
	  return theIndex;
	}

	// (FIRST || ANY) && (GE || GT) - If at the last item in the
	// list return -1, else return suceeding item.

	if ((theOccurrence == OTCLIB_FIRST || theOccurrence == OTCLIB_ANY)
	 && (thePlacement == OTCLIB_GE || thePlacement == OTCLIB_GT))
	{
	  if (theIndex == index_->population()-1)
	    return -1;

	  return theIndex+1;
	}

	// LAST && (GE || GT) - If at last item in the list return -1,
	// else return last item in the list.

	if (theOccurrence == OTCLIB_LAST
	 && (thePlacement == OTCLIB_GE || thePlacement == OTCLIB_GT))
	{
	  if (theIndex == index_->population()-1)
	    return -1;

	  return index_->population()-1;
	}
      }

      theNode = theNode->right();
      if (theNode != 0)
      {
	fullCount += tmpCount;
	tmpCount = theNode->count();
      }
    }
    else
    {
      u_int theIndex = fullCount + tmpCount - 1;

      // ANY && (LE || EQ || GE) - Found item, this will do.

      if (theOccurrence == OTCLIB_ANY && (thePlacement == OTCLIB_LE
       || thePlacement == OTCLIB_EQ || thePlacement == OTCLIB_GE))
      {
	return theIndex;
      }

      // FIRST && LE - Found item. Can return first item in the list.

      if (theOccurrence == OTCLIB_FIRST && thePlacement == OTCLIB_LE)
	return 0;

      // LAST && GE - Found item. Can return last item in the list.

      if (theOccurrence == OTCLIB_LAST && thePlacement == OTCLIB_GE)
	return index_->population()-1;

      // ANY && LT - Found item. Change this case to LAST && LT.

      if (theOccurrence == OTCLIB_ANY && thePlacement == OTCLIB_LT)
	theOccurrence = OTCLIB_LAST;

      // ANY && GT - Found item. Change this case to FIRST && GT.
      // return -1, else we return last item in the list.

      if (theOccurrence == OTCLIB_ANY && thePlacement == OTCLIB_GT)
	theOccurrence = OTCLIB_FIRST;

      // LAST && LT / FIRST && GT - We need to invert FIRST/LAST for
      // these cases, but remember we have done it so we can check
      // later.

      bool theInverted = false;

      if (theOccurrence == OTCLIB_LAST && thePlacement == OTCLIB_LT)
      {
	// Becomes FIRST && LT.

	theInverted = true;
	theOccurrence = OTCLIB_FIRST;
      }
      else if (theOccurrence == OTCLIB_FIRST && thePlacement == OTCLIB_GT)
      {
	// Becomes LAST && GT.

	theInverted = true;
	theOccurrence = OTCLIB_LAST;
      }

      // FIRST && EQ - FIRST && LT - LAST (INVERTED to FIRST) && LT

      if (theOccurrence == OTCLIB_FIRST)
      {
	OTC_Linkable* theLink = ((OTC_AVLLinkNode*)theNode)->link();
	theLink = theLink->prev();
	while (theLink != 0)
	{
	  if (theLink->isLink() && !theLink->isDead())
	  {
	    T& tmpItem2 = ((OTC_Bucket<T>*)theLink)->item();
	    if (rank(theItem,tmpItem2) != 0)
	      break;
	    theIndex--;
	  }
	  theLink = theLink->prev();
	}

	// FIRST && LT - If item is the first item in the list, we
	// return -1, else we return first item in the list.

	// LAST && LT - If item is the first item in the list, we
	// return -1, else we return item before current. Note
	// that LAST was inverted to FIRST.

	if (thePlacement == OTCLIB_LT)
	{
	  if (theIndex == 0)
	    return -1;

	  if (theInverted != false)
	    return theIndex-1;

	  return 0;
	}

	// FIRST && EQ

	return theIndex;
      }

      // LAST && EQ - LAST && GT - FIRST (INVERTED to LAST) && LT

      if (theOccurrence == OTCLIB_LAST)
      {
	OTC_Linkable* theLink = ((OTC_AVLLinkNode*)theNode)->link();
	theLink = theLink->next();
	while (theLink != 0)
	{
	  if (theLink->isLink() && !theLink->isDead())
	  {
	    T& tmpItem3 = ((OTC_Bucket<T>*)theLink)->item();
	    if (rank(theItem,tmpItem3) != 0)
	      break;
	    theIndex++;
	  }
	  theLink = theLink->next();
	}

	// LAST && GT - If item is the last item in the list, we
	// return -1, else we return last item in the list.

	// FIRST && GT - If item is the last item in the list, we
	// return -1, else we return item after current. Note
	// that FIRST was inverted to LAST.

	if (thePlacement == OTCLIB_GT)
	{
	  if (theIndex == index_->population()-1)
	    return -1;

	  if (theInverted != false)
	    return theIndex+1;

	  return index_->population()-1;
	}

	// LAST && EQ

	return theIndex;
      }

      // Shouldn't get here.

      return theIndex;
    }
  }

  return -1;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Range OTC_OrderedList<T>::range(T const& theItem) const
{
  OTC_AVLLinkNode* theNode;
  OTC_AVLLinkNode* theStartNode;
  OTC_AVLLinkNode* theEndNode;

  theNode = (OTC_AVLLinkNode*)index_->root();

  theNode = range_(theNode,theItem,0,theStartNode,theEndNode);

  if (theNode == 0)
    return 0;

  u_int theLower;
  u_int theLength;

  theLower = theStartNode->index();
  theLength = theEndNode->index() - theLower + 1;
  OTC_Range range(theLower,theLength);

  return range;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Range OTC_OrderedList<T>::range(
 T const& theItem,
 int (*theRankFn)(T const&, T const&)
) const
{
  OTC_AVLLinkNode* theNode;
  OTC_AVLLinkNode* theStartNode;
  OTC_AVLLinkNode* theEndNode;

  theNode = (OTC_AVLLinkNode*)index_->root();

  theNode = range_(theNode,theItem,theRankFn,theStartNode,theEndNode);

  if (theNode == 0)
    return 0;

  u_int theLower;
  u_int theLength;

  theLower = theStartNode->index();
  theLength = theEndNode->index() - theLower + 1;
  OTC_Range range(theLower,theLength);

  return range;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_OrderedList<T>::item(u_int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex < index_->population()),
   "OTC_OrderedList<T>::item(u_int)",
   "index out of range");

  if (theIndex == 0)
    return first();

  else if (theIndex == index_->population()-1)
    return last();

  else
  {
    OTC_AVLNode* theNode = index_->node(theIndex);
    return ((OTC_Bucket<T>*)((OTC_AVLLinkNode*)theNode)->link())->item();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_OrderedList<T>::first() const
{
  OTC_Link* aLink = list_->first();
  OTCLIB_ENSURE_FN((aLink != 0),
   "OTC_OrderedList<T>::first()",
   "collection empty");
 
  return ((OTC_Bucket<T>*)aLink)->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_OrderedList<T>::last() const
{
  OTC_Link* aLink = list_->last();
  OTCLIB_ENSURE_FN((aLink != 0),
   "OTC_OrderedList<T>::last()",
   "collection empty");
 
  return ((OTC_Bucket<T>*)aLink)->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
bool OTC_OrderedList<T>::contains(T const& theItem) const
{
  OTC_AVLNode* theNode = index_->root();
  while (theNode != 0)
  {
    T& tmpItem = ((OTC_Bucket<T>*)
     ((OTC_AVLLinkNode*)theNode)->link())->item();
    int theRank = rank(theItem,tmpItem);
    if (theRank < 0)
      theNode = theNode->left();
    else if (theRank > 0)
      theNode = theNode->right();
    else
      return true;
  }
  return false;
}

/* ------------------------------------------------------------------------- */
template<class T>
u_int OTC_OrderedList<T>::frequency(T const& theItem) const
{
  OTC_AVLNode* theNode = index_->root();
  while (theNode != 0)
  {
    T& tmpItem1 = ((OTC_Bucket<T>*)
     ((OTC_AVLLinkNode*)theNode)->link())->item();
    int theRank = rank(theItem,tmpItem1);
    if (theRank < 0)
      theNode = theNode->left();
    else if (theRank > 0)
      theNode = theNode->right();
    else
    {
      u_int theCount = 1;
      OTC_Linkable* theLink = 0;
      OTC_Link* theStartLink = ((OTC_AVLLinkNode*)theNode)->link();
      theLink = theStartLink->prev();
      while (theLink != 0)
      {
	if (theLink->isLink() && !theLink->isDead())
	{
	  T& tmpItem2 = ((OTC_Bucket<T>*)theLink)->item();
	  if (rank(theItem,tmpItem2) != 0)
	    break;
	  theCount++;
	}
	theLink = theLink->prev();
      }
      theLink = theStartLink->next();
      while (theLink != 0)
      {
	if (theLink->isLink() && !theLink->isDead())
	{
	  T& tmpItem3 = ((OTC_Bucket<T>*)theLink)->item();
	  if (rank(theItem,tmpItem3) != 0)
	    break;
	  theCount++;
	}
	theLink = theLink->next();
      }
      return theCount;
    }
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Iterator<T> OTC_OrderedList<T>::items(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T>* theIter;
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Iterator<T> OTC_OrderedList<T>::items(
 T const& theItem,
 int (*theRankFn)(T const&,T const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_AVLLinkNode* theNode;
  OTC_AVLLinkNode* theStartNode;
  OTC_AVLLinkNode* theEndNode;

  theNode = (OTC_AVLLinkNode*)index_->root();

  theNode = range_(theNode,theItem,theRankFn,theStartNode,theEndNode);

  if (theNode == 0)
    return 0;

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;

  theStartLink = theStartNode->link()->prev();
  theEndLink = theEndNode->link()->next();

  OTC_Anchor* theStartAnchor = 0;
  OTC_Anchor* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
    theStartLink->addAfter(theStartAnchor);
    theStartLink = theStartAnchor;
    theEndLink->addBefore(theEndAnchor);
    theEndLink = theEndAnchor;
  }

  OTC_Cursor<T>* theIter;
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theStartLink,
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
OTC_Cursor<T>* OTC_OrderedList<T>::items_(
 int theStart,
 u_int theLength,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  if (theLength == 0)
    return 0;

  OTCLIB_ENSURE_FN((theStart>=0 && theStart+theLength<=index_->population()),
   "OTC_OrderedList<T>::items_(int, u_int, OTC_Direction, OTC_Protection)",
   "invalid range");

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;

  theStartLink = ((OTC_AVLLinkNode*)index_->node(theStart))->link();
  theEndLink = ((OTC_AVLLinkNode*)index_->node(theStart+theLength-1))->link();

  OTC_Linkable* theStartAnchor = 0;
  OTC_Linkable* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
  }

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
  theIter = new OTC_ItemCursor< T,OTC_Bucket<T> >(list_,theStartLink,
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
OTC_AVLLinkNode* OTC_OrderedList<T>::range_(
 OTC_AVLLinkNode* theRoot,
 T const& theItem,
 int (*theRankFn)(T const&, T const&),
 OTC_AVLLinkNode*& theStartNode,
 OTC_AVLLinkNode*& theEndNode
) const
{
  int theRank;
  OTC_AVLLinkNode* tmpNode;
  OTC_AVLLinkNode* theNode;

  theStartNode = 0;
  theEndNode = 0;

  tmpNode = theRoot;

  while (tmpNode != 0)
  {
    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)tmpNode->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theItem,tmpBucket->item());
    else
      theRank = rank(theItem,tmpBucket->item());

    if (theRank == 0)
      break;
    else if (theRank < 0)
      tmpNode = (OTC_AVLLinkNode*)tmpNode->left();
    else
      tmpNode = (OTC_AVLLinkNode*)tmpNode->right();
  }

  if (tmpNode == 0)
    return 0;

  theNode = tmpNode;

  OTC_AVLLinkNode* tmpStartNode;
  OTC_AVLLinkNode* tmpEndNode;

  tmpStartNode = theNode;
  tmpNode = (OTC_AVLLinkNode*)theNode->prev();

  while (tmpNode != 0)
  {
    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)((OTC_AVLLinkNode*)tmpNode)->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theItem,tmpBucket->item());
    else
      theRank = rank(theItem,tmpBucket->item());

    if (theRank != 0)
      break;

    tmpStartNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->prev();
  }

  tmpEndNode = theNode;
  tmpNode = (OTC_AVLLinkNode*)theNode->next();

  while (tmpNode != 0)
  {
    OTC_Bucket<T>* tmpBucket;
    tmpBucket = (OTC_Bucket<T>*)((OTC_AVLLinkNode*)tmpNode)->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theItem,tmpBucket->item());
    else
      theRank = rank(theItem,tmpBucket->item());

    if (theRank != 0)
      break;

    tmpEndNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->next();
  }

  theStartNode = tmpStartNode;
  theEndNode = tmpEndNode;

  return theNode;
}

/* ------------------------------------------------------------------------- */
template<class T>
int OTC_OrderedList<T>::rank(T const& item1, T const& item2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(item1,item2) :
   OTC_RankActions<T>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ORDLIST_C */
