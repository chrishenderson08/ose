/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcensure.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/ensure.h>
#include <OTC/debug/precfail.hh>
#include <OTC/debug/throwerr.hh>
#include <OTC/debug/logstrm.hh>

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_ensure(
 char const* theFile,
 int theLine,
 char const* theCondition,
 char const* theDescription,
 char const* theFunction
)
{
  OTCERR_PreconditionFailure exception(
   theFile,theLine,theCondition,theDescription,theFunction
  );
  OTCLIB_THROW(exception);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_ensure_w(
 char const* theFile,
 int theLine,
 char const* theCondition,
 char const* theDescription,
 char const* theFunction
)
{
  // Create an exception class so we can display it, but do not
  // actually throw it. This function is only to display a warning.

  OTCERR_PreconditionFailure exception(
   theFile,theLine,theCondition,theDescription,theFunction
  );

  char buf[2048];
  OTC_LogStream outs(buf,sizeof(buf));
  outs.setLevel(OTCLIB_LOG_WARNING);
  exception.display(outs);
}

/* ------------------------------------------------------------------------- */
