#ifndef OTC_MEMORY_HEAP_HH
#define OTC_MEMORY_HEAP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/heap.hh
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

#include <OTC/OTC.h>

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/heap.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_HeapHeader;

class OSE_EXPORT OTC_HeapFooter
{
  public:

    OTC_HeapHeader*	ulink;

    bool		used;
};

class OSE_EXPORT OTC_HeapHeader
{
  public:

    OTC_HeapHeader*	llink;

    OTC_HeapHeader*	rlink;

    size_t		size;

    bool		used;

    OTC_HeapFooter*	footer();

    void*		block()
				{ return (void*)(this+1); }

    OTC_HeapHeader*	prev()
				{
				  return (((OTC_HeapFooter*)
				   (void*)this)-1)->ulink;
				}

    OTC_HeapHeader*	next();
};

inline OTC_HeapFooter* OTC_HeapHeader::footer()
{ return (OTC_HeapFooter*)(((char*)this)+size)-1; }

inline OTC_HeapHeader* OTC_HeapHeader::next()
{ return (OTC_HeapHeader*)(((char*)this)+size); }

class OSE_EXPORT OTC_Heap
    // = TITLE
    //    Memory allocator.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_Heap> provides heap style memory management, for a
    //     chunk of memory. The class uses the boundary tag method of
    //     allocation. The implementation is based on ideas from the book
    //     <{C++, A Guide for C Programmers}> by <{Sharam Hekmatpour}>.
{
  private:

    OTC_HeapHeader*	header(void* theMemory) const
				{ return ((OTC_HeapHeader*)theMemory)-1; }

    void		insertBlock(OTC_HeapHeader* theHeader)
				{
				  theHeader->used = false;
				  theHeader->footer()->used = false;
				  theHeader->llink = freeList_;
				  theHeader->rlink = freeList_->rlink;
				  theHeader->rlink->llink = theHeader;
				  theHeader->llink->rlink = theHeader;
				}

    void		removeBlock(OTC_HeapHeader* theHeader)
				{
				  freeList_ = theHeader->llink;
				  theHeader->rlink->llink = theHeader->llink;
				  theHeader->llink->rlink = theHeader->rlink;
				}

  public:

    // = CONSTRUCTION

			OTC_Heap(char* theMemory, size_t theSize);
				// Creates a memory heap using <theMemory>,
				// where <theSize> is the size of the
				// memory. <theSize> must be greater than or
				// equal to <OTC_Heap::minimum()>. If
				// <theSize> is less, <0> will always be
				// returned by <allocate()>. <theMemory> is
				// not deleted if this class is ever deleted,
				// ie., it is your responsibility to delete
				// <theMemory> if required.

    // = ALLOCATION/DEALLOCATION

    void*		allocate(size_t theSize);
				// Allocates and returns a piece of memory of
				// <theSize>. If there is insufficient memory,
				// <0> is returned.

    void		release(void* theMemory);
				// Returns <theMemory> back to the heap. If
				// the memory was not originally allocated
				// from this heap, the result is undefined.

    // = QUERY

    static size_t	minimum();
				// Returns the minimum size permitted, for
				// the array of memory the heap is to use.

    // = DEBUG

    void		dump(ostream& outs) const;
				// Dumps information about the heap structure
				// to <outs>. Useful for debugging purposes
				// only.

  private:

    char*		memory_;
				// Pointer to array from which blocks of
				// memory are being allocated.

    OTC_HeapHeader*	freeList_;
				// Pointer to free list objects.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_HEAP_HH */
