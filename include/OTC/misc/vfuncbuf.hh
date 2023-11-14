#ifndef OTC_MISC_VFUNCBUF_HH
#define OTC_MISC_VFUNCBUF_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/vfuncbuf.hh
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
#pragma interface "OTC/misc/vfuncbuf.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_VFuncBuf : public streambuf
    // = TITLE
    //	   Streambuf class for use by <OTC_FuncStream>.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   <OTC_VFuncBuf> is used to used to manage the buffer used by
    //     <OTC_FuncStream>, to format messages which are to be sent to an
    //     arbitrary function. Accumulated data is automatically passed on
    //     when the stream using this class is flushed, or the buffer used in
    //     this class is filled.
    //     
    // = SEE ALSO
    //	   <OTC_FuncStream>
{
  public:

			~OTC_VFuncBuf();

    // = INITIALISATION

			OTC_VFuncBuf(char* theBuffer, size_t theSize);
				// <theBuffer> should be the buffer into
				// which messages are going to be formatted.
				// <theSize> should be the size of the buffer
				// you have provided. If <theBuffer> and
				// <theSize> are <0>, an internal buffer of
				// size <1024> bytes will be used. If
				// <theSize> isn't greater than <1024>, the
				// internal buffer will still be used.

			OTC_VFuncBuf(OTC_BufferingFlag theType);
				// <theType> can be set to <OTCLIB_BUFFERED>
				// or <OTCLIB_UNBUFFERED>. If <OTCLIB_BUFFERED>
				// is used, the internal buffer space of <1024>
				// bytes will be used. If <OTCLIB_UNBUFFERED>,
				// characters will be passed through to the
				// output one at a time. Ie., the buffer will
				// not be used at all. The latter is obviously
				// not as efficient, however may be more
				// appropriate if it is important to always
				// keep in sync with the target without
				// flushing.

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

    // = CALLBACK

    virtual void	handle(char const* theBuffer, size_t theLength) = 0;
				// Must be overridden in a derived class.
				// Is called when the streambuf is flushed
				// or the internal buffer fills up. The
				// derived class should do what ever it
				// needs to with <theBuffer>, where the size
				// of the buffer is given by <theLength>.
				// The derived class should not keep a
				// pointer to the buffer beyond the call.

    // = STREAM FLUSHING

    int			sync();
				// Passes the contents of the buffer
				// to the supplied function.

  private:

			OTC_VFuncBuf(OTC_VFuncBuf const&);
				// Do not provide an implementation for this.

    OTC_VFuncBuf&	operator=(OTC_VFuncBuf const&);
				// Do not provide an implementation for this.

    OTC_BufferingFlag	bufType_;
				// Whether buffering is enabled or not.

    char		buffer_[1024];
				// A default buffer.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MISC_VFUNCBUF_HH */
