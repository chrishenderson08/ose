/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcsighndle.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/sighndle.hh"
#endif
#endif

#include <OTC/system/sighndle.hh>

#include <stdlib.h>
#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#include <signal.h>
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

/* ------------------------------------------------------------------------- */
typedef OTC_SignalFunction func_0;

#if 0
typedef void (*func_11)();
typedef void (*func_12)(int);
typedef void (*func_13)(...);
typedef void (*func_14)(int,...);
typedef int (*func_15)();
typedef int (*func_16)(int);
typedef int (*func_17)(...);
typedef int (*func_18)(int,...);

inline void set(func_11& dst, func_0 src) { dst = (func_11)src; }
inline void set(func_12& dst, func_0 src) { dst = (func_12)src; }
inline void set(func_13& dst, func_0 src) { dst = (func_13)src; }
inline void set(func_14& dst, func_0 src) { dst = (func_14)src; }
inline void set(func_15& dst, func_0 src) { dst = (func_15)src; }
inline void set(func_16& dst, func_0 src) { dst = (func_16)src; }
inline void set(func_17& dst, func_0 src) { dst = (func_17)src; }
inline void set(func_18& dst, func_0 src) { dst = (func_18)src; }
#endif

extern "C"
{
  typedef void (*func_21)();
  typedef void (*func_22)(int);
  typedef void (*func_23)(...);
  typedef void (*func_24)(int,...);
  typedef int (*func_25)();
  typedef int (*func_26)(int);
  typedef int (*func_27)(...);
  typedef int (*func_28)(int,...);
};

inline void set(func_21& dst, func_0 src) { dst = (func_21)src; }
inline void set(func_22& dst, func_0 src) { dst = (func_22)src; }
// inline void set(func_23& dst, func_0 src) { dst = (func_23)src; }
// inline void set(func_24& dst, func_0 src) { dst = (func_24)src; }
inline void set(func_25& dst, func_0 src) { dst = (func_25)src; }
inline void set(func_26& dst, func_0 src) { dst = (func_26)src; }
// inline void set(func_27& dst, func_0 src) { dst = (func_27)src; }
// inline void set(func_28& dst, func_0 src) { dst = (func_28)src; }

/* ------------------------------------------------------------------------- */
OTC_SignalFunction OTC_SignalHandler::install(
 int theSignal,
 OTC_SignalFunction theFunc
)
{
  // Value of <0> usually indicates the default
  // signal handler, but do this just to make sure.

  if (theFunc == 0)
    theFunc = (OTC_SignalFunction)SIG_DFL;

#ifdef HAVE_SIGACTION
  struct sigaction act;
  struct sigaction oact;
  memset(&act,0,sizeof(struct sigaction));
  memset(&oact,0,sizeof(struct sigaction));

  // Use magic overloading of the set() method to
  // try and cope with all the different prototypes
  // that a signal handler in the sigaction struct
  // can actually be specified as. Some of these
  // combinations haven't been seen, but provide
  // them just in case.

  set(act.sa_handler,theFunc);

  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(theSignal,(struct sigaction*)&act,(struct sigaction*)&oact);
  return (OTC_SignalFunction)oact.sa_handler;
#else
#if defined(SIG_PF)
  return (OTC_SignalFunction)::signal(theSignal,(SIG_PF)theFunc);
#else
  return (OTC_SignalFunction)::signal(theSignal,theFunc);
#endif
#endif
}

/* ------------------------------------------------------------------------- */
