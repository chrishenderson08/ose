/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otcpool.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/pool.hh"
#endif
#endif

#include <OTC/memory/pool.hh>
#include <OTC/memory/align.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_Pool");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_Pool::OTC_Pool(size_t theSize)
  : blocks_(0),
    blockCount_(0),
    elements_(0),
    capacity_(0),
    allocated_(0),
    total_(0)
{
  OTCLIB_ASSERT(theSize != 0);
  size_t theBlockSize = 2040;
  char const* szenv = getenv("OTCLIB_POOLBLOCKSIZE");
  if (szenv != 0)
    theBlockSize = atoi(szenv);

  initialise(theSize,theBlockSize);
}

/* ------------------------------------------------------------------------- */
void OTC_Pool::initialise(size_t theSize, size_t theBlockSize)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Pool::initialise(theSize,theBlockSize)");

  OTCLIB_TRACER(MODULE) << "@object - " << (void*)this << endl;
  OTCLIB_TRACER(MODULE) << "theSize = " << theSize << endl;
  OTCLIB_TRACER(MODULE) << "theBlockSize = " << theBlockSize << endl;

  size_t const ALSZ = OTC_Alignment::ofDouble();

  size_t const ELTSZ = sizeof(OTC_Pool::OTC_PoolElement);
  size_t const BLKSZ = sizeof(OTC_Pool::OTC_PoolBlock);

  OTCLIB_TRACER(MODULE) << "ALSZ = " << ALSZ << endl;

  // Calculate offset into block of memory, where first element will
  // be located. Header information for the block will be placed in
  // this area.

  headerSize_ = ((BLKSZ + ALSZ - 1) / ALSZ) * ALSZ;

  // Ensure that header size is at least the size of <OTC_PoolBlock>.

  if (headerSize_ < BLKSZ)
    headerSize_ = BLKSZ;

  // Calculate a size for a single element. We need to make sure that the
  // size of the element is rounded up to a multiple of the alignment
  // requirement for the type <double>. This is done as <double> usually
  // has the most strict alignment requirements.

  elementSize_ = ((theSize + ALSZ - 1) / ALSZ) * ALSZ;

  // Ensure that element size is at least the size of <OTC_PoolElement>.

  if (elementSize_ < ELTSZ)
    elementSize_ = ELTSZ;

  // Calculate how many elements we can fit in to the suggested maximum size
  // for each block of memory we get from the operating system. Note that we
  // must allow for an instance of <OTC_PoolBlock> at the start of the
  // memory which is allocated. The actual elements which follow the block
  // header must start at a memory location which has correct alignment
  // for holding a value of type <double>. Again, assume worst case here and
  // use the size of the type <double> for alignment. Further note that we
  // must be able to fit at least one element into the allocated block of
  // memory, if we can't then we will allocate more memory than what the user
  // suggested.

  elementCount_ = 0;

  if (theBlockSize > ALSZ)
    elementCount_ = (theBlockSize - headerSize_) / elementSize_;

  if (elementCount_ == 0)
    elementCount_ = 1;

  blockSize_ = headerSize_ + elementCount_ * elementSize_;

  OTCLIB_TRACER(MODULE) << "headerSize_ = " << headerSize_ << endl;
  OTCLIB_TRACER(MODULE) << "elementSize_ = " << elementSize_ << endl;
  OTCLIB_TRACER(MODULE) << "elementCount_ = " << elementCount_ << endl;
  OTCLIB_TRACER(MODULE) << "blockSize_ = " << blockSize_ << endl;
}

/* ------------------------------------------------------------------------- */
OTC_Pool::~OTC_Pool()
{
  OTCLIB_ENSURE_FN((allocated_ == 0),
   "OTC_Pool::~OTC_Pool()",
   "elements still allocated");

  OTC_Pool::OTC_PoolBlock* theNext;
  while (blocks_ != 0)
  {
    theNext = blocks_->next;
    delete [] (char*)blocks_;
    blocks_ = theNext;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Pool::grabMemory(size_t theSize)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Pool::grabMemory()");

  OTCLIB_TRACER(MODULE) << "@object - " << (void*)this << endl;

  // Work out the size of the block to allocate and the number
  // of elements in it.

  size_t theBlockSize = blockSize_;
  size_t theElementCount = elementCount_;

  if (theSize > theBlockSize)
  {
    theBlockSize = theSize;
    theElementCount = (theBlockSize - headerSize_) / elementSize_;
  }

  capacity_ += theElementCount;

  // Allocate a new block, and insert it into the list of all blocks so it
  // can be released when the pool is destroyed.

  OTC_Pool::OTC_PoolBlock* theBlock;
  theBlock = (OTC_Pool::OTC_PoolBlock*) new char[theBlockSize];
  OTCLIB_ASSERT_M(theBlock != 0);

  theBlock->next = blocks_;
  blocks_ = theBlock;

  blockCount_++;

  // Link together all elements in the new block.

  OTC_Pool::OTC_PoolElement* theElement;
  theElement = (OTC_Pool::OTC_PoolElement*)((char*)theBlock + headerSize_);

  size_t n = theElementCount;
  while (--n > 0)
  {
    theElement->next = (OTC_Pool::OTC_PoolElement*)
     ((char*)theElement + elementSize_);
    theElement = theElement->next;
  }

  // The last element of the block points to the existing free list,
  // which normally is empty (but needn't be). The new free list
  // starts with the first element of the newly allocated block.

  theElement->next = elements_;
  elements_ = (OTC_Pool::OTC_PoolElement*)((char*)theBlock + headerSize_);
}

/* ------------------------------------------------------------------------- */
