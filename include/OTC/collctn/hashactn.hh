#ifndef OTC_COLLCTN_HASHACTN_HH
#define OTC_COLLCTN_HASHACTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/hashactn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/hashactn.hh"
#pragma interface "OTC/collctn/hashactn.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_HashActions
    // = TITLE
    //     Default actions for calculating a hash value for an object.
    //
    // = CLASS TYPE
    //     Static
    //
    // = DESCRIPTION
    //     The default method used for determining a hash value for an
    //     object. This class may be overridden for a particular type,
    //     to define type specific behaviour.
    //     
    // = NOTES
    //     If the default behaviour is used, the type <T>, must be able to be
    //     converted to an integer.
{
  public:

    // = ACTIONS

    static int		hash(T const& theItem)
				{ return int(theItem); }
				// Returns a hash value for <theItem>.
};

#if !defined(OSE_NO_PARTIAL_SPECIALISATION)
template<class T>
class OTC_HashActions<T*>
{
  public:
    static int		hash(T* theItem)
      				{ return int(size_t(theItem)); }
};
#endif

/* ------------------------------------------------------------------------- */

#ifndef OTC_COLLCTN_STRGACTN_HH
#include <OTC/collctn/strgactn.hh>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HASHACTN_HH */
