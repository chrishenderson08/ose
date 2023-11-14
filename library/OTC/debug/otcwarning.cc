/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcwarning.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/warning.h>
#include <OTC/debug/logstrm.hh>

#include <stdio.h>

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_warning(
 char const* theFile,
 int theLine,
 char const* theMessage
)
{
  if (theFile == 0)
    theFile = "";
  if (theMessage == 0)
    theMessage = "";

  char buf[2048];
  OTC_LogStream outs(buf,sizeof(buf));

  outs.setLevel(OTCLIB_LOG_WARNING);

  outs << theMessage << EOL;
  outs << "Location: " << theFile << ", line " << theLine << EOL;
  outs << flush;
}

/* ------------------------------------------------------------------------- */
