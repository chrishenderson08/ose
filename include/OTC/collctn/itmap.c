#ifndef OTC_COLLCTN_ITMAP_C
#define OTC_COLLCTN_ITMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/itmap.c
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
OTC_ITMap<T1,T2>::~OTC_ITMap()
{
  index_->unReference();
  list_->unReference();

  delete iter_;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_ITMap<T1,T2>::OTC_ITMap(OTC_ITMap<T1,T2> const& theMap)
  : rankFn_(theMap.rankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  iter_ = new OTC_LinkIterator(list_->items());
  OTCLIB_ASSERT_M(iter_ != 0);

  OTC_LinkIterator iter = theMap.list_->items(OTCLIB_UNSAFE);
  for (iter.resetFirst(); iter.isLink(); iter.next())
  {
    T1& theKey = ((OTC_PairBucket<T1,T2>*)iter.link())->key();
    T2& theItem = ((OTC_PairBucket<T1,T2>*)iter.link())->item();
    add(theKey,theItem);
  }
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_ITMap<T1,T2>::OTC_ITMap(int (*theRankFn)(T1 const&,T1 const&))
  : rankFn_(theRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_AVLTree;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  iter_ = new OTC_LinkIterator(list_->items());
  OTCLIB_ASSERT_M(iter_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_ITMap<T1,T2>::OTC_ITMap(OTC_ITMap<T1,T2> const& theMap, OTC_ShallowCopy)
  : rankFn_(theMap.rankFn_)
{
  list_ = theMap.list_;
  list_->reference();

  index_ = theMap.index_;
  index_->reference();

  iter_ = new OTC_LinkIterator(list_->items());
  OTCLIB_ASSERT_M(iter_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_ITMap<T1,T2>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_ITMap<T1,T2>& OTC_ITMap<T1,T2>::operator=(OTC_ITMap<T1,T2> const& theMap)
{
  if (&theMap != this)
  {
    removeAll();

    rankFn_ = theMap.rankFn_;
    OTC_LinkIterator iter = theMap.list_->items(OTCLIB_UNSAFE);
    for (iter.resetFirst(); iter.isLink(); iter.next())
    {
      T1& theKey = ((OTC_PairBucket<T1,T2>*)iter.link())->key();
      T2& theItem = ((OTC_PairBucket<T1,T2>*)iter.link())->item();
      add(theKey,theItem);
    }
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_ITMap<T1,T2>::add(T1 const& theKey, T2 const& theItem)
{
  int theRank;
  OTC_AVLLinkNode* theLinkNode;

  theLinkNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndex_(theLinkNode,theKey,theLinkNode);

  OTCLIB_ENSURE_FN((theRank != 0),
   "OTC_ITMap<T1,T2>::add(T1 const&, T2 const&)",
   "key already exists");

  OTC_PairBucket<T1,T2>* theLink;
  theLink = new OTC_PairBucket<T1,T2>(theKey,theItem);
  OTCLIB_ASSERT_M(theLink != 0);

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

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_ITMap<T1,T2>::remove(T1 const& theKey, OTC_Protection theProtection)
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndex_(theNode,theKey,theNode);

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE || theRank == 0),
   "OTC_ITMap<T1,T2>::remove(T1 const&, OTC_Protection)",
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
bool OTC_ITMap<T1,T2>::contains(T1 const& theKey) const
{
  OTC_AVLLinkNode* theNode;
  theNode = (OTC_AVLLinkNode*)index_->root();

  return searchIndex_(theNode,theKey,theNode) == 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_ITMap<T1,T2>::remove()
{
  OTCLIB_ENSURE_FN((isValid() != false),
   "OTC_ITMap<T1,T2>::remove()",
   "iterator not located on valid key/item pair");

  remove(key_());
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2& OTC_ITMap<T1,T2>::item_(T1 const& theKey) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndex_(theNode,theKey,theNode);

  OTCLIB_ENSURE_FN((theRank == 0),
   "OTC_ITMap<T1,T2>::item_(T1 const&)",
   "key doesn't exist");

  return ((OTC_PairBucket<T1,T2>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2* OTC_ITMap<T1,T2>::pItem_(T1 const& theKey) const
{
  int theRank;
  OTC_AVLLinkNode* theNode;

  theNode = (OTC_AVLLinkNode*)index_->root();
  theRank = searchIndex_(theNode,theKey,theNode);

  if (theRank != 0)
    return 0;

  return &((OTC_PairBucket<T1,T2>*)theNode->link())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_ITMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_ITMap<T1,T2>::pickKey()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_ITMap<T1,T2>::pickItem() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_ITMap<T1,T2>::pickItem()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_ITMap<T1,T2>::rank(T1 const& key1, T1 const& key2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1& OTC_ITMap<T1,T2>::key_() const
{
  OTCLIB_ENSURE_FN((iter_->isLink() && !iter_->link()->isDead()),
   "OTC_ITMap<T1,T2>::key_()",
   "no key/item pair under iterator");

  OTC_PairBucket<T1,T2>* aBucket = (OTC_PairBucket<T1,T2>*)iter_->link();
  return aBucket->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2& OTC_ITMap<T1,T2>::item_() const
{
  OTCLIB_ENSURE_FN((iter_->isLink() && !iter_->link()->isDead()),
   "OTC_ITMap<T1,T2>::item_()",
   "no key/item pair under iterator");

  OTC_PairBucket<T1,T2>* aBucket = (OTC_PairBucket<T1,T2>*)iter_->link();
  return aBucket->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_ITMap<T1,T2>::applyKeys(
 OTC_Visitor<T1>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_LinkIterator theIterator = list_->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T1& theKey = ((OTC_PairBucket<T1,T2>*)theIterator.link())->key();
    progress = theApplicator.action(theKey);

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
template<class T1, class T2>
void OTC_ITMap<T1,T2>::applyItems(
 OTC_Visitor<T2>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_LinkIterator theIterator = list_->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T2& theItem = ((OTC_PairBucket<T1,T2>*)theIterator.link())->item();
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
template<class T1, class T2>
void OTC_ITMap<T1,T2>::applyItems(
 OTC_Worker<T2>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
{
  OTC_LinkIterator theIterator = list_->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T2& theItem = ((OTC_PairBucket<T1,T2>*)theIterator.link())->item();
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
template<class T1, class T2>
void OTC_ITMap<T1,T2>::applyPairs(
 OTC_PairVisitor<T1,T2>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  OTC_LinkIterator theIterator = list_->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T1& theKey = ((OTC_PairBucket<T1,T2>*)theIterator.link())->key();
    T2& theItem = ((OTC_PairBucket<T1,T2>*)theIterator.link())->item();
    progress = theApplicator.action(theKey,theItem);

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
template<class T1, class T2>
void OTC_ITMap<T1,T2>::applyPairs(
 OTC_PairWorker<T1,T2>& theApplicator,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
{
  OTC_LinkIterator theIterator = list_->items(theProtection);

  if (theDirection == OTCLIB_FORWARD)
    theIterator.resetFirst();
  else
    theIterator.resetLast();

  theApplicator.start();

  OTC_Progress progress = OTCLIB_CONTINUE;

  while (theIterator.isLink())
  {
    T1& theKey = ((OTC_PairBucket<T1,T2>*)theIterator.link())->key();
    T2& theItem = ((OTC_PairBucket<T1,T2>*)theIterator.link())->item();
    progress = theApplicator.action(theKey,theItem);

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
template<class T1, class T2>
int OTC_ITMap<T1,T2>::searchIndex_(
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

#endif /* OTC_COLLCTN_ITMAP_C */
