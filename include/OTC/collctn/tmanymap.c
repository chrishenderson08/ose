#ifndef OTC_COLLCTN_TMANYMAP_C
#define OTC_COLLCTN_TMANYMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/tmanymap.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/prbucket.hh>

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TManyMap<T1,T2>::~OTC_TManyMap()
{
  count_->unReference();
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TManyMap<T1,T2>::OTC_TManyMap(
 int (*theKeyRankFn)(T1 const&,T1 const&),
 int (*theItemRankFn)(T2 const&,T2 const&)
)
  : keyRankFn_(theKeyRankFn), itemRankFn_(theItemRankFn)
{
  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  count_ = new OTC_RCount;
  OTCLIB_ASSERT_M(count_ != 0);
  count_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TManyMap<T1,T2>::OTC_TManyMap(OTC_TManyMap<T1,T2> const& theMap)
  : keyRankFn_(theMap.keyRankFn_), itemRankFn_(theMap.itemRankFn_)
{
  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  count_ = new OTC_RCount;
  OTCLIB_ASSERT_M(count_ != 0);
  count_->reference();

  OTC_PairIterator<T1,T2> thePairs;
  thePairs = theMap.pairs();
  while (thePairs.isValid())
  {
    add(thePairs.key(),thePairs.item());
    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TManyMap<T1,T2>::OTC_TManyMap(
 OTC_TManyMap<T1,T2> const& theMap,
 OTC_ShallowCopy
)
  : keyRankFn_(theMap.keyRankFn_), itemRankFn_(theMap.itemRankFn_)
{
  index_ = theMap.index_;
  index_->reference();

  list_ = theMap.list_;
  list_->reference();

  count_ = theMap.count_;
  count_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TManyMap<T1,T2>::removeAll()
{
  count_->set(0);
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
u_int OTC_TManyMap<T1,T2>::frequencyKey(T1 const& theKey) const
{
  OTC_AVLTreeNode* theIndexNode;
  theIndexNode = (OTC_AVLTreeNode*)index_->root();

  if (searchIndexKey_(theIndexNode,theKey,theIndexNode) == 0)
    return theIndexNode->index()->population();

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_TManyMap<T1,T2>::containsKey(T1 const& theKey) const
{
  OTC_AVLTreeNode* theIndexNode;
  theIndexNode = (OTC_AVLTreeNode*)index_->root();

  return searchIndexKey_(theIndexNode,theKey,theIndexNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_TManyMap<T1,T2>::contains(
 T1 const& theKey,
 T2 const& theItem
) const
{
  OTC_AVLTreeNode* theIndexNode;
  theIndexNode = (OTC_AVLTreeNode*)index_->root();

  if (searchIndexKey_(theIndexNode,theKey,theIndexNode) != 0)
    return false;

  OTC_AVLLinkNode* theLinkNode;
  theLinkNode = (OTC_AVLLinkNode*)theIndexNode->index()->root();

  return searchGroupItem_(theLinkNode,theItem,theLinkNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TManyMap<T1,T2>& OTC_TManyMap<T1,T2>::operator=(
 OTC_TManyMap<T1,T2> const& theMap
)
{
  if (&theMap != this)
  {
    removeAll();

    OTC_PairIterator<T1,T2> thePairs;
    thePairs = theMap.pairs();
    while (thePairs.isValid())
    {
      add(thePairs.key(),thePairs.item());
      thePairs.next();
    }
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TManyMap<T1,T2>::add(T1 const& theKey, T2 const& theItem)
{
  // Search for key.

  OTC_AVLTreeNode* theIndexNode;
  theIndexNode = (OTC_AVLTreeNode*)index_->root();

  int theKeyRank;
  theKeyRank = searchIndexKey_(theIndexNode,theKey,theIndexNode);

  if (theKeyRank != 0)
  {
    // Key does not exist.

    OTC_AVLTreeNode* theNewIndexNode;
    theNewIndexNode = new OTC_AVLTreeNode;
    OTCLIB_ASSERT_M(theNewIndexNode != 0);

    if (theIndexNode == 0)
    {
      index_->addRoot(theNewIndexNode);
    }
    else if (theKeyRank < 0)
    {
      theIndexNode->addBefore(theNewIndexNode);
    }
    else
    {
      theIndexNode->addAfter(theNewIndexNode);
    }

    theIndexNode = theNewIndexNode;
  }

  // Search for item.

  OTC_AVLLinkNode* theLinkNode;
  theLinkNode = (OTC_AVLLinkNode*)theIndexNode->index()->root();

  int theItemRank;
  theItemRank = searchGroupItem_(theLinkNode,theItem,theLinkNode);

  OTCLIB_ENSURE_FN((theItemRank != 0),
   "OTC_TManyMap<T1,T2>::add(T1 const&, T2 const&)",
   "key/item pair exists");

  // Item does not exist.

  OTC_Link* theLink;
  theLink = link_(theKey,theItem);

  OTC_AVLLinkNode* theNewLinkNode;
  theNewLinkNode = new OTC_AVLLinkNode(theLink);
  OTCLIB_ASSERT_M(theNewLinkNode != 0);

  if (theLinkNode == 0)
  {
    theIndexNode->index()->addRoot(theNewLinkNode);

    if (theIndexNode->prev() != 0)
    {
      OTC_AVLTreeNode* theOldIndexNode;
      theOldIndexNode = (OTC_AVLTreeNode*)theIndexNode->prev();

      OTC_AVLLinkNode* theOldLinkNode;
      theOldLinkNode = (OTC_AVLLinkNode*)theOldIndexNode->index()->last();

      theOldLinkNode->link()->addAfter(theLink);
    }
    else if (theIndexNode->next() != 0)
    {
      OTC_AVLTreeNode* theOldIndexNode;
      theOldIndexNode = (OTC_AVLTreeNode*)theIndexNode->next();

      OTC_AVLLinkNode* theOldLinkNode;
      theOldLinkNode = (OTC_AVLLinkNode*)theOldIndexNode->index()->first();

      theOldLinkNode->link()->addBefore(theLink);
    }
    else
      list_->addLast(theLink);
  }
  else if (theItemRank < 0)
  {
    theLinkNode->addBefore(theNewLinkNode);
    theLinkNode->link()->addBefore(theLink);
  }
  else
  {
    theLinkNode->addAfter(theNewLinkNode);
    theLinkNode->link()->addAfter(theLink);
  }

  count_->inc();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TManyMap<T1,T2>::add(OTC_PairIterator<T1,T2> thePairs)
{
  while (thePairs.isValid())
  {
    add(thePairs.key(),thePairs.item());

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairIterator<T1,T2> OTC_TManyMap<T1,T2>::pairs(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_PairCursor<T1,T2>* theIter;
  theIter = new OTC_KeyItemCursor< T1,T2,OTC_PairBucket<T1,T2> >(
   list_,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Cursor<T2>* OTC_TManyMap<T1,T2>::items_(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_AVLTreeNode* theNode;
  OTC_AVLTreeNode* theStartNode;
  OTC_AVLTreeNode* theEndNode;

  theNode = (OTC_AVLTreeNode*)index_->root();

  theNode = rangeIndexKey_(theNode,theKey,theRankFn,theStartNode,theEndNode);

  if (theNode == 0)
    return 0;

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;
  OTC_AVLLinkNode* theStartLinkNode;
  OTC_AVLLinkNode* theEndLinkNode;

  theStartLinkNode = (OTC_AVLLinkNode*)theStartNode->index()->first();
  theEndLinkNode = (OTC_AVLLinkNode*)theEndNode->index()->last();

  theStartLink = theStartLinkNode->link()->prev();
  theEndLink = theEndLinkNode->link()->next();

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

  OTC_Cursor<T2>* theIter;
  theIter = new OTC_ItemCursor< T2,OTC_PairBucket<T1,T2> >(
   list_,theStartLink,theEndLink,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairCursor<T1,T2>* OTC_TManyMap<T1,T2>::pairsKey_(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_AVLTreeNode* theNode;
  OTC_AVLTreeNode* theStartNode;
  OTC_AVLTreeNode* theEndNode;

  theNode = (OTC_AVLTreeNode*)index_->root();

  theNode = rangeIndexKey_(theNode,theKey,theRankFn,theStartNode,theEndNode);

  if (theNode == 0)
    return 0;

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;
  OTC_AVLLinkNode* theStartLinkNode;
  OTC_AVLLinkNode* theEndLinkNode;

  theStartLinkNode = (OTC_AVLLinkNode*)theStartNode->index()->first();
  theEndLinkNode = (OTC_AVLLinkNode*)theEndNode->index()->last();

  theStartLink = theStartLinkNode->link()->prev();
  theEndLink = theEndLinkNode->link()->next();

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

  OTC_PairCursor<T1,T2>* theIter;
  theIter = new OTC_KeyItemCursor< T1,T2,OTC_PairBucket<T1,T2> >(
   list_,theStartLink,theEndLink,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_TManyMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((population() != 0),
   "OTC_TManyMap<T1,T2>::pickKey()",
   "collection must not be empty");
 
  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TManyMap<T1,T2>::removeKey(
 T1 const& theKey,
 OTC_Protection theProtection
)
{
  // Do the deletion ourselves rather than
  // getting an iterator and deleting one
  // at a time.

  OTC_AVLTreeNode* theIndexNode;
  theIndexNode = (OTC_AVLTreeNode*)index_->root();

  int theRank;
  theRank = searchIndexKey_(theIndexNode,theKey,theIndexNode);

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TManyMap<T1,T2>::removeKey(T1 const&, OTC_Protection)",
   "key doesn't exist");

  if (theRank != 0)
    return;

  OTC_AVLNode* theNode;
  theNode = theIndexNode->index()->root()->first();

  OTC_Link* theLink;

  while (theNode != 0)
  {
    theLink = ((OTC_AVLLinkNode*)theNode)->link();
    theLink->kill();

    count_->dec();

    theNode = theNode->next();
  }

  theIndexNode->unlink();
  delete theIndexNode;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TManyMap<T1,T2>::remove(
 T1 const& theKey,
 T2 const& theItem,
 OTC_Protection theProtection
)
{
  OTC_AVLTreeNode* theIndexNode;
  theIndexNode = (OTC_AVLTreeNode*)index_->root();

  int theRank;
  theRank = searchIndexKey_(theIndexNode,theKey,theIndexNode);

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TManyMap<T1,T2>::remove(T1 const&, T2 const&, OTC_Protection)",
   "key doesn't exist");

  if (theRank != 0)
    return;

  OTC_AVLLinkNode* theLinkNode;
  theLinkNode = (OTC_AVLLinkNode*)theIndexNode->index()->root();

  theRank = searchGroupItem_(theLinkNode,theItem,theLinkNode);

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TManyMap<T1,T2>::remove(T1 const&, T2 const&, OTC_Protection)",
   "key/item pair doesn't exist");

  if (theRank != 0)
    return;

  OTC_Link* theLink;
  theLink = theLinkNode->link();
  theLink->kill();

  theLinkNode->unlink();
  delete theLinkNode;

  if (theIndexNode->index()->isEmpty())
  {
    theIndexNode->unlink();
    delete theIndexNode;
  }

  count_->dec();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TManyMap<T1,T2>::rankKey(T1 const& key1, T1 const& key2) const
{
  return (keyRankFn_ != 0) ? (*keyRankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}
 
/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TManyMap<T1,T2>::rankItem(T2 const& item1, T2 const& item2) const
{
  return (itemRankFn_ != 0) ? (*itemRankFn_)(item1,item2) :
   OTC_RankActions<T2>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_AVLTreeNode* OTC_TManyMap<T1,T2>::rangeIndexKey_(
 OTC_AVLTreeNode* theRoot,
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_AVLTreeNode*& theStartNode,
 OTC_AVLTreeNode*& theEndNode
) const
{
  int theRank;
  OTC_AVLTreeNode* tmpIndexNode;
  OTC_AVLTreeNode* theIndexNode;

  theStartNode = 0;
  theEndNode = 0;

  tmpIndexNode = theRoot;

  while (tmpIndexNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)
     ((OTC_AVLLinkNode*)tmpIndexNode->index()->root())->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rankKey(theKey,tmpBucket->key());

    if (theRank == 0)
      break;
    else if (theRank < 0)
      tmpIndexNode = (OTC_AVLTreeNode*)tmpIndexNode->left();
    else
      tmpIndexNode = (OTC_AVLTreeNode*)tmpIndexNode->right();
  }

  if (tmpIndexNode == 0)
    return 0;

  theIndexNode = tmpIndexNode;

  OTC_AVLTreeNode* tmpStartNode;
  OTC_AVLTreeNode* tmpEndNode;

  tmpStartNode = theIndexNode;
  tmpIndexNode = (OTC_AVLTreeNode*)theIndexNode->prev();

  while (tmpIndexNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)
     ((OTC_AVLLinkNode*)tmpIndexNode->index()->root())->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rankKey(theKey,tmpBucket->key());

    if (theRank != 0)
      break;

    tmpStartNode = tmpIndexNode;
    tmpIndexNode = (OTC_AVLTreeNode*)tmpIndexNode->prev();
  }

  tmpEndNode = theIndexNode;
  tmpIndexNode = (OTC_AVLTreeNode*)theIndexNode->next();

  while (tmpIndexNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)
     ((OTC_AVLLinkNode*)tmpIndexNode->index()->root())->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rankKey(theKey,tmpBucket->key());

    if (theRank != 0)
      break;

    tmpEndNode = tmpIndexNode;
    tmpIndexNode = (OTC_AVLTreeNode*)tmpIndexNode->next();
  }

  theStartNode = tmpStartNode;
  theEndNode = tmpEndNode;

  return theIndexNode;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TManyMap<T1,T2>::searchIndexKey_(
 OTC_AVLTreeNode* theRoot,
 T1 const& theKey,
 OTC_AVLTreeNode*& theNode
) const
{
  theNode = 0;

  OTC_AVLNode* tmpNode = 0;
  OTC_AVLNode* tmpParent = 0;
  int theRank = 0;

  if (theRoot == 0)
    return 1;

  tmpNode = theRoot;

  while (tmpNode != 0)
  {
    tmpParent = tmpNode;

    OTC_AVLNode* tmpRoot;
    tmpRoot = ((OTC_AVLTreeNode*)tmpNode)->index()->root();
    OTCLIB_ASSERT(tmpRoot != 0);

    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)((OTC_AVLLinkNode*)tmpRoot)->link();

    theRank = rankKey(theKey,tmpBucket->key());
    if (theRank == 0)
      break;
    else if (theRank < 0)
      tmpNode = tmpNode->left();
    else
      tmpNode = tmpNode->right();
  }

  if (tmpNode != 0)
  {
    theNode = (OTC_AVLTreeNode*)tmpNode;
    return 0;
  }

  theNode = (OTC_AVLTreeNode*)tmpParent;
  return theRank;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TManyMap<T1,T2>::searchGroupItem_(
 OTC_AVLLinkNode* theRoot,
 T2 const& theItem,
 OTC_AVLLinkNode*& theNode
) const
{
  theNode = 0;

  OTC_AVLNode* tmpNode = 0;
  OTC_AVLNode* tmpParent = 0;
  int theRank = 0;

  if (theRoot == 0)
    return 1;

  tmpNode = theRoot;

  while (tmpNode != 0)
  {
    tmpParent = tmpNode;

    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)((OTC_AVLLinkNode*)tmpNode)->link();

    theRank = rankItem(theItem,tmpBucket->item());
    if (theRank == 0)
      break;
    else if (theRank < 0)
      tmpNode = tmpNode->left();
    else
      tmpNode = tmpNode->right();
  }

  if (tmpNode != 0)
  {
    theNode = (OTC_AVLLinkNode*)tmpNode;
    return 0;
  }

  theNode = (OTC_AVLLinkNode*)tmpParent;
  return theRank;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Link* OTC_TManyMap<T1,T2>::link_(T1 const& theKey, T2 const& theItem)
{
  OTC_PairBucket<T1,T2>* theBucket;
  theBucket = new OTC_PairBucket<T1,T2>(theKey,theItem);
  OTCLIB_ASSERT_M(theBucket != 0);

  return theBucket;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TMANYMAP_C */
