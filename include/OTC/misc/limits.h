#ifndef OTC_MISC_LIMITS_H
#define OTC_MISC_LIMITS_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     misc/limits.h
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

/*
// Longest permissable path length. Make a large guess if we can't find any
// thing to set it from.
*/

#ifndef MAXPATHLEN
#define MAXPATHLEN 4096
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_LIMITS_H */
