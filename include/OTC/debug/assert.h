#ifndef OTC_DEBUG_ASSERT_H
#define OTC_DEBUG_ASSERT_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/assert.h
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

#ifndef OSE_OSE_H
#include <OSE/OSE.h>
#endif

#ifndef OTC_MISC_MACROS_H
#include <OTC/misc/macros.h>
#endif
#ifndef OTC_DEBUG_STOPHERE_H
#include <OTC/debug/stophere.h>
#endif

/* ------------------------------------------------------------------------- */

/*
// [OTCLIB_ASSERT]
//
// = TITLE
//     Macro to aid in program verification.
//
// = SYNOPSIS
//     #define OTCLIB_ASSERT(expression) ...
//     #define OTCLIB_ASSERT_W(expression) ...
//     #define OTCLIB_ASSERT_M(expression) ...
//
// = DESCRIPTION
//     The macro <OTCLIB_ASSERT()> can be used to aid in program
//     verification. The macro is used to indicate the expectation that the
//     expression is to be true at this point in the program. If the
//     expression is false, an exception of type <OTCERR_AssertionFailure> is
//     thrown. Compiling with the compiler option <-DNDEBUG> effectively
//     deletes <OTCLIB_ASSERT()> from the program. If you are using <makeit>,
//     the <NDEBUG> symbol is automatically defined when you use the <opt>
//     variant.
//
//     If you want an assertion failure like with <OTCLIB_ASSERT()>, but do
//     not want an exception to be raised but only a warning generated via
//     the logger, you can use <OTCLIB_ASSERT_W()>. This macro also becomes
//     ineffective when the <NDEBUG> symbol is defined.
//
//     If your assertion check is specifically checking for the success
//     of <operator new()> or <malloc()>, rather than use <OTCLIB_ASSERT()>,
//     you should use <OTCLIB_ASSERT_M()>. This macro, rather than
//     generating an exception of type <OTCERR_AssertionFailure> will
//     generate an exception of type <OTCERR_OutOfMemory>. Note that this
//     macro is not disabled by defining <NDEBUG>.
//
//     Note that the expression evaluated in the condition, should not
//     produce any side effects, which the program relies upon. If this is
//     done, when the assertions are compiled out of code, the program may
//     stop functioning correctly.
//
// = EXAMPLE
// = BEGIN<CODE>
//     OTCLIB_ASSERT(TRUE == FALSE);
// = END<CODE>
//
// = FORMAT
//     If the exception is caught and the information it contains is dumped
//     to the logger, the format of the output will be:
// = BEGIN<CODE>
//     ERROR: Assertion failed
//     ERROR: Location: "assert_.cc", line 33
//     ERROR: Condition: TRUE == FALSE
// = END<CODE>
//
// = NOTES
//     The macros may be used in either C or C++ code.
//
//     A string literal should not be used as part of the condition as some
//     preprocessors cannot handle converting to a string, a value which has
//     a string in it.
//
// = SEE ALSO
//     <OTCERR_AssertionFailure>, <OTCERR_OutOfMemory>, <OTC_Logger>
*/

#if defined(__cplusplus)

extern "C" OSE_EXPORT void otclib_assert(
 char const* theFile, int theLine, char const* theMessage
);

extern "C" OSE_EXPORT void otclib_assert_w(
 char const* theFile, int theLine, char const* theMessage
);

extern "C" OSE_EXPORT void otclib_assert_m(
 char const* theFile, int theLine
);

#else

extern OSE_EXPORT void otclib_assert();
extern OSE_EXPORT void otclib_assert_w();
extern OSE_EXPORT void otclib_assert_m();

#endif

#ifdef NDEBUG
#define OTCLIB_ASSERT(ex) ((void)0)
#else
/*
#if defined(__STDCPP__)
*/
#define OTCLIB_ASSERT(ex) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_assert(__FILE__,__LINE__,#ex),0)))
/*
#else
#define OTCLIB_ASSERT(ex) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_assert(__FILE__,__LINE__,"ex"),0)))
#endif
*/
#endif

#ifdef NDEBUG
#define OTCLIB_ASSERT_W(ex) ((void)0)
#else
/*
#if defined(__STDCPP__)
*/
#define OTCLIB_ASSERT_W(ex) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_assert_w(__FILE__,__LINE__,#ex),0)))
/*
#else
#define OTCLIB_ASSERT_W(ex) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_assert_w(__FILE__,__LINE__,"ex"),0)))
#endif
*/
#endif

#define OTCLIB_ASSERT_M(ex) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_assert_m(__FILE__,__LINE__),0)))

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_ASSERT_H */
