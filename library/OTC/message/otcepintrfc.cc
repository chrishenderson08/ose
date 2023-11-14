/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcepintrfc.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/epintrfc.hh"
#endif
#endif

#include <OTC/message/epintrfc.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EPInterface");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_EPInterface::~OTC_EPInterface()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPInterface::~OTC_EPInterface()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_EPInterface::OTC_EPInterface(OTC_EndPoint* theEndPoint)
  : endPoint_(theEndPoint)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPInterface::OTC_EPInterface(OTC_EndPoint*)");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
