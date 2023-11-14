/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/_tracetag.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#define OTCLIB_TRACE 1

#include <OTC/debug/tracetag.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

/* ------------------------------------------------------------------------- */

OTC_TraceTag SYSTEM("SYSTEM");
OTC_TraceTag LIBRARY("LIBRARY");
OTC_TraceTag PROGRAM("PROGRAM");

int main()
{
  OTCLIB_TRACESWITCH(HERE,(LIBRARY >= 2 || PROGRAM));

  OTCLIB_MARKBLOCK(HERE,"main()");

  OTCLIB_TRACER(HERE) << "HERE" << endl;
  OTCLIB_TRACER(HERE || SYSTEM) << "HERE || SYSTEM" << endl;

  OTCLIB_TRACER(LIBRARY >= 0) << "LIBRARY >= 0" << endl;
  OTCLIB_TRACER(LIBRARY >= 1) << "LIBRARY >= 1" << endl;
  OTCLIB_TRACER(LIBRARY >= 2) << "LIBRARY >= 2" << endl;

  OTCLIB_TRACER(PROGRAM) << "PROGRAM" << endl;
  OTCLIB_TRACER(SYSTEM) << "SYSTEM" << endl;
  OTCLIB_TRACER(PROGRAM || LIBRARY) << "PROGRAM || LIBRARY" << endl;
  OTCLIB_TRACER(PROGRAM >= 1 || LIBRARY) << "PROGRAM >= 1 || LIBRARY" << endl;

  OTCLIB_TRACER(LIBRARY == 0) << "LIBRARY == 0" << endl;
  OTCLIB_TRACER(LIBRARY >= 1 && LIBRARY <= 3) << "LIBRARY >= 1 && LIBRARY <= 3" << endl;
  OTCLIB_TRACER(LIBRARY <= 1 || LIBRARY >= 3) << "LIBRARY <= 1 || LIBRARY >= 3" << endl;
  OTCLIB_TRACER(!LIBRARY) << "!LIBRARY" << endl;

  return 0;
}

/* ------------------------------------------------------------------------- */
