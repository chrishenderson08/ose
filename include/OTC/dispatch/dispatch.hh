#ifndef OTC_DISPATCH_DISPATCH_HH
#define OTC_DISPATCH_DISPATCH_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/dispatch.hh
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

#include <OTC/dispatch/jobqueue.hh>
#include <OTC/thread/task.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/dispatch.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Dispatcher
    // = TITLE
    //     Dispatcher of jobs.
    //
    // = CLASS TYPE
    //     Static
    //
    // = DESCRIPTION
    //     <OTC_Dispatcher> is the collection point for jobs to be executed.
    //     To schedule jobs, you should use the <schedule()> function. When
    //     you run the dispatcher by calling <run()>, it will continually
    //     poll the job queue for the next available job, execute the job
    //     and then destroy it. If there is not pending job to be run, the
    //     jobqueue will block, waiting for some event to occur. The
    //     dispatcher can be stopped, ie. made to from the <run()> routine by
    //     calling <stop()>.
    //     
    // = SEE ALSO
    //     <OTC_JobQueue>, <OTC_Job>
{
  public:

    friend class	OTCEV_Signal;

    // = INITIALISATION
    //     One of the following functions must be called before any jobs are
    //     scheduled to be run.

    static void		initialise(
			 void (*theFunc)()=0
			);
				// Initialises the dispatcher to use a
				// standard job queue, namely an instance
				// of <OTC_JobQueue>. If <theFunc> is
				// supplied, that function will be called
				// prior to each job being executed.

    static void		initialise(OTC_JobQueue* theJobQueue);
				// Initialise the dispatcher but supply your
				// own job queue by supplying <theJobQueue>.

    // = EXECUTION

    static int		dispatch(int theActions=0, int theOptions=0);
				// Provided that the dispatcher hasn't been
				// stopped, a single job is executed. If the
				// dispatcher had already been stopped, a
				// value of <-1> is returned. If there was no
				// job to execute a value of <0> is returned.
				// If there was no job to execute, and
				// <theActions> has a value of <0>, the
				// dispatcher will also be placed into the
				// stopped state. If a job was executed, a
				// value of <1> is returned. Except for the
				// value <0>, the meaning of the value of
				// <theActions> is dependent on the type of
				// job queue. A value of <0> will always
				// indicate that all sources should be
				// checked for jobs. The meaning of the
				// second argument, <theOptions> is also
				// dependent on the type of the job queue. It
				// is provided as a means of passing in any
				// auxiliary information needed for it to
				// work out what to run.

    static int		run();
				// Executes any jobs which have been
				// scheduled, until the dispatcher is stopped
				// through a call to <stop()>. If the
				// dispatcher runs out of jobs, then <0> is
				// returned and the dispatcher stopped. If
				// the dispatcher is stopped by user code
				// then <-1> is returned.

    static int		run(OTC_Condition* theCondition);
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

    static void		stop();
				// Will cause <run()> to return to the
				// caller, even if there are still jobs
				// to be executed.

    static void		reset();
				// If the dispatcher is in the stopped
				// state, resets it to the waiting state.
				// Raises an exception if called when
				// the dispatcher is not in the stopped
				// state.

    static bool		isWaiting();
				// Returns <true> if the dispatcher is
				// currently waiting to be run, otherwise
				// returns <false>.

    static bool		isRunning();
				// Returns <true> if the dispatcher is
				// currently running, otherwise returns
				// <false>. A value of <false> is also
				// returned if the dispatcher has been
				// stopped, but the <run()> routine has not
				// yet returned.

    static bool		isStopped();
				// Returns <true> if the dispatcher had been
				// running, but had been subsequently
				// stopped, otherwise returns <false>. It is
				// possible that this will return <true>
				// while <run()> is still executing as
				// <stop()> may have only been called by the
				// current job and thus <run()> hasn't had an
				// opportunity to return.

    static void         wakeup(int theType=0);
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
				// is called from a signal handler, and
				// the default value of <0> if called from
				// anything but a signal handler. The
				// function does not obtain the pointer to
				// the job queue inside a mutex lock. Thus in
				// a multithreaded environment if this
				// function is used before the dispatcher has
				// been initialised the results are
				// undefined. There is no need to call this
				// function if all you have done is scheduled
				// a new job as the job queue will wake
				// itself up.

    // = THREADING

    static OTC_Task*	task();
    				// Returns a task object which can be used
				// to run the dispatcher within the context
				// of a separate thread.

    // = SCHEDULING

    static void		schedule(
			 OTC_Job* theJob,
			 int theType=OTCLIB_STANDARD_JOB
			);
				// Add <theJob> to the queue of jobs to be
				// executed. <theType> defines whether
				// <theJob> is an idle job, standard job, or
				// priority job. The default value for
				// <theType> is <OTCLIB_STANDARD_JOB>. To
				// define <theJob> as being an idle job
				// use <OTCLIB_IDLE_JOB> and for a priority
				// job, use <OTCLIB_PRIORITY_JOB>.

    // = WARNINGS
    //     If the library is compiled with <NDEBUG> symbol defined, warnings
    //     will be disabled by default, else they will be logged.

    static bool		warningsEnabled();
				// Returns <true> if logging of warnings
				// about undeliverable events is enabled.

    static void		enableWarnings();
    				// Enable logging of warnings about any
				// undeliverable events.

    static void		disableWarnings();
    				// Disable logging of warnings about any
				// undeliverable events.

  public:

    // Following is kept public for backwards compatability only.

    static OTC_JobQueue*	queue();
				// Returns a pointer to the job queue.
				// If the dispatcher has not yet been
				// initialised, then <0> is returned.

  private:

			OTC_Dispatcher();
				// Do not define an implementation for this.

			OTC_Dispatcher(OTC_Dispatcher const&);
				// Do not define an implementation for this.

    OTC_Dispatcher&	operator=(OTC_Dispatcher const&);
				// Do not define an implementation for this.

    static long		processId()
      				{ return processId_; }
    				// Returns the process ID.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static bool		initialised_;
    				// Initialisation flag.

    static bool		warnings_;
    				// Flag indicating if event warning messages
				// are enabled.

    static OTC_JobQueue*	globJobQueue_;
				// Queue to which jobs are added and from
				// which they are later obtained for
				// execution.

    static long		processId_;
    				// Process ID. This is cached so that under
				// linux can know which of multiple signals
				// should be ignored.

    static OTC_Task*	task_;
    				// Separate task object for running dispatcher
				// in a separate thread.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_DISPATCH_HH */
