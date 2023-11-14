#ifndef OTC_THREAD_CVMUTEX_HH
#define OTC_THREAD_CVMUTEX_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/cvmutex.hh
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

#include <OTC/misc/types.h>

#if defined(OSE_WITH_THREADS)
#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/thread/cvmutex.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_CVMutex
    // = TITLE
    //	   A condition variable for mutexes.
    //
    // = DESCRIPTION
    //	   The <OTC_CVMutex> class is used in conjunction with
    //     <OTC_NRMutex> in order to allow waiting on the condition of
    //     some variable. An instance of this class should not outlive
    //     the instance of <OTC_NRMutex> being used.
    //
    // = SEE ALSO
    //	   <OTC_NRMutex>
{
  public:

    // = INITIALISATION

			OTC_CVMutex(OTC_NRMutex const& theMutex);
				// Initialises the condition variable.
				// <theMutex> is the mutex the thread has
				// locked.

    // = DESTRUCTION

			~OTC_CVMutex();
				// Destroys the condition variable.

    // = WAITING

    void		wait() const;
				// Blocks thread until some thing gives
				// notification that variable of interest has
				// changed.

    // = NOTIFICATION

    void		signal() const;
				// Notifies one of the waiting threads that
				// variable of interest has changed.

    void		broadcast() const;
				// Notifies all of the waiting threads that
				// variable of interest has changed.

  private:

			OTC_CVMutex(OTC_CVMutex const&);
				// Do not define an implementation for this.

    OTC_CVMutex const&		operator=(OTC_CVMutex const&);
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

    OTC_NRMutex const&	mutex_;
				// Mutex by which access to variable is
				// controlled.

#if defined(OSE_WITH_THREADS)

    pthread_cond_t	variable_;
				// Condition variable.

#endif
};

#if !defined(OSE_WITH_THREADS)
inline OTC_CVMutex::OTC_CVMutex(OTC_NRMutex const& theMutex)
 : mutex_(theMutex) {}
inline OTC_CVMutex::~OTC_CVMutex() {}
inline void OTC_CVMutex::wait() const {}
inline void OTC_CVMutex::signal() const {}
inline void OTC_CVMutex::broadcast() const {}
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_CVMUTEX_HH */
