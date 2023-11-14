/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcosstream.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/osstream.hh"
#endif
#endif

#include <OTC/text/osstream.hh>

/* ------------------------------------------------------------------------- */
OTC_OSStream::~OTC_OSStream()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_OSStream::OTC_OSStream(OTC_String& theString, OTC_BufferingFlag theType)
  : OTC_StringBufWrapper(theString,theType),
    ostream(OTC_StringBufWrapper::streambuf())
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
