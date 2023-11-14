#ifndef OTC_MISC_FUNCBUF_HH
#define OTC_MISC_FUNCBUF_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/funcbuf.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OSE/OSE.h>

#ifndef OTC_MISC_TYPES_H
#include <OTC/misc/types.h>
#endif
#ifndef OTC_MISC_BUFFLAG_HH
#include <OTC/misc/bufflag.hh>
#endif

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/misc/funcbuf.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_FuncBuf : public streambuf
    // = TITLE
    //	   Streambuf class for use by <OTC_FuncStream>.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   <OTC_FuncBuf> is used to used to manage the buffer used by
    //     <OTC_FuncStream>, to format messages which are to be sent to an
    //     arbitrary function. Accumulated data is automatically passed on
    //     when the stream using this class is flushed, or the buffer used in
    //     this class is filled.
    //     
    // = SEE ALSO
    //	   <OTC_FuncStream>
{
  public:

			~OTC_FuncBuf();

    // = INITIALISATION

			OTC_FuncBuf(
			 void (*theFunc)(char const*,size_t),
			 char* theBuffer,
			 size_t theSize
			);
				// Initialises the class to manage the buffer
				// being used to format messages by
				// <OTC_FuncStream>. <theFunc> should be the
				// function to which accumulated data is
				// passed off to when required. <theBuffer>
				// should be the buffer into which messages
				// are going to be formatted. <theSize>
				// should be the size of the buffer you have
				// provided. If <theBuffer> and <theSize>
				// are <0>, an internal buffer of size <1024>
				// bytes will be used. If <theSize> isn't
				// greater than <1024>, the internal buffer
				// will still be used.

			OTC_FuncBuf(
			 void (*theFunc)(char const*,size_t),
			 OTC_BufferingFlag theType
			);
				// Initialises the class to manage the buffer
				// being used to format messages by
				// <OTC_FuncStream>. <theFunc> should be the
				// function to which accumulated data is
				// passed off to when required. <theType> can
				// be set to <OTCLIB_BUFFERED> or
				// <OTCLIB_UNBUFFERED>. If <OTCLIB_BUFFERED>
				// is used, the internal buffer space of
				// <1024> bytes will be used. If
				// <OTCLIB_UNBUFFERED>, characters will be
				// passed through to the output one at a
				// time. Ie., the buffer will not be used at
				// all. The latter is obviously not as
				// efficient, however may be more appropriate
				// if it is important to always keep in sync
				// with the target without flushing.

  public:

#if defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
    int                 pubsync()
                                { return sync(); }
#endif

  protected:

    // These are overrides of virtual functions in the <streambuf> base
    // class.

#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)

    int_type		overflow(int_type c);

    int_type		underflow();

#else

    int			overflow(int c);

    int			underflow();

#endif

    // = STREAM FLUSHING

    int			sync();
				// Passes the contents of the buffer
				// to the supplied function.

  private:

			OTC_FuncBuf(OTC_FuncBuf const&);
				// Do not provide an implementation for this.

    OTC_FuncBuf&	operator=(OTC_FuncBuf const&);
				// Do not provide an implementation for this.

    void		(*func_)(char const*,size_t);
				// Function to which buffer contents are
				// passed.

    OTC_BufferingFlag	bufType_;
				// Whether buffering is enabled or not.

    char		buffer_[1024];
				// A default buffer.
};

class OSE_EXPORT OTC_FuncBufWrapper
{
  public:

			OTC_FuncBufWrapper(
			 void (*theFunc)(char const*,size_t),
			 char* theBuffer,
			 size_t theSize
			)
			  : streamBuf_(theFunc,theBuffer,theSize) {}

    OTC_FuncBuf*	streambuf()
    				{ return &streamBuf_; }

  private:

    OTC_FuncBuf		streamBuf_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_FUNCBUF_HH */
