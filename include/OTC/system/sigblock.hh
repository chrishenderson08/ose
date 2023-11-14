#ifndef OTC_SYSTEM_SIGBLOCK_HH
#define OTC_SYSTEM_SIGBLOCK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/sigblock.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include <signal.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/system/sigblock.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SignalBlock
    // = TITLE
    //     Class to assist in the blocking of UNIX signals.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     An instance of the <OTC_SignalBlock> class can be created on
    //     the stack, to cause blocking of the specified signals within
    //     the scope of that code block. The options are to block either
    //     one signal explicitly, all signals, or no signals. Note that
    //     <SIGABRT> will not be blocked if a request is made to block
    //     all signals.
{
  public:

			OTC_SignalBlock(int theSignal=-1);
				// Causes <theSignal> to be blocked.
				// If <theSignal> is <0>, no signals
				// will be blocked. The special value
				// of <-1> for <theSignal> will result
				// in all signals, with the exception
				// of <SIGABRT> being blocked. <theSignal>
				// has the default value of <-1>, ie.,
				// block all signals.

			~OTC_SignalBlock()
				{ release(); }
				// Unblocks the signals previously
				// blocked by the constructor.

   void			release();
				// Unblocks the signals previously
				// blocked by the constructor.

  private:

			OTC_SignalBlock(OTC_SignalBlock const&);
				// Do not define an implementation for this.

    OTC_SignalBlock&	operator=(OTC_SignalBlock const&);
				// Do not define an implementation for this.

    int			signal_;
				// Original signal number passed to
				// the constructor.

#if defined(HAVE_SIGPROCMASK)
    sigset_t		oldMask_;
				// Previous signal mask, compatable with
				// SYSV style signal blocking.
#else
    int			oldMask_;
				// Previous signal mask, compatable with
				// BSD style signal blocking.
#endif
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_SYSTEM_SIGBLOCK_HH */
