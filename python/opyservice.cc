/*
// ============================================================================
//
// = FILENAME
//     opyservice.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "service.hh"
#include "servbind.hh"
#include "report.hh"
#include "request.hh"
#include "response.hh"
#include "sbscrptn.hh"
#include "pbntfctn.hh"
#include "failure.hh"
#include "opaque.hh"
#include "dispatch.hh"

#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/grpannc.hh>

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Service::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Service",
  sizeof(OPY_Service),
  0,
  OPY_Service::sys_dealloc,
  0,
  OPY_Service::sys_getattr,
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
PyMethodDef OPY_Service::gMethods_[] = {
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
  { "scheduleCronAction",
    OPY_Agent::mfn_scheduleCronAction, 1 },
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
  { "serviceBinding",
    OPY_Service::mfn_serviceBinding, 1 },
  { "serviceVisibility",
    OPY_Service::mfn_serviceVisibility, 1 },
  { "announceAvailability",
    OPY_Service::mfn_announceAvailability, 1 },
  { "joinGroup",
    OPY_Service::mfn_joinGroup, 1 },
  { "leaveGroup",
    OPY_Service::mfn_leaveGroup, 1 },
  { "sendRequest",
    OPY_Service::mfn_sendRequest, 1 },
  { "sendMessage",
    OPY_Service::mfn_sendMessage, 1 },
  { "subscribeReports",
    OPY_Service::mfn_subscribeReports, 1 },
  { "unsubscribeReports",
    OPY_Service::mfn_unsubscribeReports, 1 },
  { "publishReport",
    OPY_Service::mfn_publishReport, 1 },
  { "sendReport",
    OPY_Service::mfn_sendReport, 1 },
  { "expireReports",
    OPY_Service::mfn_expireReports, 1 },
  { "expireAllReports",
    OPY_Service::mfn_expireAllReports, 1 },
  { "purgeReports",
    OPY_Service::mfn_purgeReports, 1 },
  { "matchSubject",
    OPY_Service::mfn_matchSubject, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Service::~OPY_Service()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OPY_Service::OPY_Service(
 PyObject* theOuter,
 char const* theServiceName,
 char const* theServiceAudience,
 OTC_ServiceVisibility theServiceVisibility
)
  : OPY_Monitor(theOuter),
    broker_(this,theServiceName,theServiceAudience,theServiceVisibility)
{
  ob_type = &gType_;

  // _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  char* theServiceName = 0;
  char* theServiceAudience = 0;
  int theServiceVisibility = 0;

  if (!PyArg_ParseTuple(theArgs,"Ossi",&theOuter,
   &theServiceName,&theServiceAudience,&theServiceVisibility))
  {
    return 0;
  }

  OPY_Service* theAgent;
  theAgent = new OPY_Service(theOuter,theServiceName,
   theServiceAudience,OTC_ServiceVisibility(theServiceVisibility));
  OTCLIB_ASSERT_M(theAgent != 0);

  return (PyObject*)theAgent;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Service::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Service*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Service*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_serviceBinding(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_Service* theSelf;
  theSelf = (OPY_Service*)theInstance;

  OPY_ServiceBinding* theBinding;
  theBinding = new OPY_ServiceBinding(
   theSelf->broker_.serviceBinding());
  OTCLIB_ASSERT_M(theBinding != 0);

  return theBinding;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_serviceVisibility(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_Service* theSelf;
  theSelf = (OPY_Service*)theInstance;

  return Py_BuildValue("i",int(theSelf->broker_.serviceVisibility()));
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_announceAvailability(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_Service* theSelf;
  theSelf = (OPY_Service*)theInstance;

  theSelf->broker_.announceAvailability();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_joinGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->joinGroup(theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_leaveGroup(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theServiceGroup = 0;

  if (!PyArg_ParseTuple(theArgs,"s",&theServiceGroup))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->leaveGroup(theServiceGroup);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_sendRequest(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char* theMethodName = 0;
  PyObject* thePayloadObject = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"OsOO",
   &theBindingObject,&theMethodName,&thePayloadObject,&theEncoder))
  {
    return 0;
  }

  if (!OPY_ServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  OTC_ServiceBinding* theBinding;
  theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

  OTC_SVPayload thePayload;

  if (!objectToPayload(thePayloadObject,thePayload,theEncoder))
    return 0;

  OTC_String theConversationId;

  theConversationId = theBroker->sendRequest(theBinding,
   theMethodName,thePayload);

  return Py_BuildValue("s",theConversationId.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_sendMessage(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char* theMessageId = 0;
  char* theDescription = 0;
  char* theContent = 0;
  int theContentLength = 0;
  char* theContentType = 0;
  int theSequenceNumber = 0;

  if (!PyArg_ParseTuple(theArgs,"Osss#si",
   &theBindingObject,&theMessageId,&theDescription,&theContent,
   &theContentLength,&theContentType,&theSequenceNumber))
  {
    return 0;
  }

  if (!OPY_ServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  OTC_ServiceBinding* theBinding;
  theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

  OTC_String theConversationId;

  theConversationId = theBroker->sendMessage(theBinding,theMessageId,
   theDescription,theContent,theContentLength,theContentType,
   theSequenceNumber);

  return Py_BuildValue("s",theConversationId.string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_subscribeReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;

  char* theServiceName = 0;
  char* theSubjectPattern = 0;

#if 0
  if (!PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->subscribeReports(theServiceName,theSubjectPattern);
#endif

  if (PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
  {
    OTC_SVBroker* theBroker;
    theBroker = &((OPY_Service*)theInstance)->broker_;

    theBroker->subscribeReports(theServiceName,theSubjectPattern);
  }
  else if (PyArg_ParseTuple(theArgs,"Os",&theBindingObject,
   &theSubjectPattern) && OPY_ServiceBinding::isType(theBindingObject))
  {
    PyErr_Clear();

    OTC_SVBroker* theBroker;
    theBroker = &((OPY_Service*)theInstance)->broker_;

    OTC_ServiceBinding* theBinding;
    theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

    theBroker->subscribeReports(theBinding,theSubjectPattern);
  }
  else
    return 0;

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_unsubscribeReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;

  char* theServiceName = 0;
  char* theSubjectPattern = 0;

#if 0
  if (!PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->unsubscribeReports(theServiceName,theSubjectPattern);
#endif

  if (PyArg_ParseTuple(theArgs,"ss",&theServiceName,&theSubjectPattern))
  {
    OTC_SVBroker* theBroker;
    theBroker = &((OPY_Service*)theInstance)->broker_;

    theBroker->unsubscribeReports(theServiceName,theSubjectPattern);
  }
  else if (PyArg_ParseTuple(theArgs,"Os",&theBindingObject,
   &theSubjectPattern) && OPY_ServiceBinding::isType(theBindingObject))
  {
    PyErr_Clear();

    OTC_SVBroker* theBroker;
    theBroker = &((OPY_Service*)theInstance)->broker_;

    OTC_ServiceBinding* theBinding;
    theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

    theBroker->unsubscribeReports(theBinding,theSubjectPattern);
  }
  else
    return 0;


  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_publishReport(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSubjectName = 0;
  PyObject* thePayloadObject = 0;
  int theLifeTime = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"sOiO",
   &theSubjectName,&thePayloadObject,&theLifeTime,&theEncoder))
  {
    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  OTC_SVPayload thePayload;

  if (!objectToPayload(thePayloadObject,thePayload,theEncoder))
    return 0;

  theBroker->publishReport(theSubjectName,
   thePayload,theLifeTime);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_sendReport(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theBindingObject = 0;
  char* theSubjectName = 0;
  PyObject* thePayloadObject = 0;
  PyObject* theEncoder = 0;

  if (!PyArg_ParseTuple(theArgs,"OsOO",
   &theBindingObject,&theSubjectName,&thePayloadObject,&theEncoder))
  {
    return 0;
  }

  if (!OPY_ServiceBinding::isType(theBindingObject))
  {
    PyErr_SetString(PyExc_TypeError,
     "agent description not a service binding object");

    return 0;
  }

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  OTC_ServiceBinding* theBinding;
  theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

  OTC_SVPayload thePayload;

  if (!objectToPayload(thePayloadObject,thePayload,theEncoder))
    return 0;

  OTC_String theConversationId;

  theBroker->sendReport(theBinding,theSubjectName,thePayload);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_expireReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSubjectPattern = 0;
  int theAge = 0;

  if (!PyArg_ParseTuple(theArgs,"si",&theSubjectPattern,&theAge))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->expireReports(theSubjectPattern,theAge);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_expireAllReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theAge = 0;

  if (!PyArg_ParseTuple(theArgs,"i",&theAge))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->expireReports(theAge);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_purgeReports(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  theBroker->purgeReports();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::mfn_matchSubject(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char* theSubjectPattern = 0;
  char* theSubjectName = 0;

  if (!PyArg_ParseTuple(theArgs,"ss",&theSubjectPattern,&theSubjectName))
    return 0;

  OTC_SVBroker* theBroker;
  theBroker = &((OPY_Service*)theInstance)->broker_;

  bool theResult;
  theResult = theBroker->matchSubject(theSubjectPattern,theSubjectName);

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
void OPY_Service::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_PublisherNotification::typeId())
  {
    OTCEV_PublisherNotification* theNotification;
    theNotification = (OTCEV_PublisherNotification*)theEvent;

    enterPython();

    OPY_PublisherNotification* theObject;
    theObject = new OPY_PublisherNotification(theNotification);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handlePublisherNotification","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Subscription::typeId())
  {
    OTCEV_Subscription* theSubscription;
    theSubscription = (OTCEV_Subscription*)theEvent;

    enterPython();

    OPY_Subscription* theObject;
    theObject = new OPY_Subscription(theSubscription);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleSubscription","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Report::typeId())
  {
    OTCEV_Report* theReport;
    theReport = (OTCEV_Report*)theEvent;

    enterPython();

    OPY_Report* theObject;
    theObject = new OPY_Report(theReport);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleReport","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Opaque::typeId())
  {
    OTCEV_Opaque* theMessage;
    theMessage = (OTCEV_Opaque*)theEvent;

    enterPython();

    OPY_Opaque* theObject;
    theObject = new OPY_Opaque(theMessage);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleMessage","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    enterPython();

    OPY_Request* theObject;
    theObject = new OPY_Request(theRequest);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleRequest","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    enterPython();

    OPY_Response* theObject;
    theObject = new OPY_Response(theResponse);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleResponse","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    enterPython();

    OPY_Failure* theObject;
    theObject = new OPY_Failure(theFailure);
    OTCLIB_ASSERT_M(theObject != 0);

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleFailure","O",theObject);

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    Py_XDECREF(theObject);

    leavePython();
  }
  else if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
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

#if 0
      PyObject* theGroup = Py_None;
      Py_XINCREF(theGroup);
#endif

      PyObject* theResult;
      theResult = PyObject_CallMethod(outer_,
       // "_handleRegistry","OOi",theObject,theGroup,
       "_handleRegistry","OOi",theObject,Py_None,
       theAnnouncement->status());

      if (theResult == 0)
      {
	PyErr_Print();
	PyErr_Clear();
      }
      else
	Py_XDECREF(theResult);
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

#if 0
      Py_XINCREF(Py_None);
#endif

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
    OPY_Monitor::handle(theEvent);

    return;
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Service::payloadToObject(
 OTC_ROPayload const& thePayload,
 PyObject* theCallback
)
{
  if (thePayload.nodeType() == "invalid")
  {
    PyErr_SetString(PyExc_TypeError,"invalid payload");

    return 0;
  }
  else if (thePayload.nodeType() == "scalar")
  {
    if (thePayload.valueType() == "xsd:string")
    {
      return Py_BuildValue("s#",
       thePayload.value().string(),
       thePayload.value().length());
    }
    else if (thePayload.valueType() == "xsd:integer" ||
     thePayload.valueType() == "xsd:long" ||
     thePayload.valueType() == "xsd:int" ||
     thePayload.valueType() == "xsd:short" ||
     thePayload.valueType() == "xsd:byte" ||
     thePayload.valueType() == "xsd:unsignedLong" ||
     thePayload.valueType() == "xsd:unsignedInt" ||
     thePayload.valueType() == "xsd:unsignedShort" ||
     thePayload.valueType() == "xsd:unsignedByte")
    {
      bool theStatus;
      theStatus = true;

      int theValue = 0;

      (thePayload,theStatus) >>= theValue;

      if (theStatus == false)
      {
	PyObject* theObject;
	char* theNext = 0;

	theObject = PyLong_FromString(
	 (char*)thePayload.value().string(),&theNext,10);

	if (theObject == 0)
	  return 0;

	int theLength;
	theLength = theNext - thePayload.value().string();

	if (theNext == 0 || *theNext != EOS ||
	 theLength != int(thePayload.value().length()))
	{
	  PyErr_SetString(PyExc_TypeError,"int conversion failed");

	  return 0;
	}

	return theObject;
      }

      return Py_BuildValue("i",theValue);
    }
    else if (thePayload.valueType() == "xsd:double" ||
     thePayload.valueType() == "xsd:float")
    {
      bool theStatus;
      theStatus = true;

      double theValue = 0;

      (thePayload,theStatus) >>= theValue;

      if (theStatus == false)
      {
	PyErr_SetString(PyExc_TypeError,"float conversion failed");

	return 0;
      }

      return Py_BuildValue("d",theValue);
    }
    else
    {
      PyObject* theArgs;
      theArgs = Py_BuildValue("(s,s#)",
       thePayload.valueType().string(),
       thePayload.value().string(),
       thePayload.value().length());

      PyObject* theResult;
      theResult = PyEval_CallObject(theCallback,theArgs);

      Py_XDECREF(theArgs);

      if (theResult == 0)
	return 0;

      return theResult;
    }
  }
  else if (thePayload.nodeType() == "index")
  {
    PyObject* theDictionary;
    theDictionary = PyDict_New();

    if (theDictionary == 0)
      return 0;

    OTC_ROPayload theElement;
    theElement = thePayload.firstChild();

    while (theElement.isValid())
    {
      PyObject* theKey = 0;
      PyObject* theValue = 0;

      theKey = PyString_FromStringAndSize(
       theElement.nodeName().string(),theElement.nodeName().length());
      theValue = payloadToObject(theElement,theCallback);

      if (theValue == 0)
      {
	Py_XDECREF(theKey);
	Py_XDECREF(theDictionary);

	return 0;
      }

      if (PyDict_SetItem(theDictionary,theKey,theValue) == -1)
      {
	Py_XDECREF(theKey);
	Py_XDECREF(theValue);
	Py_XDECREF(theDictionary);

	return 0;
      }

      Py_XDECREF(theKey);
      Py_XDECREF(theValue);

      theElement = theElement.nextSibling();
    }

    return theDictionary;
  }
  else if (thePayload.nodeType() == "array")
  {
    PyObject* theList;
    theList = PyList_New(0);

    if (theList == 0)
      return 0;

    OTC_ROPayload theElement;
    theElement = thePayload.firstChild();

    while (theElement.isValid())
    {
      PyObject* theValue = 0;

      theValue = payloadToObject(theElement,theCallback);

      if (theValue == 0)
      {
	Py_XDECREF(theList);

	return 0;
      }

      if (PyList_Append(theList,theValue) == -1)
      {
	Py_XDECREF(theValue);
	Py_XDECREF(theList);

	return 0;
      }

      Py_XDECREF(theValue);

      theElement = theElement.nextSibling();
    }

    return theList;
  }

  PyErr_SetString(PyExc_TypeError,"invalid payload element");

  return 0;
}

/* ------------------------------------------------------------------------- */
bool OPY_Service::objectToPayload(
 PyObject* theObject,
 OTC_RWPayload thePayload,
 PyObject* theCallback
)
{
  if (PyString_Check(theObject))
  {
    char* theString = 0;
    int theLength = 0;

    if (!PyArg_Parse(theObject,"s#",&theString,&theLength))
      return false;

    thePayload.assign(theString,theLength);

    return true;
  }
#if defined(PyBool_Check)
  else if (PyInt_Check(theObject) && !PyBool_Check(theObject))
#else
  else if (PyInt_Check(theObject))
#endif
  {
    int theValue = 0;

    if (!PyArg_Parse(theObject,"i",&theValue))
      return false;

    thePayload <<= theValue;

    return true;
  }
  else if (PyFloat_Check(theObject))
  {
    double theValue = 0;

    if (!PyArg_Parse(theObject,"d",&theValue))
      return false;

    thePayload <<= theValue;

    return true;
  }
  else if (theObject == Py_None)
  {
    thePayload <<= OTC_ROPayload::nullValue();

    return true;
  }
  else if (PyDict_Check(theObject))
  {
    thePayload <<= OTC_ROPayload::nullIndex();

    PyObject* theItems;
    theItems = PyDict_Items(theObject);

    int theSize = PyList_Size(theItems);

    for (int i=0; i<theSize; i++)
    {
      PyObject* theKey;
      PyObject* theValue;
      PyObject* theItem;

      theItem = PyList_GET_ITEM(theItems,i);

      theKey = PyTuple_GET_ITEM(theItem,0);
      theValue = PyTuple_GET_ITEM(theItem,1);

      char const* theKeyString = 0;
      u_int theKeyLength = 0;

      if (!PyArg_Parse(theKey,"s#",&theKeyString,&theKeyLength))
      {
	Py_XDECREF(theItems);

	return false;
      }

      if (!objectToPayload(theValue,
       thePayload.sibling(theKeyString,theKeyLength),theCallback))
      {
	Py_XDECREF(theItems);

	return false;
      }
    }

    Py_XDECREF(theItems);

    return true;
  }
  else if (PyList_Check(theObject))
  {
    thePayload <<= OTC_ROPayload::nullArray();

    int theSize = PyList_Size(theObject);

    for (int i=0; i<theSize; i++)
    {
      PyObject* theItem;
      theItem = PyList_GET_ITEM(theObject,i);
      if (!objectToPayload(theItem,thePayload[-1],theCallback))
	return false;
    }

    return true;
  }
  else if (PyTuple_Check(theObject))
  {
    thePayload <<= OTC_ROPayload::nullArray();

    int theSize = PyTuple_Size(theObject);

    for (int i=0; i<theSize; i++)
    {
      PyObject* theItem;
      theItem = PyTuple_GET_ITEM(theObject,i);
      if (!objectToPayload(theItem,thePayload[-1],theCallback))
	return false;
    }

    return true;
  }
  else if (PyLong_Check(theObject))
  {
    PyObject* theString;
    theString = PyObject_Str(theObject);

    if (theString == 0)
      return false;

    char* tmpString = 0;
    tmpString = PyString_AsString(theString);

    if (tmpString == 0 || *tmpString == EOS)
    {
      Py_XDECREF(theString);

      return false;
    }

    thePayload.assign(tmpString,"xsd:long");

    Py_XDECREF(theString);

    return true;
  }
  else
  {
    PyObject* theArgs;
    theArgs = Py_BuildValue("(O)",theObject);

    PyObject* theResult;
    theResult = PyEval_CallObject(theCallback,theArgs);

    Py_XDECREF(theArgs);

    if (theResult == 0)
      return false;

    if (!PyTuple_Check(theResult) || PyTuple_Size(theResult) != 2)
    {
      Py_XDECREF(theResult);

      PyErr_SetString(PyExc_RuntimeError,"invalid result from encoder");

      return false;
    }

    PyObject* theItem;
    theItem = PyTuple_GET_ITEM(theResult,0);

    if (theItem == Py_None)
    {
      theItem = PyTuple_GET_ITEM(theResult,1);

      if (theItem != theObject)
      {
	// Data has been translated so need to
	// represent it to the encoder.

	bool tmpResult;
	tmpResult = objectToPayload(theItem,thePayload,theCallback);

	Py_XDECREF(theResult);

	return tmpResult;
      }
      else
      {
	Py_XDECREF(theResult);

	PyErr_SetString(PyExc_RuntimeError,"invalid result from encoder");

	return false;
      }
    }

    char* theType = 0;
    char* theString = 0;
    int theLength = 0;

    if (!PyArg_ParseTuple(theResult,"ss#",&theType,&theString,&theLength))
    {
      Py_XDECREF(theResult);

      PyErr_SetString(PyExc_RuntimeError,"invalid result from encoder");

      return false;
    }

    thePayload.assign(theString,theLength,theType);

    Py_XDECREF(theResult);

    return true;
  }

  PyErr_SetString(PyExc_TypeError,"object type not supported");

  return false;
}

/* ------------------------------------------------------------------------- */
