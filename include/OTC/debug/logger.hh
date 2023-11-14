#ifndef OTC_DEBUG_LOGGER_HH
#define OTC_DEBUG_LOGGER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/logger.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_OTC_H
#include <OTC/OTC.h>
#endif
#ifndef OTC_THREAD_MUTEX_HH
#include <OTC/thread/mutex.hh>
#endif

#include <stdio.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/logger.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_LogLevel
    // = TITLE
    //     Priority levels for messages displayed using the logger.
{
  OTCLIB_LOG_EMERGENCY = 0,
  OTCLIB_LOG_ALERT = 1,
  OTCLIB_LOG_CRITICAL = 2,
  OTCLIB_LOG_ERROR = 3,
  OTCLIB_LOG_WARNING = 4,
  OTCLIB_LOG_NOTICE = 5,
  OTCLIB_LOG_INFO = 6,
  OTCLIB_LOG_DEBUG = 7
};

extern OSE_EXPORT char const* const OTCLIB_LOGLEVELMESSAGES[];

class OSE_EXPORT OTC_Logger
    // = TITLE
    //     Base class for additional message loggers.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     <OTC_Logger> is the base class for additional message loggers. The
    //     class is also the entry point for displaying a message using the
    //     logger.
    //     
    //     If you wish to log messages to destinations, in addition to those
    //     which may already be active, you should create a derived version
    //     of <OTC_Logger> which defines <log()> to send the message to the
    //     appropriate destination. When you create an instance of the
    //     derived class, it will automatically be linked in to the list of
    //     active loggers; when the instance is destroyed, it will
    //     automatically unlink itself from the list of active loggers.
    //
    //     When the <OTC_Logger> class is compiled, if the symbol <NDEBUG> is
    //     not defined, in addition to broadcasting messages to user provided
    //     loggers, the logger will display messages on standard error using
    //     file descriptor <2>. To turn off output to standard error, you can
    //     define the environment variable <OTCLIB_NOLOGSTDERR>. If the OSE
    //     C++ libraries have been installed in the standard manner, the
    //     symbol <NDEBUG> is not defined when the <dbg>, <prf> and <std>
    //     variants of the libraries are compiled. Therefore, if you link
    //     your programs with these variants of the library, messages by
    //     default will be displayed on standard error.
    //     
    //     If the <OTC_Logger> class is compiled with <NDEBUG> defined, the
    //     logger will <[NOT]> log messages to standard error. To turn on
    //     output to standard error, you can define the environment variable
    //     <OTCLIB_LOGSTDERR>. If the OSE C++ libraries are installed in
    //     the standard manner, the symbol <NDEBUG> is defined when the
    //     <opt> variant of the library is compiled.
    //     
    //     Regardless of whether <NDEBUG> was defined when the <OTC_Logger>
    //     class was compiled, you can enable saving of messages to a file,
    //     by defining the environment variable <OTCLIB_LOGFILE> to be the
    //     name of a file in which to save them. By default, this file will
    //     be truncated the first time it is opened. If you prefer messages
    //     to be appended to the file, you should define the environment
    //     variable <OTCLIB_APPENDLOGFILE>.
    //     
    //     An example of the messages displayed by the standard logger are:
    //
    // = BEGIN<CODE>
    //     EMERGENCY: EMERGENCY level
    //     ALERT: ALERT level
    //     CRITICAL: CRITICAL level
    //     ERROR: ERROR level
    //     WARNING: WARNING level
    //     NOTICE: NOTICE level
    //     INFO: INFO level
    //     DEBUG: DEBUG level
    // = END<CODE>
    //
    //     The text after the colon is the message. The text before
    //     the colon corresponds to the priority level at which the message
    //     was logged. Display of a long format message containing a time
    //     stamp and the process ID of the program can be enabled by setting
    //     the environment variable <OTCLIB_LOGLONGFORMAT>. If multiple
    //     programs are saving messages to the same log file you should
    //     enable this option.
    //     
    // = NOTES
    //     Any logger you write should avoid doing anything that would
    //     require memory to be allocated. This is necessary, as it could be
    //     that the message is to notify you that memory has been exhausted.
    //     Also, your logger should avoid using <form()> as the message
    //     could have been constructed originally using the function. Using
    //     <form()> again, without first copying the message, would result in
    //     the original message being lost.
{
  public:

    // = DESTRUCTION

    virtual		~OTC_Logger();
				// Removes this logger from the list of
				// all active logger modules.

    // = BROADCAST
    //     For each of the following functions, <theLevel> can have
    //     the values:
    //
    // = BEGIN<CODE>
    //       OTCLIB_LOG_EMERGENCY
    //       OTCLIB_LOG_ALERT
    //       OTCLIB_LOG_CRITICAL
    //       OTCLIB_LOG_ERROR
    //       OTCLIB_LOG_WARNING
    //       OTCLIB_LOG_NOTICE
    //       OTCLIB_LOG_INFO
    //       OTCLIB_LOG_DEBUG
    // = END<CODE>
    //
    //     These are analogous to the values accepted by <syslog()>.

    static void		notify(
			 char const* theChannel,
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 int theLength=-1
			);
				// If required, this logs <theMessage> to
				// standard error and a file and then
				// broadcasts <theMessage> to any active
				// loggers. The value <theLevel> is the
				// priority level of the message. If
				// <theLength> is negative, the default, it
				// indicates that <theMessage> is a null
				// terminated string. Otherwise, <theLength>
				// indicates the length of the string.
				// <theChannel> is a string used, to target
				// a specific user defined logger. If the
				// first character of <theChannel> is not a
				// number or a letter, the message will not
				// be output to standard error or saved in a
				// log file, only being passed onto user
				// defined loggers. Thus using a channel
				// name that starts with a punctuation
				// character, space or unprintable character
				// will hide the logged message except to
				// derived logger classes. Channel
				// information is not output in messages to
				// standard error or the log file.

    static void		notify(
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 int theLength=-1
			)
				{ notify(0,theLevel,theMessage,theLength); }
				// If required, this logs <theMessage> to
				// standard error and a file and then
				// broadcasts <theMessage> to any active
				// loggers. The value <theLevel> is the
				// priority level of the message. If
				// <theLength> is negative, the default, it
				// indicates that <theMessage> is a null
				// terminated string. Otherwise, <theLength>
				// indicates the length of the string.

    // = STDERR OUTPUT
    //     Note that enabling and disabling of log output has no effect
    //     when an alternate file descriptor is provided for log messages.

    static bool		stderrOutputEnabled();
				// Returns <true> if output of log messages
				// to <stderr> is enabled.

    static void		enableStderrOutput();
				// Enables output of log messages to
				// <stderr>.

    static void		disableStderrOutput();
				// Disables output of log messages to
				// <stderr>.

    // = LOG FORMAT

    static bool		longFormatEnabled();
				// Returns <true> if output of long format
				// log message prefix is enabled. By
				// default it isn't, unless overridden
				// using the environment variable called
				// <OTCLIB_LOGLONGFORMAT>. The long format
				// log message prefix contains date/time,
				// process ID and thread ID.

    static void		enableLongFormat();
				// Enables output of long format log message
				// prefix.

    static void		disableLongFormat();
				// Disables output of long format log message
				// prefix.

    // = LOG FILE
    //     Note that log file names should always be absolute pathnames.
    //     This is because the file is opened each time a message needs
    //     to be written. If the file name was not absolute and the
    //     working directory of the program was changed, the file would
    //     be written to the new working directory.
    //
    //     A number of special tags may be embedded into the name of a file.
    //     These are <%p>, <%h>, <%d>, <%m>, <%y> and <%Y>. The tag <%p> will
    //     be replaced with the process ID, and <%h> with the hostname of the
    //     machine. The remainder are used for embedding the date into the file
    //     name, with <%Y> being four digit year. To embed a <%> character in
    //     the filename, you should use <%%>.

    static char const*	logFile();
				// If a log file is currently being used,
				// the name of the file is returned.
				// Returns <0> if no log file specified.

    static void		setLogFile(char const* theFile);
				// Sets the name of the log file. All
				// log messages will now go to this
				// file until the process terminates
				// or the file is changed.

  protected:

    // = CONSTRUCTION

			OTC_Logger();
				// Inserts this logger in to the list of
				// active loggers.

    // = LOGGING

    virtual void	log(
			 char const* theChannel,
			 OTC_LogLevel theLevel,
			 char const* theMessage,
			 u_int theLength
			) = 0;
				// Must be redefined in a derived class by
				// your logger. <theChannel> can be used
				// to identify a specific user defined logger.
				// <theLevel> is the priority of the logged
				// messages and <theMessage> the actual
				// message. <theLength> is the length of the 
				// message. Your derived class is free to use
				// the values of the arguments as it wishes,
				// however, it should not assume
				// responsibility for deleting either of the
				// strings.

  private:

			OTC_Logger(OTC_Logger const&);
				// Do not define an implementation for this.

    OTC_Logger&		operator=(OTC_Logger const&);
				// Do not define an implementation for this.

    void		link();
				// Links this logger into the chain of
				// loggers.

    void		unlink();
				// Unlinks this logger from the chain of
				// loggers.

    static void		initialise();
				// Initialises logger first time around.

    static OTC_Mutex	mutex_;
				// Lock for threads. Use a recursive mutex as
				// call for additional loggers to log message
				// may result in a new message being logged.

    static bool		initialised_;
				// Set to <true> if the class has been
				// initialised.

    static bool		useStderr_;
				// Set to <true> if logging is to be sent to
				// standard error.

    static char const*	logFile_;
				// If logging is also going to a file
				// then this is the name of the file,
				// otherwise it is <0>.

    static char*	lastLogFile_;
				// If logging is going to a file, this will
				// be the expansion of the requested log
				// file name after '%' escape characters are
				// processed.

    static bool		appendLogFile_;
				// Set to <true> if log entries should be
				// appended to existing log file.

    static OTC_Logger*	head_;
				// First logger in the chain.

    static FILE*	stream_;
				// The stream used when sending output
				// to <stderr>. By default this will be
				// <stderr>.

    static bool		longFormat_;
				// Set to <true> if long format error message
				// should be used.

    static char		hostName_[];
				// Name of the host.

    OTC_Logger*		next_;
				// Logger following this one in the chain.

    OTC_Logger*		prev_;
				// Logger preceeding this one in the chain.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_LOGGER_HH */
