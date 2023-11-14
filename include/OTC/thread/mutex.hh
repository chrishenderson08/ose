#ifndef OTC_THREAD_MUTEX_HH
#define OTC_THREAD_MUTEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/mutex.hh
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
#pragma interface "OTC/thread/mutex.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Mutex
    // = TITLE
    //     A mutex class for when threads are being used.
    //
    // = DESCRIPTION
    //     A mutex class for blocking execution of code by multiple
    //     threads. The class does not prevent a thread from locking
    //     the same mutex a second time. Another thread would be locked
    //     out though. If threads are not available this class doesn't
    //     do anything.
{
  public:

    // = INITIALISATION

			OTC_Mutex();
				// Initialises the mutex.

    // = DESTRUCTION

			~OTC_Mutex();
				// Destroys the mutex.

    // = LOCKING

    void		lock() const;
				// Attempts to acquire the mutex. Will block
				// this thread if another thread has already
				// acquired the lock. A thread may call this
				// function multiple times. When this occurs,
				// the number of calls to the function must
				// be matched with a corresponding number of
				// calls to the <unlock()> function before the
				// lock is released.

    void		unlock() const;
				// Releases the lock on this mutex. Will only
				// do this though when the number of calls to
				// this function matches the number of
				// successful calls to <lock()>. It is assumed
				// that the thread from which this function
				// is called is the thread which called
				// <lock()> in the first place.

  private:

			OTC_Mutex(OTC_Mutex const&);
				// Do not define an implementation for this.

    OTC_Mutex&		operator=(OTC_Mutex const&);
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

#if defined(PTHREAD_MUTEX_RECURSIVE)

    pthread_mutexattr_t	attr_;
				// Mutex attributes.

#else

    int			nest_;
				// Nesting level.

    pthread_t		thread_;
				// Thread locking mutex.

#endif
#endif
};

#if !defined(OSE_WITH_THREADS)
inline OTC_Mutex::OTC_Mutex() {}
inline OTC_Mutex::~OTC_Mutex() {}
inline void OTC_Mutex::lock() const {}
inline void OTC_Mutex::unlock() const {}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_MUTEX_HH */
