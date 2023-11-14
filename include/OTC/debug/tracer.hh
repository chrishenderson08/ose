#ifndef OTC_DEBUG_TRACER_HH
#define OTC_DEBUG_TRACER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/tracer.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_MISC_FUNCSTRM_HH
#include <OTC/misc/funcstrm.hh>
#endif
#ifndef OTC_DEBUG_TRCESTRM_HH
#include <OTC/debug/trcestrm.hh>
#endif
#ifndef OTC_THREAD_MUTEX_HH
#include <OTC/thread/mutex.hh>
#endif

#include <OTC/stdlib/iostream.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/tracer.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

#ifdef OTCLIB_TRACE
#define OTCLIB_MARKBLOCK(lvl,function) \
 OTC_Tracer otclib_tracer(function,__FILE__,__LINE__,int(lvl)); \
 if (&otclib_tracer){;}
#define OTCLIB_DOTRACE(function) OTCLIB_MARKBLOCK(1,function)
#define OTCLIB_TRACER(lvl) \
  if (!OTC_Tracer::enabled(int(lvl))) ; else otclib_tracer()
#else
#define OTCLIB_MARKBLOCK(lvl,function)
#define OTCLIB_DOTRACE(function)
#define OTCLIB_TRACER(lvl) \
 if (1) ; else cerr
#endif

class OTC_LogStream;

class OSE_EXPORT OTC_Tracer
    // = TITLE
    //     Class for tracing execution path through a function.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     Used to trace execution path through a function, by printing out
    //     messages when an instance of the class is created, and
    //     automatically, when the class is destroyed on function exit.
    //
    //     An example of how the class is used.
    //     
    // = BEGIN<CODE>
    //     main()
    //     {
    //       OTC_Tracer tracer("main()");
    //       tracer() << "Some info." << endl;
    //     }
    // = END<CODE>
    //
    //     This would produce the following output.
    //
    // = BEGIN<INDENT>
    // = BEGIN<NOFILL>
    //     @enter - main()
    //     Some info.
    //     @exit - main()
    // = END<NOFILL>
    // = END<INDENT>
    //
    //     To encourage you to leave debugging statements in your code, a
    //     number of macros have been provided, that achieve the above
    //     result, but that can be conditionally compiled into your code,
    //     only when required. These are the <OTCLIB_MARKBLOCK> and
    //     <OTCLIB_TRACER> macros and are used in the following manner.
    //     
    // = BEGIN<CODE>
    //     main()
    //     {
    //       OTCLIB_MARKBLOCK(1,"main()");
    //       OTCLIB_TRACER(1) << "Hello World" << endl;
    //     }
    // = END<CODE>
    //
    //     It is not necessary to use the <OTCLIB_MARKBLOCK> macro in
    //     a block to be able to use the <OTCLIB_TRACER> macro. The
    //     <OTCLIB_TRACER> can be used in a block by itself. If you do
    //     use <OTCLIB_TRACER> without <OTCLIB_MARKBLOCK>, it will no
    //     however be safe if used in a multithreaded application.
    //
    //     To compile the debugging statements into your code, the
    //     preprocessor symbol <OTCLIB_TRACE> must be defined, ie.,
    //     <-DOTCLIB_TRACE> must be supplied as an argument to the compiler.
    //     
    //     The argument to the <OTCLIB_TRACER> macro represents a debug
    //     level. If the debug level set in the <OTC_Tracer> class is greater
    //     than the value of the argument, and the argument is positive,
    //     the line will be run. The <OTCLIB_MARKBLOCK> macro takes
    //     two arguments. The first argument is a trace level. The second
    //     argument is the string which will be passed to the construtor of
    //     the <OTC_Tracer> class. A level of <0> indicates that the
    //     information should always be displayed. Higher values should be
    //     used for successive levels of verbosity.
    //     
    //     The initial debug level use to to determine what is displayed will
    //     be taken from the environment variable <OTCLIB_TRACELEVEL> or will
    //     be set to <0> if this isn't defined or the value in the variable
    //     was negative, ie., by default, no trace output is displayed. If
    //     you use this feature a lot, ie., you are a code developer, you may
    //     wish to permanently set this environment variable to <1> or
    //     higher.
    //     
    //     By default, the output from the <OTC_Tracer> class is sent to
    //     <stderr> via the <clog> stream. You can divert the trace output to
    //     a file by setting the environment variable <OTCLIB_TRACEFILE> to
    //     be the name of the file. If the file cannot be opened, the output
    //     will still be sent to <stderr>.
    //     
    //     If you are using the <OTCLIB_TRACER> and <OTCLIB_MARKBLOCK> macros,
    //     you can enable the generation of additional trace information
    //     containing file and line information. This is enabled by setting
    //     the environment variable <OTCLIB_TRACEINFO>. The additional
    //     information is output, just before the <@enter> line for a
    //     function. The format of the additional information will be:
    //     
    // = BEGIN<CODE>
    //     @location - "file.cc", line 42
    // = END<CODE>
    //     
    //
    // = NOTES
    //     The <OTC_Tracer> class and the <OTCLIB_TRACER> and
    //     <OTCLIB_MARKBLOCK> macros can be used in a multithreaded
    //     application with a few caveats. The first is that the
    //     <OTCLIB_MARKBLOCK> macro should always be used when
    //     <OTCLIB_TRACER> is being used. The second is that indenting
    //     information will not be correct. You can turn off indenting by
    //     setting the environment variable <OTCLIB_NOLOGINDENT>. The third
    //     is that lines of output from different threads can be intermixed.
    //     To identify which thread is producing what, you should divert
    //     trace output to the logger by setting the environment variable
    //     <OTCLIB_LOGTRACE> and enable long message prefixes by setting the
    //     environment variable <OTCLIB_LOGLONGFORMAT> so that the thread id
    //     is visible. Finally, the feature whereby instances of the
    //     <OTC_Tracer> class are chained together so as to be able to dump
    //     out a rudimentary stack trace, is disabled when threads are
    //     available.
    //     
    // = SEE ALSO
    //     <ostream>
{
  public:

    // = CONSTRUCTION

			OTC_Tracer(
			 char const* thePrototype,
			 char const* theFile,
			 u_int theLine,
			 int theEnable=0
			);
				// <thePrototype> should be a string
				// describing the function and its arguments.
				// This string will be printed out with a
				// message indicating that the function has
				// been entered. <theFile> and <theLine>
				// should identify the file and line at which
				// the <OTC_Tracer> class was created. These
				// would normally be passed automatically,
				// by the <OTCLIB_MARKBLOCK> macro, the symbols
				// <__FILE__> and <__LINE__>. When this
				// constructor is used, the message indicating
				// entry and exit of the function is only
				// displayed if enabled. To enable output,
				// <theEnable> should be <0> or a positive
				// value. To disable output, <theEnable>
				// should be negative.

			OTC_Tracer(char const* thePrototype);
				// <thePrototype> should be a string
				// describing the function and its arguments.
				// This string will be printed out with a
				// message indicating that the function has
				// been entered.

    // = DESTRUCTION

			~OTC_Tracer();
				// Prints out a message to say that the
				// function is being exited.

    // = STREAM ACCESS

    ostream&		operator()()
				{ return stream_; }
				// Returns a stream for this tracer object.
				// When this stream is flushed, data sent to
				// central trace stream.

    // = FUNCTION NAME

    char const*		prototype() const
				{ return prototype_; }
				// Returns the prototype.

    // = TRACE STATE

    static bool		enabled(int theLevel);
				// Returns <true> if tracing is enabled for
				// <theLevel> given the state of <OTC_Tracer>
				// as set by functions below.

    // = GLOBAL TRACE
    //     Enabling of a global trace can be done by setting the environment
    //     variable <OTCLIB_GLOBALTRACE> as well as through the following
    //     functions.

    static bool		globalTrace();
				// Returns <true> if a global tracing is
				// enabled, regardless of other settings.

    static void		enableGlobalTrace();
				// Enables a global trace.

    static void		disableGlobalTrace();
				// Disables a global trace.

    // = TRACE LEVEL
    //     A global trace will override the effect of these settings and will
    //     result in everything being displayed.

    static int		level();
				// Returns the debug level.

    static void		setLevel(int theLevel);
				// Set the debug level.

    // = TRACE INDENT
    //    Trace indenting is by default turned on. You can turn identing
    //    off by setting the environment variable <OTCLIB_NOTRACEINDENT>.

    static bool		traceIndent();
				// Returns <true> if trace indenting is
				// enabled.

    static void		enableTraceIndent();
				// Enables trace indenting.

    static void		disableTraceIndent();
				// Disables trace indenting.

    // = STACK TRACE

    static OTC_Tracer const*	last();
				// Returns a pointer to the most recently
				// created instance of this class, or <0> if
				// none are currently in existence.

    OTC_Tracer const*	prev() const
				{ return prev_; }
				// Returns a pointer to the instance
				// of this class created prior to this
				// instance or <0> if there wasn't one.

    // = STAND ALONE TRACE

    friend OSE_EXPORT ostream&	otclib_tracer();
				// Allows you to do away with having
				// <OTCLIB_MARKBLOCK> macro in a function.

    static void		notify(char const* theBuffer, size_t theSize);
				// Displays the contents of <theBuffer>.
				// <theSize> is the length of data in
				// <theBuffer>.

    // = STREAMS

    static ostream*	stream();
				// Returns the stream to which trace output
				// is currently being directed. If the
				// trace facility hasn't yet been initialised
				// this will return <0>.

    static void		setStream(ostream* theStream);
				// Set the trace facility to use <theStream>
				// for output. If the trace is being diverted
				// to the logger, this has no affect until
				// such that diversion of trace output to
				// the logger is disabled.

    static bool		logTrace();
				// Returns <true> if trace output is being
				// diverted to the logger.

    static void		enableLogTrace();
				// Enables diversion of trace output to the
				// logger. Sending trace to logger can also
				// be enabled by setting the environment
				// variable <OTCLIB_LOGTRACE>.

    static void		disableLogTrace();
				// Disables diversion of trace output to
				// the logger.

    static void		setLogChannel(char const* theChannel);
				// Specifies the log channel if trace is
				// being sent to the logger. If this is
				// not explicitly set, defaults to what
				// is defined by the envrionment variable
				// <OTCLIB_TRACECHANNEL> or if that isn't
				// set the default empty channel. If
				// <theChannel> is <0>, log channel is set
				// back to the default empty channel, no
				// matter what it had already been set to.

  private:

			OTC_Tracer(OTC_Tracer const&);
				// Do not define an implementation for this.

    OTC_Tracer&		operator=(OTC_Tracer const&);
				// Do not define an implementation for this.

    static void		initialise();
				// Reads values of various environment
				// variables to set up class.

    char const*		prototype_;
				// The string describing the function and its
				// arguments.

    bool		enable_;
				// Set to <true> if entry and exit tags
				// should be displayed.

    OTC_FuncStream	stream_;
				// Stream class for this scope.

    OTC_Tracer*		prev_;
				// Pointer to instance of this class
				// created prior to this instance.

    static OTC_Mutex	mutex_;
				// Lock for threads. Use recursive
				// lock as streambuf associated with
				// trace stream can callback to
				// <OTC_Tracer> from inside of the
				// locks held by the constructor and
				// destructor.

    static int		globInit_;
				// Indicates if static data has been
				// initialised. Has value <0> if not
				// initialised.

    static bool		globTraceAll_;
				// Indicates whether all tracing is turned
				// on regardless of other settings.

    static int		globLevel_;
				// The debug level. Used by <OTCLIB_TRACER>
				// macro to determine if line should be
				// executed or not.

    static bool		globInfo_;
				// Indicates whether additional line and
				// file information should be output.

    static bool		globIndent_;
				// Indicates if trace indenting is to occur.

    static bool		globLogTrace_;
				// Set to <true> if trace is being diverted
				// to the logger.

    static ostream*	globStream_;
				// Will stream onto a file, or <clog> if
				// no output to a file requested.

    static char*	globLogChannel_;
				// Log channel to use trace is going to
				// the logger.

    static OTC_LogStream*	globLogStream_;
				// Instance of log stream if trace output
				// is to be sent there.

    static OTC_TraceStream*	globTraceStream_;
				// Indenting stream class, everything goes
				// through this.

    static OTC_FuncStream*	globAnonymousStream_;
				// Stream used to collect data when the
				// <OTCLIB_MARKBLOCK> macro isn't used.

    static OTC_Tracer*	globLast_;
				// Pointer to last instance of this class
				// created.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TRACER_HH */
