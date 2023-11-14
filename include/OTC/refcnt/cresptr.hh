#ifndef OTC_REFCNT_CRESPTR_HH
#define OTC_REFCNT_CRESPTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/cresptr.hh
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

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/refcnt/cresptr.hh"
#endif
#pragma interface "OTC/refcnt/cresptr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_CResPtr
    // = TITLE
    //	   Const version of a smart pointer to help manage a resource.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Designed to manage classes which contain the ability to keep a
    //	   reference count, such as classes derived from <OTC_Resource>.
    //
    // = SEE ALSO
    //	   <OTC_Resource>
{
  public:

    // = CONSTRUCTION

			OTC_CResPtr();
				// Creates a nil pointer class.

			OTC_CResPtr(T const* theItem);
				// Creates a pointer class for <theItem>.

			OTC_CResPtr(OTC_CResPtr<T> const& theRP);
				// Creates a pointer class for the object
				// pointed to by <theRP>.

    // = DESTRUCTION

			~OTC_CResPtr();
				// Decrements the reference count associated
				// with the object. If the object is no
				// longer required, it will be deleted.

    // = ACCESS

    T const*		item() const
				{ return object_; }
				// Returns the object being pointed at.

    T const*		operator->() const
				{ return itemNoNull_(); }
				// Returns the object being pointed at. An
				// exception is generated if no item is held.

			operator T const*() const
				{ return object_; }
				// Returns the object being pointed at.

    T const&		operator*() const
				{ return *itemNoNull_(); }
				// Returns a reference to the object being
				// pointed at. An exception is generated if
				// no item is held.

    bool		operator!() const
				{ return object_ == 0; }

    bool		operator==(int theItem) const
				{ return int(object_) == theItem; }

    bool		operator!=(int theItem) const
				{ return int(object_) != theItem; }

    bool		operator==(T const* theItem) const
				{ return object_ == theItem; }

    bool		operator!=(T const* theItem) const
				{ return object_ != theItem; }

    // = ASSIGNMENT
    //	   With all the following, the object being assigned to this class
    //	   will have its reference count incremented, the count on the
    //	   currently held object will be decremented and if it is no longer
    //	   required, it will be deleted.

    void		setItem(OTC_CResPtr<T> const& theRP);
				// Sets this class to point at the object
				// held by <theRP>.

    void		setItem(T const* theItem);
				// Sets this class to point at <theItem>.

    OTC_CResPtr<T>&	operator=(OTC_CResPtr<T> const& theRP);
				// Sets this class to point at the object
				// held by <theRP>.

    OTC_CResPtr<T>&	operator=(T const* theItem);
				// Sets this class to point at <theItem>.

    // = QUERY

    u_int		numRefs() const;
				// Returns the number of references to the
				// object held by this class, or <0> if this
				// class does not reference an object.

  protected:

    // These make a hole in the type safety to allow a non const derived
    // version of this class.

    T*			item_() const
				{ return object_; }
				// Returns the object being pointed at.

    T*			itemNoNull_() const;
				// Returns the object being pointed at.
				// Raises an exception if pointer is a
				// null pointer.

  private:

    T*			object_;
				// The item being held.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/refcnt/cresptr.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_CRESPTR_HH */
