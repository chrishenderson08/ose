#ifndef OTC_DEBUG_WARNING_H
#define OTC_DEBUG_WARNING_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/warning.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
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
// [OTCLIB_WARNING]
//
// = TITLE
//     Macro to display an error message then continue execution.
//
// = SYNOPSIS
//     #define OTCLIB_WARNING(message) ...
//
// = DESCRIPTION
//     The macro <OTCLIB_WARNING()> should be used when an error occurs, and
//     where it is possible to continue execution. The message supplied as
//     argument to the macro will be sent to the logger at <WARNING> level.
//     In addition to this, information about the file in which this
//     occurred, and the line number within that file will also be sent to
//     the logger.
//
// = EXAMPLES
// = BEGIN<CODE>
//     if (aStr == 0)
//     {
//       OTCLIB_WARNING("Nil pointer");
//       aStr = "";
//     }
// = END<CODE>
//
// = FORMAT
//     If the exception is caught and the information it contains is dumped
//     to the logger, the format of the output will be:
//
// = BEGIN<CODE>
//     WARNING: Nil pointer
//     WARNING: Location: "warning_.cc", line 33
// = END<CODE>
//
// = NOTES
//     The macro may be used in either C or C++ code.
//
// = SEE ALSO
//     <OTC_Logger>
*/

#if defined(__cplusplus)
extern "C" OSE_EXPORT void otclib_warning(
 char const* theFile, int theLine, char const* theMessage
);
#else
extern OSE_EXPORT void otclib_warning();
#endif

#define OTCLIB_WARNING(msg) \
 otclib_error_stop_here(), otclib_warning(__FILE__,__LINE__,msg)

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_WARNING_H */
