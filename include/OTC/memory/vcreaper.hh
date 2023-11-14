#ifndef OTC_MEMORY_VCREAPER_HH
#define OTC_MEMORY_VCREAPER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/vcreaper.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/memory/vcreaper.hh"
#pragma interface "OTC/memory/vcreaper.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_VecReaper
    // = TITLE
    //     Destroys arrays of objects.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_VecReaper> assists in ensuring that arrays of
    //     objects allocated from the free store are deleted when the stack
    //     is unwound as a result of an exception. This is achieved by using
    //     an instance of this class as a handle to an array of objects
    //     allocated using new. Once an array of objects is grabbed using an
    //     instance of this class, the section of code in which an exception
    //     could be raised is executed. If an exception does occur then the
    //     instance of this class will be destroyed and the array of objects
    //     deleted. If no exception occurs a call can be made to release
    //     the array of objects. When the destructor is finally called the
    //     array of objects would not be deleted.
    //     
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void function()
    //     {
    //       OTC_VecReaper<Object> xxxArray;
    //       Object* theArray = new Object[2];
    //       OTCLIB_ASSERT_M(theArray != 0);
    //       xxxArray.grab(theArray);
    //
    //       ... code which could throw an exception
    //
    //       xxxArray.release();
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Reaper>, <OTC_MallocReaper>
{
  public:

    // = INITIALISATION

			OTC_VecReaper()
			 : data_(0) {}
				// Initialises the handle to <0>.

    // = DESTRUCTION

			~OTC_VecReaper()
				{ if (data_) delete [] data_; }
				// If the handle references an array, ie.,
				// the handle is not <0>, the array is
				// deleted.

    // = GRAB/RELEASE

    void		grab(T* theData)
				{ data_ = theData; }
				// Sets the handle to <theData>.
                                // If the handle was set to point at another
                                // array, it will now point to the new
                                // array and the first array will not be
                                // changed.

    void		release()
				{ data_ = 0; }
                                // Sets the handle to <0>. This is equivalent
                                // to calling <grab(0)>.

  private:

			OTC_VecReaper(OTC_VecReaper<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_VecReaper<T>&	operator=(OTC_VecReaper<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T*			data_;
				// Pointer to the array.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_VCREAPER_HH */
