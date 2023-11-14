#ifndef OTC_THREAD_TASK_HH
#define OTC_THREAD_TASK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/task.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include <OTC/thread/thread.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/thread/task.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_TaskState
{
  OTCLIB_TASK_STATE_UNKNOWN,
  OTCLIB_TASK_STATE_STARTING,
  OTCLIB_TASK_STATE_RUNNING,
  OTCLIB_TASK_STATE_STOPPING,
  OTCLIB_TASK_STATE_STOPPED,
  OTCLIB_TASK_STATE_FAILED
};

class OSE_EXPORT OTC_TaskAborted {};

class OSE_EXPORT OTC_Task
    // = TITLE
    //     A base class for a threadable task.
    //
    // = DESCRIPTION
    //     The <OTC_Task> class is a base class for a threadable task. A
    //     derived class should override the <execute()> method to provide
    //     the code to be run in the context of the separate thread.
    //
    //     If a task is long running, it should periodically call the method
    //     <OTC_Task::isStopping()> to determine whether it has been
    //     requested that the task be stopped prematurely to its normal
    //     termination. The task should either by its own means complete in
    //     this situation, or otherwise call <OTC_Task::abort()>. Calling
    //     <OTC_Task::abort()> method will raise an exception of type
    //     <OTC_TaskAborted> in order to force a return back to the scope in
    //     which <execute()> was called. In the event that an exception
    //     besides that used to abort execution of the thread occurs, it
    //     will be caught and the <fail()> method called to indicate that an
    //     uncaught exception has occurred. Note that the <isRunning()>
    //     and <abort()> methods are static member functions and can be
    //     called from code which is not actually within the derived class.
    //     The <isStopping()> method will use knowledge of which task is
    //     actually running to determine if <stop()> has actually been
    //     called for it, irrespective of the fact that no handle to the
    //     actual task object itself was available to the caller.
{
    friend class	OTC_EVThreadPool;

  public:

    // = DESTRUCTION

    virtual		~OTC_Task();
				// If the thread associated with the task is
				// still running, will attempt to stop it and
				// then wait for its completion. Note that if
				// the thread is still running at this point,
				// there is the risk it is probably accessing
				// data which has already been deleted since
				// any derived class component of this object
				// will already have been destroyed.

    // = INITIALISATION

			OTC_Task();
				// The constructor does not actually create
				// the separate thread. It is necessary to
				// call the <start()> method to initiate the
				// creation of the thread.

    // = STATUS

    OTC_TaskState	state() const;
    				// Returns the execution state of the task.

    // = CONTROL

    void		start();
    				// If the task has not already been started,
				// it will be started. Once a task has been
				// stopped or it has failed, it cannot be
				// restarted. If threading is not compiled
				// into the library, the <execute()> method
				// is called within the context of the
				// calling thread and will need to run to
				// completion before this method returns.

    void		stop();
				// Requests that the current task be stopped.
				// The associated thread is not actually
				// cancelled or killed, instead an internal
				// flag is set which can be periodically
				// checked using the static member function
				// <OTC_Task::isStopping()>. If the task has
				// been marked for termination the code
				// should then call the static member function
				// <OTC_Task::abort()> or otherwise stop what
				// it is doing, allowing the member function
				// <execute()> to return. Note that calling
				// <stop()> will NOT cause a thread to break
				// out of any waits on mutexes or condition
				// variables. It would be necessary for a
				// derived class to implement <shutdown()>
				// in order to do such things. This method
				// does nothing when thread support is not
				// compiled into the library.

    void		wait();
				// Causes the calling thread to wait until
				// the thread associated with this task has
				// exited. If necessary to prematurely stop
				// the task, the <stop()> method must be
				// called first.

    // = SHUTDOWN
    //     The following are static member functions and can even be called
    //     in code which is not actually a part of the derived class. The
    //     methods can be used whenever a task is carrying out a long running
    //     activity but where there is a chance that that activity will
    //     need to be prematurely terminated.

    static bool		isStopping();
				// Returns <true> if the currently executing
				// task, ie., that in which the caller may be
				// running, has been marked for termination.
				// If the caller is not running as part of
				// any thread associated with a task, <false>
				// will always be returned.

    static void		abortTask();
    				// In order to force a return to the highest
				// calling scope where the member function
				// <execute()> was called, will raise an
				// exception of type <OTC_TaskAborted>. If
				// called outside of the context of a task,
				// this would cause the program to abort if
				// the exception is not caught. This method
				// should only be called after a call to the
				// static member function <isStopping() and
				// only it that method returns a true value.

  protected:

    // = EXECUTION

    virtual void	execute();
    				// Should be overriden in a derived class to
				// provide code to be executed within the
				// context of the separate thread.

    virtual void	shutdown();
    				// Can be overriden in a derived class to
				// provide code to be executed when the
				// task is to be stopped. This method will
				// be called by <stop()> after the task
				// running status has been updated to
				// "stopping". A derived class might provide
				// code which will signal a condition
				// variable or use some other mechanism
				// to otherwise wake up the task and thus
				// notice that it has been stopped.

    virtual void	fail();
    				// Will be called when an exception occurs
				// which is not caught. A derived class can
				// override this method to output logging
				// messages or take some other action. The
				// method will be called after the stack
				// has been unwound due to the exception.

  private:

			OTC_Task(OTC_Task const&);
				// Do not define an implementation for this.

    OTC_Task&		operator=(OTC_Task const&);
				// Do not define an implementation for this.

    static void*        threadFunc_(void* theArg);
                                // Thread function.

    void		run_();
    				// Runs the actual code associated with the
				// task.

    static OTC_NRMutex  mutex_;
                                // Lock for threads.

    static bool 	 initialised_;
                                // Thread specific context key flag.

    OTC_TaskState	state_;
    				// Current state of task.

#if defined(OSE_WITH_THREADS)

    static pthread_key_t        context_;
                                // Thread specific context key.

    pthread_t		thread_;
				// POSIX thread handle.

#endif
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_TASK_HH */
