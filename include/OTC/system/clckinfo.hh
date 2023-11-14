#ifndef OTC_SYSTEM_CLCKINFO_HH
#define OTC_SYSTEM_CLCKINFO_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/clckinfo.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include <time.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/clckinfo.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_ClockInfo
    // = TITLE
    //     Holds time recorded by a stopwatch for program timing.
    //
    // = DESCRIPTION
    //     The <OTC_ClockInfo> class keeps track of time accumulated against
    //     a particular stopwatch.
    //     
    // = SEE ALSO
    //     <OTC_StopWatch>
{
    friend		class OTC_StopWatch;

  public:

			~OTC_ClockInfo();
				// Unlinks this instance from chain of
				// instances it is in. Since named instances
				// are kept forever and unnamed instances
				// aren't linked together, this should never
				// ever really get called.

    // = INITIALISATION

			OTC_ClockInfo(
			 char const* theName=0,
			 OTC_ClockInfo* thePrev=0,
			 OTC_ClockInfo* theNext=0
			);
				// Makes a copy of <theName> if not a
				// null pointer. Links this instance
				// into the list defined by <thePrev>
				// and <theNext>.

    // = NAMING

    char const*		name() const
    				{ return name_; }
    				// Returns the name of the stopwatch.
				// Will return <0> if the stopwatch
				// was unnamed.

    // = TIMING

    void		reset();
    				// Resets all counters and timers back to
				// zero. Will also stop the stopwatch if
				// it is currently running.

    void		start();
    				// Starts the stopwatch. Has no affect
				// if the stopwatch is already running.

    void		stop();
    				// Stops the stopwatch. Has not affect
				// if the stopwatch is not running.

    bool		isRunning() const
    				{ return running_; }
				// Returns <true> if the stopwatch is
				// currently running.

    double		current() const;
				// Returns the amount of time the stopwatch
				// has been running for any currently active
				// period. Returns <-1> if the stopwatch is
				// not currently active.

    // Following do not cover any period for which the stopwatch may be
    // currently running. Times are all returned as number of seconds
    // expressed as a floating point value.

    int			samples() const
    				{ return samples_; }
				// Returns the number of times that the
				// stopwatch has been started and then
				// stopped.

    double		total() const;
    				// Returns the total amount of time that
				// the stopwatch has been running. This
				// covers multiple periods for which the
				// stopwatch has been active.

    double		average() const;
    				// Returns the average time the stopwatch
				// was running across each period.

    double		last() const;
    				// Returns the amount of time the stopwatch
				// was active for the last period. Returns
				// <-1> if there was no previous period.

    // = CHAINING

    OTC_ClockInfo*	prev() const
    				{ return prev_; }
				// Returns previous instance in chain of
				// instances.

    OTC_ClockInfo*	next() const
    				{ return next_; }
				// Returns next instance in chain of
				// instances.

  private:

			OTC_ClockInfo(OTC_ClockInfo const&);
				// Do not define an implementation for this.

    OTC_ClockInfo&	operator=(OTC_ClockInfo const&);
				// Do not define an implementation for this.

    char*		name_;
    				// The name for this instance of the
				// stopwatch.

    OTC_ClockInfo*	prev_;
    				// Pointer to the previous instance of
				// this object in chain of all instances.

    OTC_ClockInfo*	next_;
    				// Pointer to the next instance of
				// this object in chain of all instances.

    bool		running_;
				// Has value <true> if stopwatch is currently
				// running.

    int			samples_;
				// Number of periods for which the stopwatch
				// has been active. Doesn't include any
				// current period.

    clock_t		accumulated_;
				// The number of clock ticks accumulated
				// which the stopwatch has been active during
				// prior periods. Ie., doesn't include
				// current period.

    clock_t		checkpoint_;
    				// The clock value for the start of the
				// current period or <-1> if the stopwatch
				// is not running.

    clock_t		previous_;
    				// The number of clock ticks accumulated
				// during the previous active period.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_CLCKINFO_HH */
