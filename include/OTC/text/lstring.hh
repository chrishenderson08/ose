#ifndef OTC_TEXT_LSTRING_HH
#define OTC_TEXT_LSTRING_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/lstring.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/lstring.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_LString : public OTC_String
    // = TITLE
    //     String class which uses locale conventions for ordering.
    //     
    // = DESCRIPTION
    //     The <OTC_LString> class is a derived version of <OTC_String>.
    //     The sole purpose of this class is to allow locale ordering
    //     conventions to be used when a string is used as a key in a
    //     collection where <OTC_RankActions> is used.
    //
    //     There is no need to use this class explicitly everywhere. The
    //     only place you should need to use it is in the argument type
    //     when declaring the template collection. For example, to have
    //     an ordered list of strings with locale ordering use the
    //     template <OTC_OrderedList\<OTC_LString\>>. The iterator for
    //     the list will also need to use <OTC_LString> but everywhere
    //     else you can still use <OTC_String>.
    //
    //     The <strcoll()> function is used to determine locale ordering.
    //     If the operating system doesn't provide this function, normal
    //     ASCII ordering will be used. Since <strcoll()> requires null
    //     terminate strings, you cannot have embedded nulls in strings
    //     which you wish to order based on locale conventions. If there
    //     are embedded nulls, that null and anything after it will be
    //     ignored. If locale ordering is applied to a string with an
    //     embedded null, a warning will be displayed on the logger.
    //
    // = SEE ALSO
    //     <OTC_String>, <OTC_RankActions>
{
  public:

			~OTC_LString();

    // = INITIALISATION

			OTC_LString();
				// Creates an empty string with a notionally
				// undefined value. The string will behave
				// exactly like an empty string, but
				// <isUndefined()> will return true until the
				// value of the string has been set.

			OTC_LString(OTC_Capacity theCapacity)
			  : OTC_String(theCapacity) {}
				// Creates an empty string with enough
				// underlying capacity to hold a string of
				// <theCapacity>. This is equivalent to
				// the old <OTC_String> constructor
				// taking <u_int>.

			OTC_LString(OTC_Length theLength)
			  : OTC_String(theLength) {}
				// Creates a string of length <theLength>.
				// The string is not initialised and
				// will be filled with garbage. This is
				// equivalent to the old <OTC_Buffer>
				// constructor taking <u_int>.

			OTC_LString(
			 OTC_Length theLength,
			 OTC_Capacity theCapacity
			)
			  : OTC_String(theLength,theCapacity) {}
				// Creates a string of length <theLength>.
				// The string is not initialised and
				// will be filled with garbage. The
				// underlying buffer capacity will at the
				// same time be set to at least
				// <theCapacity>.

			OTC_LString(char const* theString)
			  : OTC_String(theString) {}
				// Creates a string holding a copy of the
				// null terminated string <theString>. If
				// <theString> is <0>, an empty string is
				// created.

			OTC_LString(char const* theString, size_t theNum)
			  : OTC_String(theString,theNum) {}
				// Creates a string holding a copy of the
				// first <theNum> characters of <theString>.
				// If <theString> is <0>, and <theNum> is
				// not <0>, an exception will be raised.

			OTC_LString(char theChar, size_t theNum=1)
			  : OTC_String(theChar,theNum) {}
				// Creates a string containing <theNum>
				// occurrences of <theChar>. If <theNum>
				// is <0> an empty string is created.

			OTC_LString(OTC_String const& theString)
			  : OTC_String(theString) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it.

			OTC_LString(OTC_LString const& theString)
			  : OTC_String(theString) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it.

			OTC_LString(OTC_String const& theString, size_t theNum)
			  : OTC_String(theString,theNum) {}
				// Creates a string holding a copy of the
				// first <theNum> characters of <theString>.
				// If the length of <theString> is less
				// than <theNum>, an exception is raised.

			OTC_LString(OTC_SObject const& theString)
			  : OTC_String(theString) {}
				// Creates a string which references the same
				// character string obtained from
				// <theString>. A copy is only made of the
				// string when an attempt is made to modify
				// it.

			OTC_LString(OTC_CString const& theString)
			  : OTC_String(theString) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it. <OTC_CString>
				// is a constant string.

			OTC_LString(OTC_RString const& theString)
			  : OTC_String(theString) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it. <OTC_RString>
				// is the raw string used to implement this
				// class.

			OTC_LString(OTC_TString const& theString)
			  : OTC_String(theString) {}
				// Creates a string which references the same
				// character string as held by <theString>. A
				// copy is only made of the string when an
				// attempt is made to modify it. Instances of
				// <OTC_TString> are returned from the
				// <operator+()> defined for strings.

    // = ASSIGNMENT

    OTC_LString&	operator=(char theChar)
				{
				  replace(0,length(),theChar,1);
				  return *this;
				}
				// Sets this string to be the single
				// character <theChar>.

    OTC_LString&	operator=(char const* theString)
				{
				  replace(0,length(),theString,
				   (theString ? ::strlen(theString) : 0));
				  return *this;
				}
				// Sets this string to be <theString>. If
				// <theString> is <0> the result is an empty
				// string.

    OTC_LString&	operator=(OTC_String const& theString)
				{ assign(theString); return *this; }
				// Sets this string to reference the same
				// character string as <theString>. A copy
				// is only made of the string when an attempt
				// is made to modify it. See description of
				// delayed copy mechanism above.

    OTC_LString&	operator=(OTC_LString const& theString)
				{ assign(theString); return *this; }
				// Sets this string to reference the same
				// character string as <theString>. A copy
				// is only made of the string when an attempt
				// is made to modify it. See description of
				// delayed copy mechanism above.
};

/* ========================================================================= */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<OTC_LString>
{
  public:
    static int		rank(OTC_LString const& s1, OTC_LString const& s2)
				{ return s1.rank(s2,OTCLIB_LOCALEORDER); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<OTC_LString>
{
  public:
    static int		hash(OTC_LString const& s)
				{ return s.hash(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_LSTRING_HH */
