#ifndef OTC_REFCNT_CTRPTR_HH
#define OTC_REFCNT_CTRPTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/ctrptr.hh
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

#include <OTC/refcnt/cctrptr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/ctrptr.hh"
#pragma interface "OTC/collctn/ctrptr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_CtrPtr : public OTC_CCtrPtr<T>
    // = TITLE
    //     Smart pointer to help manage resources.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Designed to manage classes which do not contain the ability to
    //     keep a reference count.
{
  public:

    // = CONSTRUCTION

			OTC_CtrPtr() {}
				// Creates a nil pointer class.

			OTC_CtrPtr(T* theItem)
			  : OTC_CCtrPtr<T>(theItem) {}
				// Creates a pointer class for <theItem>.

			OTC_CtrPtr(OTC_CtrPtr<T> const& theCtrPtr)
			  : OTC_CCtrPtr<T>(theCtrPtr) {}
				// Creates a pointer class for the
				// object pointed to by <theCtrPtr>.

    // = ACCESS

    T*			item() const
				{ return item_(); }
				// Returns the object being pointed at.

    T*			operator->() const
				{ return itemNoNull_(); }
				// Returns the object being pointed at.
				// If no item is held, an exception
				// is raised.

			operator T*() const
				{ return item_(); }
				// Returns the object being pointed at.

    T&			operator*() const
				{ return *itemNoNull_(); }
				// Returns a reference to the item being
				// held. If no item is held, an exception
				// is raised.

    // = ASSIGNMENT
    //     With all the following, the count on the currently held
    //     object will be decremented, and if it is no longer required,
    //     it will be deleted.

    OTC_CtrPtr<T>&	operator=(OTC_CtrPtr<T> const& theCtrPtr)
				{ setItem(theCtrPtr); return *this; }
				// Sets this class to point at the object
				// held by <theCtrPtr>.

    OTC_CtrPtr<T>&	operator=(T* theItem)
				{ setItem(theItem); return *this; }
				// Sets this class to point at <theItem>.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_CTRPTR_HH */
