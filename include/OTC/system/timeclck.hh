#ifndef OTC_SYSTEM_TIMECLCK_HH
#define OTC_SYSTEM_TIMECLCK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/timeclck.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/system/stopwtch.hh>

/* ------------------------------------------------------------------------- */

#if defined(OTCLIB_TIMING)
#define OTCLIB_TIMEBLOCK(name) \
 OTC_TimeClock otclib_trigger_ ## name(name)
#else
#define OTCLIB_TIMEBLOCK(name)
#endif

class OTC_TimeClock
    // = TITLE
    //     Used as stack based object to start/stop a stopwatch.
    //
    // = SYNOPSIS
    //     #define OTCLIB_TIMEBLOCK ...
    //
    // = DESCRIPTION
    //     The <OTC_TimeClock> class is used to create a stack based object
    //     which will start a stopwatch when it is created and stop it when
    //     it is destroyed. This provides a simple means of starting
    //     a stopwatch and having it be stopped when that scope has been
    //     exited. The instance  of <OTC_StopWatch> must exist for the
    //     life of the instance of this class to which it is provided.
    //
    //     A macro <OTCLIB_TIMEBLOCK> is provided to be used in conjunction
    //     with the macros supplied with <OTC_StopWatch> for creating a
    //     stopwatch. These macros will only be compiled into code when
    //     the preprocessor symbol <OTCLIB_TIMING> is defined.
    //
    // = SEE ALSO
    //     <OTC_StopWatch>
{
  public:

    // = INITIALISATION

    			OTC_TimeClock(OTC_StopWatch& theStopWatch)
			  : stopwatch_(theStopWatch)
			  	{ stopwatch_.start(); }
				// Runs <start()> method of <theStopWatch>.

    // = DESTRUCTION

    			~OTC_TimeClock()
				{ stopwatch_.stop(); }
				// Runs <stop()> method of the stopwatch
				// object provided in the constructor.

    OTC_StopWatch&	stopwatch() const
    				{ return stopwatch_; }
				// Returns a reference to the stopwatch.

  private:

    			OTC_TimeClock(OTC_TimeClock const&);
				// Do not define an implementation for this.

    OTC_TimeClock&		operator=(OTC_TimeClock const&);
				// Do not define an implementation for this.

    OTC_StopWatch&	stopwatch_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_TIMECLCK_HH */
