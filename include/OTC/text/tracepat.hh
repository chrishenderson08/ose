#ifndef OTC_TEXT_TRACEPAT_HH
#define OTC_TEXT_TRACEPAT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/tracepat.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/trcswtch.hh>
#include <OTC/text/extendre.hh>
#include <OTC/thread/nrmutex.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/text/tracepat.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

#if defined(OTCLIB_TRACE)
#define OTCLIB_SETTRACEPATTERN(pattern) OTC_TracePattern::set(pattern)
#define OTCLIB_TRACEPATTERN(name,string) OTC_TracePattern name(string)
#define OTCLIB_STATIC_TRACEPATTERN(name,string) \
 static OTC_TracePattern name(string)
#else
#define OTCLIB_SETTRACEPATTERN(pattern)
#define OTCLIB_TRACEPATTERN(name,string)
#define OTCLIB_STATIC_TRACEPATTERN(name,string)
#endif

class OSE_EXPORT OTC_TracePattern : public OTC_TraceSwitch
    // = TITLE
    //     Trace switch enabled through a pattern definition.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_TracePattern> is a derived version of the class
    //     <OTC_TraceSwitch>, where the switch is enabled if the string
    //     associated with the switch matches a regular expression.
    //
    //     The regular expression used to perform the match, can be defined
    //     using the environment variable <OTCLIB_TRACEPATTER>, or through
    //     program control. If set under program control, all switches will
    //     be reevaluated at that point and their status reset accordingly.
    //
    // = BEGIN<CODE>
    //     void Object::function()
    //     {
    //       static OTC_TracePattern FUNCTION("Object::function()");
    //       OTCLIB_TRACER(FUNCTION) << "some text" << endl;
    //       return 0;
    //     }
    //
    //     OTCLIB_TRACEPATTERN="Object::" program
    // = END<CODE>
    //
    //     The type of pattern excepted is that of an extended regular
    //     expression, as implemented by the <OTC_ExtendedRegex> class.
    //     
    //     Macros are also provided for creating and setting the pattern.
    //     Code for these macros will be compiled into your code, only
    //     if the preprocessor symbol <OTCLIB_TRACE> is defined. These
    //     macros are <OTCLIB_TRACEPATTERN>, <OTCLIB_STATIC_TRACEPATTERN>
    //     and <OTCLIB_SETTRACEPATTERN>. The macro <OTCLIB_STATIC_TRACEPATTERN>
    //     creates an instance of the class which has static extent.
    //
    // = BEGIN<CODE>
    //     void Object::function()
    //     {
    //       OTCLIB_STATIC_TRACEPATTERN(FUNCTION,"Object::function()");
    //       OTCLIB_TRACER(FUNCTION) << "some text" << endl;
    //       OTCLIB_SETTRACEPATTERN("Object::");
    //       OTCLIB_TRACER(FUNCTION) << "some text" << endl;
    //       return 0;
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_TraceSwitch>, <OTC_ExtendedRegex>, <OTC_Pattern>
{
  public:

			~OTC_TracePattern();

    // = INITIALISATION

			OTC_TracePattern(char const* theString);
				// <theString> will be used to match
				// against the pattern to determine if
				// this switch is enabled. <theString>
				// should be a literal string.

    // = QUERY/CHANGE

    bool		enabled() const;
				// Returns <true> if the switch is enabled,
				// otherwise returns <false>.

    char const*		string() const
				{ return string_; }
				// Returns the string associated with this
				// switch. This string is what is used to
				// match against the pattern to determine
				// if the switch is enabled.

    static void		set(char const* thePattern);
				// Resets the pattern for matches. This will
				// result in the status of all switches being
				// reevaluated.

  private:

    bool		state_;
				// The state of this switch.

    char const*		string_;
				// The string associated with this
				// switch. Matches against the pattern
				// to determine if this switch is enabled.

    static OTC_NRMutex	mutex_;
				// Lock for threads.

    static OTC_ExtendedRegex*	globRegexp_;
				// Regular expression used to match against
				// strings associated with each of the
				// switches.

    static OTC_TracePattern*	globHead_;
				// Pointer to most recently created switch.

    OTC_TracePattern*	prev_;
				// Pointer to switch before this switch in
				// the list of all switches.

    OTC_TracePattern*	next_;
				// Pointer to switch after this switch in
				// the list of all switches.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_TEXT_TRACEPAT_HH */
