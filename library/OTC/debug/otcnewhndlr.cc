/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcnewhndlr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/outofmem.hh>
#include <OTC/debug/throwerr.hh>

/* ------------------------------------------------------------------------- */
OSE_EXPORT void otclib_new_handler()
{
  OTCERR_OutOfMemory exception;
  OTCLIB_THROW(exception);
}

/* ------------------------------------------------------------------------- */
