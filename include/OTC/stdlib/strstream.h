#ifndef OTC_STDLIB_STRSTREAM_H
#define OTC_STDLIB_STRSTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/strstream.h
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
#include <strstream>
using std::istrstream;
using std::ostrstream;
using std::strstream;
#else
#ifdef _MSC_VER
#include <strstrea.h>
#else
#include <strstream.h>
#endif
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_STRSTREAM_H */
