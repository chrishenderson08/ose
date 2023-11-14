#ifndef OTC_STDLIB_OSTREAM_H
#define OTC_STDLIB_OSTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/ostream.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OSE_OSE_H
#include <OSE/OSE.h>
#endif

/* ------------------------------------------------------------------------- */

#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
#include <ostream>
using std::ios;
using std::streambuf;
using std::ostream;
using std::dec;
using std::endl;
using std::ends;
using std::flush;
#else
#if defined(__APPLE_CC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
// Big hack due to g++ 2.95.2 on Mac OS X 
// being missing the "ostream.h" header file.
#include <OTC/stdlib/iostream.h>
#else
#include <ostream.h>
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_OSTREAM_H */
