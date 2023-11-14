#ifndef OTC_SYSTEM_STOPWTCH_HH
#define OTC_SYSTEM_STOPWTCH_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/stopwtch.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/system/clckinfo.hh>

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/stopwtch.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

#if defined(OTCLIB_TIMING)
#define OTCLIB_STOPWATCH(name) OTC_StopWatch name(#name)
#define OTCLIB_STATIC_STOPWATCH(name) static OTC_StopWatch name(#name)
#define OTCLIB_STOPWATCH_REPORT(stream) OTC_StopWatch::report(stream)
#else
#define OTCLIB_STOPWATCH(name)
#define OTCLIB_STATIC_STOPWATCH(name)
#define OTCLIB_STOPWATCH_REPORT(stream)
#endif

class OSE_EXPORT OTC_StopWatch
    // = TITLE
    //     Used to record time taken for code to get something done.
    //
    // = SYNOPSIS
    //     #define OTCLIB_STOPWATCH ...
    //     #define OTCLIB_STATIC_STOPWATCH ...
    //     #define OTCLIB_STOPWATCH_REPORT ...
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_StopWatch> class is a wrapper around the system <clock()>
    //     function and provides a means of recording how much actual CPU
    //     time it takes for code to get something done. When doing
    //     performance measurements of code this is the prefered method as
    //     using wall clock time does not take into consideration that the
    //     program does not have exclusive access to the CPU but is time
    //     sharing with other processes.
    //     
    //     When you create a stopwatch it can be named. If this is done
    //     the information it records can be dumped out as part of a summary
    //     on all stopwatches to a stream. Note that if two instances of
    //     the <OTC_StopWatch> class use the same name, they will reference
    //     the same clock details. This will be the case if the two instances
    //     exist at the same time or distinct in time. That is, the clock
    //     details related to a named instance of <OTC_StopWatch> are kept
    //     around for the life of the program to allow the summary to be
    //     output at any time. This does however mean that you could start
    //     timing from one instance and stop it from a second due to
    //     referencing the same clock information.
    //
    //     Rather than create instances of this class specifically, you can
    //     use macros which will only conditionally compile the timing code
    //     into your program when <OTCLIB_TIMING> is defined. This
    //     allows you to leave code in your files and not remove it thus
    //     enabling it to be used later. To create a stopwatch use the macro
    //     <OTCLIB_STOPWATCH()> supply a single argument of the name of the
    //     stopwatch. The variable name for the instance will be that used as
    //     argument. If it is desired to make the instance have static scope
    //     use instead <OTCLIB_STATIC_STOPWATCH()>. To generate a report
    //     for all named timers, use <OTCLIB_STOPWATCH_REPORT()> with a
    //     single argument of an output stream. For starting and stoping
    //     of these stopwatches, use the <OTCLIB_TIMEBLOCK()> macro provided
    //     with the <OTC_TimeClock> class.
    //
    // = EXAMPLE
    //
    //     OTCLIB_STOPWATCH(TIMER1);
    //  
    //     for (int j=1; j<5; j++)
    //     {
    //       OTCLIB_TIMEBLOCK(TIMER1);
    //  
    //       for (int i=1; i != 10000000; i++) ;
    //     }
    //  
    //     OTCLIB_STOPWATCH_REPORT(cout);
    //  
    // = NOTES
    //     Note that if the clock resolution is high, the time may wrap
    //     around in a short period of time. For clocks with a resolution of
    //     about 1 million ticks per second, this will occur after about 36
    //     minutes. This class will not give correct results at the time
    //     that the value returned by <clock()> wraps around. Nor can the
    //     class record times greater than the maximum value which can be
    //     attained before a wrap around occurs. This means that if you
    //     are using this class, you would want to have done all your
    //     measurements before the process has accumulated a total of about
    //     36 minutes of CPU time. Remember that this is not the same as
    //     36 minutes of wall clock time.
    //
    //     This class is not protected against use from multiple threads
    //     in a multithreaded application. The class can still be used
    //     in a multithreaded application provided that instances of the
    //     <OTC_StopWatch> class are created as global scope objects such
    //     that creation of each is finished before any threads have
    //     been created. Be warned though that the class will not give
    //     very good results in a multithreaded application as other
    //     threads could be running and consuming time and thus giving an
    //     incorrect time value for the thread which is using the instance
    //     of the stopwatch class.
    //
    // = SEE ALSO
    //     <OTC_ClockInfo>, <OTC_TimeClock>
{
  public:

			~OTC_StopWatch();

    // = INITIALISATION

			OTC_StopWatch(char const* theName=0);
				// Creates a stopwatch with <theName>. The
				// stopwatch is not started. If <theName>
				// is a null pointer, any information will
				// not be printed out in a summary.

    // = TIMING

    void		reset()
    				{ clock_->reset(); }
    				// Resets all counters and timers back to
				// zero. Will also stop the stopwatch if
				// it is currently running.

    void		start()
    				{ clock_->start(); }
    				// Starts the stopwatch. Has no affect
				// if the stopwatch is already running.

    void		stop()
    				{ clock_->stop(); }
    				// Stops the stopwatch. Has not affect
				// if the stopwatch is not running.

    bool		isRunning() const
    				{ return clock_->isRunning(); }
				// Returns <true> if the stopwatch is
				// currently running.

    double		current() const
    				{ return clock_->current(); }
				// Returns the amount of time the stopwatch
				// has been running for any currently active
				// period. Returns <-1> if the stopwatch is
				// not currently active.

    // Following do not cover any period for which the stopwatch may be
    // currently running. Times are all returned as number of seconds
    // expressed as a floating point value.

    int			samples() const
    				{ return clock_->samples(); }
				// Returns the number of times that the
				// stopwatch has been started and then
				// stopped.

    double		total() const
    				{ return clock_->total(); }
    				// Returns the total amount of time that
				// the stopwatch has been running. This
				// covers multiple periods for which the
				// stopwatch has been active.

    double		average() const
    				{ return clock_->average(); }
    				// Returns the average time the stopwatch
				// was running across each period.

    double		last() const
    				{ return clock_->last(); }
    				// Returns the amount of time the stopwatch
				// was active for the last period. Returns
				// <-1> if there was no previous period.

    // = SUMMARY

    static void		report(ostream& theStream);
    				// Dumps a report about the accumulated
				// statistics of all named stopwatches.

    // = CONTROL

    static void		resetAll();
    				// Resets all named stopwatches.

    // Following allow stopwatches to be created and used without having to
    // created instances of <OTC_StopWatch>. This will not be as effecient
    // as it will be necessary to always be traversing the list of clock
    // details to search for the stopwatch with the defined name.

    static void		create(char const* theName);
    				// Creates a stopwatch with <theName>.
				// Doesn't do anything if <theName> is
				// a null pointer or if <theName> already
				// exists.

    static void		reset(char const* theName);
    				// Resets the stop watch with <theName>.

    static void		start(char const* theName);
    				// Starts the stopwatch with <theName>.

    static void		stop(char const* theName);
    				// Stops the stopwatch with <theName>.

  private:

			OTC_StopWatch(OTC_StopWatch const&);
				// Do not define an implementation for this.

    OTC_StopWatch&	operator=(OTC_StopWatch const&);
				// Do not define an implementation for this.

    OTC_ClockInfo*	clock_;
    				// Pointer to clock information for this
				// stopwatch.

    static OTC_ClockInfo*	head_;
    				// Head of list of named stopwatches.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_STOPWTCH_HH */
