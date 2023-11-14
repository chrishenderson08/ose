#ifndef OTC_REFCNT_CCTRVPTR_HH
#define OTC_REFCNT_CCTRVPTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/cctrvptr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/refcnt/count.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/refcnt/cctrvptr.hh"
#endif
#pragma interface "OTC/refcnt/cctrvptr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_CCtrVecPtr
    // = TITLE
    //	   Smart pointer to help manage resources.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Designed to manage vectors of classes.
{
  public:

    // = CONSTRUCTION

			OTC_CCtrVecPtr();
				// Creates a nil pointer class.

			OTC_CCtrVecPtr(T const* theItem);
				// Creates a pointer class for <theItem>.

			OTC_CCtrVecPtr(OTC_CCtrVecPtr<T> const& theCtrPtr);
				// Creates a pointer class for the
				// object pointed to by <theCtrPtr>.

    // = DESTRUCTION

			~OTC_CCtrVecPtr();
				// Decrements the reference count associated
				// with the object, and destroys it if it is
				// no longer being used.

    // = ACCESS

    T const*		item() const
				{ return array_; }
				// Returns the object being pointed at.

    T const&		operator[](u_int theIndex) const
				{ return itemNoNull_()[theIndex]; }
				// Returns a reference to the object in the
				// vector at location <theIndex>. Note that
				// no bounds checking is done. Also, if no
				// item is held, an exception is generated.

			operator T const*() const
				{ return array_; }
				// Returns the object being pointed at.

    T const&		operator*() const
				{ return *itemNoNull_(); }
				// Returns a reference to the first object in
				// the vector being held. If no item is held,
				// an exception is generated.

    bool		operator!() const
				{ return array_ == 0; }

    bool		operator==(int theItem) const
				{ return int(array_) == theItem; }

    bool		operator!=(int theItem) const
				{ return int(array_) != theItem; }

    bool		operator==(T const* theItem) const
				{ return array_ == theItem; }

    bool		operator!=(T const* theItem) const
				{ return array_ != theItem; }

    // = ASSIGNMENT
    //	   With all the following, the count on the currently held object
    //	   will be decremented, and if it is no longer required, it will be
    //	   deleted.

    void		setItem(OTC_CCtrVecPtr<T> const& theCtrPtr);
				// Sets this class to point at the object
				// held by <theCtrPtr>.

    void		setItem(T const* theItem);
				// Sets this class to point at <theItem>.

    OTC_CCtrVecPtr<T>&	operator=(OTC_CCtrVecPtr<T> const& theCtrPtr);
				// Sets this class to point at the object
				// held by <theCtrPtr>.

    OTC_CCtrVecPtr<T>&	operator=(T const* theItem);
				// Sets this class to point at <theItem>.

    // = QUERY

    u_int		numRefs() const;
				// Returns the number of references to the
				// object held by this class.

  protected:

    // These make a hole in the type safety to allow a non const derived
    // version of this class.

    T*			item_() const
				{ return array_; }
				// Returns the object being pointed at.

    T*			itemNoNull_() const;
				// Returns the object being pointed at.
				// Raises an exception if pointer is a
				// null pointer.

  private:

    void		cleanup();
				// Performs the job of deleting the object,
				// when no longer needed.

    T*			array_;
				// The item being held.

    OTC_Count*		count_;
				// The counter keeping the reference count.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/refcnt/cctrvptr.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_CCTRVPTR_HH */
