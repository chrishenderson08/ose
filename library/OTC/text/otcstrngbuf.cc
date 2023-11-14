/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/otcstrngbuf.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/text/strngbuf.hh"
#endif
#endif

#include <OTC/text/strngbuf.hh>

#ifndef zapeof
#define zapeof(c) ((c)&0377)
#endif

/* ------------------------------------------------------------------------- */
OTC_StringBuf::~OTC_StringBuf()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_StringBuf::OTC_StringBuf(OTC_String& theString, OTC_BufferingFlag theType)
  : OTC_VFuncBuf(theType), string_(theString)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_StringBuf::handle(char const* theBuffer, size_t theSize)
{
  string_.append(theBuffer,theSize);
}

/* ------------------------------------------------------------------------- */
