#ifndef OTC_STDLIB_FSTREAM_H
#define OTC_STDLIB_FSTREAM_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/fstream.h
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
#include <fstream>
using std::ofstream;
using std::ifstream;
using std::fstream;
#else
#include <fstream.h>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_FSTREAM_H */
