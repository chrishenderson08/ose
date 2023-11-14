/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onsmonitor.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "ONS/agent/monitor.hh"
#endif
#endif

#include <ONS/agent/monitor.hh>

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/svrgstry.hh>
#include <OTC/dispatch/evreaper.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
static OTC_Mutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("ONS_Monitor");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}
 
#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
ONS_Monitor::~ONS_Monitor()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::~ONS_Monitor()");

  OTC_SVBroker::registry()->unsubscribeAgent(id());
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::unsubscribeServiceName(OTC_String const& theServiceName)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::unsubscribeServiceName()");

  OTCLIB_TRACER(MODULE) << "serviceName = " << theServiceName << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "NAME";
  tmpKey += EOS;
  tmpKey += theServiceName;

  OTC_SVBroker::registry()->unsubscribeServiceName(id(),theServiceName);

  registryCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::unsubscribeServiceGroup(OTC_String const& theServiceGroup)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::unsubscribeServiceGroup()");

  OTCLIB_TRACER(MODULE) << "serviceGroup = " << theServiceGroup << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "GROUP";
  tmpKey += EOS;
  tmpKey += theServiceGroup;

  OTC_SVBroker::registry()->unsubscribeServiceGroup(id(),theServiceGroup);

  registryCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::unsubscribeServiceAddress(
 OTC_String const& theServiceAddress,
 OTC_String const& theSubscription
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::unsubscribeServiceAddress()");

  OTCLIB_TRACER(MODULE) << "serviceAddress = " << theServiceAddress << endl;
  OTCLIB_TRACER(MODULE) << "subscription = " << theSubscription << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(255);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "ADDRESS";
  tmpKey += EOS;
  tmpKey += theServiceAddress;
  tmpKey += EOS;
  tmpKey += theSubscription;

  OTC_SVBroker::registry()->unsubscribeServiceAddress(
   id(),theServiceAddress,theSubscription);

  registryCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::unsubscribeRegistry()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::unsubscribeRegistry()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(15);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "REGISTRY";

  OTC_SVBroker::registry()->unsubscribeRegistry(id());

  registryCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::allocateContexts(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::allocateContexts()");

  ONS_Agent::allocateContexts(theNumThreads);
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::filter(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::filter()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  OTC_EventReaper xxxEvent;

  xxxEvent.grab(theEvent);

  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*)theEvent;

    handleServiceAnnouncement(theAnnouncement);
  }
  else if (theEvent->type() == OTCEV_GroupAnnouncement::typeId())
  {
    OTCEV_GroupAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_GroupAnnouncement*)theEvent;

    handleGroupAnnouncement(theAnnouncement);
  }
  else
  {
    xxxEvent.release();

    ONS_Agent::filter(theEvent);

    return;
  }

  xxxEvent.release();

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::handleServiceAnnouncement(
 OTCEV_ServiceAnnouncement* theAnnouncement
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::handleServiceAnnouncement()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_ServiceBinding* theBinding;
    theBinding = theAnnouncement->serviceBinding();

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_NAME)
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "NAME";
      tmpKey += EOS;
      tmpKey += theBinding->serviceName();

      if (registryCallbacks_.contains(tmpKey))
	theCallback = registryCallbacks_.item(tmpKey);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OTC_Capacity tmpCapacity(255);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "ADDRESS";
      tmpKey += EOS;
      tmpKey += theBinding->serviceAddress();
      tmpKey += EOS;
      tmpKey += theAnnouncement->subscriptionName();

      if (registryCallbacks_.contains(tmpKey))
	theCallback = registryCallbacks_.item(tmpKey);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_REGISTRY)
    {
      OTC_Capacity tmpCapacity(15);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "REGISTRY";

      if (registryCallbacks_.contains(tmpKey))
	theCallback = registryCallbacks_.item(tmpKey);
    }

    if (theCallback != 0)
      theCallback->reference();

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theAnnouncement,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Monitor" << EOL;
    theLogger << "Method: handleServiceAnnouncement()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::handleGroupAnnouncement(
 OTCEV_GroupAnnouncement* theAnnouncement
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::handleGroupAnnouncement()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_GROUP)
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "GROUP";
      tmpKey += EOS;
      tmpKey += theAnnouncement->serviceGroup();

      if (registryCallbacks_.contains(tmpKey))
	theCallback = registryCallbacks_.item(tmpKey);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OTC_ServiceBinding* theBinding;
      theBinding = theAnnouncement->serviceBinding();

      OTC_Capacity tmpCapacity(255);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "ADDRESS";
      tmpKey += EOS;
      tmpKey += theBinding->serviceAddress();
      tmpKey += EOS;
      tmpKey += theAnnouncement->subscriptionName();

      if (registryCallbacks_.contains(tmpKey))
	theCallback = registryCallbacks_.item(tmpKey);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_REGISTRY)
    {
      OTC_ServiceBinding* theBinding;
      theBinding = theAnnouncement->serviceBinding();

      OTC_Capacity tmpCapacity(15);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "REGISTRY";

      if (registryCallbacks_.contains(tmpKey))
	theCallback = registryCallbacks_.item(tmpKey);
    }

    if (theCallback != 0)
      theCallback->reference();

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
      theCallback->execute(theAnnouncement,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Monitor" << EOL;
    theLogger << "Method: handleGroupAnnouncement()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::subscribeServiceName_(
 ONS_EventCallback* theCallback,
 OTC_String const& theServiceName
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::subscribeServiceName_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Monitor::subscribeServiceName_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  OTCLIB_TRACER(MODULE) << "serviceName = " << theServiceName << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "NAME";
  tmpKey += EOS;
  tmpKey += theServiceName;

  registryCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTC_SVBroker::registry()->subscribeServiceName(id(),theServiceName);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::subscribeServiceGroup_(
 ONS_EventCallback* theCallback,
 OTC_String const& theServiceGroup
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::subscribeServiceGroup_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Monitor::subscribeServiceGroup_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  OTCLIB_TRACER(MODULE) << "serviceGroup = " << theServiceGroup << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "GROUP";
  tmpKey += EOS;
  tmpKey += theServiceGroup;

  registryCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTC_SVBroker::registry()->subscribeServiceGroup(id(),theServiceGroup);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_ServiceStatus ONS_Monitor::subscribeServiceAddress_(
 ONS_EventCallback* theCallback,
 OTC_String const& theServiceAddress,
 OTC_String const& theSubscription
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::subscribeServiceAddress_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Monitor::subscribeServiceAddress_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTCLIB_SERVICE_WITHDRAWN;

  OTCLIB_TRACER(MODULE) << "serviceAddress = " << theServiceAddress << endl;
  OTCLIB_TRACER(MODULE) << "subscription = " << theSubscription << endl;

  OTC_ServiceStatus theStatus;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(255);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "ADDRESS";
  tmpKey += EOS;
  tmpKey += theServiceAddress;
  tmpKey += EOS;
  tmpKey += theSubscription;

  registryCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  theStatus = OTC_SVBroker::registry()->subscribeServiceAddress(id(),
   theServiceAddress,theSubscription);

  xxxMutex.release();

  mutex_.unlock();

  return theStatus;
}

/* ------------------------------------------------------------------------- */
void ONS_Monitor::subscribeRegistry_(
 ONS_EventCallback* theCallback
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Monitor::subscribeRegistry_()");

  OTCLIB_ENSURE_FN_W((theCallback != 0),
   "ONS_Monitor::subscribeRegistry_()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(15);
  OTC_String tmpKey(tmpCapacity);

  tmpKey += "REGISTRY";

  registryCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  OTC_SVBroker::registry()->subscribeRegistry(id());

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
