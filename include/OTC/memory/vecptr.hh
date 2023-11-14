#ifndef OTC_MEMORY_VECPTR_HH
#define OTC_MEMORY_VECPTR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/vecptr.hh
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
#pragma implementation "OTC/memory/vecptr.hh"
#pragma interface "OTC/memory/vecptr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_VecPtr
    // = TITLE
    //	   Smart pointer to help manage resources.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Designed such that the array of objects being managed is only
    //	   created when required. Also deletes the array of objects when the
    //	   pointer class is destroyed.
{
  private:

    T*			item_() const
				{
				  if (array_ == 0)
				  {
				    ((OTC_VecPtr<T>*)this)->array_ =
				     new T[size_];
				    OTCLIB_ASSERT_M(array_ != 0);
				  }
				  return array_;
				}
				// Returns the array of objects being pointed
				// at. If the array of objects had not already
				// been created, it will be created.
  public:

    // = CONSTRUCTION

			OTC_VecPtr(size_t theSize)
			  : size_(theSize), array_(0) {}
				// Initialises the class to an undefined
				// state. The array of objects is only
				// created when it is required. <theSize> is
				// the desired length of the array.

    // = DESTRUCTION

			~OTC_VecPtr()
				{ if (array_ != 0) delete [] array_; }
				// Deletes the array of objects if it had
				// be created.

    // = ACCESS

    T*			item()
				{ return item_(); }
				// Returns the array of objects being pointed
				// at. If the array of objects had not already
				// been created, it will be created.

    T const*		item() const
				{ return item_(); }
				// Returns the array of objects being pointed
				// at. If the array of objects had not already
				// been created, it will be created.

    T&			operator[](size_t theIndex)
				{ return item()[theIndex]; }
				// Returns a reference to the object in
				// the array at position <theIndex>.
				// No bounds checking is performed.

    T const&		operator[](size_t theIndex) const
				{ return item()[theIndex]; }
				// Returns a reference to the object in
				// the array at position <theIndex>.
				// No bounds checking is performed.

			operator T*()
				{ return item(); }
				// Returns the array of objects being pointed
				// at.

			operator T const*() const
				{ return item(); }
				// Returns the array of objects being pointed
				// at.

    T&			operator*()
				{ return *item(); }
				// Returns a reference to the first object in
				// the vector being held. If no item is held,
				// an exception is generated.

    T const&		operator*() const
				{ return *item(); }
				// Returns a reference to the first object in
				// the vector being held. If no item is held,
				// an exception is generated.

    // = QUERY

    size_t		size() const
				{ return size_; }
				// Returns the size of the array.

    bool		isUndefined() const
				{ return array_ == 0; }
				// Returns <true> if the array of items has
				// not yet been created.

  private:

			OTC_VecPtr(OTC_VecPtr<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_VecPtr<T>&	operator=(OTC_VecPtr<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    size_t		size_;
				// The size of the array.

    T*			array_;
				// The array of items being held.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_VECPTR_HH */
