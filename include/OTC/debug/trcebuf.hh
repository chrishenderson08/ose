#ifndef OTC_DEBUG_TRCEBUF_HH
#define OTC_DEBUG_TRCEBUF_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/trcebuf.hh
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

#ifndef OTC_OTC_H
#include <OTC/OTC.h>
#endif

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/trcebuf.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OTC_TraceStream;

class OSE_EXPORT OTC_TraceBuf : public streambuf
    // = TITLE
    //     Streambuf which does work of indenting trace information.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_TraceBuf> does the real work of collecting the
    //     trace information and indenting it when displayed.
    //
    // = SEE ALSO
    //     <OTC_Tracer>
{
  public:

			~OTC_TraceBuf();

    // = INITIALISATION

			OTC_TraceBuf(OTC_TraceStream* theStream);
				// <theStream> should be the instance
				// of <OTC_TraceStream> this class is
				// being held by.

  public:

#if defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
    int			pubsync()
				{ return sync(); }
#endif

  protected:

#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)

    int_type		overflow(int_type c);

    int_type		underflow();

#else

    int			overflow(int c);

    int			underflow();

#endif

    int			sync();

  private:

			OTC_TraceBuf(OTC_TraceBuf const&);
				// Do not provide an implementation for this.

    OTC_TraceBuf&	operator=(OTC_TraceBuf const&);
				// Do not provide an implementation for this.

    OTC_TraceStream*	stream_;
				// Pointer to parent object.

    char		buffer_[2048];
				// Buffer for collecting output before
				// processing.

    bool		startOfLine_;
				// Flag indicating if the next character
				// will be the first character on the line.
};

class OSE_EXPORT OTC_TraceBufWrapper
{
  public:

			OTC_TraceBufWrapper(OTC_TraceStream* theStream)
			  : streamBuf_(theStream) {}

    OTC_TraceBuf*	streambuf()
    				{ return &streamBuf_; }

  private:

    OTC_TraceBuf	streamBuf_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TRCEBUF_HH */
