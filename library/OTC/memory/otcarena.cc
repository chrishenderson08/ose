/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otcarena.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/arena.hh"
#endif
#endif

#include <OTC/memory/arena.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_ArenaBlock::~OTC_ArenaBlock()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Arena::OTC_Arena(size_t theAlign)
  : blockSize_(2040),
    slop_(16),
    align_(theAlign),
    full_(0),
    free_(0),
    allocated_(0)
{
  OTCLIB_ENSURE_FN((theAlign != 0),
   "OTC_Arena::OTC_Arena(size_t)",
   "invalid memory alignment");

  char const* szenv = 0;
  int tmpBlockSize = blockSize_;
  szenv = getenv("OTCLIB_ARENABLOCKSIZE");
  if (szenv != 0)
    tmpBlockSize = atoi(szenv);

  int tmpSlop = slop_;
  szenv = getenv("OTCLIB_ARENASLOPSIZE");
  if (szenv != 0)
    tmpSlop = atoi(szenv);

  OTCLIB_ENSURE_FN((tmpBlockSize >= 0),
   "OTC_Arena::OTC_Arena(size_t)",
   "invalid block size");
  OTCLIB_ENSURE_FN((tmpSlop >= 0),
   "OTC_Arena::OTC_Arena(size_t)",
   "invalid slop size");

  blockSize_ = tmpBlockSize;
  slop_ = tmpSlop;
}

/* ------------------------------------------------------------------------- */
OTC_Arena::OTC_Arena(size_t theBlockSize, size_t theSlop, size_t theAlign)
  : blockSize_(theBlockSize), slop_(theSlop), align_(theAlign),
    full_(0), free_(0)
{
  OTCLIB_ENSURE_FN((theAlign != 0),
   "OTC_Arena::OTC_Arena(size_t,size_t,size_t)",
   "invalid memory alignment");
}

/* ------------------------------------------------------------------------- */
OTC_Arena::~OTC_Arena()
{
  while (full_ != 0)
  {
    OTC_ArenaBlock* tmpBlock;
    tmpBlock = full_->next;
    delete [] full_->block;
    delete full_;
    full_ = tmpBlock;
  }

  while (free_ != 0)
  {
    OTC_ArenaBlock* tmpBlock;
    tmpBlock = free_->next;
    delete [] free_->block;
    delete free_;
    free_ = tmpBlock;
  }
}

/* ------------------------------------------------------------------------- */
size_t OTC_Arena::freeBlocks() const
{
  size_t theResult = 0;

  OTC_ArenaBlock* tmpBlock;
  tmpBlock = free_;

  while (tmpBlock != 0)
  {
    theResult++;
    tmpBlock = tmpBlock->next;
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
size_t OTC_Arena::fullBlocks() const
{
  size_t theResult = 0;

  OTC_ArenaBlock* tmpBlock;
  tmpBlock = full_;

  while (tmpBlock != 0)
  {
    theResult++;
    tmpBlock = tmpBlock->next;
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
void* OTC_Arena::allocate(size_t theSize)
{
  // Increment count of allocate memory.

  allocated_ += theSize;

  // Check if request is larger than block size. If it is then
  // don't both checking available memory, just allocate it.

  if (theSize > blockSize_)
    return allocateNewBlock(theSize);

  // Allow for memory alignment.

  if ((theSize % align_) != 0)
    theSize = (theSize/align_ + 1) * align_;

  // Check if we have any blocks.

  if (free_ == 0)
    return allocateNewBlock(theSize);

  // Search all the blocks.

  OTC_ArenaBlock* tmpBlock1;
  OTC_ArenaBlock* tmpBlock2;
  tmpBlock1 = free_;
  tmpBlock2 = 0;

  while (tmpBlock1 != 0 && tmpBlock1->size < theSize)
  {
    tmpBlock2 = tmpBlock1;
    tmpBlock1 = tmpBlock1->next;
  }

  // Nothing available, allocate new block.

  if (tmpBlock1 == 0)
    return allocateNewBlock(theSize);

  // Allocate from block we found.

  char* tmpString;
  tmpString = tmpBlock1->free;
  tmpBlock1->free += theSize;
  tmpBlock1->size -= theSize;

  // If amount of available space drops below slop factor
  // move it to full block list and don't use it anymore.

  if (tmpBlock1->size <= slop_)
  {
    if (tmpBlock2 == 0)
    {
      tmpBlock2 = full_;
      full_ = tmpBlock1;
      free_ = tmpBlock1->next;
      tmpBlock1->next = tmpBlock2;
    }
    else
    {
      OTC_ArenaBlock* tmpBlock3;
      tmpBlock3 = full_;
      full_ = tmpBlock1;
      tmpBlock2->next = tmpBlock1->next;
      tmpBlock1->next = tmpBlock3;
    }
  }

  return tmpString;
}

/* ------------------------------------------------------------------------- */
void OTC_Arena::release(size_t theBlocks)
{
  allocated_ = 0;

  OTC_ArenaBlock* tmpKeep = 0;

  while (free_ != 0)
  {
    OTC_ArenaBlock* tmpBlock;
    tmpBlock = free_->next;

    if (theBlocks > 0 &&
     free_->capacity == blockSize_)
    {
      free_->free = free_->block;
      free_->size = free_->capacity;
      free_->next = tmpKeep;
      tmpKeep = free_;

      theBlocks--;
    }
    else
    {
      delete [] free_->block;
      delete free_;
    }

    free_ = tmpBlock;
  }

  while (full_ != 0)
  {
    OTC_ArenaBlock* tmpBlock;
    tmpBlock = full_->next;

    if (theBlocks > 0 &&
     full_->capacity == blockSize_)
    {
      full_->free = full_->block;
      full_->size = full_->capacity;
      full_->next = tmpKeep;
      tmpKeep = full_;

      theBlocks--;
    }
    else
    {
      delete [] full_->block;
      delete full_;
    }

    full_ = tmpBlock;
  }

  free_ = tmpKeep;
}

/* ------------------------------------------------------------------------- */
void* OTC_Arena::allocateNewBlock(size_t theSize)
{
  OTCLIB_ASSERT(theSize != 0);

  size_t allocSize = theSize;
  if (theSize < blockSize_)
    allocSize = blockSize_;

  OTC_ArenaBlock* tmpBlock;

  tmpBlock = new OTC_ArenaBlock;
  OTCLIB_ASSERT_M(tmpBlock != 0);

  tmpBlock->block = new char[allocSize];
  OTCLIB_ASSERT_M(tmpBlock->block != 0);

  if (theSize > blockSize_)
  {
    tmpBlock->free = 0;
    tmpBlock->size = 0;
    tmpBlock->capacity = 0;
    tmpBlock->next = full_;
    full_ = tmpBlock;
  }
  else
  {
    tmpBlock->free = tmpBlock->block + theSize;
    tmpBlock->size = blockSize_ - theSize;
    tmpBlock->capacity = blockSize_;

    if (tmpBlock->size <= slop_)
    {
      tmpBlock->next = full_;
      full_ = tmpBlock;
    }
    else
    {
      tmpBlock->next = free_;
      free_ = tmpBlock;
    }
  }

  return tmpBlock->block;
}

/* ------------------------------------------------------------------------- */
