/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcendpoint.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/endpoint.hh"
#endif
#endif

#include <OTC/message/endpoint.hh>
#include <OTC/message/epintrfc.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/system/program.hh>
#include <OTC/text/symbol.hh>
#include <OTC/collctn/ilink.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EndPoint");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EndPoint::mutex_;
OTC_HMap<OTC_String,OTC_EndPoint*>* OTC_EndPoint::gLocal_ = 0;
OTC_HMap<OTC_String,OTC_EndPoint*>* OTC_EndPoint::gRemote_ = 0;

/* ------------------------------------------------------------------------- */
OTC_EndPoint::~OTC_EndPoint()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::~OTC_EndPoint");

  mutex_.lock();

  gLocal_->remove(localAddress_,OTCLIB_UNSAFE);

  mutex_.unlock();

  if (handler_ != 0)
    delete handler_;
}

/* ------------------------------------------------------------------------- */
OTC_EndPoint::OTC_EndPoint(
 OTC_EndPointType theType,
 char const* theInterface,
 char const* theProtocol
)
  : type_(theType),
    interface_(theInterface),
    protocol_(theProtocol),
    handler_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::OTC_EndPoint(...)");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  localAddress_ = OTC_Symbol(
   OTC_Program::uniqueId("$EP?",OTCLIB_ID_SHORT_FORMAT));

  OTCLIB_TRACER(MODULE) << "local-address = " << localAddress_ << endl;

  if (gLocal_ == 0)
  {
    gLocal_ = new OTC_HMap<OTC_String,OTC_EndPoint*>;
    OTCLIB_ASSERT_M(gLocal_ != 0);
  }

  OTCLIB_ENSURE_FN((!gLocal_->contains(localAddress_)),
   "OTC_EndPoint::OTC_EndPoint(...)",
   "duplicate local address");

  OTC_EndPoint* thisObject = this;
  gLocal_->add(localAddress_,thisObject);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::start()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::start()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::stop(int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::stop()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::suspendInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::suspendInput()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::resumeInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::resumeInput()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::attach(OTC_EPInterface* theHandler)
{
  OTCLIB_ENSURE_FN((theHandler != 0),
   "OTC_EndPoint::attach(OTC_EPInterface*)",
   "invalid interface object");

  handler_ = theHandler;
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::suspendAllInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::suspendAllInput()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gLocal_ != 0)
  {
    OTC_Iterator<OTC_EndPoint*> theEndPoints;

    theEndPoints = gLocal_->items();
    while (theEndPoints.isValid())
    {
      theEndPoints.item()->suspendInput();
      theEndPoints.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::resumeAllInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::resumeAllInput()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gLocal_ != 0)
  {
    OTC_Iterator<OTC_EndPoint*> theEndPoints;

    theEndPoints = gLocal_->items();
    while (theEndPoints.isValid())
    {
      theEndPoints.item()->resumeInput();
      theEndPoints.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_EndPoint* OTC_EndPoint::local(OTC_String const& theLocalAddress)
{
  OTC_EndPoint* theResult = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gLocal_ != 0)
  {
    OTC_EndPoint** thePtr;
    thePtr = gLocal_->pItem(theLocalAddress);

    if (thePtr != 0)
      theResult = *thePtr;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_EndPoint* OTC_EndPoint::remote(OTC_String const& theRemoteAddress)
{
  OTC_EndPoint* theResult = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gRemote_ != 0)
 {
    OTC_EndPoint** thePtr;
    thePtr = gRemote_->pItem(theRemoteAddress);

    if (thePtr != 0)
      theResult = *thePtr;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_EndPoint*> OTC_EndPoint::endPoints()
{
  OTC_Iterator<OTC_EndPoint*> theResult;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gLocal_ != 0)
    theResult = gLocal_->items();

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::bindRemoteAddress(char const* theRemoteAddress)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::bindRemoteAddress(char const*)");

  if (theRemoteAddress != 0)
  {
    while (*theRemoteAddress != EOS && isspace(*theRemoteAddress))
      theRemoteAddress++;
  }

  OTCLIB_ENSURE_FN((theRemoteAddress != 0 && *theRemoteAddress != EOS),
   "OTC_EndPoint::bindRemoteAddress(char const*)",
   "invalid address");

  OTCLIB_TRACER(MODULE) << "remote-address = " << theRemoteAddress << endl;
  OTCLIB_TRACER(MODULE) << "local-address = " << localAddress_ << endl;

  inBox_.bind(this,theRemoteAddress);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gRemote_ == 0)
  {
    gRemote_ = new OTC_HMap<OTC_String,OTC_EndPoint*>;
    OTCLIB_ASSERT_M(gRemote_ != 0);
  }

  OTCLIB_ENSURE_FN((!gRemote_->contains(theRemoteAddress)),
   "OTC_EndPoint::bindRemoteAddress(char const*)",
   "duplicate remote address");

  OTC_EndPoint* thisObject = this;
  gRemote_->add(theRemoteAddress,thisObject);

  xxxMutex.release();

  mutex_.unlock();

#if 0
  OTCEV_Connection* theEvent;
  theEvent = new OTCEV_Connection(localAddress_,
   inBox_.name(),interface(),protocol(),
   localTransport(),remoteTransport(),
   OTCLIB_CONNECTION_ACTIVE);
  OTCLIB_ASSERT_M(theEvent != 0);

  notifyObservers(theEvent,OTCLIB_PRIORITY_JOB);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::unbindRemoteAddress()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::unbindRemoteAddress()");

  OTCLIB_TRACER(MODULE) << "address = " << inBox_.name() << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gRemote_ != 0 && !inBox_.name().isEmpty())
    gRemote_->remove(inBox_.name(),OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();

#if 0
  OTCEV_Connection* theEvent;
  theEvent = new OTCEV_Connection(localAddress_,
   inBox_.name(),interface(),protocol(),
   localTransport(),remoteTransport(),
   OTCLIB_CONNECTION_INACTIVE);
  OTCLIB_ASSERT_M(theEvent != 0);
#endif

  inBox_.unbind();

#if 0
  notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
#endif
}

/* ------------------------------------------------------------------------- */
void OTC_EndPoint::accept(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EndPoint::accept()");

  if (theEvent == 0)
    return;

  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCLIB_ENSURE_FN((handler_ != 0),
     "OTC_EndPoint::accept(OTC_Event*)",
     "no valid interface object");

    theEvent->deliver(handler_);
  }
  else
  {
    OTC_EVAgent::accept(theEvent);
  }
}

/* ------------------------------------------------------------------------- */
