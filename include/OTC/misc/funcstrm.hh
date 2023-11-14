#ifndef OTC_MISC_FUNCSTRM_HH
#define OTC_MISC_FUNCSTRM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/funcstrm.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_MISC_FNBUF_HH
#include <OTC/misc/funcbuf.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/misc/funcstrm.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_FuncStream : private OTC_FuncBufWrapper, public ostream
    // = TITLE
    //     Streams interface which passes result to a function.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_FuncStream> is a streams class which passes
    //     the accumulated data off to a C function each time the stream
    //     is flushed or the internal buffer fills up.
    //     
    // = SEE ALSO
    //     <OTC_FuncBuf>
{
  public:

			~OTC_FuncStream();

    // = CONSTRUCTION

			OTC_FuncStream(
			 void (*theFunc)(char const*,size_t),
			 char* theBuffer=0,
			 size_t theSize=0
			);
				// Initialises the stream. <theFunc> should
				// be the function to which data accumulated
				// by the stream should be sent. <theBuffer>
				// should be a block of memory in which the
				// class can format the messages. <theSize>
				// should be the size of the buffer you have
				// provided. If <theBuffer> and <theSize>
				// are <0>, an internal buffer of size <1024>
				// bytes will be used.

  private:

			OTC_FuncStream(OTC_FuncStream const&);
				// Do not define an implementation for this.

    OTC_FuncStream&	operator=(OTC_FuncStream const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_FUNCSTRM_HH */
