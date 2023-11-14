#ifndef OTC_DISPATCH_EVTHREAD_HH
#define OTC_DISPATCH_EVTHREAD_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/evthread.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/linklist.hh>
#include <OTC/thread/task.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/cvmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/evthread.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_Event;
class OTC_EVCallback;
class OTC_EVThreadPool;

#if defined(OSE_WITH_THREADS)
class OSE_EXPORT OTC_EVThreadHandle
    // = TITLE
    //     Handle holding details of a specific thread in a thread pool.
    //
    // = DESCRIPTION
    //     The <OTC_EVThreadHandle> class holds details of the POSIX thread
    //     handle for the thread created with a thread pool. A pointer back
    //     to the thread pool and the ID of the thread within the context of
    //     the thread pool is also stored. A pointer to an instance of this
    //     class is stored with a thread specific data area, to allow more
    //     easy manipulation of thread specific data within the context of
    //     the user of a thread pool.
{
  public:

    pthread_t		thread;
    				// POSIX thread handle.

    u_int		threadId;
    				// ID of thread within context of the
				// associated thread pool.

    OTC_TaskState	threadState;
    				// Current state of the thread.

    OTC_EVThreadPool*	threadPool;
    				// Associated thread pool.
};
#endif

class OSE_EXPORT OTC_EVThreadPool
    // = TITLE
    //	   Pool of threads for handling of events.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_EVThreadPool> class implements a pool of threads along
    //     with a queueing mechanism for events. When a thread becomes
    //     available to handle a new event, it will grab the first event on
    //     the queue and execute the callback function provided, passing the
    //     event as argument to the callback function. Callback functions
    //     are provided as an object which is a derived implementation of
    //     <OTC_EVCallback>.
    //
    //     When threads aren't supported, an attempt to queue an event, will
    //     result in that event being handled immediately. This means that as
    //     long as an event doesn't result in an action which never stops or
    //     which only stops when some other event sets some state, the code
    //     will work in either threaded or non threaded environments.
    //
    // = SEE ALSO
    //	   <OTC_Event>, <OTC_EVAgent>, <OTC_EVHandler>, <OTC_EVCallback>
{
    friend		class OTC_EVAgent;

  public:

    // = DESTRUCTION

    virtual		~OTC_EVThreadPool();
				// Cleans up any threads. Note that it
				// is assumed that <shutdown()> has already
				// been called to halt the processing of
				// events. If <shutdown()> hasn't been
				// called when the destructor had been
				// called, threads might suddenly find that
				// the data they are trying to use doesn't
				// exist or is in an invalid state.

    // = INITIALISATION

			OTC_EVThreadPool(
			 u_int theNumThreads=0,
			 u_int theQueueSize=0,
			 u_int theThreshold=0
			);
				// In a threaded environment <theNumThreads>
				// is the number of threads to create to
				// handle incoming events. Where threads
				// aren't supported, <theNumThreads> is
				// ignored. Where threads are created, if
				// <theQueueSize> is <0>, all events will be
				// queued and the event queue will be allowed
				// to grow to any size. If <theQueueSize> is
				// non zero and <theThreshold> is also non
				// zero, when the number of events in the
				// queue exceeds the value in <theThreshold>,
				// the <throttle()> function is called. Note
				// that the constructor doesn't create the
				// threads. This is done by <startup()>.

    // = STARTUP/SHUTDOWN

    virtual void	startup();
				// When use of threads has been requested,
				// causes the threads to be started. You can
				// override this method in a derived class to
				// do your own initialisation, however, the
				// first thing a derived version should do is
				// to call this base class version of the
				// function.

    virtual void	shutdown();
				// When threads are active, it will suspend
				// further processing of events and wait for
				// currently active threads to finish
				// handling the events they responded to.
				// When the threads are no longer doing
				// anything they are made to kill themselves.
				// If threads are being used, must be called
				// before the class is deleted to ensure that
				// threads are not still trying to access any
				// data when it is being deleted. If desired,
				// a derived class can override this member
				// function to do its own shutdown
				// operations. The derived class member
				// function would normally call this function
				// as the first thing it does to ensure that
				// any threads have been stopped. The only
				// exception to this might be to first set
				// some flag which is checked by long running
				// operations to see if they should abort
				// before finishing. This function should not
				// be called by a thread bound to this
				// thread pool as it will hang waiting for
				// itself to finish. Even if no threads are
				// being used, a derived class might override
				// this to do its own shutdown operations. If
				// this is done, it should assume that
				// threads are being used and still call this
				// base class version of the function. Once
				// threads have been shutdown, it isn't
				// possible to restart them.

    // = THREADS

    void		suspend();
				// Suspends processing of any available
				// events by threads. This doesn't stop
				// any threads which may already be
				// handling an event. This function has no
				// affect when there are no active threads,
				// that is, events will still be handled.

    void		resume();
				// Resumes the processing of events by
				// threads. This function has no affect when
				// there are no active threads. That is,
				// events will still be handled.

    void		limit(u_int theNumThreads);
				// Temporarily limits the number of threads
				// which are allowed to handle incoming
				// events to <theNumThreads>. This might be
				// used during an initialisation phase to set
				// the number of threads to <1> so that
				// events are handled in the order received
				// until a state is reached where having
				// multiple threads running and where the
				// events may appear to be processed out of
				// order is okay. To resume all available
				// threads, provide the argument of <0>. If
				// you want to stop all threads for a while,
				// use <suspend()> instead. Note that if
				// there are threads currently handling
				// events such that more threads are
				// active than <theNumThreads>, currently
				// running threads are not stopped. It just
				// means that the excess threads when they
				// finally finish with the event they are
				// handling will not necessarily grab the
				// next event if there are some available.
				// Instead, the excess threads will wait
				// until the number of active threads is
				// less than <theNumThreads>. This function
				// has no affect when there are no active
				// threads. That is, events will still be
				// handled.

    // = CONTEXT

    u_int		threadId() const;
				// Returns an ID identifying the calling
				// thread within the context of this thread
				// pool. If thread support is not compiled
				// in to the library, <0> will be returned.
				// If the calling thread is not from this
				// thread pool, <0> will be returned.
				// Otherwise, the thread IDs will range form
				// <1> up to the number of threads.

    // = EVENTS

    void		process(
			 OTC_Event* theEvent,
			 OTC_EVCallback* theCallback
			);
				// Called to have <theEvent> passed to
				// <theCallback> in the context of a separate
				// thread from the thread pool. When there
				// are no active threads, this is done
				// immediately in the context of the calling
				// thread. When there are active threads,
				// if there are threads waiting for events
				// to handle and there are no other pending
				// events, the callback will be carried out
				// by an available thread immediately. If
				// there are no available threads or there
				// are other pending events, the event will
				// be queued until a thread is available
				// to handle the event. If a queue size has
				// been defined and the queue is full,
				// the calling thread will block. If a queue
				// size has been defined and adding an
				// event to the queue would exceed the
				// threshold value if defined, <throttle()>
				// will be called. Note that once the
				// callback has been called, <theCallback>
				// will be destroyed by calling its
				// <destroy()> member function. By default
				// that function calls delete on itself.
				// Thus the callback object must have been
				// allocated using <operator new()> or
				// the <destroy()> method must have been
				// overloaded to take some other action.

  protected:

    // = OVERLOAD

    virtual void	throttle(
			 OTC_Event* theEvent,
			 OTC_EVCallback* theCallback
			);
				// Called when the number of events would
				// exceed the overload threshold value of the
				// event queue or the maximum size of the
				// event queue has been reached. The default
				// implementation will block the calling
				// thread until the number of events in the
				// queue drops below the maximum queue size
				// and will then add the event to the queue.
				// If the threshold value is less than the
				// maximum queue size and there is space
				// still in the queue, this function will not
				// actually block but will be able to add the
				// event straight away. A derived class might
				// override the function to interogate the
				// event and perhaps send back some response
				// to the sender indicating that the handler is
				// overloaded and then perhaps destroy the
				// event and the callback object.

  private:

			OTC_EVThreadPool(OTC_EVThreadPool const&);
				// Do not define an implementation for this.

    OTC_EVThreadPool&	operator=(OTC_EVThreadPool const&);
				// Do not define an implementation for this.

    void		queue_(
			 OTC_Event* theEvent,
			 OTC_EVCallback* theCallback
			);
				// If the agent is using a thread, adds the
				// event to the internal queue, otherwise
				// calls <execute_()>. Note that if a queue
				// threshold is set and the event would cause
				// that value to be exceeded or the maximum
				// size of the event queue is reached, the
				// <throttle()> function is called.

    void		execute_(
			 OTC_Event* theEvent,
			 OTC_EVCallback* theCallback
			);
				// Calls <theCallback> with <theEvent>
				// and then destroys the callback object
				// by calling its <destroy()> method.

#if defined(OSE_WITH_THREADS)

    static void*	threadFunc_(void* theArg);
				// Thread function.

    static OTC_NRMutex  mutex_;
    				// Lock for threads.

    static bool		initialised_;
    				// Thread specific context key flag.

    static pthread_key_t	context_;
    				// Thread specific context key.

    OTC_EVThreadHandle*	threads_;
    				// Array of thread handles.

    u_int		numThreads_;
				// Number of threads to create.

    u_int		numActive_;
				// Current number of threads allowed to
				// run.

    OTC_LinkList*	eventQueue_;
				// Queue of events to be dealt with.

    u_int		eventCount_;
				// Number of events in queue.

    u_int		queueSize_;
				// When number of events in queue exceeds
				// this value start blocking attempts to
				// add events to the queue.

    u_int		queueThreshold_;
				// When number of events in queue exceeds
				// this value, start calling <throttle()>.

    OTC_NRMutex		queueLock_;
				// Mutex lock for queue and state variables.

    OTC_CVMutex		eventReady_;
				// Condition variable for availability
				// of events.

    OTC_CVMutex		queueReady_;
				// Queue ready to except more events.
				// Used by <throttle()> when number of
				// events in queue exceeds threshold
				// value. This variable will be signalled
				// when the number of events drops below
				// threshold.

    OTC_CVMutex		shutdownReady_;
				// Used to notify of threads finishing
				// up with current event when shutdown
				// is in progress.

    u_int		queueWaitingId_;
				// The ID of the next blocked thread to
				// be allowed to put an event on the queue.
				// This is not the same as the thread ID.

    u_int		queueId_;
				// The next available ID for threads which
				// need to block waiting on the ability
				// to put an event on the queue.

    u_int		waiting_;
				// Number of threads waiting for events
				// to be placed in the queue.

    bool		suspended_;
				// Will be <true> if handling of new events
				// by threads is suspended.

    bool		shutdown_;
				// Will be <true> if handling of events has
				// been shutdown.

    u_int		exited_;
				// Number of threads which have exited
				// subsequent to a shutdown request.

#endif
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVTHREAD_HH */
