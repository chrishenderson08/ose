#ifndef OTC_TEXT_TSTRING_HH
#define OTC_TEXT_TSTRING_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/tstring.hh
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

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/tstring.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_TString
    // = TITLE
    //     Temporary string resulting from addition of strings.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_TString> is return by <operator+()> when performed on
    //     instances of <OTC_String>. The only things which can be done with
    //     this class are dump its value to a stream, and construct an
    //     instance of <OTC_String>. The result of this class being returned
    //     from <operator+()> is that automatic conversion to <char const*>
    //     is not possible.
    //
    // = SEE ALSO
    //     <OTC_String>, <OTC_RString>
{
    friend class	OTC_String;
    friend class	OTC_Symbol;

  public:

			OTC_TString(OTC_TString const& theString)
			  : data_(theString.data_) {}

    // = INITIALISATION

			OTC_TString(OTC_RString const& theData)
			  : data_(theData) {}
				// Creates the string from the raw string
				// data.

    // = CONVERSION

    OTC_RString		rawString() const
				{ return data_; }
				// Returns underlying string object.

    // = STREAMS OUTPUT

#if 0

    friend OSE_EXPORT ostream&	operator<<(
				 ostream& outs,
				 OTC_TString const& theString
				)
				{ return outs << theString.data_; }
				// Dumps <theString> to the stream <outs>.
				// Width and justification specifications
				// are honoured.

#endif

  private:

    OTC_TString&	operator=(OTC_TString const&);
				// Do not define an implementation for this.

    OTC_RString		data_;
				// Raw string data.
};

/* ------------------------------------------------------------------------- */

inline ostream& operator<<(
 ostream& outs,
 OTC_TString const& theString
)
{ return outs << theString.rawString(); }

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_TSTRING_HH */
