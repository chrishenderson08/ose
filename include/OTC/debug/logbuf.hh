#ifndef OTC_DEBUG_LOGBUF_HH
#define OTC_DEBUG_LOGBUF_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/logbuf.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_DEBUG_LOGGER_HH
#include <OTC/debug/logger.hh>
#endif

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/logbuf.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_LogBuf : public streambuf
    // = TITLE
    //	   Streambuf class for use by <OTC_LogStream>.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   <OTC_LogBuf> is used to used to manage the buffer used by
    //	   <OTC_LogStream>, to format messages which are to be sent to the
    //	   logger. Messages are automatically sent to the logger at the set
    //	   priority level, when the stream using this class is flushed,
    //	   or the buffer used in this class is filled.
    //
    // = SEE ALSO
    //	   <OTC_Logger>, <OTC_LogStream>
{
  public:

			~OTC_LogBuf();

    // = INITIALISATION

			OTC_LogBuf(char* theBuffer, size_t theSize);
				// Initialises the class to manage the buffer
				// being used to format messages by
				// <OTC_LogStream>. <theBuffer> should be the
				// buffer into which messages are going to be
				// formatted. <theSize> should be the size of
				// the buffer you have provided.

    // = LOG LEVEL

    char const*		setChannel(char const* theChannel)
				{
				  char const* tmpChannel = channel_;
				  channel_ = theChannel;
				  return tmpChannel;
				}
				// Sets the logger channel. Returns the
				// previously defined channel.

    char const*		setTarget(char const* theChannel)
				{ return setChannel(theChannel); }
				// Calls <setChannel>. The concept name
				// and thus the member function name was
				// changed. This is kept for backwards
				// compatability.

    OTC_LogLevel	setLevel(OTC_LogLevel theLevel)
				{
				  OTC_LogLevel tmpLevel = level_;
				  level_ = theLevel;
				  return tmpLevel;
				}
				// Sets the priority level for messages. When
				// a message is flushed or the buffer is
				// filled, the message will be sent to the
				// logger at priority level <theLevel>.

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
				// Writes out the message in the buffer
				// to the logger, using the current
				// message priority level.

  private:

			OTC_LogBuf(OTC_LogBuf const&);
				// Do not provide an implementation for this.

    OTC_LogBuf&		operator=(OTC_LogBuf const&);
				// Do not provide an implementation for this.

    char const*		channel_;
				// Logger channel.

    OTC_LogLevel	level_;
				// Priority level used when messages
				// are sent to the logger.

    char		buffer_[1024];
				// A default buffer.
};

class OSE_EXPORT OTC_LogBufWrapper
{
  public:

			OTC_LogBufWrapper(char* theBuffer, size_t theSize)
			  : streamBuf_(theBuffer,theSize) {}

    OTC_LogBuf*		streambuf()
    				{ return &streamBuf_; }

  private:

    OTC_LogBuf		streamBuf_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_LOGBUF_HH */
