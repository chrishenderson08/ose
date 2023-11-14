#ifndef OTC_MEMORY_ARENA_HH
#define OTC_MEMORY_ARENA_HH

/*
// This file is part of the OSE C++ class library.
// 
// Copyright 1993 OTC LIMITED
// Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
*/

#include <OTC/memory/mpobject.hh>
#include <OTC/memory/align.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/arena.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

//! \internal
//! \class OTC_ArenaBlock arena.hh OTC/memory/arena.hh
//!
//! \brief Class to keep track of arena block information.
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_ArenaBlock : public OTC_MPObject
{
  public:

			OTC_ArenaBlock() {}

			~OTC_ArenaBlock();

    char*		block;

    char*		free;

    size_t		size;

    size_t		capacity;

    OTC_ArenaBlock*	next;
};

//! \class OTC_Arena arena.hh OTC/memory/arena.hh
//! \ingroup memory_management
//!
//! \brief A memory allocator which concatenates requests into blocks.
//!
//! The \c OTC_Arena class implements a memory allocator which obtains blocks
//! of memory using operator \c new and then parcels the memory out in
//! pieces. All memory allocated must be released at the same time. One can
//! release memory for reuse through the same arena without actually
//! destroying the whole arena.
//!
//! \note If a request for memory greater than the block size is requested,
//! it will be allocated directly from the free store.
//!
//! \note The default block size is 2040. A weird value calculated by looking
//! at size of blocks allocated by GNU malloc and BSD malloc. Sun malloc used
//! first fit, so the size doesn't matter too much when using it.
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_Arena
{
  public:

    //! \name Arena Construction

    //@{

			OTC_Arena(size_t theAlign=OTC_Alignment::ofDouble());
				///< Initialises the class. The block size
				///< used for allocating memory will be 2040
				///< bytes unless overridden by the
				///< environment variable \c
				///< OTCLIB_ARENABLOCKSIZE. When the amount of
				///< free space in a block falls below 16
				///< bytes, the class will stop trying to
				///< allocate memory from that block. The slop
				///< value can be overridden by setting the
				///< environment variable \c
				///< OTCLIB_ARENASLOPSIZE. Memory returned
				///< will be aligned according to \a theAlign.
				///< The default is to align memory to that
				///< required by the type \c double. If \a
				///< theAlign is 0 an exception is raised.

			OTC_Arena(
			 size_t theBlockSize,
			 size_t theSlop,
			 size_t theAlign=OTC_Alignment::ofDouble()
			);
				///< Initialises the class. \a theBlockSize
				///< should be the minimum amount of memory
				///< allocated from the free store. When the
				///< amount of free space in a block decreases
				///< below \a theSlop, the class stops trying
				///< to allocate from that block. Memory
				///< returned will be aligned according to \a
				///< theAlign. The default is to align memory
				///< to that required by the type \c double.
				///< If \a theAlign is 0 an exception is
				///< raised.

    //@}


    //! \name Arena Destruction

    //@{

			~OTC_Arena();
				///< Returns all memory to the free store.

    //@}

    //! \name Arena Characteristics

    //@{

    size_t		blockSize() const
      				{ return blockSize_; }
				///< Returns the size of the blocks being
				///< allocated.

    size_t		slop() const
      				{ return slop_; }
				///< Returns the threshold which determines
				///< when a block is full. That is, when a
				///< block has less than this number of bytes
				///< available, it is marked as full.


    size_t		align() const
      				{ return align_; }
				///< Returns the alignment constraint with
				///< respect to the start of any memory
				///< returned.

    //@}

    //! \name Memory Blocks

    //@{

    //! \internal The following are calculated on request as it isn't
    //! anticipated that they would be used for anything other than debugging
    //! or performance analysis.

    size_t		freeBlocks() const;
				///< Returns the number of blocks allocated
				///< which still have free space in them.

    size_t		fullBlocks() const;
				///< Returns the number of blocks allocated
				///< which have been marked as full.

    size_t		allocated() const
				{ return allocated_; }
				///< Returns the total number of bytes which
				///< are in use across all blocks. Ie., sum of
				///< the sizes of blocks returned by the
				///< \c allocate() member function.
    //@}

    //! \name Memory Allocation

    //@{

    void*		allocate(size_t theSize);
				///< Returns a piece of memory of size
				///< \a theSize.

    void		release(size_t theBlocks=1);
				///< Indicates you are finished with the
				///< memory that has been allocated by the
				///< arena, but that you don't want to destroy
				///< the arena just yet. \a theBlocks is the
				///< maximum number of blocks you want the
				///< arena to keep hold of. That is, this
				///< number of blocks will not actually be
				///< freed but will be available for reuse by
				///< the arena. Blocks which were greater in
				///< size than the specified block size will
				///< not be kept.

    //@}

  private:

			OTC_Arena(OTC_Arena const&);
				// Do not define an implementation for this.

    OTC_Arena&		operator=(OTC_Arena const&);
				// Do not define an implementation for this.

    void*		allocateNewBlock(size_t theSize);
				///< Allocates a new block of memory and
				///< allocate \a theSize memory out of it.

    size_t		blockSize_;
				///< Minimum block size in which allocations
				///< of free store memory should be made.

    size_t		slop_;
				///< When the amount of free space in a block
				///< decreases below this amount we stop
				///< trying to allocate more strings from it.

    size_t		align_;
				///< Memory alignment.

    OTC_ArenaBlock*	full_;
				///< The blocks of memory which are full.

    OTC_ArenaBlock*	free_;
				///< The blocks of memory which still hold
				///< some free space.

    size_t		allocated_;
				///< Number of bytes which have been
				///< allocated.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_ARENA_HH */
