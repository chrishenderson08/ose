#ifndef OTC_TEXT_CSTRING_HH
#define OTC_TEXT_CSTRING_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/cstring.hh
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

#include <OTC/text/rstring.hh>

#include <string.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/cstring.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_CString
    // = TITLE
    //     Allows exact length raw strings to be created.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_CString> provides a mechanism for creating raw strings
    //     which use only the amount of memory which is required for
    //     the string. <OTC_CString> should be used when creating
    //     constant strings.
    //
    // = SEE ALSO
    //     <OTC_String>, <OTC_RString>
{
    friend class	OTC_String;

  public:

    virtual		~OTC_CString();

    // = INITIALISATION

			OTC_CString(char const* theString=0);
				// Creates a copy of <theString>. Only
				// uses enough memory to hold the string.

			OTC_CString(char theChar, size_t theNum=1);
				// Creates a string holding <theNum>
				// instances of <theChar>. Only uses enough
				// memory to hold the string.

			OTC_CString(char const* theString, size_t theNum);
				// Creates a copy of the first <theNum>
				// characters of <theString>. Only
				// uses enough memory to hold the string.
				// Raises an exception if <theString> is
				// <0> but <theLength> is greater than <0>.

                        OTC_CString(OTC_CString const& theString)
                          : data_(theString.data_) {}
                                // Creates a delayed copy of <theString>.

    // = CONVERSION

    OTC_RString 	rawString() const
				{ return data_; }
				// Returns the underlying string data.

    // = STREAMS OUTPUT

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_CString const& theString
				)
				{ return outs << theString.data_; }
				// Dumps <theString> to the stream <outs>.
				// Width and justification specifications are
				// honoured.

#endif

  private:

    OTC_CString&	operator=(OTC_CString const&);
				// Do not define an implementation for this.

    OTC_RString		data_;
				// The raw string data.
};

/* ------------------------------------------------------------------------- */

inline ostream& operator<<(
 ostream& outs,
 OTC_CString const& theString
)
{ return outs << theString.rawString(); }

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_CSTRING_HH */
