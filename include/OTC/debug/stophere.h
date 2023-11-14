#ifndef OTC_DEBUG_STOPHERE_H
#define OTC_DEBUG_STOPHERE_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/stophere.h
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

#ifndef OSE_OSE_H
#include <OSE/OSE.h>
#endif

/* ------------------------------------------------------------------------- */

/*
// Called by macros which generate exceptions. Allows a breakpoint to be
// set from inside a debugger to catch when an error occurs.
*/

#if defined(__cplusplus)
extern "C" OSE_EXPORT void otclib_error_stop_here();
#else
extern OSE_EXPORT void otclib_error_stop_here();
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_STOPHERE_H */
