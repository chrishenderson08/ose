#ifndef OTC_DEBUG_TRACETAG_HH
#define OTC_DEBUG_TRACETAG_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/tracetag.hh
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

#ifndef OTC_MISC_MACROS_H
#include <OTC/misc/macros.h>
#endif
#ifndef OTC_DEBUG_TRCSWTCH_HH
#include <OTC/debug/trcswtch.hh>
#endif
#ifndef OTC_DEBUG_TAGINFO_HH
#include <OTC/debug/taginfo.hh>
#endif
#ifndef OTC_THREAD_NRMUTEX_HH
#include <OTC/thread/nrmutex.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/tracetag.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

#if defined(OTCLIB_TRACE)
#define OTCLIB_SETTRACETAG(name,value) OTC_TraceTag::set(#name,value)
#define OTCLIB_TRACETAG(name) OTC_TraceTag name(#name)
#define OTCLIB_STATIC_TRACETAG(name) static OTC_TraceTag name(#name)
#else
#define OTCLIB_SETTRACETAG(name,value)
#define OTCLIB_TRACETAG(name)
#define OTCLIB_STATIC_TRACETAG(name)
#endif

class OSE_EXPORT OTC_TraceTag : public OTC_TraceSwitch
    // = TITLE
    //     A named trace switch.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_TraceTag> is a named version of a trace switch. The
    //     name allows the status of the switch to be set through either an
    //     environment variable, or program control. In addition, a trace
    //     level may be associated with the named switch, when used in
    //     conjunction with the comparison and logic operators, the trace
    //     level allows expressions to be used to determine if trace output
    //     should be generated for a particular statement. For example:
    //
    // = BEGIN<CODE>
    //     main()
    //     {
    //       OTC_TraceSwitch LIBRARY("LIBRARY");
    //       OTC_TraceSwitch PROGRAM("PROGRAM");
    //       OTCLIB_TRACER(LIBRARY || PROGRAM > 4) << "some text" << endl;
    //       return 0;
    //     }
    // = END<CODE>
    //
    //     The status of a named switch can be set using the environment
    //     variable <OTCLIB_TRACETAGS>. When set, the environment variable
    //     should contain a list of the named switches which should be
    //     enabled. You may optionally include an assignment in the list
    //     to set the trace level of a switch to a defined value. If you
    //     do not set the trace level of a switch, it will default to <0>.
    //     If multiple trace tag names are listed they should be separated
    //     by a single space.
    //
    // = BEGIN<CODE>
    //     OTCLIB_TRACETAGS="PROGRAM=5 LIBRARY" program
    // = END<CODE>
    //
    //     Macros are provided to allow you to create named switches. Code
    //     for these macros will be compiled into your code, only
    //     if the preprocessor symbol <OTCLIB_TRACE> is defined. These
    //     macros are <OTCLIB_TRACETAG>, <OTCLIB_STATIC_TRACETAG>
    //     and <OTCLIB_SETRACETAG>. The macro <OTCLIB_STATIC_TRACEPATTERN>
    //     creates an instance of the class which has static extent.
    //     <OTCLIB_SETTRACETAG> is provided to allow you to set the trace
    //     level of a switch in your program. Setting the trace level of a
    //     switch, will result in all switches of that name in your program
    //     being updated.
    //     
    // = BEGIN<CODE>
    //     OTCLIB_STATIC_TRACETAG(PROGRAM);
    //
    //     main()
    //     {
    //       OTCLIB_TRACETAG(MAIN);
    //       OTCLIB_TRACER(PROGRAM) << "some text" << endl;
    //       OTCLIB_SETTRACETAG(MAIN,0); // Make sure MAIN is enabled.
    //       OTCLIB_TRACER(MAIN || PROGRAM) << "some text" << endl;
    //     }
    // = END<CODE>
    //     
    // = SEE ALSO
    //     <OTC_TraceSwitch>, <OTC_TagInfo>
{
  public:

        		~OTC_TraceTag();

    // = INITIALISATION

			OTC_TraceTag(char const* theName);
				// Creates a named switch with tag <theName>.

    // = QUERY/CHANGE

    bool		enabled() const;
				// Returns <true> if the switch is enabled.

    int			level() const
				{ return tagInfo_->level(); }
				// Returns the trace level of this named
				// switch.

    char const*		name() const
				{ return tagInfo_->name(); }
				// Returns the name of this switch.

    static int		set(char const* theName, int theLevel=0);
				// Sets the trace level of all named switches
				// with the tag <theName> to <theLevel>.

    static OTC_TagInfo*	switches()
				{ return globTagInfo_; }
				// Returns a pointer to a list of objects for
				// each of the named switches. Browsing the
				// objects returned by this function is
				// not going to be safe within a threaded
				// application.

    // = COMPARISON OPERATORS

    OTC_TraceSwitch	operator==(int theLevel) const;
				// Returns an enabled switch if this switch
				// is enabled, and the trace level for this
				// switch is the same as <theLevel>.

    OTC_TraceSwitch	operator!=(int theLevel) const;
				// Returns an enabled switch if this switch
				// is enabled, and the trace level for this
				// switch is not the same as <theLevel>.

    OTC_TraceSwitch	operator<(int theLevel) const;
				// Returns an enabled switch if this switch
				// is enabled, and the trace level for this
				// switch is less than <theLevel>.

    OTC_TraceSwitch	operator<=(int theLevel) const;
				// Returns an enabled switch if this switch
				// is enabled, and the trace level for this
				// switch is less than or equal to
				// <theLevel>.

    OTC_TraceSwitch	operator>(int theLevel) const;
				// Returns an enabled switch if this switch
				// is enabled, and the trace level for this
				// switch is is greater than <theLevel>.

    OTC_TraceSwitch	operator>=(int theLevel) const;
				// Returns an enabled switch if this switch
				// is enabled, and the trace level for this
				// switch is is greater than or equal to
				// <theLevel>.

  private:

			OTC_TraceTag(OTC_TraceTag const&);
				// Do not provide an implementation for this.

    OTC_TraceTag&	operator=(OTC_TraceTag const&);
				// Do not provide an implementation for this.

    OTC_TagInfo*	tagInfo_;
				// Pointer to status information for this
				// named tag.

    static OTC_NRMutex	mutex_;
				// Lock for threads. Use non recursive
				// mutex as acquiring of lock within
				// scope of lock isn't going to happen.

    static char const*	globTagVariable_;
				// Cached pointer to string holding
				// details of <OTCLIB_TRACETAGS>
				// environment variable.

    static OTC_TagInfo*	globTagInfo_;
				// Pointer to chain of objects holding
				// status information for each trace tag.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TRACETAG_HH */
