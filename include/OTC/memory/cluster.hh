#ifndef OTC_MEMORY_CLUSTER_HH
#define OTC_MEMORY_CLUSTER_HH

/*
// This file is part of the OSE C++ class library.
// 
// Copyright 1994 TELSTRA CORPORATION LIMITED
// Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
*/

#include <OTC/memory/mcobject.hh>
#include <OTC/memory/mpobject.hh>
#include <OTC/memory/arena.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/cluster.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

//! \class OTC_Cluster cluster.hh OTC/memory/cluster.hh
//! \ingroup memory_management
//!
//! \brief Cluster of objects which are to be deleted at the same time.
//!
//! The \c OTC_Cluster class is a pool of objects which are to be deleted at
//! the same time. Memory for the objects is allocated from a memory arena.
//! Actual class objects to be allocated from the cluster need to be derived
//! from the \c OTC_MCObject class. Character strings can be allocated from
//! the cluster using the \c allocate() member function.
//!
//! \code
//!   class Foo : public OTC_MCObject { protected: ~Foo(); };
//!
//!   OTC_Cluster cluster;
//!   Foo* foo1 = new (cluster) Foo;
//!   Foo* foo2 = new (*OTC_Cluster::of(foo1)) Foo;
//!   char* str1 = OTC_Cluster::of(foo1)->allocate(256);
//! \endcode
//!
//! \see OTC_MCObject, OTC_Arena

class OSE_EXPORT OTC_Cluster : public OTC_MPObject
{
    friend class	OTC_MCObject;

  public:

    //! \name Cluster Construction

    //@{

			OTC_Cluster();
				///< Creates an empty cluster. Memory
				///< allocation used by the \c OTC_Arena class
				///< will be as per the default for that
				///< class.

			OTC_Cluster(size_t theBlockSize, size_t theSlop=16);
				///< Creates an empty cluster. \a theBlockSize
				///< and \a theSlop are passed onto the \c
				///< OTC_Arena. \a theSlop defaults to 16.

    //@}

    //! \name Cluster Destruction

    //@{

			~OTC_Cluster();
				///< The destructor for each object in the
				///< cluster is invoked, and memory released
				///< back to the system. The order in which
				///< objects are destroyed is such that the
				///< most recently allocated object is
				///< destroyed first.

    //@}

    //! \name Memory Allocation

    //@{

    char*		allocate(size_t theLength)
      				{ return (char*)arena_.allocate(theLength); }
				///< Allocates and returns a block of memory
				///< of size \a theLength. The memory is
				///< associated with this memory cluster and
				///< will be destroyed at the same time that
				///< the cluster is destroyed. The memory
				///< which is returned has worst case
				///< alignment and thus is suitable for
				///< holding characters strings or arrays of
				///< objects containing integers or floating
				///< point values. Due to the memory returned
				///< needing to be aligned, when allocations
				///< aren't for a multiple of the alignment
				///< size, usually 8 bytes, memory can be
				///< wasted.

    size_t		allocated() const
				{ return arena_.allocated(); }
				///< Returns the total number of bytes which
				///< have been allocated from the memory
				///< cluster. This includes both explicit
				///< allocations and those resulting from
				///< creating derived instances of the memory
				///< cluster object \c OTC_MCObject.

    //@}

    //! \name Object Cluster

    //@{

    static OTC_Cluster*	of(OTC_MCObject const* theObject)
      				{ return (theObject!=0)?theObject->cluster_:0; }
				///< Returns the cluster associated with a
				///< particular instance of \c OTC_MCObject
				///< given by \a theObject. If \a theObject is
				///< 0 or \a theObject wasn't allocated from a
				///< cluster then 0 will be returned.

    //@}

  private:

			OTC_Cluster(OTC_Cluster const&);
				///< Do not define an implementation for this.

    OTC_Cluster&	operator=(OTC_Cluster const&);
				///< Do not define an implementation for this.

    OTC_Arena		arena_;
				///< Arena from which memory is allocated.
				///< This memory arena has worst case
				///< alignment so that objects can have
				///< members of type double and not have
				///< problems.

    OTC_MCObject*	objects_;
				///< Pointer to most recently allocated object
				///< in this cluster.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_CLUSTER_HH */
