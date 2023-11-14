#ifndef OTC_REFCNT_CTRVPTR_HH
#define OTC_REFCNT_CTRVPTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/ctrvptr.hh
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

#include <OTC/refcnt/cctrvptr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/ctrvptr.hh"
#pragma interface "OTC/collctn/ctrvptr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_CtrVecPtr : public OTC_CCtrVecPtr<T>
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

			OTC_CtrVecPtr() {}
				// Creates a nil pointer class.

			OTC_CtrVecPtr(T* theItem)
			  : OTC_CCtrVecPtr<T>(theItem) {}
				// Creates a pointer class for <theItem>.

			OTC_CtrVecPtr(OTC_CtrVecPtr<T> const& theCtrPtr)
			  : OTC_CCtrVecPtr<T>(theCtrPtr) {}
				// Creates a pointer class for the
				// object pointed to by <theCtrPtr>.

    // = ACCESS

    T*			item() const
				{ return item_(); }
				// Returns the object being pointed at.

    T&			operator[](u_int theIndex) const
				{ return itemNoNull_()[theIndex]; }
				// Returns a reference to the object in the
				// vector at location <theIndex>. Note that
				// no bounds checking is done. Also, if no
				// item is held, an exception is generated.

			operator T*() const
				{ return item_(); }
				// Returns the object being pointed at.

    T&			operator*() const
				{ return *itemNoNull_(); }
				// Returns a reference to the first object in
				// the vector being held. If no item is held,
				// an exception is generated.

    // = ASSIGNMENT
    //	   With all the following, the count on the currently held object
    //	   will be decremented, and if it is no longer required, it will be
    //	   deleted.

    OTC_CtrVecPtr<T>&	operator=(OTC_CtrVecPtr<T> const& theCtrPtr)
				{ setItem(theCtrPtr); return *this; }
				// Sets this class to point at the object
				// held by <theCtrPtr>.

    OTC_CtrVecPtr<T>&	operator=(T* theItem)
				{ setItem(theItem); return *this; }
				// Sets this class to point at <theItem>.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_CTRVPTR_HH */
