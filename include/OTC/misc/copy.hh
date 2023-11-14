#ifndef OTC_MISC_COPY_HH
#define OTC_MISC_COPY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/copy.hh
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
#pragma interface "OTC/misc/copy.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Copy
    // = TITLE
    //     Encapsulates a range of copy functions.
    //
    // = DESCRIPTION
    //     The <OTC_Copy> class encapsulates a number of copy functions for
    //     different types. For backward compatability, each of the functions
    //     is available through overloading using the single function
    //     <otclib_memmove(...)>. The functions use 4 byte integer copying
    //     to speed performance when possible. The functions do not check
    //     for null pointers.
{
  public:

    // = COPY FUNCTIONS

    static void		copy(
			 char* theString1,
			 char const* theString2,
			 size_t theLength
			);
				// Copies <theLength> characters from
				// <theString2> to <theString1>.

    friend inline void	otclib_memmove(
			 char* theString1,
			 char const* theString2,
			 size_t theLength
			)
				{
				  OTC_Copy::copy(theString1,
				   theString2,theLength);
				}
				// Calls <OTC_Copy::copy()>.

#if defined(HAVE_WCHAR_T)

    // Following are for wide character strings. These are only available
    // when the symbol <HAVE_WCHAR_T> is defined.

    static void		copy(
			 wchar_t* theString1,
			 wchar_t const* theString2,
			 size_t theLength
			)
				{
				  OTC_Copy::copy((char*)theString1,
				   (char const*)theString2,
				   theLength*sizeof(wchar_t)
				  );
				}
				// Copies <theLength> characters from
				// <theString2> to <theString1>.

    friend inline void	otclib_memmove(
			 wchar_t* theString1,
			 wchar_t const* theString2,
			 size_t theLength
			)
				{
				  OTC_Copy::copy((char*)theString1,
				   (char const*)theString2,
				   theLength*sizeof(wchar_t)
				  );
				}
				// Calls <OTC_Copy::copy()>.

#endif

  private:

			OTC_Copy(OTC_Copy const&);
				// Do not define an implementation for this.

    OTC_Copy&		operator=(OTC_Copy const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_COPY_HH */
