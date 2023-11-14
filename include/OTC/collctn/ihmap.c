#ifndef OTC_COLLCTN_IHMAP_C
#define OTC_COLLCTN_IHMAP_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ihmap.c
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
OTC_IHMap<T1,T2>::~OTC_IHMap()
{
  index_->unReference();
  list_->unReference();

  delete iter_;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_IHMap<T1,T2>::OTC_IHMap(OTC_IHMap<T1,T2> const& theMap)
  : rankFn_(theMap.rankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
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
OTC_IHMap<T1,T2>::OTC_IHMap(int (*theRankFn)(T1 const&,T1 const&))
  : rankFn_(theRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  iter_ = new OTC_LinkIterator(list_->items());
  OTCLIB_ASSERT_M(iter_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_IHMap<T1,T2>::OTC_IHMap(OTC_IHMap<T1,T2> const& theMap, OTC_ShallowCopy)
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
void OTC_IHMap<T1,T2>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
OTC_IHMap<T1,T2>& OTC_IHMap<T1,T2>::operator=(OTC_IHMap<T1,T2> const& theMap)
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
void OTC_IHMap<T1,T2>::add(T1 const& theKey, T2 const& theItem)
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      if (tmpAvailableIndex == -1)
        tmpAvailableIndex = tmpEntryIndex;
    }

    if (tmpEntry->isEmpty())
      break;

    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();

      tmpRank = rank(theKey,tmpIndexEntry->key());

      OTCLIB_ENSURE_FN((tmpRank != 0),
       "OTC_IHMap<T1,T2>::add(T1 const&, T2 const&)",
       "key already exists");
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  tmpIndexEntry = new OTC_PairBucket<T1,T2>(theKey,theItem);
  OTCLIB_ASSERT_M(tmpIndexEntry != 0);

  list_->addLast(tmpIndexEntry);
  index_->add(tmpAvailableIndex,tmpIndexEntry,tmpHashValue);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_IHMap<T1,T2>::remove(T1 const& theKey, OTC_Protection theProtection)
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
      {
        index_->remove(tmpEntryIndex);
        tmpIndexEntry->kill();
        return;
      }
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTCLIB_ENSURE_FN((theProtection == OTCLIB_UNSAFE),
   "OTC_IHMap<T1,T2>::remove(T1 const&, OTC_Protection)",
   "key doesn't exist");
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
bool OTC_IHMap<T1,T2>::contains(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        return true;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_IHMap<T1,T2>::remove()
{
  OTCLIB_ENSURE_FN((isValid() != false),
   "OTC_IHMap<T1,T2>::remove()",
   "iterator not located on valid key/item pair");

  remove(key_());
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2& OTC_IHMap<T1,T2>::item_(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        break;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  OTCLIB_ENSURE_FN((tmpEntry != 0 && tmpEntry->isOccupied()),
   "OTC_IHMap<T1,T2>::item_()",
   "key does not exist");

  return tmpIndexEntry->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2* OTC_IHMap<T1,T2>::pItem_(T1 const& theKey) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_PairBucket<T1,T2>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T1>::hash(theKey);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_PairBucket<T1,T2>*)tmpEntry->item();
      tmpRank = rank(theKey,tmpIndexEntry->key());
      if (tmpRank == 0)
        return &tmpIndexEntry->item();
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1 const& OTC_IHMap<T1,T2>::pickKey() const
{
  OTCLIB_ENSURE_FN((population() != 0),
   "OTC_IHMap<T1,T2>::pickKey()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2 const& OTC_IHMap<T1,T2>::pickItem() const
{
  OTCLIB_ENSURE_FN((population() != 0),
   "OTC_IHMap<T1,T2>::pickItem()",
   "collection must not be empty");

  return ((OTC_PairBucket<T1,T2>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
int OTC_IHMap<T1,T2>::rank(T1 const& key1, T1 const& key2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(key1,key2) :
   OTC_RankActions<T1>::rank(key1,key2);
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T1& OTC_IHMap<T1,T2>::key_() const
{
  OTCLIB_ENSURE_FN((iter_->isLink() && !iter_->link()->isDead()),
   "OTC_IHMap<T1,T2>::key_()",
   "no key/item pair under iterator");

  OTC_PairBucket<T1,T2>* aBucket = (OTC_PairBucket<T1,T2>*)iter_->link();
  return aBucket->key();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
T2& OTC_IHMap<T1,T2>::item_() const
{
  OTCLIB_ENSURE_FN((iter_->isLink() && !iter_->link()->isDead()),
   "OTC_IHMap<T1,T2>::item_()",
   "no key/item pair under iterator");

  OTC_PairBucket<T1,T2>* aBucket = (OTC_PairBucket<T1,T2>*)iter_->link();
  return aBucket->item();
}

/* ------------------------------------------------------------------------- */
template<class T1, class T2>
void OTC_IHMap<T1,T2>::applyKeys(
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
void OTC_IHMap<T1,T2>::applyItems(
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
void OTC_IHMap<T1,T2>::applyItems(
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
void OTC_IHMap<T1,T2>::applyPairs(
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
void OTC_IHMap<T1,T2>::applyPairs(
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

#endif /* OTC_COLLCTN_IHMAP_C */
