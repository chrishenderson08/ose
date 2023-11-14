#ifndef OTC_THREAD_MXREAPER_HH
#define OTC_THREAD_MXREAPER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/mxreaper.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/thread/reaper.hh"
#pragma interface "OTC/thread/reaper.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_MutexReaper
    // = TITLE
    //     Unlocks thread mutexes.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_MutexReaper> assists in ensuring that mutex objects
    //     are unlocked when an exception occurs within the bounds of a
    //     locked mutex. This is achieved by using an instance of this class
    //     as a handle to a mutex. Once a mutex is grabbed using an
    //     instance of this class, the section of code in which an exception
    //     could be raised is executed. If an exception does occur then the
    //     instance of this class will be destroyed and the mutex unlocked.
    //     If no exception occurs a call can be made to release the mutex,
    //     when the destructor is finally called the mutex would not be
    //     unlocked.
    //     
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void function()
    //     {
    //       globMutex_.lock();
    //       OTC_MutexReaper<OTC_Mutex> xxxMutex;
    //       xxxMutex.grab(globMutex_);
    //
    //       ... code which could throw an exception
    //
    //       xxxMutex.release();
    //       globMutex_.unlock();
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Mutex>, <OTC_NRMutex>
{
  public:

#if defined(OSE_WITH_THREADS)

    // = INITIALISATION

			OTC_MutexReaper()
			 : mutex_(0) {}
				// Initialises the handle to <0>.

    // = DESTRUCTION

			~OTC_MutexReaper()
				{ if (mutex_) mutex_->unlock(); }
				// If the handle references a mutex, ie.,
				// it is not <0>, the mutex is unlocked.

    // = GRAB/RELEASE

    void		grab(T const& theMutex)
				{ mutex_ = &theMutex; }
				// Sets the handle to <theMutex>.
				// If the handle was set to point at another
				// mutex, it will now point to the new
				// mutex and the first mutex will not be
				// changed.

    void		grab(T const* theMutex)
				{ mutex_ = theMutex; }
				// Sets the handle to <theMutex>.
				// If the handle was set to point at another
				// mutex, it will now point to the new
				// mutex and the first mutex will not be
				// changed.

    void		release()
				{ mutex_ = 0; }
				// Sets the handle to <0>. This is equivalent
				// to calling <grab(0)>.

#else

  public:

			OTC_MutexReaper() {}
			~OTC_MutexReaper() {}
    void		grab(T const&) {}
    void		grab(T const*) {}
    void		release() {}

#endif

  private:

			OTC_MutexReaper(OTC_MutexReaper<T> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_MutexReaper<T>&	operator=(OTC_MutexReaper<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    T const*		mutex_;
				// Pointer to the mutex.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_MXREAPER_HH */
