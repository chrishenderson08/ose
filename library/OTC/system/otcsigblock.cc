/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcsigblock.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/sigblock.hh"
#endif
#endif

#include <OTC/system/sigblock.hh>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_SignalBlock::OTC_SignalBlock(int theSignal)
  : signal_(theSignal)
{
  if (signal_ == 0)
    return;

#if defined(OSE_SYS_UNIX)
#if defined(HAVE_SIGPROCMASK)
  sigset_t theNewMask;
  if (theSignal == -1)
  {
    sigfillset(&theNewMask);
    sigdelset(&theNewMask,SIGABRT);
  }
  else
  {
    sigemptyset(&theNewMask);
    sigaddset(&theNewMask,theSignal);
  }

  sigprocmask(SIG_BLOCK,&theNewMask,&oldMask_);
#else
  int theNewMask = 0;
  if (theSignal == -1)
  {
    theNewMask = sigmask(SIGABRT);
    theNewMask ^= ~0;
  }
  else
    theNewMask = sigmask(theSignal);

  oldMask_ = sigblock(theNewMask);
#endif
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_SignalBlock::release()
{
  if (signal_ == 0)
    return;

#if defined(OSE_SYS_UNIX)
#if defined(HAVE_SIGPROCMASK)
  sigprocmask(SIG_SETMASK,&oldMask_,0);
#else
  sigsetmask(oldMask_);
#endif
#endif

  signal_ = 0;
}

/* ------------------------------------------------------------------------- */
