#ifndef OTC_STDLIB_IOMANIP_H
#define OTC_STDLIB_IOMANIP_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     stdlib/iomanip.h
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
#include <iomanip>
using std::skipws;
using std::setw;
#else
#include <iomanip.h>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_STDLIB_IOMANIP_H */
