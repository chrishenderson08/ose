/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcassert.cc
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

#include <OTC/debug/assert.h>
#include <OTC/debug/asrtfail.hh>
#include <OTC/debug/outofmem.hh>
#include <OTC/debug/throwerr.hh>
#include <OTC/debug/logstrm.hh>

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_assert(
 char const* theFile,
 int theLine,
 char const* theMessage
)
{
  OTCERR_AssertionFailure exception(theFile,theLine,theMessage);
  OTCLIB_THROW(exception);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_assert_w(
 char const* theFile,
 int theLine,
 char const* theMessage
)
{
  // Create an exception class so we can display it, but do not
  // actually throw it. This function is only to display a warning.

  OTCERR_AssertionFailure exception(theFile,theLine,theMessage);

  char buf[2048];
  OTC_LogStream outs(buf,sizeof(buf));
  outs.setLevel(OTCLIB_LOG_WARNING);
  exception.display(outs);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_assert_m(char const* theFile, int theLine)
{
  OTCERR_OutOfMemory exception(theFile,theLine);
  OTCLIB_THROW(exception);
}

/* ------------------------------------------------------------------------- */
