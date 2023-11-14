#ifndef OTC_COLLCTN_COPYACTN_HH
#define OTC_COLLCTN_COPYACTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/copyactn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#include <stdlib.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/copyactn.hh"
#endif
#pragma interface "OTC/collctn/copyactn.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_CopyActions
    // = TITLE
    //     Default method for copying arrays.
    //
    // = CLASS TYPE
    //     Static
    //
    // = DESCRIPTION
    //     This class encapsulates the default method used for copying arrays.
    //     The default method may be overridden for a type by providing a
    //     template override class for that type. An override version of the
    //     class is provided for the type <char>. The override version in
    //     this case used a version of <memcpy()> which copies four bytes
    //     at a time.
{
  public:

    static void		copy(T* theDest, T const* theSrc, size_t theLength);
				// Copies <theLength> number of objects of
				// type <T> from <theSrc> to <theDest>.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/copyactn.c>
#endif

#ifndef OTC_COLLCTN_STRGACTN_HH
#include <OTC/collctn/strgactn.hh>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_COPYACTN_HH */
