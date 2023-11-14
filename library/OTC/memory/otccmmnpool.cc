/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otccmmnpool.cc
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
#pragma implementation "OTC/memory/cmmnpool.hh"
#endif
#endif

#include <OTC/memory/cmmnpool.hh>
#include <OTC/memory/pool.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_CommonPool::mutex_;
bool OTC_CommonPool::init_ = false;
bool OTC_CommonPool::disable_ = false;
OTC_Pool** OTC_CommonPool::pools_ = 0;
size_t OTC_CommonPool::numPools_ = 0;
size_t OTC_CommonPool::increment_ = 0;
size_t OTC_CommonPool::largest_ = 0;
size_t* OTC_CommonPool::index_ = 0;
size_t OTC_CommonPool::allocated_ = 0;
size_t OTC_CommonPool::total_ = 0;

/* ------------------------------------------------------------------------- */
void OTC_CommonPool::initialise()
{
  // This function does not lock the thread mutex as it is expected that it
  // is locked by the allocate() function. Avoids having a recursive mutex.

  init_ = true;

  if (getenv("OTCLIB_NOCOMMONPOOL") != 0)
    disable_ = true;

  if (disable_ == false && pools_ == 0)
  {
    // Initialise statics here. Do this in order to avoid problems with
    // initialised static data in SunOS 4 Shared libraries.

    char* theVariable = 0;

    theVariable = getenv("OTCLIB_COMMONPOOLS");
    if (theVariable != 0)
    {
      int theNumber;
      theNumber = atoi(theVariable);
      if (theNumber <= 0)
	disable_ = true;
      else
	numPools_ = theNumber;
    }
    else
    {
      numPools_ = 8;
    }

    increment_ = sizeof(double);

    largest_ = numPools_ * increment_;

    pools_ = new OTC_Pool*[numPools_];
    OTCLIB_ASSERT_M(pools_ != 0);

    char tmpBuffer1[64];
    char tmpBuffer2[64];

    strcpy(tmpBuffer1,"OTCLIB_POOLBLOCKSIZE_");
    strcpy(tmpBuffer2,"OTCLIB_POOLPREALLOCATE_");

    char* tmpPointer1 = tmpBuffer1 + strlen(tmpBuffer1);
    char* tmpPointer2 = tmpBuffer2 + strlen(tmpBuffer2);

    size_t theSize = increment_;
    for (size_t i=0; i<numPools_; i++)
    {
      int theBlockSize = -1;
      int thePreAllocate = -1;

      sprintf(tmpPointer1,"%d",i+1);
      theVariable = getenv(tmpBuffer1);
      if (theVariable != 0)
	theBlockSize = atoi(theVariable);

      sprintf(tmpPointer2,"%d",i+1);
      theVariable = getenv(tmpBuffer2);
      if (theVariable != 0)
	thePreAllocate = atoi(theVariable);

      if (theBlockSize > 0)
	pools_[i] = new OTC_Pool(theSize,theBlockSize);
      else
	pools_[i] = new OTC_Pool(theSize);

      OTCLIB_ASSERT_M(pools_[i] != 0);

      if (thePreAllocate != -1)
	pools_[i]->grabMemory(thePreAllocate);

      theSize += increment_;
    }

    index_ = new size_t[largest_+1];
    OTCLIB_ASSERT_M(index_ != 0);

    index_[0] = 0;
    for (size_t j=1; j<=largest_; j++)
      index_[j] = (j-1)/increment_;
  }
}

/* ------------------------------------------------------------------------- */
void* OTC_CommonPool::allocate(size_t theSize)
{
  void* theData;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (init_ == false)
    initialise();

  if (!disable_ && (theSize <= largest_))
    theData = pools_[index_[theSize]]->allocate();
  else
    theData = new char[theSize];

  allocated_++;
  total_++;

  xxxMutex.release();

  mutex_.unlock();

  return theData;
}

/* ------------------------------------------------------------------------- */
void OTC_CommonPool::release(void* theMemory, size_t theSize)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!disable_ && (theSize <= largest_))
    pools_[index_[theSize]]->release(theMemory);
  else
    delete [] (char*)theMemory;

  allocated_--;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_CommonPool::usageSummary(ostream& theStream)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  theStream << "commonpool.numPools() = ";
  theStream << OTC_CommonPool::numPools() << EOL;
  theStream << "commonpool.allocated() = ";
  theStream << OTC_CommonPool::allocated_ << EOL;
  theStream << "commonpool.total() = ";
  theStream << OTC_CommonPool::total_ << EOL;

  if (OTC_CommonPool::numPools() > 0)
  {
    OTC_Pool const* thePool = 0;
    thePool = OTC_CommonPool::pool(OTC_CommonPool::numPools());

    theStream << "commonpool.maxSize() = ";
    theStream << thePool->elementSize() << EOL;

    for (size_t i=1; i<=OTC_CommonPool::numPools(); i++)
    {
      thePool = OTC_CommonPool::pool(i);

      if (thePool->blockCount() != 0)
      {
	theStream << "commonpool[" << i << "].elementSize() = ";
	theStream << thePool->elementSize() << EOL;
	theStream << "commonpool[" << i << "].blockSize() = ";
	theStream << thePool->blockSize() << EOL;
	theStream << "commonpool[" << i << "].blockCount() = ";
	theStream << thePool->blockCount() << EOL;
	theStream << "commonpool[" << i << "].elementCount() = ";
	theStream << thePool->elementCount() << EOL;
	theStream << "commonpool[" << i << "].capacity() = ";
	theStream << thePool->capacity() << EOL;
	theStream << "commonpool[" << i << "].allocated() = ";
	theStream << thePool->allocated() << EOL;
	theStream << "commonpool[" << i << "].available() = ";
	theStream << thePool->available() << EOL;
	theStream << "commonpool[" << i << "].total() = ";
	theStream << thePool->total() << EOL;

	theStream << flush;
      }
    }
  }

  theStream << flush;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
