#ifndef OTC_MEMORY_MPOBJECT_HH
#define OTC_MEMORY_MPOBJECT_HH

/*
// This file is part of the OSE C++ class library.
// 
// Copyright 1994 TELSTRA CORPORATION LIMITED
// Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
*/

#include <OTC/memory/cmmnpool.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/mpobject.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4291)
#endif

/* ------------------------------------------------------------------------- */

//! \class OTC_MPObject mpobject.hh OTC/memory/mpobject.hh
//! \ingroup memory_management
//!
//! \brief Base class for objects allocated against the common memory pool.
//!
//! The \c OTC_MPObject can be used as a base class to cause the memory for
//! the instance of the derived class object to be allocated from the set of
//! common memory pools.
//!
//! By default the common memory pools manage objects which require up to 64
//! bytes of memory. If a class which is derived from this class is actually
//! greater in size than 64 bytes, the memory request will be passed straight
//! through to the memory allocation library provided by the operating system.
//! Thus this class can always be safely used, even if some derived class
//! might exceed the largest size managed by the common memory pool system.
//!
//! \see OTC_CommonPool

class OSE_EXPORT OTC_MPObject
{
  public:

    virtual		~OTC_MPObject();

    //! \name Memory Allocation

    //@{

    void*		operator new(size_t theSize)
    				{ return OTC_CommonPool::allocate(theSize); }
				///< Allocates and returns memory of \c
				///< theSize which has been allocated by using
				///< the common memory pools.

    void		operator delete(void* theMemory, size_t theSize)
    				{ OTC_CommonPool::release(theMemory,theSize); }
				///< Returns to the common memory pools \c
				///< theMemory with \c theSize.

    //@}

  protected:

			OTC_MPObject() {}

  private:

			OTC_MPObject(OTC_MPObject const&) {}
				///< Do not define an implementation for this.

    OTC_MPObject&	operator=(OTC_MPObject const&);
				///< Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_MPOBJECT_HH */
