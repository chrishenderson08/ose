/*
// ============================================================================
//
// = FILENAME
//     opymonitor.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "monitor.hh"
#include "servbind.hh"
#include "dispatch.hh"

#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Monitor::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Monitor",
  sizeof(OPY_Monitor),
  0,
  OPY_Monitor::sys_dealloc,
  0,
  OPY_Monitor::sys_getattr,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_Monitor::gMethods_[] = {
  { "suspendEvents",
    OPY_Agent::mfn_suspendEvents, 1 },
  { "resumeEvents",
    OPY_Agent::mfn_resumeEvents, 1 },
  { "activateThreading",
    OPY_Agent::mfn_activateThreading, 1 },
  { "suspendThreading",
    OPY_Agent::mfn_suspendThreading, 1 },
  { "resumeThreading",
    OPY_Agent::mfn_resumeThreading, 1 },
  { "shutdownThreading",
    OPY_Agent::mfn_shutdownThreading, 1 },
  { "threadingEnabled",
    OPY_Agent::mfn_threadingEnabled, 1 },
  { "id",
    OPY_Agent::mfn_id, 1 },
  { "startTimer",
    OPY_Agent::mfn_startTimer, 1 },
  { "cancelTimer",
    OPY_Agent::mfn_cancelTimer, 1 },
  { "cancelAllTimers",
    OPY_Agent::mfn_cancelAllTimers, 1 },
  { "setAlarm",
    OPY_Agent::mfn_setAlarm, 1 },
  { "cancelAlarm",
    OPY_Agent::mfn_cancelAlarm, 1 },
  { "cancelAllAlarms",
    OPY_Agent::mfn_cancelAllAlarms, 1 },
  { "scheduleAction",
    OPY_Agent::mfn_scheduleAction, 1 },
  { "cancelAction",
    OPY_Agent::mfn_cancelAction, 1 },
  { "cancelAllActions",
    OPY_Agent::mfn_cancelAllActions, 1 },
  { "subscribeSignal",
    OPY_Agent::mfn_subscribeSignal, 1 },
  { "unsubscribeSignal",
    OPY_Agent::mfn_unsubscribeSignal, 1 },
  { "unsubscribeAllSignals",
    OPY_Agent::mfn_unsubscribeAllSignals, 1 },
  { "scheduleShutdown",
    OPY_Agent::mfn_scheduleShutdown, 1 },
  { "suspendShutdown",
    OPY_Agent::mfn_suspendShutdown, 1 },
  { "resumeShutdown",
    OPY_Agent::mfn_resumeShutdown, 1 },
  { "subscribeShutdown",
    OPY_Agent::mfn_subscribeShutdown, 1 },
  { "unsubscribeShutdown",
    OPY_Agent::mfn_unsubscribeShutdown, 1 },
  { "subscribeSocket",
    OPY_Agent::mfn_subscribeSocket, 1 },
  { "unsubscribeSocket",
    OPY_Agent::mfn_unsubscribeSocket, 1 },
  { "unsubscribeAllSockets",
    OPY_Agent::mfn_unsubscribeAllSockets, 1 },
  { "serviceAgents",
    OPY_Monitor::mfn_serviceAgents, 1 },
  { "serviceGroups",
    OPY_Monitor::mfn_serviceGroups, 1 },
  { "lookupServiceName",
    OPY_Monitor::mfn_lookupServiceName, 1 },
  { "lookupServiceAddress",
    OPY_Monitor::mfn_lookupServiceAddress, 1 },
  { "lookupServiceGroup",
    OPY_Monitor::mfn_lookupServiceGroup, 1 },
  { "unsubscribeAllRegistry",
    OPY_Monitor::mfn_unsubscribeAllRegistry, 1 },
  { "subscribeRegistry",
    OPY_Monitor::mfn_subscribeRegistry, 1 },
  { "unsubscribeRegistry",
    OPY_Monitor::mfn_unsubscribeRegistry, 1 },
  { "subscribeServiceName",
    OPY_Monitor::mfn_subscribeServiceName, 1 },
  { "unsubscribeServiceName",
    OPY_Monitor::mfn_unsubscribeServiceName, 1 },
  { "subscribeServiceAddress",
    OPY_Monitor::mfn_subscribeServiceAddress, 1 },
  { "unsubscribeServiceAddress",
    OPY_Monitor::mfn_unsubscribeServiceAddress, 1 },
  { "subscribeServiceGroup",
    OPY_Monitor::mfn_subscribeServiceGroup, 1 },
  { "unsubscribeServiceGroup",
    OPY_Monitor::mfn_unsubscribeServiceGroup, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Monitor::~OPY_Monitor()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OPY_Monitor::OPY_Monitor(PyObject* theOuter)
  : OPY_Agent(theOuter)
{
  ob_type = &gType_;

  // _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theOuter))
  {
    return 0;
  }

  OPY_Monitor* theAgent;
  theAgent = new OPY_Monitor(theOuter);
  OTCLIB_ASSERT_M(theAgent != 0);

  return (PyObject*)theAgent;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Monitor::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Monitor*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Monitor*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_serviceAgents(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->serviceAgents();

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theBindings.isValid())
  {
    OPY_ServiceBinding* theObject;
    theObject = new OPY_ServiceBinding(theBindings.item());
    OTCLIB_ASSERT_M(theObject != 0);

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theBindings.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_serviceGroups(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;

  if (!PyArg_ParseTuple(theArgs,"|z",&theServiceAddress))
    return 0;

  OTC_Iterator<OTC_String> theGroups;
  theGroups = OTC_SVBroker::registry()->serviceGroups(theServiceAddress);

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theGroups.isValid())
  {
    PyObject* theObject;
    theObject = Py_BuildValue("s",theGroups.item().string());

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theGroups.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_lookupServiceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceName = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceName))
    return 0;

  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceName(theServiceName);

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theBindings.isValid())
  {
    OPY_ServiceBinding* theObject;
    theObject = new OPY_ServiceBinding(theBindings.item());
    OTCLIB_ASSERT_M(theObject != 0);

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theBindings.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_lookupServiceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceAddress))
    return 0;

  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceAddress(
   theServiceAddress);

  if (!theBindings.isValid())
  {
    Py_XINCREF(Py_None);

    return Py_None;
  }

  OPY_ServiceBinding* theObject;
  theObject = new OPY_ServiceBinding(theBindings.item());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_lookupServiceGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_Iterator<OTC_ServiceBinding*> theBindings;
  theBindings = OTC_SVBroker::registry()->lookupServiceGroup(
   theServiceGroup);

  PyObject* theList;
  theList = PyList_New(0);

  if (theList == 0)
    return 0;

  while (theBindings.isValid())
  {
    OPY_ServiceBinding* theObject;
    theObject = new OPY_ServiceBinding(theBindings.item());
    OTCLIB_ASSERT_M(theObject != 0);

    if (PyList_Append(theList,theObject) == -1)
    {
      Py_XDECREF(theObject);
      Py_XDECREF(theList);

      return 0;
    }

    Py_XDECREF(theObject);

    theBindings.next();
  }

  return theList;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_unsubscribeAllRegistry(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker::registry()->unsubscribeAgent(
   ((OPY_Monitor*)theInstance)->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_subscribeRegistry(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker::registry()->subscribeRegistry(
   ((OPY_Monitor*)theInstance)->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_unsubscribeRegistry(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker::registry()->unsubscribeRegistry(
   ((OPY_Monitor*)theInstance)->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_subscribeServiceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceName = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceName))
    return 0;

  OTC_SVBroker::registry()->subscribeServiceName(
   ((OPY_Monitor*)theInstance)->id(),theServiceName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_unsubscribeServiceName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceName = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceName))
    return 0;

  OTC_SVBroker::registry()->unsubscribeServiceName(
   ((OPY_Monitor*)theInstance)->id(),theServiceName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_subscribeServiceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;
  char* theSubscriptionName = 0;

  if (!PyArg_ParseTuple(theArgs,"s|s",&theServiceAddress,&theSubscriptionName))
    return 0;

  OTC_ServiceStatus theStatus;

  theStatus = OTC_SVBroker::registry()->subscribeServiceAddress(
   ((OPY_Monitor*)theInstance)->id(),theServiceAddress,theSubscriptionName);

  return Py_BuildValue("i",theStatus);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_unsubscribeServiceAddress(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceAddress = 0;
  char* theSubscriptionName = 0;

  if (!PyArg_ParseTuple(theArgs,"s|s",&theServiceAddress,&theSubscriptionName))
    return 0;

  OTC_SVBroker::registry()->unsubscribeServiceAddress(
   ((OPY_Monitor*)theInstance)->id(),theServiceAddress,theSubscriptionName);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_subscribeServiceGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker::registry()->subscribeServiceGroup(
   ((OPY_Monitor*)theInstance)->id(),theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Monitor::mfn_unsubscribeServiceGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker::registry()->unsubscribeServiceGroup(
   ((OPY_Monitor*)theInstance)->id(),theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OPY_Monitor::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*)theEvent;

    enterPython();

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_REGISTRY)
    {
      OPY_ServiceBinding* theObject;
      theObject = new OPY_ServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theGroup = Py_None;
      Py_XINCREF(theGroup);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleRegistry","OOi",theObject,theGroup,
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_NAME)
    {
      OPY_ServiceBinding* theObject;
      theObject = new OPY_ServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceName","Oi",theObject,
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OPY_ServiceBinding* theObject;
      theObject = new OPY_ServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      Py_XINCREF(Py_None);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceAddress","OOis",theObject,Py_None,
       theAnnouncement->status(),
       theAnnouncement->subscriptionName().string());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }

    leavePython();
  }
  else if (theEvent->type() == OTCEV_GroupAnnouncement::typeId())
  {
    OTCEV_GroupAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_GroupAnnouncement*)theEvent;

    enterPython();

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_REGISTRY)
    {
      OPY_ServiceBinding* theObject;
      theObject = new OPY_ServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleRegistry","Osi",theObject,
       theAnnouncement->serviceGroup().string(),
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OPY_ServiceBinding* theObject;
      theObject = new OPY_ServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceAddress","Osis",theObject,
       theAnnouncement->serviceGroup().string(),
       theAnnouncement->status(),
       theAnnouncement->subscriptionName().string());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_GROUP)
    {
      OPY_ServiceBinding* theObject;
      theObject = new OPY_ServiceBinding(
       theAnnouncement->serviceBinding());
      OTCLIB_ASSERT_M(theObject != 0);

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       "_handleServiceGroup","Osi",theObject,
       theAnnouncement->serviceGroup().string(),
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);

      Py_XDECREF(theObject);
    }

    leavePython();
  }
  else
  {
    OPY_Agent::handle(theEvent);

    return;
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
