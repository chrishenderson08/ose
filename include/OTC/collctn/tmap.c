#ifndef OTC_COLLCTN_TMAP_C
#define OTC_COLLCTN_TMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/tmap.c
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/kcursor.hh>
#include <OTC/collctn/icursor.hh>
#include <OTC/collctn/kicursor.hh>
#include <OTC/collctn/prbucket.hh>

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TMap<T1,T2>::~OTC_TMap()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TMap<T1,T2>::OTC_TMap(OTC_TMap<T1,T2> const& theMap)
  : rankFn_(theMap.rankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  OTC_PairIterator<T1,T2> iter = 0;
  iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    add(iter.key(),iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TMap<T1,T2>::OTC_TMap(int (*theRankFn)(T1 const&,T1 const&))
  : rankFn_(theRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TMap<T1,T2>::OTC_TMap(OTC_TMap<T1,T2> const& theMap, OTC_ShallowCopy)
  : rankFn_(theMap.rankFn_)
{
  list_ = theMap.list_;
  list_->reference();

  index_ = theMap.index_;
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TMap<T1,T2>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TMap<T1,T2>& OTC_TMap<T1,T2>::operator=(OTC_TMap<T1,T2> const& theMap)
{
  if (&theMap != this)
  {
    removeAll();

    rankFn_ = theMap.rankFn_;
    OTC_PairIterator<T1,T2> iter = 0;
    iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      add(iter.key(),iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TMap<T1,T2>::add(
 T1 const& theKey,
 T2 const& theItem,
 OTC_Replacement theFlag
)
{
  int theRank;
  OTC_AVLLinkNode* theLinkNode;

  theLinkNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndexKey_(theLinkNode,theKey,theLinkNode);

  OTCLIB_ENSURE_FN((theRank != 0 || theFlag == OTCLIB_REPLACE),
   "OTC_TMap<T1,T2>::add(T1 const&, T2 const&, OTC_Replacement)",
   "key already exists");

  OTC_PairBucket<T1,T2>* theLink;
  theLink = new OTC_PairBucket<T1,T2>(theKey,theItem);
  OTCLIB_ASSERT_M(theLink != 0);

  if (theLinkNode != 0 && theRank == 0)
  {
    theLinkNode->link()->addBefore(theLink);
    theLinkNode->link()->kill();
    theLinkNode->setLink(theLink);
  }
  else
  {
    OTC_AVLLinkNode* theNode;
    theNode = new OTC_AVLLinkNode(theLink);
    OTCLIB_ASSERT_M(theNode != 0);

    if (theLinkNode == 0)
    {
      index_->addRoot(theNode);
      list_->addLast(theLink);
    }
    else if (theRank < 0)
    {
      theLinkNode->addBefore(theNode);
      theLinkNode->link()->addBefore(theLink);
    }
    else
    {
      theLinkNode->addAfter(theNode);
      theLinkNode->link()->addAfter(theLink);
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TMap<T1,T2>::add(
 OTC_PairIterator<T1,T2> thePairs,
 OTC_Replacement theFlag
)
{
  while (thePairs.isValid())
  {
    add(thePairs.key(),thePairs.item(),theFlag);

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TMap<T1,T2>::remove(T1 const& theKey, OTC_Protection theFlag)
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndexKey_(theNode,theKey,theNode);

  OTCLIB_ENSURE_FN((theFlag == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TMap<T1,T2>::remove(T1 const&, OTC_Protection)",
   "key doesn't exist");

  if (theRank != 0)
    return;

  OTC_PairBucket<T1,T2>* theLink;
  theLink = (OTC_PairBucket<T1,T2>*)theNode->link();

  theNode->unlink();
  delete theNode;

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_TMap<T1,T2>::contains(T1 const& theKey) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)index_->root();

  return searchIndexKey_(theNode,theKey,theNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2& OTC_TMap<T1,T2>::item_(T1 const& theKey) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndexKey_(theNode,theKey,theNode);

  OTCLIB_ENSURE_FN((theRank == 0),
   "OTC_TMap<T1,T2>::item_(T1 const&)",
   "key doesn't exist");

  return ((OTC_PairBucket<T1,T2>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2* OTC_TMap<T1,T2>::pItem_(T1 const& theKey) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndexKey_(theNode,theKey,theNode);

  if (theRank != 0)
    return 0;

  return &((OTC_PairBucket<T1,T2>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_TMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_TMap<T1,T2>::pickKey()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_TMap<T1,T2>::pickItem() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_TMap<T1,T2>::pickItem()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T1> OTC_TMap<T1,T2>::keys(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T1>* theIter;
  theIter = new OTC_KeyCursor< T1,OTC_PairBucket<T1,T2> >(list_,
   theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T1> OTC_TMap<T1,T2>::keys(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_AVLLinkNode* theNode;
  OTC_AVLLinkNode* theStartNode;
  OTC_AVLLinkNode* theEndNode;

  theNode = (OTC_AVLLinkNode*)index_->root();

  theNode = rangeKey_(theNode,theKey,theRankFn,theStartNode,theEndNode);

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

  OTC_Cursor<T1>* theIter;
  theIter = new OTC_KeyCursor< T1,OTC_PairBucket<T1,T2> >(list_,theStartLink,
   theEndLink,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Cursor<T2>* OTC_TMap<T1,T2>::items_(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T2>* theIter;
  theIter = new OTC_ItemCursor< T2,OTC_PairBucket<T1,T2> >(list_,
   theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Cursor<T2>* OTC_TMap<T1,T2>::items_(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_AVLLinkNode* theNode;
  OTC_AVLLinkNode* theStartNode;
  OTC_AVLLinkNode* theEndNode;

  theNode = (OTC_AVLLinkNode*)index_->root();

  theNode = rangeKey_(theNode,theKey,theRankFn,theStartNode,theEndNode);

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

  OTC_Cursor<T2>* theIter;
  theIter = new OTC_ItemCursor< T2,OTC_PairBucket<T1,T2> >(list_,theStartLink,
   theEndLink,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairCursor<T1,T2>* OTC_TMap<T1,T2>::pairs_(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_PairCursor<T1,T2>* theIter;
  theIter = new OTC_KeyItemCursor< T1,T2,OTC_PairBucket<T1,T2> >(list_,
   theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}
/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairCursor<T1,T2>* OTC_TMap<T1,T2>::pairsKey_(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_AVLLinkNode* theNode;
  OTC_AVLLinkNode* theStartNode;
  OTC_AVLLinkNode* theEndNode;

  theNode = (OTC_AVLLinkNode*)index_->root();

  theNode = rangeKey_(theNode,theKey,theRankFn,theStartNode,theEndNode);

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

  OTC_PairCursor<T1,T2>* theIter;
  theIter = new OTC_KeyItemCursor< T1,T2,OTC_PairBucket<T1,T2> >(list_,
   theStartLink,theEndLink,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);

  if (theStartAnchor != 0)
    theStartAnchor->kill();
  if (theEndAnchor != 0)
    theEndAnchor->kill();

  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TMap<T1,T2>::rank(T1 const& key1, T1 const& key2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_AVLLinkNode* OTC_TMap<T1,T2>::rangeKey_(
 OTC_AVLLinkNode* theRoot,
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
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
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)tmpNode->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rank(theKey,tmpBucket->key());

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
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)((OTC_AVLLinkNode*)tmpNode)->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rank(theKey,tmpBucket->key());

    if (theRank != 0)
      break;

    tmpStartNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->prev();
  }

  tmpEndNode = theNode;
  tmpNode = (OTC_AVLLinkNode*)theNode->next();

  while (tmpNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)((OTC_AVLLinkNode*)tmpNode)->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rank(theKey,tmpBucket->key());

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
template<class T1, class T2>
int OTC_TMap<T1,T2>::searchIndexKey_(
 OTC_AVLLinkNode* theRoot,
 T1 const& theKey,
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

    theRank = rank(theKey,tmpBucket->key());
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

#endif /* OTC_COLLCTN_TMAP_C */
