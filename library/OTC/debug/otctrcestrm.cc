/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otctrcestrm.cc
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

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/trcestrm.hh"
#endif
#endif

#include <OTC/debug/trcestrm.hh>
#include <OTC/debug/trcebuf.hh>
#include <OTC/debug/tracer.hh>

/* ------------------------------------------------------------------------- */
bool OTC_TraceStream::initialised_ = false;
int OTC_TraceStream::objectIndex_ = 0;
int OTC_TraceStream::levelIndex_ = 0;

/* ------------------------------------------------------------------------- */
OTC_TraceStream::~OTC_TraceStream()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_TraceStream::OTC_TraceStream()
  : OTC_TraceBufWrapper(this),
    ostream(OTC_TraceBufWrapper::streambuf())
{

  if (initialised_ == false)
  {
    initialised_ = true;

    // Allocate space in parameter pool. Each instance has its own
    // set of parameters.

    objectIndex_ = ios::xalloc();
    levelIndex_ = ios::xalloc();
  }

  // Make sure ours are allocated and also initialised.

  pword(objectIndex_) = (ostream*)this;
  iword(levelIndex_) = 0;
}

/* ------------------------------------------------------------------------- */
long OTC_TraceStream::level()
{
  return iword(levelIndex_);
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& OTCLIB_TRACEIN(ostream& os)
{
  if (OTC_TraceStream::initialised_ == false)
    return os;

  // Sanity check.

  // if (os.pword(OTC_TraceStream::objectIndex_) != &os)
  //  return os;

  // Flush it in case we have output an embedded EOL which hasn't
  // been processed yet.

  os.flush();

  // Increment index.

  long& theIndentLevel = os.iword(OTC_TraceStream::levelIndex_);
  theIndentLevel++;

  return os;
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& OTCLIB_TRACEOUT(ostream& os)
{
  if (OTC_TraceStream::initialised_ == false)
    return os;

  // Sanity check.

  // if (os.pword(OTC_TraceStream::objectIndex_) != &os)
  //  return os;

  // Flush it in case we have output an embedded EOL which hasn't
  // been processed yet.

  os.flush();

  // Decrement index.

  long& theIndentLevel = os.iword(OTC_TraceStream::levelIndex_);
  OTCLIB_ASSERT(theIndentLevel > 0);
  if (theIndentLevel > 0)
    theIndentLevel--;

  return os;
}

/* ------------------------------------------------------------------------- */
