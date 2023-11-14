#ifndef OTC_SYSTEM_SIGHNDLE_HH
#define OTC_SYSTEM_SIGHNDLE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/sighndle.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include <signal.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/sighndle.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

typedef void (*OTC_SignalFunction)(int);

class OSE_EXPORT OTC_SignalHandler
    // = TITLE
    //     Class for installing a signal handler.
    //
    // = DESCRIPTION
    //     Provides a method for installing a signal handler. If the
    //     <sigaction()> function is available it will be used to
    //     register the signal handler. When registered, it will be
    //     done such that slow system calls will be restarted after
    //     a signal has occurred. If <sigaction()> is not available,
    //     the <signal()> function will be used to register the signal
    //     handler. On BSD systems, using this function will result
    //     in slow system calls being restarted. On SYSVR3
    //     machines, the <signal()> function does not result in slow
    //     system calls being restarted. Also, the signal handler gets
    //     desregistered upon the signal occuring.
    //
    //     The prototype for the signal handler must match:
    //
    // = BEGIN<CODE>
    //       void function(int);
    // = END<CODE>
    //
    //     The signal handler will be passed the number of the signal
    //     which occurred when it is called.
{
  public:

    // = REGISTRATION

    static OTC_SignalFunction	install(
			 int theSignal,
			 OTC_SignalFunction theFunction=0
			);
				// Installs <theFunction> as the signal
				// handler for <theSignal>. If <theFunction>
				// is <0>, the default signal handler for
				// <theSignal> is installed. The value
				// returned is the previously registered
				// signal handler.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_SIGHNDLE_HH */
