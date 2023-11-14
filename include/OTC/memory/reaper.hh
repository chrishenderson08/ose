#ifndef OTC_MEMORY_REAPER_HH
#define OTC_MEMORY_REAPER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/reaper.hh
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
#pragma implementation "OTC/memory/reaper.hh"
#pragma interface "OTC/memory/reaper.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Reaper
    // = TITLE
    //     Destroys single objects.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_Reaper> assists in ensuring that single objects
    //     allocated from the free store are deleted when the stack is
    //     unwound as a result of an exception. This is achieved by using
    //     an instance of this class as a handle to an object allocated using
    //     new. Once an object is grabbed using an instance of this class,
    //     the section of code in which an exception could be raised is
    //     executed. If an exception does occur then the instance of this
    //     class will be destroyed and the object deleted. If no exception
    //     occurs a call can be made to release the object, when the destructor
    //     is finally called the object would not be deleted.
    //     
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void function()
    //     {
    //       OTC_Reaper<Object> xxxObject;
    //       Object* theObject = new Object;
    //       OTCLIB_ASSERT_M(theObject != 0);
    //       xxxObject.grab(theObject);
    //
    //       ... code which could throw an exception
    //
    //       xxxObject.release();
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_VecReaper>, <OTC_MallocReaper>
{
  public:

    // = INITIALISATION

			OTC_Reaper()
			 : data_(0) {}
				// Initialises the handle to <0>.

    // = DESTRUCTION

			~OTC_Reaper()
				{ if (data_) delete data_; }
				// If the handle references an object, ie.,
				// it is not <0>, the object is deleted.

    // = GRAB/RELEASE

    void		grab(T* theData)
				{ data_ = theData; }
				// Sets the handle to <theData>.
				// If the handle was set to point at another
				// object, it will now point to the new
				// object and the first object will not be
				// changed.

    void		release()
				{ data_ = 0; }
				// Sets the handle to <0>. This is equivalent
				// to calling <grab(0)>.

  private:

			OTC_Reaper(OTC_Reaper<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_Reaper<T>&	operator=(OTC_Reaper<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T*			data_;
				// Pointer to the object.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_REAPER_HH */
