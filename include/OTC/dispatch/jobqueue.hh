#ifndef OTC_DISPATCH_JOBQUEUE_HH
#define OTC_DISPATCH_JOBQUEUE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/jobqueue.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/joblist.hh>
#include <OTC/dispatch/condtion.hh>
#include <OTC/thread/nrmutex.hh>

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/jobqueue.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_JobQueueState
{
  OTCLIB_WAITING=0,
  OTCLIB_RUNNING,
  OTCLIB_STOPPED
};

enum OTC_WakeupActions
{
  OTCLIB_WAKEUP_OTHER=0x0000,
  OTCLIB_WAKEUP_SIGNAL=0x0001,
  OTCLIB_WAKEUP_ALARM=0x0002,
  OTCLIB_WAKEUP_TIMEOUT=0x0004,
  OTCLIB_WAKEUP_IOEVENT=0x0008,
  OTCLIB_WAKEUP_IDLE_JOB=0x0010,
  OTCLIB_WAKEUP_STANDARD_JOB=0x0020,
  OTCLIB_WAKEUP_PRIORITY_JOB=0x0040
};

class OTC_IOSubscription;

class OSE_EXPORT OTC_JobQueue
    // = TITLE
    //	   Job queue which understands signals, time, I/O events and jobs.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     <OTC_JobQueue> is the standard job queue. It understands how to
    //     manage signals, alarms, timers, I/O events and jobs.
    //     
    //	   When the <next()> member function is invoked to return the
    //	   next job to execute, the class first checks for any signals
    //	   which may have occurred. If no signals have occurred, a check
    //	   is made for any alarms which may have expired. Checks are then
    //     made for priority jobs, standard jobs, timers, io events and
    //     finally idle jobs.
    //     
    //	   When a request is made for a specific type of job, the valid
    //	   flags which can OR'ed together are:
    //
    // = BEGIN<INDENT>
    // = BEGIN<NOFILL>
    //	   - <OTCLIB_SIGNAL_JOBS>
    //	   - <OTCLIB_ALARM_JOBS>
    //	   - <OTCLIB_TIMEOUT_JOBS>
    //	   - <OTCLIB_IO_JOBS>
    //	   - <OTCLIB_PRIORITY_JOBS>
    //	   - <OTCLIB_STANDARD_JOBS>
    //	   - <OTCLIB_IDLE_JOBS>
    // = END<NOFILL>
    // = END<INDENT>
    //
    //	   If <OTCLIB_DONT_WAIT> is included in the set of flags, the
    //	   function will return without executing a job if the function would
    //	   have needed to block in order to get a job. The value <0> can be
    //	   used to select all event sources, or <OTCLIB_DONT_WAIT> by itself
    //	   if all jobs are to be selected but blocking should not occur.
    //     If multithreading support is compiled in and the value
    //     <OTCLIB_THREADS_DONT_WAIT> is included in the set of flags,
    //     the function will not halt if the only possible cause of event
    //     stimulus is due to an alternate thread scheduling a job or
    //     subscribing to an event source.
    //
    //     These flags can be supplied to the <dispatch()> member function
    //     of this class or the <OTC_Dispatcher> class if dispatching single
    //     events at a time.
    //     
    // = NOTES
    //     If you derive from this class to try and produce your own job
    //     queue variant, it must be internally thread safe if you want it to
    //     work in a thread environment. The <OTC_Dispatcher> class does not
    //     protect use of the class from different threads.
    //     
    //     On a UNIX system, this class will use up two file descriptors
    //     as a way of monitoring when signals occur when the select/poll
    //     call is blocked. When multithreading is used, an additional
    //     two file descriptors are used for determining when events of
    //     interest are subscribed or scheduled when blocked in the
    //     select/poll call.
    //
    //     Only one instance of this class should ever be created. There
    //     are no checks on whether more than one is created. If more than
    //     one is created, the results are undefined.
    //
    // = SEE ALSO
    //	   <OTC_Job>, <OTCEV_Signal>, <OTCEV_Alarm>, <OTCEV_Timeout>,
    //     <OTCEV_IOEvent>, <OTC_Dispatcher>
{
  public:

    virtual		~OTC_JobQueue();

    // = CONSTRUCTION

			OTC_JobQueue(
			 void (*theSetupFunc)()=0
			);
				// Inititialises the job queue. The function
				// <theSetupFunc> can be supplied by the user
				// and will be called just prior to <next()>
				// being called each time an attempt is made
				// to find a new job to execute.

    // = QUEUEING

    virtual void	add(OTC_Job* theJob, int theType);
				// Adds <theJob> to the end of an appropriate
				// queue. Generates an exception if <theJob>
				// is <0>. <theType> is used to indicate
				// whether the job is an idle, standard or
				// priority job. Values for <theType> are
				// <OTCLIB_IDLE_JOB>, <OTCLIB_STANDARD_JOB>
				// and <OTCLIB_PRIORITY_JOB>.

    virtual OTC_Job*	next(int theActions, int theOptions);
				// Returns a job to be executed. The type of
				// job and whether the function blocks is
				// determined by <theActions>. If <theActions>
				// is <0>, all event sources will be checked
				// and the function will block if necessary.
				// If non zero, <theActions> should be the
				// OR'ed combination of the flags defined
				// by the enum <OTC_JobActions> as described
				// above. <theOptions> is not used by this
				// job queue and thus its value is ignored.

    // = EXECUTION

    virtual int		dispatch(int theActions=0, int theOptions=0);
				// Executes a single job. Note that this
				// function doesn't check to see if the
				// dispatcher is in a running state, it is
				// assumed that this type of check has
				// already been made. If there was no job to
				// execute and <theActions> is <0> a value of
				// <0> is returned and the dispatcher is
				// stopped. If <theActions> is non zero a
				// value of <0> will be returned and the
				// dispatcher wouldn't be stopped. If a job
				// was executed, a value of <1> is returned.
				// A value of <0> for <theActions> indicates
				// that all event sources should be checked.
				// If non zero, <theActions> should be the
				// OR'ed combination of the flags defined
				// by the enum <OTC_JobActions> as described
				// above. <theOptions> is not used by this
				// job queue and thus its value is ignored.

    virtual int		run();
				// Executes any jobs which have been
				// scheduled, until the dispatcher is stopped
				// through a call to <stop()>. If the
				// dispatcher runs out of jobs, then <0> is
				// returned and the dispatcher stopped. If
				// the dispatcher is stopped by user code
				// then <-1> is returned.

    virtual int		run(OTC_Condition* theCondition);
				// Executes any jobs which have been
				// scheduled, until the dispatcher is stopped
				// through a call to <stop()> or
				// <theCondition> is satisfied. If the
				// dispatcher runs out of jobs, then <0> is
				// returned and the dispatcher stopped. If
				// <theCondition> had been satisified, then
				// <1> will be returned. If the dispatcher is
				// stopped by user code then <-1> is
				// returned. Note that <theCondition> is
				// evaluated and tested after each job has
				// been executed, thus at least one job must
				// have run. That <theCondition> is evaluated
				// after each job means you can use the
				// number of calls to evaulate the condition
				// as indicating how many jobs had been
				// executed and use that as a means of
				// setting the condition.

    virtual void	stop();
				// Will cause <run()> to return to the
				// caller, even if there are still jobs to be
				// executed.

    virtual void	reset();
				// If the dispatcher is in the stopped state,
				// resets it to the waiting state. Raises an
				// exception if called when
                                // the dispatcher is not in the stopped
                                // state.

    virtual void	start();
				// Called when the dispatcher transitions
				// from being in a waiting state to a running
				// state. Could be redefined in a derived
				// class to perform special additional actions,
				// however, the last thing the derived class
				// function must do is to call the base class
				// version of the function.

    bool		isWaiting();
				// Returns <true> if the dispatcher is
				// currently waiting to be run, otherwise
				// returns <false>.

    bool		isRunning();
				// Returns <true> if the dispatcher is
				// currently running, otherwise returns
				// <false>. A value of <false> is also
				// returned if the dispatcher has been
				// stopped, but the <run()> routine has not
				// yet returned.

    bool		isStopped();
				// Returns <true> if the dispatcher had been
				// running, but had been subsequently
				// stopped, otherwise returns <false>. It is
				// possible that this will return <true>
				// while <run()> is still executing as
				// <stop()> may have only been called by the
				// current job and thus <run()> hasn't had an
				// opportunity to return.

    virtual void	wakeup(int theType=0);
                                // The purpose of this function is to wake
                                // up, from a secondary thread or from a
                                // signal handler, the main thread, where the
                                // main thread is blocked in a call to
                                // select/poll. The purpose is so that the
                                // main thread will recheck subscriptions
                                // as well as the availability of signal
                                // events straight away, otherwise it
                                // wouldn't know about changes until it
                                // returned from the select/poll call.
                                // <theType> should be <1> if this function
				// is called from a signal handler, and the
				// default value of <0> if called from
				// anything but a signal handler. The
				// function does not obtain the pointer to
				// the job queue inside a mutex lock. Thus in
				// a multithreaded environment if this
				// function is used before the dispatcher has
				// been initialised the results are
				// undefined. There is no need to call this
				// function if all you have done is scheduled
				// a new job as the job queue will wake
				// itself up. Note that on Win32 this
				// function can result in the function
				// <WSACancelBlockingCall()> being called.

  protected:

    // = USER SETUP FUNCTION

    void		runUserSetup()
				{ if (setup_ != 0) setup_(); }
				// Runs the user supplied setup function.
				// This is done prior to each attempt to
				// find a job.

    // = SIGNAL/WAKEUP PIPES

    void		clearPipes();
				// Clears the pipes, used to implement signal
				// and cross thread wakeup events, of any
				// character input we know to be pending.

    int			signalPipe() const
				{ return signalFds_[0]; }
				// Returns the descriptor of the input side
				// of the pipe used for signal notification.

    int			wakeupPipe() const
				{ return wakeupFds_[0]; }
				// Returns the descriptor of the input side
				// of the pipe used for cross thread wakeup
				// notification.

    // = JOB QUEUEING

    bool		isPriorityJobsEmpty();
				// Returns <true> if the priority job queue
				// is empty.

    OTC_Job*		nextPriorityJob();
				// Returns the next priority job or <0>
				// if there aren't any.

    bool		isStandardJobsEmpty();
				// Returns <true> if the executable standard
				// job queue is empty.

    OTC_Job*		nextStandardJob();
				// Returns the next standard job or <0>
				// if there aren't any.

    bool		isPendingStandardJobsEmpty();
				// Returns <true> if the pending standard job
				// queue is empty.

    void		updateStandardJobs();
				// Copies any standard jobs in the pending
				// standard jobs queue, to the executable
				// standard jobs queue.

    bool		isIdleJobsEmpty();
				// Returns <true> if the executable idle job
				// queue is empty.

    OTC_Job*		nextIdleJob();
				// Returns the next idle job or <0>
				// if there aren't any.

    bool		isPendingIdleJobsEmpty();
				// Returns <true> if the pending idle job
				// queue is empty.

    void		updateIdleJobs();
				// Copies any idle jobs in the pending idle
				// jobs queue, to the executable idle jobs
				// queue.

  private:

			OTC_JobQueue(OTC_JobQueue const&);
				// Do not define an implementation fo this.

    OTC_JobQueue&	operator=(OTC_JobQueue const&);
				// Do not define an implementation fo this.

    OTC_NRMutex		mutex_;
				// General mutex to prevent more than
				// one call to <next()> at a time.

    OTC_NRMutex		rmutex_;
				// Mutex for running state of job queue.

    OTC_NRMutex		qmutex_;
				// Mutex for job queues.

    OTC_JobQueueState	state_;
				// Initially set to <OTCLIB_WAITING>. The
				// first time that <dispatch()> is called
				// this is set to <OTCLIB_RUNNING>. If
				// <stop()> is called, this is set to
				// <OTCLIB_STOPPED>. When in the stopped
				// state, calling <reset()> sets this back to
				// <OTCLIB_WAITING>.

    void		(*setup_)();
				// Setup function to be called prior to
				// <next()> being called. Supplied by the
				// user to setup information for event
				// input or to perform other actions for
				// each job executed.

    int			signalFds_[2];
                                // Pipe for signal notification. Not used
				// on Win32, use wakeup pipe as signal
				// handler runs as a thread.

    int			signalCount_;
                                // Number of characters expected to be
                                // read from signal pipe.

    int			wakeupFds_[2];
                                // Pipe for subscription notifications.

    int			wakeupCount_;
                                // Number of characters expected to be
                                // read from wakeup pipe.

    int			wakeupFlag_;
				// When non zero, means that we at point
				// in dispatcher that blocking may occur
				// and thus for thread wakeup flag, need
				// to use socket to wake up main thread.

#if defined(OSE_SYS_WIN32)

    HANDLE		wmutex_;
				// Mutex for wakeup pipe. Always in use on
				// Win32. Not just when threads requested.

#else

    OTC_NRMutex		wmutex_;
				// Mutex for wakeup pipe.

#endif

    OTC_JobList*	pendingIdleJobs_;
    				// List of idle jobs not yet moved into
				// state to be processed.

    OTC_JobList*	pendingStandardJobs_;
				// List of standard jobs not yet moved into
				// state to be processed.

    OTC_JobList*	priorityJobs_;
				// List of priority jobs.

    OTC_JobList*	standardJobs_;
				// List of standard jobs.

    OTC_JobList*	idleJobs_;
				// List of idle jobs being processed.

    OTC_IOSubscription*	pendingFdEvents_;
				// Array of fd events still to be processed.

    int			nextFd_;
				// Next fd to check in set of pending fd
				// events.

    int			maxFd_;
				// Maximum numbered file descriptor which
				// may have events still to be processed.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_IOJQUEUE_HH */
