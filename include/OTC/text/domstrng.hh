#ifndef OTC_TEXT_DOMSTRNG_HH
#define OTC_TEXT_DOMSTRNG_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/domstrng.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/string.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/domstrng.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_DOMString
    // = TITLE
    //     String like object for referring to character arrays.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_DOMString> class is a special string like object for
    //     referring to character arrays which the class doesn't actually own
    //     or have responsibility for deleting. This is the scenario which
    //     occurs in the document object model classes where all string
    //     memory is contained with a memory arena, with that memory all
    //     be deleted at the same time.
    //
    //     Because the class doesn't own the character array, the memory
    //     arena holding the character array must exist for the lifetime of
    //     the instance of <OTC_DOMString> which refers to it. The main
    //     purpose of this class is for returning an object which is easily
    //     compared against other strings.
    //     
    // = SEE ALSO
    //     <OTC_String>, <OTC_RString>, <OTC_SObject>, <OTC_DOMNode>,
    //     <OTC_DOMProperty>
{
  public:

    			~OTC_DOMString();

    // = INITIALISATION

			OTC_DOMString(char const* theString=0)
			  : string_(theString?theString:""),
			    length_(theString?strlen(theString):0) {}
				// Creates a reference to the character array
				// given by <theString>. If <theString> is <0>
				// an implied reference to a zero length
				// character array is created.

			OTC_DOMString(char const* theString, size_t theLength);
				// Creates a reference to character array
				// given by <theString> where <theLength> is
				// taken to be the length of <theString>.
				// Raises an exception if <theString> is <0>
				// and <theLength> is not <0>.

			OTC_DOMString(OTC_DOMString const& theString)
			  : string_(theString.string_),
			    length_(theString.length_) {}
				// Creates a reference to the same character
				// array as is referred to by <theString>.

    OTC_DOMString&	operator=(OTC_DOMString const& theString)
      				{
				  string_ = theString.string_;
				  length_ = theString.length_;
				  return *this;
				}
				// Changes the reference to refer to the same
				// character array as is referred to by
				// <theString>.

    // = STRING
    //     Note that there is no gaurantee that the string has any
    //     form of null termination. Whether this is the case or not will
    //     depend on whether a null character exists at the appropriate
    //     position in the original character array.

    char const*		string() const
      				{ return string_; }
    				// Returns a pointer to the string.

#if 0
    			operator char const*() const
			  	{ return string_; }
    				// Returns a pointer to the string.
#endif

    size_t		length() const
      				{ return length_; }
    				// Returns the length of the string.

    bool		isEmpty() const
      				{ return length_ == 0; }
				// Returns <true> if the string is empty.

    // = COMPARISON

    int			rank(char const* theString) const
				{
				  return OTC_Rank::rank(
				   string_,length_,theString,
				   (theString?strlen(theString):0));
				}
				// Performs a comparison between this string
				// and <theString> and returns a value
				// corresponding to how the two strings
				// rank.

    int			rank(char const* theString, size_t theLength) const
				{
				  return OTC_Rank::rank(string_,
				   length_,theString,theLength);
				}
				// Performs a comparison between this string
				// and the first <theLength> characters of
				// <theString> and returns a value
				// corresponding to how the two strings rank.

    int			rank(OTC_String const& theString) const
				{
				  return OTC_Rank::rank(string_,length_,
				   theString.string(),theString.length());
				}
				// Performs a comparison between this string
				// and <theString> and returns a value
				// corresponding to how the two strings
				// rank.

    int			rank(OTC_DOMString const& theString) const
				{
				  return OTC_Rank::rank(string_,length_,
				   theString.string_,theString.length_);
				}
				// Performs a comparison between this string
				// and <theString> and returns a value
				// corresponding to how the two strings
				// rank.

    bool		operator==(char const* theString) const
				{
				  return OTC_Rank::rank(
				   string_,length_,theString,
				   (theString?strlen(theString):0)) == 0;
				}
				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		operator==(OTC_String const& theString) const
				{
				  return OTC_Rank::rank(string_,length_,
				   theString.string(),theString.length()) == 0;
				}
    				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		operator==(OTC_DOMString const& theString) const
				{
				  return OTC_Rank::rank(string_,length_,
				   theString.string_,theString.length_) == 0;
				}
    				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		operator!=(char const* theString) const
      				{
				  return OTC_Rank::rank(
				   string_,length_,theString,
				   (theString?strlen(theString):0)) != 0;
				}
    				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		operator!=(OTC_String const& theString) const
				{
				  return OTC_Rank::rank(string_,length_,
				   theString.string(),theString.length()) != 0;
				}
    				// Returns <true> if <theString> is
				// equivalent to this string.

    bool		operator!=(OTC_DOMString const& theString) const
				{
				  return OTC_Rank::rank(string_,length_,
				   theString.string_,theString.length_) != 0;
				}
    				// Returns <true> if <theString> is
				// equivalent to this string.

    friend OSE_EXPORT bool	operator==(
			 char const* s1,
			 OTC_DOMString const& s2
			)
				{
				  return OTC_Rank::rank(
				   s1,(s1?strlen(s1):0),
				   s2.string(),s2.length()) == 0;
				}

    friend OSE_EXPORT bool	operator==(
			 OTC_String const& s1,
			 OTC_DOMString const& s2
			)
				{
				  return OTC_Rank::rank(s1.string(),
				   s1.length(),s2.string(),s2.length()) == 0;
				}

    friend OSE_EXPORT bool	operator!=(
			 char const* s1,
			 OTC_DOMString const& s2
			)
				{
				  return OTC_Rank::rank(
				   s1,(s1?strlen(s1):0),
				   s2.string(),s2.length()) != 0;
				}

    friend OSE_EXPORT bool	operator!=(
			 OTC_String const& s1,
			 OTC_DOMString const& s2
			)
				{
				  return OTC_Rank::rank(s1.string(),
				   s1.length(),s2.string(),s2.length()) != 0;
				}

    // = STREAMS OUTPUT

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_DOMString const& theString
				)
				{ return outs << theString.data_; }
				// Dumps <theString> to the stream <outs>.
				// Width and justification specifications are
				// honoured. Note that this operation causes
				// a copy of the string to be made first.

#endif

  private:

    char const*		string_;
    				// The character array referred to.

    size_t		length_;
    				// The length of the character array.
};

/* ------------------------------------------------------------------------- */

OSE_EXPORT ostream& operator<<(ostream& outs, OTC_DOMString const& theString);

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_DOMSTRNG_HH */
