#ifndef OTC_DEBUG_LOGSTRM_HH
#define OTC_DEBUG_LOGSTRM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/logstrm.hh
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

#ifndef OTC_DEBUG_LOGBUF_HH
#include <OTC/debug/logbuf.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/logstrm.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_LogStream : private OTC_LogBufWrapper, public ostream
    // = TITLE
    //     Streams interface to the logger.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_LogStream> is a streams interface to the message
    //     logging system implemented by the class <OTC_Logger>. Typically,
    //     you would not need to use the <OTC_LogStream> class directly.
    //     Instead, you should call the function <OTCLIB_LOGGER()> to get
    //     access to a central instance of the <OTC_LogStream> class.
    //     The maximum size message that can be accomodated by the stream
    //     returned by <OTCLIB_LOGGER()> is <2048> characters. If this is
    //     exceeded, the message will be split over multiple lines. The
    //     priority level at which you want messages to be logged, is
    //     defined by passing the level as an argument to the
    //     <OTCLIB_LOGGER()> function.
    //
    //     Note that if your code needs to be thread safe, you should not
    //     use <OTCLIB_LOGGER()> but should create an instance of the
    //     <OTC_LogStream> class yourself.
    //     
    // = EXAMPLE
    //     An example of how you would use the <OTCLIB_LOGGER()> function is
    //     shown below.
    //     
    // = BEGIN<CODE>
    //       OTCLIB_LOGGER(OTCLIB_LOG_WARNING) << "Bad argument" << flush;
    // = END<CODE>
    //
    //     To ensure that messages are sent, the stream should be flushed
    //     explicitly. You can flush the stream by using the <flush>
    //     manipulator or by using <endl>. In either case, an end of line
    //     character will be output if necessary.
    //     
    // = SEE ALSO
    //     <OTC_LogBuf>, <OTC_Logger>
{
  public:

			~OTC_LogStream();

    // = CONSTRUCTION

			OTC_LogStream(char* theBuffer=0, size_t theSize=0);
				// Initialises the stream for formatting
				// messages for the logger. <theBuffer>
				// should be a block of memory in which the
				// class can format the messages.
				// <theSize> should be the size of the
				// buffer you have provided.

    // = LOG LEVEL

    char const*		setChannel(char const* theChannel)
				{ return streambuf()->setChannel(theChannel); }
				// Sets the logger channel. By default no
				// channel is defined. <theChannel> should be a
				// literal string, or string which will not
				// be destroyed for the life of this object.

    char const*		setTarget(char const* theChannel)
				{ return setChannel(theChannel); }
                                // Calls <setChannel>. The concept name
                                // and thus the member function name was
                                // changed. This is kept for backwards
                                // compatability.

    OTC_LogLevel	setLevel(OTC_LogLevel theLevel)
				{ return streambuf()->setLevel(theLevel); }
				// Sets the priority level at which the
				// formatted message will be sent to the
				// logger, to <theLevel>. If the priority
				// level is not set explicitly, a default
				// of <OTCLIB_LOG_DEBUG> set by the
				// constructor will be used.

  private:

			OTC_LogStream(OTC_LogStream const&);
				// Do not define an implementation for this.

    OTC_LogStream&	operator=(OTC_LogStream const&);
				// Do not define an implementation for this.
};

extern OSE_EXPORT OTC_LogStream& OTCLIB_LOGGER(OTC_LogLevel theLevel);

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_LOGSTRM_HH */
