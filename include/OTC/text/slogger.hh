#ifndef OTC_TEXT_SLOGGER_HH
#define OTC_TEXT_SLOGGER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/slogger.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/logger.hh>
#include <OTC/text/string.hh>
#include <OTC/text/regexp.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/slogger.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_SLogger : public OTC_Logger
    // = TITLE
    //     Logger which accumulates messages into a string.
    //
    // = DESCRIPTION
    //     The <OTC_SLogger> class looks for messages logged to a particular
    //     channel of interest and then accumulates the messages into a
    //     string. The accumulated messages can then be output in some way,
    //     or scanned for information of interest. Any accumulated messages
    //     can be discarded at any time. The purpose of this class is to
    //     allow one to accumulate any log messages which may occur when in a
    //     particular section of code and then give those specific messages
    //     back to the user as an indication as to what possibly went wrong
    //     with the code. If the channel used when logging the messages
    //     starts with a character other than a letter or a number, the
    //     messages will not be seen on standard error or in the log file,
    //     making it ideal for internally capturing such messages in a
    //     program.
    // 
    // = NOTES
    //     This class used to accept a regular expression. This feature
    //     was removed and only an exact channel name can now be used.
    //
    // = EXAMPLE
    // = BEGIN<CODE>
    //     OTC_SLogger tmpLogger("@HIDDEN");
    //
    //     OTC_LogStream tmpLogStream;
    //     tmpLogStream.setChannel("@HIDDEN");
    //     tmpLogStream << "message" << endl;
    //
    //     cout << tmpLogger.messages() << endl;
    //     tmpLogger.clearLog();
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Logger>
{
  public:

			~OTC_SLogger();

    // = INITIALISATION

			OTC_SLogger(char const* theChannel);
				// Creates a logger which accumulates
				// messages which have been sent to
				// <theChannel>. Note that it isn't valid
				// to try and capture messages sent to
				// the empty channel.

    // = MESSAGES

    OTC_String const&	channel() const
      				{ return channel_; }
    				// Returns the channel of interest.

    OTC_String const&	messages() const
				{ return messages_; }
				// Returns the accumulated messages. There
				// will only be a newline character on the
				// end of the message if one was part of
				// the end of the message. Individual
				// messages will be separated by a newline
				// character when they are appended as they
				// arrive.

    void		clearLog()
				{ messages_.truncate(); }
				// Deletes all currently accumulated
				// messages.

  protected:

    void		log(
			 char const* theChannel,
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 u_int theLength
			);
				// Adds <theMessage> of length <theLength> to
				// those which have already been accumulated
				// if the pattern matches <theChannel>. Note
				// that <theLevel> is ignored.

  private:

			OTC_SLogger(OTC_SLogger const&);
				// Do not define an implementation for this.

    OTC_SLogger&	operator=(OTC_SLogger const&);
				// Do not define an implementation for this.

    OTC_String		channel_;
				// The channel of interest.

    OTC_String		messages_;
				// The accumulated messages.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_SLOGGER_HH */
