#ifndef OTC_MEMORY_PTR_HH
#define OTC_MEMORY_PTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/ptr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/memory/ptr.hh"
#pragma interface "OTC/memory/ptr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Ptr
    // = TITLE
    //     Pointer which creates object only when required.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Designed such that the object being managed is only created
    //     when required. Also deletes the object when pointer class
    //     is destroyed.
    //
    // = NOTES
    //     Argument <T> must be a class object. A builtin type or pointer
    //     cannot be used for <T>.
{
  private:

    T*			item_() const
				{
				  if (object_ == 0)
				  {
				    ((OTC_Ptr<T>*)this)->object_ = new T;
				    OTCLIB_ASSERT_M(object_ != 0);
				  }
				  return object_;
				}
				// Returns the object being pointed at. If
				// the object had not previously been
				// created, it will be created.
  public:

    // = CONSTRUCTION

			OTC_Ptr()
			  : object_(0) {}
				// Initialises the class to an undefined
				// state. The object is only created when it
				// is first required.

    // = DESTRUCTION

			~OTC_Ptr()
				{ if (object_ != 0) delete object_; }
				// Destroys the object if it has been
				// created.

    // = ACCESS

    T*			item()
				{ return item_(); }
				// Returns the object being pointed at. If
				// the object had not previously been
				// created, it will be created.

    T const*			item() const
				{ return item_(); }
				// Returns the object being pointed at. If
				// the object had not previously been
				// created, it will be created.

    T*			operator->()
				{ return item_(); }
				// Returns the object being pointed at.

    T const*		operator->() const
				{ return item_(); }
				// Returns the object being pointed at.

			operator T*()
				{ return item_(); }
				// Returns the object being pointed at.

			operator T const*() const
				{ return item_(); }
				// Returns the object being pointed at.

    T&			operator*()
				{ return *item_(); }
				// Returns a reference to the item being
				// pointed at.

    T const&		operator*() const
				{ return *item_(); }
				// Returns a reference to the item being
				// pointed at.

    // = QUERY

    bool		isUndefined() const
				{ return object_ == 0; }
				// Returns <true> if the object has not yet
				// been created.

  private:

			OTC_Ptr(OTC_Ptr<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_Ptr<T>&		operator=(OTC_Ptr<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T*			object_;
				// The item being held.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_PTR_HH */
