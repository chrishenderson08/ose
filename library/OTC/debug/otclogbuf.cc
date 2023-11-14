/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otclogbuf.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/logbuf.hh"
#endif
#endif

#include <OTC/debug/logbuf.hh>

#ifndef zapeof
#define zapeof(c) ((c)&0377)
#endif

/* ------------------------------------------------------------------------- */
OTC_LogBuf::~OTC_LogBuf()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_LogBuf::OTC_LogBuf(char* theBuffer, size_t theSize)
  : channel_(0), level_(OTCLIB_LOG_DEBUG)
{
  if (theBuffer != 0 && theSize > (int)sizeof(buffer_))
    setp(theBuffer,theBuffer+theSize-2);
  else
    setp(buffer_,buffer_+sizeof(buffer_)-2);
}

/* ------------------------------------------------------------------------- */
#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
OTC_LogBuf::int_type OTC_LogBuf::overflow(int_type c)
#else
int OTC_LogBuf::overflow(int c)
#endif
{
  char* endp = pptr();

  *endp = EOS;
  *(endp+1) = EOS;

  while (*(endp-1) == '\n' && endp > pbase())
    *--endp = EOS;

#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
  if (!traits_type::eq_int_type(c,traits_type::eof()) && c != EOS)
#else
  if (c != EOF && c != EOS)
#endif
    *endp++ = char(c);

  OTC_Logger::notify(channel_,level_,pbase(),endp-pbase());

  setp(pbase(),epptr());

#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
  return traits_type::not_eof(c);
#else
  return zapeof(c);
#endif
}

/* ------------------------------------------------------------------------- */
#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
OTC_LogBuf::int_type OTC_LogBuf::underflow()
#else
int OTC_LogBuf::underflow()
#endif
{
  return 0;
}

/* ------------------------------------------------------------------------- */
int OTC_LogBuf::sync()
{
  if (pptr() && pptr() > pbase())
  {
    // LIBG++ does check explicitly for -1, so must make sure
    // we return 0 if all is okay.

#if !defined(OSE_HAVE_OLD_STYLE_IOSTREAMS)
    int_type c = overflow(traits_type::eof());
    if (traits_type::eq_int_type(c,traits_type::eof()))
      return -1;
    else
      return 0;
#else
    int c = overflow(EOF);
    return c == -1 ? -1 : 0;
#endif
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
