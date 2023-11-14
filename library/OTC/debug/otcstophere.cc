/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcstophere.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/stophere.h>

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_error_stop_here()
{
  /* Go back up one stack frame from here to find caller. */
}

/* ------------------------------------------------------------------------- */
