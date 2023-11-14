#ifndef OTC_TEXT_STRNGBUF_HH
#define OTC_TEXT_STRNGBUF_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/strngbuf.hh
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
#include <OTC/misc/vfuncbuf.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/strngbuf.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_StringBuf : public OTC_VFuncBuf
    // = TITLE
    //     Streambuf class for use on an instance of a string class.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_StringBuf> class is a derived version of <streambuf>
    //     specifically for formatting output directly into an instance of
    //     the <OTC_String> class. At the same time as the stream is
    //     being used on the string, you can use all the member functions
    //     of the <OTC_String> class. This is because for this streambuf
    //     characters are immediately appended to the target string one at
    //     a time. Although this means that modifications can be made to
    //     the string directly at the same time, it is inefficient when
    //     compared to a streambuf which buffers data prior to appending
    //     it to the target string. Therefore, this streambuf shouldn't be
    //     used when it is necessary to format large volumes of data into
    //     a target string.
    //
    // = NOTES
    //     The referenced string must exist for the lifetime of the streambuf
    //     which refers to it.
    //     
    // = SEE ALSO
    //     <OTC_OSStream>, <OTC_String>
{
  public:

			~OTC_StringBuf();

    // = INITIALISATION

			OTC_StringBuf(
			 OTC_String& theString,
			 OTC_BufferingFlag theType
			);
				// <theString> should be a reference to
				// an instance of <OTC_String> to which
				// the buffer contents should be appended.

  protected:

    void		handle(char const* theString, size_t theLength);

  private:

			OTC_StringBuf(OTC_StringBuf const&);
				// Do not provide an implementation for this.

    OTC_StringBuf&	operator=(OTC_StringBuf const&);
				// Do not provide an implementation for this.

    OTC_String&		string_;
    				// The string to which data is being appended.
};

class OSE_EXPORT OTC_StringBufWrapper
{
  public:

			OTC_StringBufWrapper(
			 OTC_String& theString,
			 OTC_BufferingFlag theType
			)
			  : streamBuf_(theString,theType) {}

    OTC_StringBuf*	streambuf()
    				{ return &streamBuf_; }

  private:

    OTC_StringBuf	streamBuf_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_STRNGBUF_HH */
