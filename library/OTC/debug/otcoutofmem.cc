/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otcoutofmem.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/outofmem.hh"
#endif
#endif

#include <OTC/debug/outofmem.hh>

/* ------------------------------------------------------------------------- */
OTCERR_OutOfMemory::~OTCERR_OutOfMemory()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCERR_OutOfMemory::OTCERR_OutOfMemory()
  : OTC_Exception("Out of memory")
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCERR_OutOfMemory::OTCERR_OutOfMemory(char const* theFile, u_int theLine)
  : OTC_Exception("Out of memory",theFile,theLine)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTCERR_OutOfMemory::OTCERR_OutOfMemory(
 OTCERR_OutOfMemory const& theException
)
  : OTC_Exception(theException)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTCERR_OutOfMemory::throwIt() const
{
  throw *this;
}

/* ------------------------------------------------------------------------- */
