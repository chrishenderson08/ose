#ifndef OTC_MEMORY_CMMNPOOL_HH
#define OTC_MEMORY_CMMNPOOL_HH

/*
// This file is part of the OSE C++ class library.
// 
// Copyright 1993 OTC LIMITED
// Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
*/

#include <OTC/memory/pool.hh>
#include <OTC/thread/nrmutex.hh>

#include <OTC/stdlib/ostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/cmmnpool.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

//! \class OTC_CommonPool cmmnpool.hh OTC/memory/cmmnpool.hh
//! \ingroup memory_management
//!
//! \brief Common memory pool specifically for small objects.
//!
//! The purpose of the common memory pools for small objects is to improve
//! runtime performance where large numbers of allocations and deletions of
//! small objects are occurring. This is especially pertinent to C++ where
//! an emphasis exists on modelling all types as classes and where more often
//! than not they are allocated from heap memory.
//!
//! The use of a set of common memory pools specifically for small objects
//! improves performance because the majority of memory allocation libraries
//! provided by the operating system do not perform very well for small
//! objects. Either the memory allocation libraries are wasteful of memory or
//! perform very badly when small objects are released and blocks of memory
//! need to be coalesced back together.
//!
//! The use of memory pools avoids these problems by allocating larger blocks
//! of memory and then dividing them up for use by small objects of a
//! particular maxiumum size only. The smaller blocks of memory are then
//! maintained in free lists according to their size until required. This
//! makes allocation of small objects quicker and has less performance impact
//! on memory managed by the operating system memory allocation library.
//!
//! The simplest way to start utilising the common memory pools for your own
//! classes is to derive your class from the class \c OTC_MPObject.
//! Alternatively, you will need to override the operators for \c new and \c
//! delete in your class to call the memory allocation routines provided by
//! this class.
//!
//! \warning If you override the operators for \c new and \c delete in your
//! class to call the memory allocation routines provided by  this class, your
//! class must provide a virtual destructor if that class could in turn be
//! derived from. If this is not done the size of the memory required as
//! passed to the overloaded operators will be incorrect.
//!
//! By default 8 memory pools are created, with the size of the pools varying
//! by \c sizeof(double). Thus, on 32 bit machines, there will be pools
//! ranging from 8 bytes to 64 bytes, with an 8 byte difference in the size of
//! each pool.
//!
//! When an allocation request is received, a block of memory from the memory
//! pool of smallest size greater than or equal to the size of memory
//! requested, is returned. If a piece of memory larger than the size of the
//! largest memory pool is requested, the global \c new and \c delete are
//! used.
//!
//! The default block size used by each pool is that determined by the \c
//! OTC_Pool class. To define the block size to be used for a specific pool,
//! you should define the \c OTCLIB_POOLBLOCKSIZE_X environment variable,
//! where 'X' is replaced with the number of the pool. Pool numbering starts
//! at '1' with the smallest pool. The value of the variable must be greater
//! than '0'.
//!
//! A pool may be made to preallocate memory from the operating system by
//! specifying the \c OTCLIB_POOLPREALLOCATE_X environment variable, where 'X'
//! is replaced with the number of the pool. The value of the environment
//! variable should be the amount of memory to preallocate. If the value
//! defined is not greater than the blocksize for that pool, the memory
//! preallocated will be that defined by the block size.
//!
//! The use of the common memory pools can be disabled at runtime with the
//! global \c new and \c delete instead used directly for each individual
//! allocation, by defining the environment variable \c OTCLIB_NOCOMMONPOOL.
//! If you wish to experiment with a larger number of memory pools, you can
//! override the environment variable \c OTCLIB_COMMONPOOLS, setting it to the
//! number of actual pools to create.
//!
//! Before and after attempting to change the behaviour of the common memory
//! pooling mechanism using the specified environment variables, it is
//! recommended that the \c usageSummary() member function be used to dump out
//! statistics of current use. This might be done on program termination or
//! periodically during the execution of a program.
//!
//! \see OTC_Pool, OTC_MPObject
//!
//! \author Graham Dumpleton

class OSE_EXPORT OTC_CommonPool
{
    friend class	OTC_CommonPoolDummy;
				///< Included to shut up GNU C++ warning.

  public:

    //! \name Memory Allocation

    //@{

    static void*	allocate(size_t theSize);
				///< Allocates and returns a piece of memory
				///< of sufficient size to hold an object of
				///< \a theSize.

    static void		release(void* theMem, size_t theSize);
				///< Releases \a theMem. \a theSize should be
				///< the same as the size used to originally
				///< allocate the memory. If the memory was
				///< not originally allocated from this pool,
				///< the result is undefined.

    //@}

    //! \name Memory Pools

    //@{

    static size_t	numPools()
				{ return numPools_; }
				///< Returns the number of memory pools.

    static OTC_Pool const*	pool(size_t theNum)
				{
				  return (!theNum || theNum > numPools_)
				   ? 0 : pools_[theNum-1];
				}
				///< Returns a pointer to the pool at position
				///< \a theNum. Pool positions start at \c 1.
				///< The largest pool number is that given by
				///< calling \c numPools().

    //@}

    //! \name Usage Statistics

    //@{

    static void		usageSummary(ostream& theStream);
				///< Dump memory pool usage summary to \a
				///< theStream.

    //@}

  private:

			OTC_CommonPool();
				///< Do not define an implementation for this.

			OTC_CommonPool(OTC_CommonPool const&);
				///< Do not define an implementation for this.

    OTC_CommonPool&	operator=(OTC_CommonPool const&);
				///< Do not define an implementation for this.

    static void		initialise();
				///< Initialises pool table and lookup index.

    static OTC_NRMutex	mutex_;
				///< Lock for threads. Use non recursive mutex
				///< as there shouldn't be any way for a
				///< thread to come across the lock again
				///< while it is locked.

    static bool		init_;
				///< Set to \c true once the class has been
				///< initialised.

    static bool		disable_;
				///< Set to \c true if memory pools are
				///< disabled.

    static OTC_Pool**	pools_;
				///< Set of memory pools.

    static size_t	numPools_;
				///< The number of pools.

    static size_t	increment_;
				///< The size of the smallest pool and the
				///< difference in size between the subsequent
				///< pools.

    static size_t	largest_;
				///< Size of the largest pool. When asked to
				///< allocate memory larger than this we use
				///< standard \c new and \c delete.

    static size_t*	index_;
				///< Fast lookup index to avoid doing
				///< divisions all the time.

    static size_t	allocated_;
				///< The current number of allocated objects.

    static size_t	total_;
				///< Total number of allocations ever made.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_CMMNPOOL_HH */
