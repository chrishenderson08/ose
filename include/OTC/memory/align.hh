#ifndef OTC_MEMORY_ALIGN_HH
#define OTC_MEMORY_ALIGN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/align.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/memory/align.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Alignment
    // = TITLE
    //     Provides alignment information.
    //
    // = DESCRIPTION
    //     Provides a means of obtaining alignment information. This sort of
    //     information is handy for when writing memory managers. All values
    //     are expressed in terms of bytes.
{
  public:

    // = ALIGNMENT DATA

    static size_t	ofChar();
				// Returns alignment requirement for
				// type <char>.

    static size_t	ofShort();
				// Returns alignment requirement for
				// type <short>.

    static size_t	ofInt();
				// Returns alignment requirement for
				// type <int>.

    static size_t	ofLong();
				// Returns alignment requirement for
				// type <long>.

    static size_t	ofFloat();
				// Returns alignment requirement for
				// type <float>.

    static size_t	ofDouble();
				// Returns alignment requirement for
				// type <double>.

    static size_t	ofStruct();
				// Returns alignment requirement for
				// a struct.

    static size_t	ofWPtr();
				// Returns alignment requirement for
				// a pointer to a struct.

    static size_t	ofBPtr();
				// Returns alignment requirement for
				// a pointer to type <char>.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_ALIGN_HH */
