/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otcheap.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/heap.hh"
#endif
#endif

#include <OTC/memory/heap.hh>
#include <OTC/memory/align.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_Heap");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_Heap::OTC_Heap(char* theMemory, size_t theSize)
  : memory_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Heap::OTC_Heap(char*,size_t)");

  OTCLIB_TRACER(MODULE) << "memory = " << (void*)theMemory << endl;
  OTCLIB_TRACER(MODULE) << "size = " << theSize << endl;

  OTCLIB_ENSURE_FN((theMemory != 0),
   "OTC_Heap(char*, size_t)",
   "invalid block of memory");

  // Reduce <theSize> to be a multiple of the alignment requirement of
  // type <double>. The type <double> generally has worst alignment
  // requirements. Need to change this as we are going to place a header
  // and footer at the end of the memory block. These need to be aligned
  // correctly. To make arithmetic easier reduce the size of the usable
  // block of memory so as to be aligned.

  size_t const ALSZ = OTC_Alignment::ofDouble();

  if ((theSize % ALSZ) != 0)
    theSize = (theSize / ALSZ) * ALSZ;

  OTCLIB_TRACER(MODULE) << "size (aligned) = " << theSize << endl;

  OTCLIB_ASSERT(theSize >= minimum());

  if (theSize < OTC_Heap::minimum())
    return;

  memory_ = theMemory;

  // Hmmm, not bothering to align these. Hope this is okay. For 32 bit
  // memory model it should as they work out to be a multiple of 8 bytes.

  size_t const HDRSZ = sizeof(OTC_HeapHeader);
  size_t const FTRSZ = sizeof(OTC_HeapFooter);

  // Work out where all the headers and footers are.

  OTC_HeapHeader* theFirstHeader;
  OTC_HeapFooter* theFirstFooter;
  OTC_HeapHeader* theBlockHeader;
  OTC_HeapFooter* theBlockFooter;
  OTC_HeapHeader* theFinalHeader;

  theFirstHeader = (OTC_HeapHeader*)(memory_);
  theFirstFooter = (OTC_HeapFooter*)(void*)(theFirstHeader+1);

  theBlockHeader = (OTC_HeapHeader*)(void*)(theFirstFooter+1);
  theBlockFooter = (OTC_HeapFooter*)(memory_+theSize-HDRSZ-FTRSZ);

  theFinalHeader = (OTC_HeapHeader*)(void*)(theBlockFooter+1);

  // Initialise the first blocks header and footer.

  theFirstHeader->llink = theBlockHeader;
  theFirstHeader->rlink = theBlockHeader;
  theFirstHeader->size = HDRSZ + FTRSZ;
  theFirstHeader->used = true;

  theFirstFooter->ulink = theFirstHeader;
  theFirstFooter->used = true;

  // Initialise the free blocks header and footer.

  theBlockHeader->llink = theFirstHeader;
  theBlockHeader->rlink = theFirstHeader;
  theBlockHeader->used = false;
  theBlockHeader->size = theSize - (2 * HDRSZ) - FTRSZ;

  OTCLIB_TRACER(MODULE) << "size (usable) = " << theBlockHeader->size << endl;

  theBlockFooter->ulink = theBlockHeader;
  theBlockFooter->used = false;

  // Initialise the final header.

  theFinalHeader->llink = 0;
  theFinalHeader->rlink = 0;
  theFinalHeader->used = true;
  theFinalHeader->size = HDRSZ;

  // Initialise the free list.

  freeList_ = theBlockHeader;
}

/* ------------------------------------------------------------------------- */
void* OTC_Heap::allocate(size_t theSize)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Heap::allocate(size_t)");

  if (memory_ == 0)
    return 0;

  OTCLIB_TRACER(MODULE) << "size (requested) = " << theSize << endl;

  size_t const HDRSZ = sizeof(OTC_HeapHeader);
  size_t const FTRSZ = sizeof(OTC_HeapFooter);
  size_t const MINSZ = HDRSZ + FTRSZ + sizeof(void*);

  // Allow for overheads and align size of memory.

  size_t const ALSZ = OTC_Alignment::ofDouble();

  theSize += HDRSZ + FTRSZ;

  OTCLIB_TRACER(MODULE) << "size (+overhead) = " << theSize << endl;

  if ((theSize % ALSZ) != 0)
    theSize = ((theSize / ALSZ) + 1) * ALSZ;

  OTCLIB_TRACER(MODULE) << "size (aligned) = " << theSize << endl;

  // Search for suitable block.

  OTC_HeapHeader* theHeader;
  OTC_HeapFooter* theFooter;

  theHeader = freeList_->rlink;

  do
  {
    if (!theHeader->used && theHeader->size >= theSize)
    {
      if (theHeader->size-theSize <= MINSZ)
      {
	// Allocate whole block.

	OTCLIB_TRACER(MODULE)
	 << "size (allocated) = " << theHeader->size << endl;

	removeBlock(theHeader);
	theFooter = theHeader->footer();
      }
      else
      {
	// Allocate from bottom of block.

	OTCLIB_TRACER(MODULE)
	 << "size (allocated) = " << theSize << endl;

	theHeader->size = theHeader->size-theSize;
	theFooter = theHeader->footer();
	theFooter->ulink = theHeader;
	theFooter->used = false;

	OTCLIB_TRACER(MODULE)
	 << "size (residual) = " << theHeader->size << endl;

	theHeader = (OTC_HeapHeader*)(void*)(theFooter+1);
	theHeader->size = theSize;
	theFooter = theHeader->footer();
	theFooter->ulink = theHeader;
      }

      theHeader->used = true;
      theFooter->used = true;
      return theHeader->block();
    }
    theHeader = theHeader->rlink;

  } while (theHeader != freeList_->rlink);

  return 0;
}

/* ------------------------------------------------------------------------- */
void OTC_Heap::release(void* theMemory)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Heap::release(void*)");

  if (theMemory == 0)
    return;

  OTC_HeapHeader* theHeader;
  OTC_HeapFooter* theFooter;
  OTC_HeapHeader* thePrev;
  OTC_HeapHeader* theNext;

  theHeader = header(theMemory);
  theFooter = theHeader->footer();
  thePrev = theHeader->prev();
  theNext = theHeader->next();

  size_t theSize = theHeader->size;

  OTCLIB_TRACER(MODULE) << "size (allocated) = " << theSize << endl;

  if (thePrev->used && theNext->used)
  {
     // Both adjacent blocks are in use.

     OTCLIB_TRACER(MODULE) << "(p&n used)" << endl;

     insertBlock(theHeader);
  }
  else if (!thePrev->used && theNext->used)
  {
    // Only the top block is free.

    OTCLIB_TRACER(MODULE) << "(n used)" << endl;

    thePrev->size += theSize;

    OTCLIB_TRACER(MODULE) << "size (merged) = " << thePrev->size << endl;

    theFooter->used = false;
    theFooter->ulink = thePrev;
  }
  else if (thePrev->used && !theNext->used)
  {
    // Only the bottom block is free.

    OTCLIB_TRACER(MODULE) << "(p used)" << endl;

    theHeader->size += theNext->size;

    OTCLIB_TRACER(MODULE) << "size (merged) = " << theHeader->size << endl;

    // theFooter->ulink = theHeader;
    theHeader->footer()->ulink = theHeader;

    removeBlock(theNext);
    insertBlock(theHeader);
  }
  else
  {
    // Both adjacent blocks are free.

    OTCLIB_TRACER(MODULE) << "(p&n free)" << endl;

    thePrev->size += (theSize + theNext->size);

    OTCLIB_TRACER(MODULE) << "size (merged) = " << thePrev->size << endl;

    thePrev->footer()->ulink = thePrev;
    removeBlock(theNext);
  }
}

/* ------------------------------------------------------------------------- */
size_t OTC_Heap::minimum()
{
  return 3*sizeof(OTC_HeapHeader) + 2*sizeof(OTC_HeapFooter) + sizeof(int);
}

/* ------------------------------------------------------------------------- */
void OTC_Heap::dump(ostream& outs) const
{
  OTC_HeapHeader* theHeader = freeList_->rlink;

  outs << "==>" << endl;

  do
  {
    outs << "header = " << (void*)theHeader << endl;
    outs << "header->llink = " << (void*)theHeader->llink << endl;
    outs << "header->rlink = " << (void*)theHeader->rlink << endl;
    outs << "header->size = " << theHeader->size << endl;
    outs << "header->used = " << theHeader->used << endl;

    OTC_HeapFooter* theFooter = theHeader->footer();

    outs << "footer = " << (void*)theFooter << endl;
    outs << "footer->ulink = " << (void*)theFooter->ulink << endl;
    outs << "footer->used = " << theFooter->used << endl;
    outs << endl;

    theHeader = theHeader->rlink;

  } while (theHeader != freeList_->rlink);

  outs << "<==" << endl;
}

/* ------------------------------------------------------------------------- */
