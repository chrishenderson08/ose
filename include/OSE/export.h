#ifndef OSE_EXPORT_H
#define OSE_EXPORT_H
/*
// ============================================================================
//
// = LIBRARY
//     OSE
//
// = FILENAME
//     export.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OSE_SYSTEM_H
#include <OSE/system.h>
#endif

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

/* Now the actual definitions. */

#if defined(OSE_SYS_WIN32)
#if defined(OSE_DLL_EXPORT)
#define OSE_EXPORT __declspec(dllexport)
#else
#if defined(OSE_DLL_IMPORT)
#define OSE_EXPORT __declspec(dllimport)
#else
#define OSE_EXPORT
#endif
#endif
#else
#define OSE_EXPORT      
#endif

/* ------------------------------------------------------------------------- */

#endif /* OSE_EXPORT_H */
