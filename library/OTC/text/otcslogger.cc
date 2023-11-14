/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcslogger.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/slogger.hh"
#endif
#endif

#include <OTC/text/slogger.hh>

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_SLogger::~OTC_SLogger()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_SLogger::OTC_SLogger(char const* theChannel)
  : channel_(theChannel)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OSE_EXPORT OTC_SLogger::log(
 char const* theChannel,
 OTC_LogLevel,
 char const* theMessage,
 u_int theLength
)
{
  if (theChannel == 0 || *theChannel == EOS)
    return;

  if (channel_ == theChannel)
  {
    if (!messages_.isEmpty())
      messages_.append(EOL);
    messages_.append(theMessage,theLength);
  }
}

/* ------------------------------------------------------------------------- */
