/*
// ============================================================================
//
// = FILENAME
//     otcpyloader.cc
//
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include "module.hh"

/* ------------------------------------------------------------------------- */

#if defined(OSE_SYS_UNIX)
#define OSE_PYLOADER_EXPORT
#else
#define OSE_PYLOADER_EXPORT __declspec(dllexport)
#endif

extern "C" OSE_PYLOADER_EXPORT void init_ose();

OSE_PYLOADER_EXPORT void init_ose()
{
  otclib_init_python();
}

/* ------------------------------------------------------------------------- */
