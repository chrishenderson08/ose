#ifndef OTC_MISC_HASH_HH
#define OTC_MISC_HASH_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/hash.hh
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
#pragma interface "OTC/misc/hash.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Hash
    // = TITLE
    //     Encapsulates a range of hash functions.
    //
    // = DESCRIPTION
    //     The <OTC_Hash> class encapsulates a number of hash functions for
    //     different types. For strings, as well as being able to provide the
    //     length of the string, a null terminated string can be provided.
    //     Hash algorithms on strings, use an array of random numbers to
    //     permutate the hash value. For backward compatability, each of the
    //     functions is available through overloading using the single
    //     function <otclib_hash(...)>.
{
  public:

    // = HASH FUNCTIONS

    static int		hash(char const* theString);
				// Returns a hash value for the null
				// terminated string given by <theString>.
				// If <theString> is <0> it is taken to
				// be an empty string.

    friend inline int	otclib_hash(char const* theString)
				{ return OTC_Hash::hash(theString); }
				// Calls <OTC_Hash::hash()>.

    static int		hash(char const* theString, size_t theLength);
				// Returns a hash value for <theString>
				// with <theLength>. If <theString> is
				// <0>, the string is taken to be an empty
				// string.

    friend inline int	otclib_hash(char const* theString, size_t theLength)
				{ return OTC_Hash::hash(theString,theLength); }
				// Calls <OTC_Hash::hash()>.

#if defined(HAVE_WCHAR_T)

    // Following are for wide character strings. These are only available
    // when the symbol <HAVE_WCHAR_T> is defined.

    static int		hash(wchar_t const* theString, size_t theLength);
				{
				  return otclib_hash((char const*)theString,
				   theLength*sizeof(wchar_t)
				  );
				}
				// Returns a hash value for <theString>
				// with <theLength>. If <theString> is
				// <0>, the string is taken to be an empty
				// string.

    friend inline int	otclib_hash(wchar_t const* theString, size_t theLength)
				{ return OTC_Hash::hash(theString,theLength); }
				// Calls <OTC_Hash::hash()>.

#endif

  private:

			OTC_Hash(OTC_Hash const&);
				// Do not define an implementation for this.

    OTC_Hash&		operator=(OTC_Hash const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_HASH_HH */
