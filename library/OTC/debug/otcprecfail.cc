/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcprecfail.cc
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
#pragma implementation "OTC/debug/precfail.hh"
#endif
#endif

#include <OTC/debug/precfail.hh>

#include <OTC/stdlib/iostream.h>

/* ------------------------------------------------------------------------- */
OTCERR_PreconditionFailure::OTCERR_PreconditionFailure(
 char const* theFile,
 u_int theLine,
 char const* theCondition,
 char const* theDescription,
 char const* theFunction
)
  : OTC_Exception("Precondition failed",theFile,theLine),
    condition_(theCondition),
    description_(theDescription),
    function_(theFunction)
{
  if (condition_ == 0)
    condition_ = "";
  if (description_ == 0)
    description_ = "";
  if (function_ == 0)
    function_ = "";
}

/* ------------------------------------------------------------------------- */
OTCERR_PreconditionFailure::OTCERR_PreconditionFailure(
 OTCERR_PreconditionFailure const& theException
)
  : OTC_Exception(theException),
    condition_(theException.condition_),
    description_(theException.description_),
    function_(theException.function_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCERR_PreconditionFailure::~OTCERR_PreconditionFailure()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTCERR_PreconditionFailure::display(ostream& outs) const
{
  OTC_Exception::display(outs);

  if (*function_ != EOS)
    outs << "Function: " << function_ << EOL;
  if (*condition_ != EOS)
    outs << "Condition: " << condition_ << EOL;
  if (*description_ != EOS)
    outs << "Description: " << description_ << EOL;
}

/* ------------------------------------------------------------------------- */
void OTCERR_PreconditionFailure::throwIt() const
{
  throw *this;
}

/* ------------------------------------------------------------------------- */
