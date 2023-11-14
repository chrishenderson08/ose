#ifndef OTC_TEXT_OSSTREAM_HH
#define OTC_TEXT_OSSTREAM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/osstream.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/strngbuf.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/osstream.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_OSStream : private OTC_StringBufWrapper, public ostream
    // = TITLE
    //     Stream class for formatting output onto a string.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_OSStream> class is a derived version of <ostream> for
    //     formatting output directly onto the end of an instance of the
    //     <OTC_String> class. At the same time as the stream is
    //     being used on the string, you can use all the member functions
    //     of the <OTC_String> class.
    //
    // = EXAMPLE
    // = BEGIN<CODE>
    //     OTC_String theString;
    //     OTC_OSStream theStream(theString);
    //
    //     theString += "{";
    //
    //     theStream << "text";
    //     theStream << ',';
    //     theStream << 144;
    //
    //     theString += "}";
    //
    // = NOTES
    //     The referenced string must exist for the lifetime of the stream
    //     which refers to it.
    //     
    // = SEE ALSO
    //     <OTC_StringBuf>, <OTC_String>
{
  public:

    			~OTC_OSStream();

    // = INITIALISATION

    			OTC_OSStream(
			 OTC_String& theString,
			 OTC_BufferingFlag theFlag=OTCLIB_UNBUFFERED
			);
				// <theString> should be a reference to
				// an instance of <OTC_String> to which
				// the buffer contents should be appended.
				// <theFlag> defaults to <OTCLIB_UNBUFFERED>
				// meaning that no buffering is used. That
				// is, data is immediately appended onto
				// <theString> as soon as it is output to
				// the stream. If <theFlag> is instead set to
				// <OTCLIB_BUFFERED>, a buffer is used to
				// accumulate data before it is appended to
				// the string. Because of the buffering, it
				// will be necessary to flush the stream to
				// see the results appended to <theString>.
				// Note that using the stream in unbuffered
				// mode is less efficient and should not be
				// used if large amounts of data is being
				// formatted.

  private:

			OTC_OSStream(OTC_OSStream const&);
    				// Do not define an implementation for this.

    OTC_OSStream&	operator=(OTC_OSStream const&);
    				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_OSSTREAM_HH */
