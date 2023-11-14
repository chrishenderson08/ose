#ifndef OTC_THREAD_NRMUTEX_HH
#define OTC_THREAD_NRMUTEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/nrmutex.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/thread.hh>

#if defined(OSE_WITH_THREADS)
#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/thread/nrmutex.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_NRMutex
    // = TITLE
    //	   A mutex class for when threads are being used.
    //
    // = DESCRIPTION
    //	   A mutex class for blocking execution of code by multiple threads.
    //	   The class implements a non recursice mutex. That is, an attempt to
    //	   acquire the mutex by any thread, even the same thread which may
    //	   already have acquired it, will result in the execution of the
    //	   thread being blocked. In other words, do not use this is
    //	   situations where a mutex could be recusively used. In this latter
    //	   case use <OTC_Mutex>.
    //
    // = SEE ALSO
    //	   <OTC_Mutex>
{
    friend class	OTC_CVMutex;

  public:

    // = INITIALISATION

			OTC_NRMutex();
				// Initialises the mutex.

    // = DESTRUCTION

			~OTC_NRMutex();
				// Destroys the mutex.

    // = LOCKING

    void		lock() const;
				// Attempts to acquire the mutex. Will block
				// this thread if the mutex is already locked.

    void		unlock() const;
				// Releases the lock on this mutex.

  private:

			OTC_NRMutex(OTC_NRMutex const&);
				// Do not define an implementation for this.

    OTC_NRMutex&	operator=(OTC_NRMutex const&);
				// Do not define an implementation for this.

    int			valid_;
				// Flag indicating if mutex has been
				// initialised. This comes into play where
				// static mutex's are created. Being in
				// static memory this will by default
				// be initialised to zero. The constructor
				// of this class will set this flag to
				// a non zero value. The destructor will
				// set this flag back to zero. If an attempt
				// is made to lock a static mutex before
				// it is initialised, the flag will be
				// zero and the request ignored. Note
				// that it is presumed here that all
				// static initialisation will occur before
				// any threads are created. Since all
				// known compilers so far ensure that
				// all statics are initialised before
				// main this is a reasonable assumption.

#if defined(OSE_WITH_THREADS)

    pthread_mutex_t	mutex_;
				// Mutex.

#endif
};

#if !defined(OSE_WITH_THREADS)
inline OTC_NRMutex::OTC_NRMutex() {}
inline OTC_NRMutex::~OTC_NRMutex() {}
inline void OTC_NRMutex::lock() const {}
inline void OTC_NRMutex::unlock() const {}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_NRMUTEX_HH */
