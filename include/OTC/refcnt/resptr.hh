#ifndef OTC_REFCNT_RESPTR_HH
#define OTC_REFCNT_RESPTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/resptr.hh
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

#include <OTC/refcnt/cresptr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/resptr.hh"
#pragma interface "OTC/collctn/resptr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_ResPtr : public OTC_CResPtr<T>
    // = TITLE
    //	   Smart pointer to help manage a resource.
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

			OTC_ResPtr() {}
				// Creates a nil pointer class.

			OTC_ResPtr(T* theItem)
			  : OTC_CResPtr<T>(theItem) {}
				// Creates a pointer class for <theItem>.

			OTC_ResPtr(OTC_ResPtr<T> const& theRP)
			  : OTC_CResPtr<T>(theRP) {}
				// Creates a pointer class for the object
				// pointed to by <theRP>.

    // = ACCESS

    T*			item() const
				{ return item_(); }
				// Returns the object being pointed at.

    T*			operator->() const
				{ return itemNoNull_(); }
				// Returns the object being pointed at. An
				// exception is generated if no item is held.

			operator T*() const
				{ return item_(); }
				// Returns the object being pointed at.

    T&			operator*() const
				{ return *itemNoNull_(); }
				// Returns a reference to the object being
				// pointed at. An exception is generated if
				// no item is held.


    // = ASSIGNMENT
    //	   With all the following, the object being assigned to this class
    //	   will have its reference count incremented, the count on the
    //	   currently held object will be decremented and if it is no longer
    //	   required, it will be deleted.

    OTC_ResPtr<T>&	operator=(T* theItem)
				{ setItem(theItem); return *this; }
				// Sets this class to point at <theItem>.

    OTC_ResPtr<T>&	operator=(OTC_ResPtr<T> const& theRP)
				{ setItem(theRP); return *this; }
				// Sets this class to point at the object
				// held by <theRP>.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_RESPTR_HH */
