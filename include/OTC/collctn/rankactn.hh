#ifndef OTC_COLLCTN_RANKACTN_HH
#define OTC_COLLCTN_RANKACTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/rankactn.hh
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/rankactn.hh"
#pragma interface "OTC/collctn/rankactn.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_RankActions
    // = TITLE
    //     Default actions for determining the rank of items.
    //
    // = CLASS TYPE
    //     Static
    //
    // = DESCRIPTION
    //     This class encapsulates the default actions which are executed to
    //     determine the rank of two items. This class may be overridden for
    //     a particular type, to produce type specific behaviour.
    //     
    // = NOTES
    //     In the default implementation, it is expected that the less than,
    //     and equals operator are either explicitly, or implicitly defined
    //     for the type.
{
  public:

    // = ACTIONS

    static int		rank(T const& theItem1, T const& theItem2)
				{
				  return ((theItem1==theItem2) ? 0 :
				   ((theItem1<theItem2) ? -1 : 1));
				}
				// Returns an integer greater than, equal to,
				// or less than <0>, according to whether
				// <theItem1> is ranked greater than, equal
				// to, or less than <theItem2>.
};

/* ------------------------------------------------------------------------- */

#ifndef OTC_COLLCTN_STRGACTN_HH
#include <OTC/collctn/strgactn.hh>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_RANKACTN_HH */
