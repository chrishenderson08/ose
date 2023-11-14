#ifndef OSE_OSE_H
#define OSE_OSE_H
/*
// ============================================================================
//
// = LIBRARY
//     OSE
// 
// = FILENAME
//     OSE.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

#if defined(OSE_TARGET_PLATFORM)
#define OSE_TARGET_PLATFORM_CONFIG_H <OSE/OSE_TARGET_PLATFORM.h>
#if !defined(MAKEIT_DEPEND)
#include OSE_TARGET_PLATFORM_CONFIG_H
#endif
#else
#if defined(_MSC_VER)
#include <OSE/i386-pc-win32.h>
#else
#include <OSE/config.h>
#endif
#endif

#include <OSE/system.h>
#include <OSE/version.h>
#include <OSE/export.h>
#include <OSE/compile.h>

/* ------------------------------------------------------------------------- */

#endif /* OSE_OSE_H */
