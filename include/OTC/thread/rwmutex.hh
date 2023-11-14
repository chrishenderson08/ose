#ifndef OTC_THREAD_RWMUTEX_HH
#define OTC_THREAD_RWMUTEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/rwmutex.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/thread.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/cvmutex.hh>

#if defined(OSE_WITH_THREADS)
#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/thread/rwmutex.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_RWMutex
    // = TITLE
    //     A reader/write mutex.
    //
    // = DESCRIPTION
    //     The <OTC_RWMutex> class is a mutex which will allow multiple
    //     readers to access the locked region, but will only ever let
    //     writers in by themselves and only one at a time.
{
  public:

    // = INITIALISATION

			OTC_RWMutex();
				// Initialises the mutex.

    // = DESTRUCTION

			~OTC_RWMutex();
				// Destroys the mutex.

    // = LOCKING

    void		readLock() const;
				// Attempts to acquire the mutex for a
				// reader. Will block this thread if the
				// lock is already held by a writer. Will
				// always succeed if not held or held by
				// any number of readers.

    void		writeLock() const;
				// Attempts to acquire the mutex for a
				// writer. Will block this thread if the
				// lock is already held by either a writer
				// or any number of readers.

    void		unlock() const;
				// Releases the lock on this mutex. For
				// a writer, the lock will be given up.
				// For a reader, the reader count will
				// be decremented and if it reaches zero
				// the lock will be given up. Preference
				// is given to waiting writers when lock
				// is released.

  private:

			OTC_RWMutex(OTC_RWMutex const&);
				// Do not define an implementation for this.

    OTC_RWMutex&	operator=(OTC_RWMutex const&);
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
#if !defined(NEED_PTHREAD_RWLOCK_INIT)

    pthread_rwlock_t	rwlock_;
				// Lock. Newer POSIX thread libraries
				// have support for this inbuilt.

#else

    OTC_NRMutex		mutex_;
				// Mutex for state variables.

    u_int		numReaders_;
				// Number of waiting readers.

    OTC_CVMutex		readers_;
				// Waiting readers wait on this.

    u_int		numWriters_;
				// Number of waiting writers.

    OTC_CVMutex		writers_;
				// Waiting writers wait on this.

    int			held_;
				// If <0>, lock not currently held. If <-1>,
				// held by a writer. If greater than <0>,
				// indicates the number of readers holding
				// the lock.

#endif
#endif
};

#if !defined(OSE_WITH_THREADS)
inline OTC_RWMutex::OTC_RWMutex() {}
inline OTC_RWMutex::~OTC_RWMutex() {}
inline void OTC_RWMutex::readLock() const {}
inline void OTC_RWMutex::writeLock() const {}
inline void OTC_RWMutex::unlock() const {}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_RWMUTEX_HH */
