#ifndef OTC_COLLCTN_HBAG_C
#define OTC_COLLCTN_HBAG_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/hbag.c
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

#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/icursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HBag<T>::~OTC_HBag()
{
  index_->unReference();
  list_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HBag<T>::OTC_HBag(OTC_HBag<T> const& theBag)
  : rankFn_(theBag.rankFn_)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();

  OTC_Iterator<T> iter = 0;
  iter = theBag.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
  for (iter.reset(); iter.isValid(); iter.next())
    add(iter.item());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HBag<T>::OTC_HBag(int (*theRankFn)(T const&,T const&))
  : rankFn_(theRankFn)
{
  list_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(list_ != 0);
  list_->reference();

  index_ = new OTC_HashTable;
  OTCLIB_ASSERT_M(index_ != 0);
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HBag<T>::OTC_HBag(OTC_HBag<T> const& theBag, OTC_ShallowCopy)
  : rankFn_(theBag.rankFn_)
{
  list_ = theBag.list_;
  list_->reference();

  index_ = theBag.index_;
  index_->reference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HBag<T>::removeAll()
{
  index_->removeAll();
  list_->removeAll();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_HBag<T>& OTC_HBag<T>::operator=(OTC_HBag<T> const& theBag)
{
  if (&theBag != this)
  {
    removeAll();

    rankFn_ = theBag.rankFn_;
    OTC_Iterator<T> iter = 0;
    iter = theBag.items(OTCLIB_FORWARD,OTCLIB_UNSAFE);
    for (iter.reset(); iter.isValid(); iter.next())
      add(iter.item());
  }

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HBag<T>::add(T const& theItem)
{
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0)
  {
    if (!tmpEntry->isOccupied())
    {
      tmpAvailableIndex = tmpEntryIndex;
      break;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  tmpIndexEntry = new OTC_Bucket<T>(theItem);
  OTCLIB_ASSERT_M(tmpIndexEntry != 0);

  list_->addLast(tmpIndexEntry);
  index_->add(tmpAvailableIndex,tmpIndexEntry,tmpHashValue);
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HBag<T>::add(OTC_Iterator<T> theItems)
{
  while (theItems.isValid())
  {
    add(theItems.item());

    theItems.next();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HBag<T>::remove(T const& theItem,OTC_Protection theProtection)
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();
      tmpRank = rank(theItem,tmpIndexEntry->item());
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
   "OTC_HBag<T>::remove(T const&, OTC_Protection)",
   "item doesn't exist");
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_HBag<T>::removeAll(T const& theItem)
{
  u_int tmpPopulation = index_->population();

  while (1)
  {
    remove(theItem,OTCLIB_UNSAFE);

    if (tmpPopulation != index_->population())
      tmpPopulation = index_->population();
    else
      break;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
bool OTC_HBag<T>::contains(T const& theItem) const
{
  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();
      tmpRank = rank(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        return true;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return false;
}

/* ------------------------------------------------------------------------- */
template<class T>
u_int OTC_HBag<T>::frequency(T const& theItem) const
{
  if (index_->population() == 0)
    return 0;

  u_int theOccurences = 0;

  int tmpRank;
  int tmpHashValue;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry = 0;
  OTC_Bucket<T>* tmpIndexEntry = 0;

  tmpHashValue = OTC_HashActions<T>::hash(theItem);
  tmpEntry = index_->searchHash(tmpHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0 && !tmpEntry->isEmpty())
  {
    if (tmpEntry->isOccupied())
    {
      tmpIndexEntry = (OTC_Bucket<T>*)tmpEntry->item();
      tmpRank = rank(theItem,tmpIndexEntry->item());
      if (tmpRank == 0)
        theOccurences++;
    }

    tmpEntry = index_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }

  return theOccurences;
}

/* ------------------------------------------------------------------------- */
template<class T>
T const& OTC_HBag<T>::pickItem() const
{
  OTCLIB_ENSURE_FN((index_->population() != 0),
   "OTC_HBag<T>::pickItem()",
   "collection must not be empty");
   
  return ((OTC_Bucket<T>*)list_->first())->item();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Iterator<T> OTC_HBag<T>::items(
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
int OTC_HBag<T>::rank(T const& item1, T const& item2) const
{
  return (rankFn_ != 0) ? (*rankFn_)(item1,item2) :
   OTC_RankActions<T>::rank(item1,item2);
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HBAG_C */
