#ifndef OTC_MISC_RANK_HH
#define OTC_MISC_RANK_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/rank.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <stdlib.h>

#include <OTC/misc/types.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/misc/rank.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_SRankType
{
  OTCLIB_ASCIIORDER,
  OTCLIB_ASCIIORDER_IGNORECASE,
  OTCLIB_LOCALEORDER
};

class OSE_EXPORT OTC_Rank
    // = TITLE
    //     Encapsulates a range of rank functions.
    //
    // = DESCRIPTION
    //     The <OTC_Rank> class encapsulates a number of rank functions for
    //     different types. For strings, as well as being able to provide the
    //     length of the string, a null terminated string can be provided.
    //     For backward compatability, each of the functions is available
    //     through overloading using the single function <otclib_rank(...)>.
    //     The results of the functions are such that if the first string
    //     ranks less than the second, a value less than <0> will be
    //     returned. If the strings rank equal, a value of <0> is returned.
    //     Otherwise a value greater than <0> is returned. The rank routines
    //     do not check for null pointers when non zero lengths are defined.
    //     
    //     In each of the functions, a rank type argument can be supplied.
    //     This defines whether ascii ordering with case senstivity, ascii
    //     ordering with case insensitivity, or locale ordering is used. The
    //     values are <OTCLIB_ASCIIORDER>, <OTCLIB_ASCIIORDER_IGNORECASE>
    //     and <OTCLIB_LOCALEORDER>. If locale ordering cannot be determined
    //     then ascii ordering will be used. Note that strings passed to the
    //     functions when using locale ordering must be null terminated. This
    //     null terminator should not be used in the length of the string
    //     however. If there is no null terminator, the result will be
    //     undefined and may cause your program to crash. A string used when
    //     performing locale ordering should not contain any embedded nulls.
    //     If there are embedded nulls, only up to that embedded null will be
    //     used and the remainder of the string will be silently ignored.
{
  public:

    // = RANK FUNCTIONS

    static int		rank(
			 char const* theString1,
			 char const* theString2,
			 OTC_SRankType theRankType=OTCLIB_ASCIIORDER
			);
				// Ranks the null terminated strings,
				// <theString1> and <theString2>. Type of
				// comparison is determined by <theRankType>.

    friend inline int	otclib_rank(
			 char const* theString1,
			 char const* theString2,
			 OTC_SRankType theRankType=OTCLIB_ASCIIORDER
			)
				{
				  return OTC_Rank::rank(theString1,
				   theString2,theRankType);
				}
				// Calls <OTC_Rank::rank()>.

    static int		rank(
			 char const* theString1,
			 size_t theLength1,
			 char const* theString2,
			 size_t theLength2,
			 OTC_SRankType theRankType=OTCLIB_ASCIIORDER
			);
				// Ranks the strings, <theString1> and
				// <theString2>. The length of the strings
				// are given by <theLength1> and
				// <theLength2>. Type of comparison is
				// determined by <theRankType>.

    friend inline int	otclib_rank(
			 char const* theString1,
			 size_t theLength1,
			 char const* theString2,
			 size_t theLength2,
			 OTC_SRankType theRankType=OTCLIB_ASCIIORDER
			)
				{
				  return OTC_Rank::rank(theString1,
				   theLength1,theString2,theLength2,
				   theRankType);
				}
				// Calls <OTC_Rank::rank()>.

#if defined(HAVE_WCHAR_T)

    // Following are for wide character strings. These are only available
    // when the symbol <HAVE_WCHAR_T> is defined.

    static int		rank(
			 wchar_t const* theString1,
			 size_t theLength1,
			 wchar_t const* theString2,
			 size_t theLength2,
			 OTC_SRankType theRankType=OTCLIB_ASCIIORDER
			);
				// Ranks the strings, <theString1> and
				// <theString2>. The length of the strings
				// are given by <theLength1> and
				// <theLength2>. Type of comparison is
				// determined by <theRankType>.

    friend inline int	otclib_rank(
			 wchar_t const* theString1,
			 size_t theLength1,
			 wchar_t const* theString2,
			 size_t theLength2,
			 OTC_SRankType theRankType=OTCLIB_ASCIIORDER
			)
				{
				  return OTC_Rank::rank(theString1,
				   theLength1,theString2,theLength2,
				   theRankType);
				}
				// Calls <OTC_Rank::rank()>.

#endif

  private:

			OTC_Rank(OTC_Rank const&);
				// Do not define an implementation for this.

    OTC_Rank&		operator=(OTC_Rank const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_RANK_HH */
