#ifndef OSE_COMPILE_H
#define OSE_COMPILE_H
/*
// ============================================================================
//
// = LIBRARY
//     OSE
//
// = FILENAME
//     compile.h
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/* Force this header into dependencies. */
extern int OSE_DEPENDENCY_MARKER;

/* ------------------------------------------------------------------------- */

#ifdef __GNUG__
#if (__GNUC__ < 3)
#define OSE_HAVE_OLD_STYLE_IOSTREAMS 1
#endif
#endif

#ifdef _MSC_VER
#if (_MSC_VER < 1400)
#define OSE_HAVE_OLD_STYLE_IOSTREAMS 1
#endif
#endif

#if defined(OSE_HAVE_OLD_STYLE_TEMPLATE_SPECIALISATION)
#define OSE_TEMPLATE_SPECIALISATION
#else 
#define OSE_TEMPLATE_SPECIALISATION template<>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OSE_COMPILE_H */
