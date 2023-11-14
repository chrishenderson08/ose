/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otceplisten.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/eplisten.hh"
#endif
#endif

#include <OTC/message/eplisten.hh>

#include <OTC/message/connect.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/nrmutex.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_EPListener");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EPListener::mutex_;
OTC_LinkList* OTC_EPListener::listeners_;

/* ------------------------------------------------------------------------- */
OTC_EPListener::~OTC_EPListener()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::~OTC_EPListener()");

  OTCEV_Shutdown::unsubscribe(id());

  OTC_Iterator<OTC_EPServer*> theServers;
  theServers = servers_.items();
  while (theServers.isValid())
  {
    theServers.item()->removeObserver(id());
    theServers.item()->stop();
    theServers.next();
  }

  mutex_.lock();

  OTC_LinkIterator theIter = listeners_->items();
  theIter.resetFirst();
  while (theIter.isLink())
  {
    OTC_VLink* theLink = (OTC_VLink*)theIter.link();
    if ((OTC_EPListener*)theLink->item() == this)
    {
      theLink->kill();
      break;
    }

    theIter.next();
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_EPListener::OTC_EPListener()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::OTC_EPListener()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (listeners_ == 0)
  {
    listeners_ = new OTC_LinkList;
    OTCLIB_ASSERT_M(listeners_ != 0);
  }

  OTC_VLink* theLink;
  theLink = new OTC_VLink((void*)this);
  OTCLIB_ASSERT_M(theLink != 0);

  listeners_->addLast(theLink);

  xxxMutex.release();

  mutex_.unlock();

  OTCEV_Shutdown::subscribe(id());
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::shutdown()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::shutdown()");

  suspendListening();
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::suspendListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::suspendListening()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::resumeListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::resumeListening()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::suspendAllListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::suspendAllListening()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_LinkIterator theIter = listeners_->items();
  theIter.resetFirst();
  while (theIter.isLink())
  {
    OTC_VLink* theLink = (OTC_VLink*)theIter.link();
    OTC_EPListener* theListener = (OTC_EPListener*)theLink->item();
    theListener->suspendListening();

    theIter.next();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::resumeAllListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::resumeAllListening()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_LinkIterator theIter = listeners_->items();
  theIter.resetFirst();
  while (theIter.isLink())
  {
    OTC_VLink* theLink = (OTC_VLink*)theIter.link();
    OTC_EPListener* theListener = (OTC_EPListener*)theLink->item();
    theListener->resumeListening();

    theIter.next();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() == OTCLIB_CONNECTION_INACTIVE)
    {
      if (servers_.contains(theConnection->localAddress()))
      {
	OTC_EPServer* theServer;
	theServer = servers_.item(theConnection->localAddress());
	theServer->removeObserver(id());

	servers_.remove(theConnection->localAddress(),OTCLIB_UNSAFE);
      }

      notifyObservers(theEvent->clone(),OTCLIB_STANDARD_JOB);
    }
    else
    {
      notifyObservers(theEvent->clone(),OTCLIB_PRIORITY_JOB);
    }
  }
  else
  {
    // Don't really expect to receive any other events.

    notifyObservers(theEvent->clone(),OTCLIB_STANDARD_JOB);
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_EPListener::accept(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EPListener::accept(OTC_Event*)");

  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    shutdown();
    theEvent->destroy();
  }
  else
  {
    OTC_EVAgent::accept(theEvent);
  }
}

/* ------------------------------------------------------------------------- */
