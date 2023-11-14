/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otcmcobject.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/mcobject.hh"
#endif
#endif

#include <OTC/memory/mcobject.hh>
#include <OTC/memory/cluster.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_MCObject::mutex_;
OTC_ClusterAllocation OTC_MCObject::gAllocations_;

/* ------------------------------------------------------------------------- */
OTC_MCObject::OTC_MCObject()
  : cluster_(0), next_(0)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  size_t theLower = 0;
  size_t theUpper = 0;

  size_t theCurrent = 0;

  theCurrent = size_t(this);

  OTC_ClusterAllocation* theAllocation1 = 0;
  OTC_ClusterAllocation* theAllocation2 = 0;

  theAllocation1 = &gAllocations_;

  while (theAllocation1 != 0)
  {
    if (theAllocation1->memory != 0)
    {
      theLower = size_t(theAllocation1->memory);
      theUpper = theLower + theAllocation1->size;

      if (theCurrent >= theLower && theCurrent < theUpper)
      {
	// Found the allocation for this instance.

	cluster_ = theAllocation1->cluster;

	next_ = cluster_->objects_;
	cluster_->objects_ = this;

	break;
      }
    }

    theAllocation2 = theAllocation1;
    theAllocation1 = theAllocation1->next;
  }

  if (theAllocation1 != 0)
  {
    if (theAllocation2 != 0)
    {
      // End of or middle of chain.

      theAllocation2->next = theAllocation1->next;

      delete theAllocation1;
    }
    else
    {
      // Head of chain.

      if (gAllocations_.next != 0)
      {
	// Copy following entry into the head.

	theAllocation1 = gAllocations_.next;

	gAllocations_.cluster = theAllocation1->cluster;
	gAllocations_.memory = theAllocation1->memory;
	gAllocations_.size = theAllocation1->size;
	gAllocations_.next = theAllocation1->next;

	delete theAllocation1;
      }
      else
      {
	// Only head of chain.

	gAllocations_.cluster = 0;
	gAllocations_.memory = 0;
	gAllocations_.size = 0;
	gAllocations_.next = 0;
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_MCObject::~OTC_MCObject()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void* OTC_MCObject::operator new(size_t theSize, OTC_Cluster& theCluster)
{
  void* theMemory;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  theMemory = theCluster.arena_.allocate(theSize);

  if (gAllocations_.cluster != 0)
  {
    OTC_ClusterAllocation* theAllocation;
    
    theAllocation = new OTC_ClusterAllocation;
    OTCLIB_ASSERT_M(theAllocation != 0);

    theAllocation->cluster = gAllocations_.cluster;
    theAllocation->memory = gAllocations_.memory;
    theAllocation->size = gAllocations_.size;
    theAllocation->next = gAllocations_.next;

    gAllocations_.next = theAllocation;
  }

  gAllocations_.cluster = &theCluster;
  gAllocations_.memory = theMemory;
  gAllocations_.size = theSize;

  xxxMutex.release();

  mutex_.unlock();

  return theMemory;
}

/* ------------------------------------------------------------------------- */
void* OTC_MCObject::operator new(size_t)
{
  OTCLIB_EXCEPTION(
   "OTC_MCObject::operator new(size_t) - Shouldn't be callable");

  void* theMemory = 0;

  return theMemory;
}

/* ------------------------------------------------------------------------- */
