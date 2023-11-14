/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcclckinfo.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/clckinfo.hh"
#endif
#endif

#include <OTC/system/clckinfo.hh>

#include <string.h>

#if !defined(CLOCKS_PER_SEC)
#define CLOCKS_PER_SEC 1000000
#endif

/* ------------------------------------------------------------------------- */
OTC_ClockInfo::~OTC_ClockInfo()
{
  if (prev_ != 0)
    prev_->next_ = next_;
  if (next_ != 0)
    next_->prev_ = prev_;

  delete [] name_;
}

/* ------------------------------------------------------------------------- */
OTC_ClockInfo::OTC_ClockInfo(
 char const* theName,
 OTC_ClockInfo* thePrev,
 OTC_ClockInfo* theNext
)
  : name_(0),
    prev_(thePrev),
    next_(theNext),
    running_(false),
    samples_(0),
    accumulated_(0),
    checkpoint_(0),
    previous_(clock_t(-1))
{
  if (theName != 0)
  {
    name_ = new char[strlen(theName)+1];
    strcpy(name_,theName);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_ClockInfo::reset()
{
  running_ = false;
  samples_ = 0;
  accumulated_ = 0;
  checkpoint_ = 0;
  previous_ = clock_t(-1);
}

/* ------------------------------------------------------------------------- */
void OTC_ClockInfo::start()
{
  if (running_ == false)
  {
    running_ = true;
    checkpoint_ = clock();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_ClockInfo::stop()
{
  if (running_ != false)
  {
    running_ = false;

    previous_ = clock() - checkpoint_;
    accumulated_ += previous_;

    samples_++;
  }
}

/* ------------------------------------------------------------------------- */
double OTC_ClockInfo::current() const
{
  if (running_ == false)
    return -1;

  return double(clock() - checkpoint_) / CLOCKS_PER_SEC;
}

/* ------------------------------------------------------------------------- */
double OTC_ClockInfo::total() const
{
  return double(accumulated_) / CLOCKS_PER_SEC;
}

/* ------------------------------------------------------------------------- */
double OTC_ClockInfo::average() const
{
  if (samples_ == 0)
    return 0;

  return (double(accumulated_) / samples_) / CLOCKS_PER_SEC;
}

/* ------------------------------------------------------------------------- */
double OTC_ClockInfo::last() const
{
  if (samples_ == 0)
    return -1;

  return double(previous_) / CLOCKS_PER_SEC;
}

/* ------------------------------------------------------------------------- */
