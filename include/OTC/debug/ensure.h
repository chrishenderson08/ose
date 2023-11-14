#ifndef OTC_DEBUG_ENSURE_H
#define OTC_DEBUG_ENSURE_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/ensure.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
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
// [OTCLIB_ENSURE]
//
// = TITLE
//     Macro to aid in program verfication.
//
// = SYNOPSIS
//     #define OTCLIB_ENSURE(expression,description) ...
//     #define OTCLIB_ENSURE_FN(expression,function,description) ...
//     #define OTCLIB_ENSURE_W(expression,description) ...
//     #define OTCLIB_ENSURE_FN_W(expression,function,description) ...
//
// = DESCRIPTION
//     The macro <OTCLIB_ENSURE()> can be used to aid in program
//     verification. The macro is used to indicate the expectation that the
//     expression is to be true at this point in the program. If expression
//     is false, an exception of type <OTCERR_PreconditionFailure> will be
//     thrown.
//
//     Note that <OTCLIB_ENSURE()> cannot be compiled out of code and should
//     be used in preference to <OTCLIB_ASSERT()>, when you wish to ensure
//     that the check will always be in the code. Although it cannot be
//     compiled out of code, it is still not recommended that the expression
//     have any side effects, which the program relies on to occur. This is
//     suggested as someone in the future could unknowingly remove the
//     precondition check altogether or replace it with an assertion failure.
//     The result would be that the program may stop functioning correctly.
//
//     The <OTCLIB_ENSURE_W()> macro behaves similar to <OTCLIB_ENSURE()>
//     except that only a warning is generated and an exception is not
//     raised. The versions of these macros with "_FN" in the name take
//     three arguments. The second argument is the name of the function,
//     pushing the description to the third argument.
//
// = EXAMPLE
// = BEGIN<CODE>
//     OTCLIB_ENSURE((TRUE == FALSE),"TRUE is not equal to FALSE");
// = END<CODE>
//
// = FORMAT
//     If the exception is caught and the information it contains is dumped
//     to the logger, the format of the output will be:
//
// = BEGIN<CODE>
//     ERROR: Precondition failed
//     ERROR: Location: "ensure_.cc", line 33
//     ERROR: Condition: (TRUE == FALSE)
//     ERROR: Description: TRUE is not equal to FALSE
// = END<CODE>
//
// = NOTES
//     The macro may be used in either C or C++ code.
//
//     A string literal should not be used as part of the condition as some
//     preprocessors cannot handle converting to a string, a value which has
//     a string in it.
//
// = SEE ALSO
//     <OTCERR_PreconditionFailure>, <OTC_Logger>
*/

#if defined(__cplusplus)

extern "C" OSE_EXPORT void otclib_ensure(
 char const* theFile, int theLine, char const* theCondition,
 char const* theDescription, char const* theFunction=0
);

extern "C" OSE_EXPORT void otclib_ensure_w(
 char const* theFile, int theLine, char const* theCondition,
 char const* theDescription, char const* theFunction=0
);

#else

extern OSE_EXPORT void otclib_ensure();
extern OSE_EXPORT void otclib_ensure_w();

#endif

/*
#if defined(__STDCPP__)
*/
#define OTCLIB_ENSURE(ex,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure(__FILE__,__LINE__,#ex,msg,0),0)))
/*
#else
#define OTCLIB_ENSURE(ex,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure(__FILE__,__LINE__,"ex",msg,0),0)))
#endif
*/

/*
#if defined(__STDCPP__)
*/
#define OTCLIB_ENSURE_FN(ex,func,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure(__FILE__,__LINE__,#ex,msg,func),0)))
/*
#else
#define OTCLIB_ENSURE_FN(ex,func,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure(__FILE__,__LINE__,"ex",msg,func),0)))
#endif
*/

/*
#if defined(__STDCPP__)
*/
#define OTCLIB_ENSURE_W(ex,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure_w(__FILE__,__LINE__,#ex,msg,0),0)))
/*
#else
#define OTCLIB_ENSURE_W(ex,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure_w(__FILE__,__LINE__,"ex",msg,0),0)))
#endif
*/

/*
#if defined(__STDCPP__)
*/
#define OTCLIB_ENSURE_FN_W(ex,func,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure_w(__FILE__,__LINE__,#ex,msg,func),0)))
/*
#else
#define OTCLIB_ENSURE_FN_W(ex,func,msg) \
 ((void)((ex)||(otclib_error_stop_here(), \
 otclib_ensure_w(__FILE__,__LINE__,"ex",msg,func),0)))
#endif
*/

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_ENSURE_H */
