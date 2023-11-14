#ifndef OTC_STDLIB_ISTREAM_H
#define OTC_STDLIB_ISTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/istream.h
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
#include <istream>
using std::ios;
using std::streambuf;
using std::istream;
using std::dec;
using std::ws;
#else
#if defined(__APPLE_CC__) && (__GNUC__ == 2) && (__GNUC_MINOR__ == 95)
// Big hack due to g++ 2.95.2 on Mac OS X 
// being missing the "istream.h" header file.
#include <OTC/stdlib/iostream.h>
#else
#include <istream.h>
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_ISTREAM_H */
