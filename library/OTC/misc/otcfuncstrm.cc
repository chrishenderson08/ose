/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/otcfuncstrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/misc/funcstrm.hh"
#endif
#endif

#include <OTC/misc/funcstrm.hh>

/* ------------------------------------------------------------------------- */
OTC_FuncStream::OTC_FuncStream(
 void (*theFunc)(char const*,size_t),
 char* theBuffer,
 size_t theSize
)
  : OTC_FuncBufWrapper(theFunc,theBuffer,theSize),
    ostream(OTC_FuncBufWrapper::streambuf())
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_FuncStream::~OTC_FuncStream()
{
  OTC_FuncBufWrapper::streambuf()->pubsync();
}

/* ------------------------------------------------------------------------- */
