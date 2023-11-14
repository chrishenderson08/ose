#ifndef OSE_SYSTEM_H
#define OSE_SYSTEM_H
/*
// ============================================================================
//
// = LIBRARY
//     OSE
// 
// = FILENAME
//     system.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

#if defined(__CYGWIN32__)
#define OSE_SYS_UNIX 1
#define OSE_SYS_CYGWIN32 1
#elif defined(unix) || defined(_unix) || defined(__unix)
#define OSE_SYS_UNIX 1
#elif defined(_AIX) || defined(LYNX) || defined(__MACH__)
#define OSE_SYS_UNIX 1
#elif defined(_WIN32)
#define OSE_SYS_WIN32 1
#endif

/* ------------------------------------------------------------------------- */

#endif /* OSE_SYSTEM_H */
