#ifndef OTC_MEMORY_POOL_HH
#define OTC_MEMORY_POOL_HH

/*
// This file is part of the OSE C++ class library.
// 
// Copyright 1993 OTC LIMITED
// Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
*/

#include <OTC/OTC.h>

#include <stddef.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/pool.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

//! \class OTC_Pool pool.hh OTC/memory/pool.hh
//! \ingroup memory_management
//!
//! \brief A fast memory allocator for objects of a specific size.
//!
//! This is a fast memory allocator modelled after a Pool class by Dag Bruck.
//! which was in turn modelled after the Pool class in the USL C++ Class
//! Library.
//!
//! \remarks Note that the constructors do not allocate an initial block of
//! memory. The first block will only be allocated the first time \c
//! allocate() is called. If the maximum block size is not of sufficient size
//! to hold one block and enough space for internal requirements, more than
//! the maximum size will be allocated.
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_Pool
{
  private:

    //! The beginning of a pool element within a block.

    struct OSE_EXPORT	OTC_PoolElement { OTC_PoolElement* next; };

    //! The beginning of a block of allocated pool memory.

    struct OSE_EXPORT	OTC_PoolBlock { OTC_PoolBlock* next; };

    OTC_PoolBlock*	blocks_;
				///< List of memory blocks from which elements
				///< are allocated.

    size_t		blockSize_;
				///< The size of a memory block.

    size_t		blockCount_;
				///< The number of memory blocks.

    size_t		headerSize_;
				///< Offset into memory of first element.

    OTC_PoolElement*	elements_;
				///< The list of free pool elements.

    size_t		elementSize_;
				///< The size of a pool element. This number
				///< is rounded up to a multiple of some small
				///< number (to ensure alignment).

    size_t		elementCount_;
				///< The number of pool elements which fit in
				///< a memory block.

    size_t		capacity_;
				///< The total number of elements managed by
				///< this pool. This includes both allocated
				///< and free.

    size_t		allocated_;
				///< Count of the number of pool elements
				///< currently allocated to users.

    size_t		total_;
				///< Total number of all allocations ever made
				///< from this memory pool.

  public:

    //! \name Pool Construction

    //@{

			OTC_Pool(size_t theSize);
				///< Creates a pool for blocks of memory of
				///< \a theSize. If the environment variable
				///< \c OTCLIB_POOLBLOCKSIZE is set to an
				///< integer value, it will be used as the the
				///< maximum block size when allocating new
				///< blocks of memory from the operating
				///< system. If the environment variable is
				///< not defined, a default of 2040 bytes will
				///< be used.

			OTC_Pool(size_t theSize, size_t theBlockSize)
			  : blocks_(0),
			    blockCount_(0),
			    elements_(0),
			    capacity_(0),
			    allocated_(0),
			    total_(0)
				{ initialise(theSize,theBlockSize); }
				///< Creates a pool for blocks of memory of \a
				///< theSize. The value \a theBlockSize, will
				///< be used as the maximum block size when
				///< allocating new blocks of memory from the
				///< operating system.

    //@}

    //! \name Pool Destruction

    //@{

			~OTC_Pool();
				///< Returns all memory allocated by the pool
				///< to the operating system, unless there is
				///< still memory allocated to the user, in
				///< which case an exception is raised.

    //@}

    //! \name Pool Characteristics

    //@{

    size_t		elementSize() const
				{ return elementSize_; }
				///< Returns the maximum size of the objects
				///< which can allocated by the pool. Note
				///< that if smaller object are allocated, the
				///< memory returned will still be of the size
				///< indicated by this function.

    size_t		blockSize() const
				{ return blockSize_; }
				///< Returns the block size used when
				///< allocating more memory from the operating
				///< system.

    size_t		blockCount() const
				{ return blockCount_; }
				// The number of blocks allocated from
				// the operating system.

    size_t		elementCount() const
				{ return elementCount_; }
				///< Returns the number of objects of the size
				///< managed by this memory pool, which can
				///< fit into each block of memory which is
				///< allocated from the operating system.

    size_t		capacity() const
				{ return capacity_; }
				///< Returns the total number of objects which
				///< can be managed by this pool. This is the
				///< sum of the free object elements and those
				///< which have already been allocated.

    size_t		allocated() const
				{ return allocated_; }
				///< Returns the number of objects currently
				///< allocated from this pool.

    size_t		available() const
				{ return capacity_ - allocated_; }
				///< Returns the number of objects which could
				///< still be allocated from this pool before
				///< more memory needed to be allocated.

    size_t		total() const
				{ return total_; }
				///< Returns the total number of allocations
				///< ever made using this memory pool.
    //@}

    //! \name Memory Allocation

    //@{

    void*		allocate()
				{
				  if (elements_ == 0)
				    grabMemory();
				  OTC_PoolElement* e = elements_;
				  elements_ = e->next;
				  allocated_++;
				  total_++;
				  return e;
				}
				///< A new element is allocated from the pool
				///< and returned. If there are no free
				///< elements, more memory will be allocated
				///< from the operating system.

    void		release(void* theMemory)
				{
				  OTCLIB_ASSERT(allocated_ != 0);
				  OTC_PoolElement* e;
				  e = (OTC_PoolElement*)theMemory;
				  e->next = elements_;
				  elements_ = e;
				  allocated_--;
				}
				///< The element of the pool addressed by \a
				///< theMemory is freed. The element must have
				///< been allocated from this pool. If the
				///< memory was not from this pool, the
				///< results are undefined.

    void		grabMemory(size_t theSize=0);
				///< Grabs a new block of memory from the
				///< operating system and splits it up in to
				///< elements that are then inserted into the
				///< free list. The size of the block of
				///< memory allocated will be the larger of \a
				///< theSize and the default block size.

    //@}

  private:

			OTC_Pool(OTC_Pool const&);
				// Do not define an implementation for this.

    OTC_Pool&		operator=(OTC_Pool const&);
				// Do not define an implementation for this.

    void		initialise(size_t theSize, size_t theBlockSize);
				///< Initialises the element size, block size
				///< and element count for this pool, given
				///< that the user wants blocks of memory of
				///< size \a theSize, and wants allocations of
				///< memory from the operating system to be in
				///< the region of \a theBlockSize.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_POOL_HH */
