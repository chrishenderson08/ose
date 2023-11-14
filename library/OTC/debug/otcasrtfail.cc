/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/otcasrtfail.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/asrtfail.hh"
#endif
#endif

#include <OTC/debug/asrtfail.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */
OTCERR_AssertionFailure::OTCERR_AssertionFailure(
 char const* theFile,
 u_int theLine,
 char const* theCondition
)
  : OTC_Exception("Assertion failed",theFile,theLine),
    condition_(theCondition)
{
  if (condition_ == 0)
    condition_ = "";
}

/* ------------------------------------------------------------------------- */
OTCERR_AssertionFailure::OTCERR_AssertionFailure(
 OTCERR_AssertionFailure const& theException
)
  : OTC_Exception(theException),
    condition_(theException.condition_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCERR_AssertionFailure::~OTCERR_AssertionFailure()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTCERR_AssertionFailure::display(ostream& outs) const
{
  OTC_Exception::display(outs);

  outs << "Condition: " << condition_ << EOL;
}

/* ------------------------------------------------------------------------- */
void OTCERR_AssertionFailure::throwIt() const
{
  throw *this;
}

/* ------------------------------------------------------------------------- */
