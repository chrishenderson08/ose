#ifndef OTC_DEBUG_TRCSWTCH_HH
#define OTC_DEBUG_TRCSWTCH_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/trcswtch.hh
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

#ifndef OTC_OTC_H
#include <OTC/OTC.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/trcswtch.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

#if defined(OTCLIB_TRACE)
#define OTCLIB_ENABLETRACESWITCH(name) name = true
#define OTCLIB_DISABLETRACESWITCH(name) name = false
#define OTCLIB_SETTRACESWITCH(name,expr) name = expr
#define OTCLIB_TRACESWITCH(name,expr) OTC_TraceSwitch name = expr
#else
#define OTCLIB_ENABLETRACESWITCH(name)
#define OTCLIB_DISABLETRACESWITCH(name)
#define OTCLIB_SETTRACESWITCH(name,expr)
#define OTCLIB_TRACESWITCH(name,expr)
#endif

class OSE_EXPORT OTC_TraceSwitch
    // = TITLE
    //     Switch class for customising trace output.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_TraceSwitch> class provides a basis for creating ways of
    //     being selective about what trace information is displayed. By
    //     overriding the <enabled()> member function in a derived class,
    //     trace output can be dynamically enabled based on some aspect of
    //     the operating environment. If the <enabled()> function returns
    //     <true>, trace output is enabled for the statement using the
    //     switch. The <OTC_TraceSwitch> class and its derived classes should
    //     be used in conjunction with the <OTCLIB_TRACER> macro. 
    //     
    // = BEGIN<CODE>
    //     OTC_TraceSwitch LIBRARY(true);
    //     OTCLIB_TRACER(LIBRARY) << "some text" << endl;
    // = END<CODE>
    //
    //     The macro <OTCLIB_TRACESWITCH> can be used to create an instance
    //     of the class. When the macro is used, the code will be
    //     compiled into your code, only when the preprocessor symbol
    //     <OTCLIB_TRACE> is defined. The macro <OTCLIB_SETTRACESWITCH> can
    //     be used set the value of an instance of the <OTC_TraceSwitch>
    //     class created using the <OTCLIB_TRACESWITCH> macro.
    //     
    // = BEGIN<CODE>
    //     OTCLIB_TRACESWITCH(LIBRARY,true);
    //     OTCLIB_TRACESWITCH(PROGRAM,true);
    //     OTCLIB_LOGGER(LIBRARY) << "some text" << endl;
    //     OTCLIB_SETTRACESWITCH(PROGRAM,false);
    //     OTCLIB_LOGGER(LIBRARY || PROGRAM) << "some text" << endl;
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Tracer>
{
  public:

        virtual		~OTC_TraceSwitch();

    // = INITIALISATION

			OTC_TraceSwitch(bool theState=false)
			  : state_(theState) {}
				// Sets the state of the switch to
				// <theState>. A value of <true> enables the
				// switch, otherwise the switch is disabled.

			OTC_TraceSwitch(OTC_TraceSwitch const& theSwitch)
			  : state_(theSwitch.enabled()) {}
				// Sets the state of this switch to the same
				// state as <theSwitch>.

    OTC_TraceSwitch&	operator=(bool theState)
				{ state_ = theState; return *this; }
				// Sets the state of this switch to
				// <theState>. A value of <true> enables the
				// switch, otherwise the switch is disabled.

    OTC_TraceSwitch&	operator=(OTC_TraceSwitch const& theSwitch)
				{ state_ = theSwitch.enabled(); return *this; }
				// Sets the state of this switch to the same
				// value as <theSwitch>.

    // = QUERY

    virtual bool	enabled() const;
				// Returns <true> if the switch is enabled,
				// otherwise returns <false>.

			operator int() const;
				// Returns <0> if the switch is enabled and
				// <-1> if the switch is disabled. Note that
				// this is the inverse of what you would
				// typically expect. This operator is to
				// satisfy the requirement that the macro
				// <OTCLIB_TRACER> be passed an <int>.

    // = LOGIC OPERATIONS

    OTC_TraceSwitch	operator||(OTC_TraceSwitch const& theSwitch) const;
				// Returns an enabled switch if either
				// this switch or <theSwitch> are enabled.

    OTC_TraceSwitch	operator&&(OTC_TraceSwitch const& theSwitch) const;
				// Returns an enabled switch if both this
				// switch and <theSwitch> are enabled.

    OTC_TraceSwitch	operator!() const;
				// Returns an enabled switch if this switch
				// is not enabled.

  private:

    OTC_TraceSwitch	operator==(int theLevel) const;
				// Do not define an implementation for this.

    OTC_TraceSwitch	operator!=(int theLevel) const;
				// Do not define an implementation for this.

    OTC_TraceSwitch	operator<(int theLevel) const;
				// Do not define an implementation for this.

    OTC_TraceSwitch	operator<=(int theLevel) const;
				// Do not define an implementation for this.

    OTC_TraceSwitch	operator>(int theLevel) const;
				// Do not define an implementation for this.

    OTC_TraceSwitch	operator>=(int theLevel) const;
				// Do not define an implementation for this.

    friend OTC_TraceSwitch	operator==(
				 OTC_TraceSwitch const&,
				 int theLevel
				);
				// Do not define an implementation for this.

    friend OTC_TraceSwitch	operator!=(
				 OTC_TraceSwitch const&,
				 int theLevel
				);
				// Do not define an implementation for this.

    friend OTC_TraceSwitch	operator<(
				 OTC_TraceSwitch const&,
				 int theLevel
				);
				// Do not define an implementation for this.

    friend OTC_TraceSwitch	operator<=(
				 OTC_TraceSwitch const&,
				 int theLevel
				);
				// Do not define an implementation for this.

    friend OTC_TraceSwitch	operator>(
				 OTC_TraceSwitch const&,
				 int theLevel
				);
				// Do not define an implementation for this.

    friend OTC_TraceSwitch	operator>=(
				 OTC_TraceSwitch const&,
				 int theLevel
				);
				// Do not define an implementation for this.

    bool		state_;
				// State of the switch. Set to <true> if the
				// switch is enabled, otherwise set to
				// <false>.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TRCSWTCH_HH */
