/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otctermfunc.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tobject.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_terminate_function()
{
  OTC_TObject::terminateAll();
  OTC_Logger::notify(OTCLIB_LOG_ALERT,"Program terminating");

  exit(1);
}

/* ------------------------------------------------------------------------- */
