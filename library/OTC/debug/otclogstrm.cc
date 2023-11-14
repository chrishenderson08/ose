/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otclogstrm.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/logstrm.hh"
#endif
#endif

#include <OTC/debug/logstrm.hh>

#include <new.h>

/* ------------------------------------------------------------------------- */
static char otclib_logbuf[2048];
static OTC_LogStream* otclib_logstrm = 0;
static double otclib_logstrm_mem[(sizeof(OTC_LogStream)/sizeof(double))+1];

/* ------------------------------------------------------------------------- */
OTC_LogStream::OTC_LogStream(char* theBuffer, size_t theSize)
  : OTC_LogBufWrapper(theBuffer,theSize),
    ostream(OTC_LogBufWrapper::streambuf())
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_LogStream::~OTC_LogStream()
{
  OTC_LogBufWrapper::streambuf()->pubsync();
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT OTC_LogStream& OTCLIB_LOGGER(OTC_LogLevel theLevel)
{
  if (otclib_logstrm == 0)
  {
    otclib_logstrm = new ((void*)&otclib_logstrm_mem)
     OTC_LogStream(otclib_logbuf,sizeof(otclib_logbuf));
  }

  otclib_logstrm->setLevel(theLevel);
  otclib_logstrm->clear();

  return *otclib_logstrm;
}

/* ------------------------------------------------------------------------- */
