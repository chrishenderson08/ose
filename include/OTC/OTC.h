#ifndef OTC_OTC_H
#define OTC_OTC_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     OTC.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int __OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

#include <OSE/OSE.h>

/* ------------------------------------------------------------------------- */

#ifndef OTC_MISC_BOOL_H
#include <OTC/misc/bool.h>
#endif
#ifndef OTC_MISC_MACROS_H
#include <OTC/misc/macros.h>
#endif
#ifndef OTC_MISC_TYPES_H
#include <OTC/misc/types.h>
#endif
#ifndef OTC_MISC_LIMITS_H
#include <OTC/misc/limits.h>
#endif

#ifndef OTC_DEBUG_ERRORS_H
#include <OTC/debug/errors.h>
#endif
#ifndef OTC_DEBUG_ASSERT_H
#include <OTC/debug/assert.h>
#endif
#ifndef OTC_DEBUG_ENSURE_H
#include <OTC/debug/ensure.h>
#endif
#ifndef OTC_DEBUG_WARNING_H
#include <OTC/debug/warning.h>
#endif

#ifdef __cplusplus
#ifndef OTC_THREAD_THREAD_HH
#include <OTC/thread/thread.hh>
#endif
#ifndef OTC_MISC_HASH_HH
#include <OTC/misc/hash.hh>
#endif
#ifndef OTC_MISC_RANK_HH
#include <OTC/misc/rank.hh>
#endif
#ifndef OTC_MISC_COPY_HH
#include <OTC/misc/copy.hh>
#endif
#ifndef OTC_DEBUG_NEWHNDLR_HH
#include <OTC/debug/newhndlr.hh>
#endif
#ifndef OTC_DEBUG_TERMFUNC_HH
#include <OTC/debug/termfunc.hh>
#endif
#ifndef OTC_DEBUG_THROWERR_HH
#include <OTC/debug/throwerr.hh>
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_OTC_H */
