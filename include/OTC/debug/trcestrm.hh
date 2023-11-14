#ifndef OTC_DEBUG_TRCESTRM_HH
#define OTC_DEBUG_TRCESTRM_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/trcestrm.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_DEBUG_TRCEBUF_HH
#include <OTC/debug/trcebuf.hh>
#endif

#include <OTC/stdlib/iostream.h>
#include <OTC/stdlib/iomanip.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/debug/trcestrm.hh"
#endif
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4275)
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_TraceStream : private OTC_TraceBufWrapper, public ostream
    // = TITLE
    //     Trace stream capable of doing indenting.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_TraceStream> is a specialised streams class which
    //     provides the ability to indent information passed through the
    //     stream. This ability is used in conjunction with the <OTC_Tracer>
    //     class, to indent trace messages with a marked scope such as a
    //     function.
    //     
    //     Two manipulators are available if explicit indenting of trace
    //     input is required. These are <OTCLIB_TRACEIN> and
    //     <OTCLIB_TRACEOUT>. The first causes indenting to be increased, the
    //     second for it to be decreased.
    //
    // = NOTES
    //     There can only ever be once instance of this class because of
    //     the way the manipulators use static data within this class to
    //     track the indent level. As there is only ever one instance
    //     and static data is set from constructor and then not changed,
    //     there is no thread protection within this class for the static
    //     data.
    //
    // = SEE ALSO
    //     <OTC_Tracer>
{
    friend class	OTC_TraceBuf;

    friend OSE_EXPORT ostream&	OTCLIB_TRACEIN(ostream&);
    friend OSE_EXPORT ostream&	OTCLIB_TRACEOUT(ostream&);

  public:

			~OTC_TraceStream();

			OTC_TraceStream();

    // = QUERY

    long		level();
				// Returns the trace indent level.

  private:

			OTC_TraceStream(OTC_TraceStream const&);
				// Do not provide an implemetation for this.

    OTC_TraceStream&	operator=(OTC_TraceStream const&);
				// Do not provide an implemetation for this.

    static bool		initialised_;
				// Whether static data has been initalised.

    static int		objectIndex_;
				// Index into stream data pool where pointer
				// for this instance is stashed.

    static int		levelIndex_;
				// Index into stream data pool where trace
				// indent level is stashed.
};

extern OSE_EXPORT ostream& OTCLIB_TRACEIN(ostream&);
extern OSE_EXPORT ostream& OTCLIB_TRACEOUT(ostream&);

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TRCESTRM_HH */
