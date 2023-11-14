/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcstopwtch.cc
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
#pragma implementation "OTC/system/stopwtch.hh"
#endif
#endif

#include <OTC/system/stopwtch.hh>

#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_ClockInfo* OTC_StopWatch::head_ = 0;

/* ------------------------------------------------------------------------- */
OTC_StopWatch::~OTC_StopWatch()
{
  if (clock_->name() == 0)
    delete clock_;
}

/* ------------------------------------------------------------------------- */
OTC_StopWatch::OTC_StopWatch(char const* theName)
  : clock_(0)
{
  if (theName != 0)
  {
    OTC_ClockInfo* theClock;
    theClock = head_;

    while (theClock != 0)
    {
      if (strcmp(theClock->name(),theName) == 0)
        break;

      theClock = theClock->next();
    }

    if (theClock == 0)
    {
      theClock = new OTC_ClockInfo(theName,0,head_);
      OTCLIB_ASSERT_M(theClock != 0);

      head_ = theClock;
    }

    clock_ = theClock;
  }
  else
  {
    clock_ = new OTC_ClockInfo;
    OTCLIB_ASSERT_M(clock_ != 0);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_StopWatch::report(ostream& theStream)
{
  OTC_ClockInfo* theClock;
  theClock = head_;

  while (theClock != 0)
  {
    theStream << theClock->name() << " ";
    theStream << theClock->total() << " ";
    theStream << theClock->samples() << " ";
    theStream << theClock->average() << endl;

    theClock = theClock->next();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_StopWatch::resetAll()
{
  OTC_ClockInfo* theClock;
  theClock = head_;

  while (theClock != 0)
  {
    theClock->reset();

    theClock = theClock->next();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_StopWatch::create(char const* theName)
{
  if (theName == 0)
    return;

  OTC_ClockInfo* theClock;
  theClock = head_;

  while (theClock != 0)
  {
    if (strcmp(theClock->name(),theName) == 0)
      break;

    theClock = theClock->next();
  }

  if (theClock == 0)
  {
    theClock = new OTC_ClockInfo(theName,0,head_);
    OTCLIB_ASSERT_M(theClock != 0);

    head_ = theClock;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_StopWatch::reset(char const* theName)
{
  if (theName == 0)
    return;

  OTC_ClockInfo* theClock;
  theClock = head_;

  while (theClock != 0)
  {
    if (strcmp(theClock->name(),theName) == 0)
      break;

    theClock = theClock->next();
  }

  if (theClock != 0)
    theClock->reset();
}

/* ------------------------------------------------------------------------- */
void OTC_StopWatch::start(char const* theName)
{
  if (theName == 0)
    return;

  OTC_ClockInfo* theClock;
  theClock = head_;

  while (theClock != 0)
  {
    if (strcmp(theClock->name(),theName) == 0)
      break;

    theClock = theClock->next();
  }

  if (theClock != 0)
    theClock->start();
}

/* ------------------------------------------------------------------------- */
void OTC_StopWatch::stop(char const* theName)
{
  if (theName == 0)
    return;

  OTC_ClockInfo* theClock;
  theClock = head_;

  while (theClock != 0)
  {
    if (strcmp(theClock->name(),theName) == 0)
      break;

    theClock = theClock->next();
  }

  if (theClock != 0)
    theClock->stop();
}

/* ------------------------------------------------------------------------- */
