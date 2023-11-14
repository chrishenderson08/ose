/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/otchashtble.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/collctn/hashtble.hh"
#endif
#endif

#include <OTC/collctn/hashtble.hh>

/* ------------------------------------------------------------------------- */
OTC_HashTable::~OTC_HashTable()
{
  delete [] hashTable_;
}

/* ------------------------------------------------------------------------- */
OTC_HashTable::OTC_HashTable(
 u_int theStartSize,
 u_int theFloorSize,
 u_int theLowThreshold,
 u_int theHighThreshold
)
  : startSize_(1),
    startPower_(1),
    population_(0)
{
  OTCLIB_ENSURE_FN((theStartSize > 0),
   "OTC_HashTable::OTC_HashTable(u_int, u_int, u_int, u_int)",
   "invalid initial hash table size");
  OTCLIB_ENSURE_FN((theLowThreshold < 50),
   "OTC_HashTable::OTC_HashTable(u_int, u_int, u_int, u_int)",
   "invalid hash table thresholds");
  OTCLIB_ENSURE_FN((theLowThreshold < theHighThreshold),
   "OTC_HashTable::OTC_HashTable(u_int, u_int, u_int, u_int)",
   "invalid hash table thresholds");
  OTCLIB_ENSURE_FN((theHighThreshold > 50 && theHighThreshold <= 100),
   "OTC_HashTable::OTC_HashTable(u_int, u_int, u_int, u_int)",
   "invalid hash table thresholds");

  // Convert initial hash table size to a power
  // of two.

  theStartSize--;
  while (theStartSize >>= 1)
    startPower_++;

  startSize_ <<= startPower_;

  // Now initialise everything.

  tableSize_ = startSize_;
  tablePower_ = startPower_;
  floorSize_ = theFloorSize;
  lowThreshold_ = theLowThreshold;
  highThreshold_ = theHighThreshold;
  lowWaterMark_ = waterMark_(lowThreshold_);
  highWaterMark_ = waterMark_(highThreshold_);

  hashTable_ = new OTC_HashEntry[tableSize_];
  OTCLIB_ASSERT_M(hashTable_ != 0);
}

/* ------------------------------------------------------------------------- */
OTC_HashEntry* OTC_HashTable::tableEntry(u_int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex < tableSize_),
   "OTC_HashTable::tableEntry(u_int)",
   "invalid index");

  return &hashTable_[theIndex];
}

/* ------------------------------------------------------------------------- */
OTC_HashEntry* OTC_HashTable::searchHash(
 int theHashValue,
 u_int& theIteration,
 u_int& theStartIndex,
 u_int& theEntryIndex
)
{
  OTCLIB_ENSURE_FN((theStartIndex < tableSize_),
   "OTC_HashTable::searchHash(int, u_int&, u_int&, u_int&)",
   "invalid index");

  if (theIteration == tableSize_)
    return 0;

  OTC_HashEntry* tmpEntry = 0;

  if (theIteration == 0)
    theStartIndex = start_(theHashValue);

  while (theIteration++ < tableSize_)
  {
    theEntryIndex = theStartIndex;

    tmpEntry = &hashTable_[theStartIndex];

    theStartIndex = mask_(theStartIndex+1);

    if (tmpEntry->isOccupied())
    {
      if (tmpEntry->hashValue() == theHashValue)
	break;
    }
    else
    {
      break;
    }
  }

  return tmpEntry;
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::add(u_int theIndex, void* theItem, int theHashValue)
{
  OTCLIB_ENSURE_FN((theIndex < tableSize_),
   "OTC_HashTable::add(u_int, void*, int)",
   "invalid index");

  OTC_HashEntry* tmpEntry;
  tmpEntry = &hashTable_[theIndex];

  OTCLIB_ENSURE_FN((!tmpEntry->isOccupied()),
   "OTC_HashTable::add(u_int, void*, int)",
   "invalid index");

  tmpEntry->markOccupied(theItem,theHashValue);
  population_++;

  if (population_ >= highWaterMark_)
    grow_();
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::replace(u_int theIndex, void* theItem, int theHashValue)
{
  OTCLIB_ENSURE_FN((theIndex < tableSize_),
   "OTC_HashTable::replace(u_int, void*, int)",
   "invalid index");

  OTC_HashEntry* tmpEntry;
  tmpEntry = &hashTable_[theIndex];

  OTCLIB_ENSURE_FN((tmpEntry->isOccupied()),
   "OTC_HashTable::replace(u_int, void*, int)",
   "invalid index");
  OTCLIB_ENSURE_FN((tmpEntry->hashValue() == theHashValue),
   "OTC_HashTable::replace(u_int, void*, int)",
   "hash doesn't match");

  tmpEntry->markOccupied(theItem,theHashValue);
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::remove(u_int theIndex)
{
  OTCLIB_ENSURE_FN((theIndex < tableSize_),
   "OTC_HashTable::remove(u_int)",
   "invalid index");

  OTC_HashEntry* tmpEntry;
  tmpEntry = &hashTable_[theIndex];

  OTCLIB_ENSURE_FN((tmpEntry->isOccupied()),
   "OTC_HashTable::remove(u_int)",
   "entry unoccupied");

  tmpEntry->markDeleted();
  population_--;

  if (population_ > floorSize_ && population_ < lowWaterMark_)
    shrink_();
  else
    rebuild_(theIndex);
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::remove(void* theItem, int theHashValue)
{
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  OTC_HashEntry* tmpEntry;

  tmpEntry = searchHash(theHashValue,
   tmpIteration,tmpStartIndex,tmpEntryIndex);

  while (tmpEntry != 0)
  {
    OTCLIB_ENSURE_FN((!tmpEntry->isEmpty()),
     "OTC_HashTable::remove(void*, int)",
     "item doesn't exist");

    if (tmpEntry->isOccupied())
    {
      if (tmpEntry->item() == theItem)
      {
	tmpEntry->markDeleted();
	population_--;

	if (population_ > floorSize_ && population_ < lowWaterMark_)
	  shrink_();
	else
	  rebuild_(tmpEntryIndex);

	break;
      }
    }

    tmpEntry = searchHash(theHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::removeAll()
{
  population_ = 0;

  if (tableSize_ > floorSize_)
  {
    delete [] hashTable_;

    tableSize_ = startSize_;
    tablePower_ = startPower_;
    lowWaterMark_ = waterMark_(lowThreshold_);
    highWaterMark_ = waterMark_(highThreshold_);

    hashTable_ = new OTC_HashEntry[tableSize_];
    OTCLIB_ASSERT_M(hashTable_ != 0);
  }
  else
  {
    OTC_HashEntry* tmpEntry;
    tmpEntry = hashTable_;

    for (u_int i=0; i<tableSize_; tmpEntry++, i++)
      tmpEntry->markEmpty();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::grow_()
{
  u_int oldTableSize = tableSize_;
  OTC_HashEntry* oldTable = hashTable_;

  tableSize_ <<= 1;
  tablePower_++;

  lowWaterMark_ = waterMark_(lowThreshold_);
  highWaterMark_ = waterMark_(highThreshold_);

  hashTable_ = new OTC_HashEntry[tableSize_];
  OTCLIB_ASSERT_M(hashTable_ != 0);

  rebuild_(oldTable,oldTableSize);

  delete [] oldTable;
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::shrink_()
{
  OTCLIB_ENSURE_FN((tableSize_ != 0),
   "OTC_HashTable::shrink_()",
   "internal error");

  u_int oldTableSize = tableSize_;
  OTC_HashEntry* oldTable = hashTable_;

  tableSize_ >>= 1;
  tablePower_--;

  OTCLIB_ENSURE_FN((population_ <= tableSize_),
   "OTC_HashTable::shrink_()",
   "insufficient table space");

  lowWaterMark_ = waterMark_(lowThreshold_);
  highWaterMark_ = waterMark_(highThreshold_);

  hashTable_ = new OTC_HashEntry[tableSize_];
  OTCLIB_ASSERT_M(hashTable_ != 0);

  rebuild_(oldTable,oldTableSize);

  delete [] oldTable;
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::rebuild_(u_int theIndex)
{
  OTCLIB_ENSURE_FN((theIndex < tableSize_),
   "OTC_HashTable::rebuild_(u_int)",
   "invalid index");

  OTC_HashEntry* tmpEntry;
  tmpEntry = &hashTable_[theIndex];

  OTCLIB_ENSURE_FN((tmpEntry->isDeleted()),
   "OTC_HashTable::rebuild_(u_int)",
   "entry not deleted");

  u_int i = theIndex;
  u_int j = theIndex;

  tmpEntry->markEmpty();

  i = mask_(i+1);
  tmpEntry = &hashTable_[i];

  while (!tmpEntry->isEmpty() && i != theIndex)
  {
    OTCLIB_ASSERT(tmpEntry->isOccupied());

    u_int r = start_(tmpEntry->hashValue());

    if (!((i >= r && r > j) || (r > j && j > i) || (j > i && i >= r)))
    {
      OTC_HashEntry* oldEntry;
      oldEntry = &hashTable_[j];

      oldEntry->markOccupied(tmpEntry->item(),tmpEntry->hashValue());

      tmpEntry->markEmpty();

      j = i;
    }

    i = mask_(i+1);
    tmpEntry = &hashTable_[i];
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HashTable::rebuild_(
 OTC_HashEntry* theHashTable,
 u_int theTableSize
)
{
  // Traverse through the old table and put each entry in the new table.
  // Note that we cannot simply call <add()> to add the entry into the
  // new table as it will result in another copy of the item being placed
  // in the list. Also note that we do not have to check that an item
  // already exists as we are already guaranteed that they are unique, thus
  // simply search for first empty slot.

  OTC_HashEntry* oldEntry;
  OTC_HashEntry* tmpEntry;

  for (u_int j=0; j<theTableSize; j++)
  {
    oldEntry = &theHashTable[j];

    if (oldEntry->isOccupied() != false)
    {
      int theHashValue = oldEntry->hashValue();
      u_int tmpIndex = start_(theHashValue);

      for (u_int i=0; i<=tableSize_; i++)
      {
        tmpEntry = &hashTable_[tmpIndex];

        if (tmpEntry->isEmpty() != false)
        {
          tmpEntry->markOccupied(oldEntry->item(),oldEntry->hashValue());
          break;
        }
        if (++tmpIndex == tableSize_)
        {
          tmpIndex = 0;
        }
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
