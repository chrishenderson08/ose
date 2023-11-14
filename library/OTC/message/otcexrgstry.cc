/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcexrgstry.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/exrgstry.hh"
#endif
#endif

#include <OTC/message/exrgstry.hh>
#include <OTC/message/exchange.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EXRegistry::mutex_;

/* ------------------------------------------------------------------------- */
OTC_EXRegistry::~OTC_EXRegistry()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_EXRegistry::OTC_EXRegistry()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EXRegistry::add(OTC_Exchange* theExchange)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCLIB_ENSURE_FN((theExchange != 0),
   "OTC_EXRegistry::add()","invalid exchange");
  OTCLIB_ENSURE_FN((!exchanges_.contains(theExchange->group())),
   "OTC_EXRegistry::add()","exchange already present for group");

  exchanges_.add(theExchange->group(),theExchange);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_EXRegistry::remove(char const* theGroup)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  exchanges_.remove(theGroup,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_Exchange* OTC_EXRegistry::lookup(char const* theGroup)
{
  OTC_Exchange* theResult = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (exchanges_.contains(theGroup))
    theResult = exchanges_.item(theGroup);

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_EXRegistry::subscribe(int theAgentId)
{
  subscribers_.addObserver(theAgentId);
}

/* ------------------------------------------------------------------------- */
void OTC_EXRegistry::unsubscribe(int theAgentId)
{
  subscribers_.removeObserver(theAgentId);
}

/* ------------------------------------------------------------------------- */
