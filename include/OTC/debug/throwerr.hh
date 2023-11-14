#ifndef OTC_DEBUG_THROWERR_HH
#define OTC_DEBUG_THROWERR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/throwerr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//
// ============================================================================
*/

#ifndef OTC_DEBUG_EXCPTION_HH
#include <OTC/debug/excption.hh>
#endif

/* ------------------------------------------------------------------------- */

/*
// Functions for throwing an exception and optionally setting a user
// defined function to be called just prior to the exception is thrown.
//
// These are now redundant. Use <throwException()> and <setThrow()> in
// the exception class instead.
*/

#define otclib_throw_exception(exception) \
 OTC_Exception::throwException(exception)

#define otclib_set_throw_function(function) \
 OTC_Exception::setThrow(function)

#define OTCLIB_THROW(exception) \
 OTC_Exception::throwException(exception,__FILE__,__LINE__)

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_THROWERR_HH */
