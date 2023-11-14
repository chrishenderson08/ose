#ifndef OTC_COLLCTN_TUNIQMAP_C
#define OTC_COLLCTN_TUNIQMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/tuniqmap.c
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
OTC_TUniqMap<T1,T2>::~OTC_TUniqMap()
{
  keyIndex_->unReference();
  itemIndex_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TUniqMap<T1,T2>::OTC_TUniqMap(OTC_TUniqMap<T1,T2> const& theMap)
  : keyRankFn_(theMap.keyRankFn_),
    itemRankFn_(theMap.itemRankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  itemIndex_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(itemIndex_ != 0);
  itemIndex_->reference();

  keyIndex_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(keyIndex_ != 0);
  keyIndex_->reference();

  OTC_PairIterator<T1,T2> iter = 0;
  iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    add(iter.key(),iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TUniqMap<T1,T2>::OTC_TUniqMap(
 int (*theKeyRankFn)(T1 const&,T1 const&),
 int (*theItemRankFn)(T2 const&,T2 const&)
)
  : keyRankFn_(theKeyRankFn),
    itemRankFn_(theItemRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  itemIndex_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(itemIndex_ != 0);
  itemIndex_->reference();

  keyIndex_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(keyIndex_ != 0);
  keyIndex_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TUniqMap<T1,T2>::OTC_TUniqMap(
 OTC_TUniqMap<T1,T2> const& theMap,
 OTC_ShallowCopy
)
  : keyRankFn_(theMap.keyRankFn_),
    itemRankFn_(theMap.itemRankFn_)
{
  list_ = theMap.list_;
  list_->reference();

  itemIndex_ = theMap.itemIndex_;
  itemIndex_->reference();

  keyIndex_ = theMap.keyIndex_;
  keyIndex_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TUniqMap<T1,T2>::removeAll()
{
  keyIndex_->removeAll();
  itemIndex_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_TUniqMap<T1,T2>& OTC_TUniqMap<T1,T2>::operator=(
 OTC_TUniqMap<T1,T2> const& theMap
)
{
  if (&theMap != this)
  {
    removeAll();

    keyRankFn_ = theMap.keyRankFn_;
    itemRankFn_ = theMap.itemRankFn_;

    OTC_PairIterator<T1,T2> iter = 0;
    iter = theMap.pairs(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      add(iter.key(),iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TUniqMap<T1,T2>::add(T1 const& theKey, T2 const& theItem)
{
  int theKeyRank;
  int theItemRank;
  OTC_AVLLinkNode* theKeyLinkNode;
  OTC_AVLLinkNode* theItemLinkNode;

  theKeyLinkNode = (OTC_AVLLinkNode*)keyIndex_->root();
  theKeyRank = searchKeyIndex_(theKeyLinkNode,theKey,theKeyLinkNode);

  OTCLIB_ENSURE_FN((theKeyRank != 0),
   "OTC_TUniqMap<T1,T2>::add(T1 const&, T2 const&)",
   "key already exists");

  theItemLinkNode = (OTC_AVLLinkNode*)itemIndex_->root();
  theItemRank = searchItemIndex_(theItemLinkNode,theItem,theItemLinkNode);

  OTCLIB_ENSURE_FN((theItemRank != 0),
   "OTC_TUniqMap<T1,T2>::add(T1 const&, T2 const&)",
   "item already exists");

  OTC_PairBucket<T1,T2>* theLink;
  theLink = new OTC_PairBucket<T1,T2>(theKey,theItem);
  OTCLIB_ASSERT_M(theLink != 0);

  OTC_AVLLinkNode* theNode;
  theNode = new OTC_AVLLinkNode(theLink);
  OTCLIB_ASSERT_M(theNode != 0);

  if (theKeyLinkNode == 0)
  {
    keyIndex_->addRoot(theNode);
    list_->addLast(theLink);
  }
  else if (theKeyRank < 0)
  {
    theKeyLinkNode->addBefore(theNode);
    theKeyLinkNode->link()->addBefore(theLink);
  }
  else
  {
    theKeyLinkNode->addAfter(theNode);
    theKeyLinkNode->link()->addAfter(theLink);
  }

  theNode = new OTC_AVLLinkNode(theLink);
  OTCLIB_ASSERT_M(theNode != 0);

  if (theItemLinkNode == 0)
  {
    itemIndex_->addRoot(theNode);
  }
  else if (theItemRank < 0)
  {
    theItemLinkNode->addBefore(theNode);
  }
  else
  {
    theItemLinkNode->addAfter(theNode);
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TUniqMap<T1,T2>::add(OTC_PairIterator<T1,T2> thePairs)
{
  while (thePairs.isValid())
  {
    add(thePairs.key(),thePairs.item());

    thePairs.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TUniqMap<T1,T2>::removeKey(
 T1 const& theKey,
 OTC_Protection theProtection
)
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)keyIndex_->root();
  theRank = searchKeyIndex_(theNode,theKey,theNode);

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TUniqMap<T1,T2>::removeKey(T1 const&, OTC_Protection)",
   "key doesn't exist");

  if (theRank != 0)
    return;

  OTC_PairBucket<T1,T2>* theLink;
  theLink = (OTC_PairBucket<T1,T2>*)theNode->link();

  theNode->unlink();
  delete theNode;

  theNode = (OTC_AVLLinkNode*)itemIndex_->root();
  theRank = searchItemIndex_(theNode,theLink->item(),theNode);
  OTCLIB_ASSERT(theRank == 0);

  theNode->unlink();
  delete theNode;

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_TUniqMap<T1,T2>::removeItem(
 T2 const& theItem,
 OTC_Protection theProtection
)
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)itemIndex_->root();
  theRank = searchItemIndex_(theNode,theItem,theNode);

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE || theRank == 0),
   "OTC_TUniqMap<T1,T2>::removeItem(T2 const&, OTC_Protection)",
   "item doesn't exist");

  if (theRank != 0)
    return;

  OTC_PairBucket<T1,T2>* theLink;
  theLink = (OTC_PairBucket<T1,T2>*)theNode->link();

  theNode->unlink();
  delete theNode;

  theNode = (OTC_AVLLinkNode*)keyIndex_->root();
  theRank = searchKeyIndex_(theNode,theLink->key(),theNode);
  OTCLIB_ASSERT(theRank == 0);

  theNode->unlink();
  delete theNode;

  theLink->kill();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_TUniqMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((keyIndex_->population() != 0),
   "OTC_TUniqMap<T1,T2>::pickKey()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_TUniqMap<T1,T2>::pickItem() const
{
  OTCLIB_ENSURE_FN((population() != 0),
   "OTC_TUniqMap<T1,T2>::pickItem()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_TUniqMap<T1,T2>::containsKey(T1 const& theKey) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)keyIndex_->root();

  return searchKeyIndex_(theNode,theKey,theNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_TUniqMap<T1,T2>::containsItem(T2 const& theItem) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)itemIndex_->root();

  return searchItemIndex_(theNode,theItem,theNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_TUniqMap<T1,T2>::item(T1 const& theKey) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)keyIndex_->root();
  theRank = searchKeyIndex_(theNode,theKey,theNode);

  OTCLIB_ENSURE_FN((theRank == 0),
   "OTC_TUniqMap<T1,T2>::item(T1 const&)",
   "key doesn't exist");

  return ((OTC_PairBucket<T1,T2>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const* OTC_TUniqMap<T1,T2>::pItem(T1 const& theKey) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)keyIndex_->root();
  theRank = searchKeyIndex_(theNode,theKey,theNode);

  if (theRank != 0)
    return 0;

  return &((OTC_PairBucket<T1,T2>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_TUniqMap<T1,T2>::key(T2 const& theItem) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)itemIndex_->root();
  theRank = searchItemIndex_(theNode,theItem,theNode);

  OTCLIB_ENSURE_FN((theRank == 0),
   "OTC_TUniqMap<T1,T2>::key(T2 const&)",
   "item doesn't exist");

  return ((OTC_PairBucket<T1,T2>*)theNode->link())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const* OTC_TUniqMap<T1,T2>::pKey(T2 const& theItem) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)itemIndex_->root();
  theRank = searchItemIndex_(theNode,theItem,theNode);

  if (theRank != 0)
    return 0;

  return &((OTC_PairBucket<T1,T2>*)theNode->link())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T1> OTC_TUniqMap<T1,T2>::keys(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T1>* theIter;
  theIter = new OTC_KeyCursor< T1,OTC_PairBucket<T1,T2> >(list_,theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_Iterator<T2> OTC_TUniqMap<T1,T2>::items(
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_Cursor<T2>* theIter;
  theIter = new OTC_ItemCursor< T2,OTC_PairBucket<T1,T2> >(list_,theDirection,
   theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_PairIterator<T1,T2> OTC_TUniqMap<T1,T2>::pairs(
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
OTC_Iterator<T1> OTC_TUniqMap<T1,T2>::keys(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  int theRank;
  OTC_AVLLinkNode* tmpNode;
  OTC_AVLLinkNode* theNode;

  tmpNode = (OTC_AVLLinkNode*)keyIndex_->root();

  while (tmpNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)tmpNode->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rankKey(theKey,tmpBucket->key());

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
      theRank = rankKey(theKey,tmpBucket->key());

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
      theRank = rankKey(theKey,tmpBucket->key());

    if (theRank != 0)
      break;

    tmpEndNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->next();
  }

  OTC_Anchor* theStartAnchor = 0;
  OTC_Anchor* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
  }

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;

  theStartLink = tmpStartNode->link();
  theEndLink = tmpEndNode->link();

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
OTC_Iterator<T2> OTC_TUniqMap<T1,T2>::items(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  int theRank;
  OTC_AVLLinkNode* tmpNode;
  OTC_AVLLinkNode* theNode;

  tmpNode = (OTC_AVLLinkNode*)keyIndex_->root();

  while (tmpNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)tmpNode->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rankKey(theKey,tmpBucket->key());

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
      theRank = rankKey(theKey,tmpBucket->key());

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
      theRank = rankKey(theKey,tmpBucket->key());

    if (theRank != 0)
      break;

    tmpEndNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->next();
  }

  OTC_Anchor* theStartAnchor = 0;
  OTC_Anchor* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
  }

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;

  theStartLink = tmpStartNode->link();
  theEndLink = tmpEndNode->link();

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
OTC_PairIterator<T1,T2> OTC_TUniqMap<T1,T2>::pairsKey(
 T1 const& theKey,
 int (*theRankFn)(T1 const&, T1 const&),
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  int theRank;
  OTC_AVLLinkNode* tmpNode;
  OTC_AVLLinkNode* theNode;

  tmpNode = (OTC_AVLLinkNode*)keyIndex_->root();

  while (tmpNode != 0)
  {
    OTC_PairBucket<T1,T2>* tmpBucket;
    tmpBucket = (OTC_PairBucket<T1,T2>*)tmpNode->link();

    if (theRankFn != 0)
      theRank = (*theRankFn)(theKey,tmpBucket->key());
    else
      theRank = rankKey(theKey,tmpBucket->key());

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
      theRank = rankKey(theKey,tmpBucket->key());

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
      theRank = rankKey(theKey,tmpBucket->key());

    if (theRank != 0)
      break;

    tmpEndNode = tmpNode;
    tmpNode = (OTC_AVLLinkNode*)tmpNode->next();
  }

  OTC_Anchor* theStartAnchor = 0;
  OTC_Anchor* theEndAnchor = 0;

  if (theProtection == OTCLIB_SAFE)
  {
    theStartAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theStartAnchor != 0);
    theEndAnchor = new OTC_Anchor;
    OTCLIB_ASSERT_M(theEndAnchor != 0);
  }

  OTC_Linkable* theStartLink;
  OTC_Linkable* theEndLink;

  theStartLink = tmpStartNode->link();
  theEndLink = tmpEndNode->link();

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
int OTC_TUniqMap<T1,T2>::rankKey(T1 const& key1, T1 const& key2) const
{
  return (keyRankFn_ != 0) ? (*keyRankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TUniqMap<T1,T2>::rankItem(T2 const& item1, T2 const& item2) const
{
  return (itemRankFn_ != 0) ? (*itemRankFn_)(item1,item2) :
   OTC_RankActions<T2>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TUniqMap<T1,T2>::searchKeyIndex_(
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
    theNode = (OTC_AVLLinkNode*)tmpNode;
    return 0;
  }

  theNode = (OTC_AVLLinkNode*)tmpParent;
  return theRank;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_TUniqMap<T1,T2>::searchItemIndex_(
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

#endif /* OTC_COLLCTN_TUNIQMAP_C */
