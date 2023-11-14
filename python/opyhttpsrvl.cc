/*
// ============================================================================
//
// = FILENAME
//     opyhttpsrvl.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef ANY
#undef ANY
#endif

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/endpoint.hh>
#include <OTC/message/congest.hh>

#include "httpsrvl.hh"
#include "dispatch.hh"

#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
#ifdef OTCLIB_TRACE
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OPY_HttpServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif


/* ------------------------------------------------------------------------- */
PyTypeObject OPY_HttpServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpServlet",
  sizeof(OPY_HttpServlet),
  0,
  OPY_HttpServlet::sys_dealloc,
  0,
  OPY_HttpServlet::sys_getattr,
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
PyMethodDef OPY_HttpServlet::gMethods_[] = {
  { "instance", OPY_HttpServlet::mfn_instance, 1 },
  { "contentLength", OPY_HttpServlet::mfn_contentLength, 1 },
  { "contentType", OPY_HttpServlet::mfn_contentType, 1 },
  { "requestMethod", OPY_HttpServlet::mfn_requestMethod, 1 },
  { "url", OPY_HttpServlet::mfn_url, 1 },
  { "protocolVersion", OPY_HttpServlet::mfn_protocolVersion, 1 },
  { "serverHost", OPY_HttpServlet::mfn_serverHost, 1 },
  { "serverPort", OPY_HttpServlet::mfn_serverPort, 1 },
  { "requestPath", OPY_HttpServlet::mfn_requestPath, 1 },
  { "queryString", OPY_HttpServlet::mfn_queryString, 1 },
  { "clientHost", OPY_HttpServlet::mfn_clientHost, 1 },
  { "clientPort", OPY_HttpServlet::mfn_clientPort, 1 },
  { "remoteUser", OPY_HttpServlet::mfn_remoteUser, 1 },
  { "headers", OPY_HttpServlet::mfn_headers, 1 },
  { "containsHeader", OPY_HttpServlet::mfn_containsHeader, 1 },
  { "header", OPY_HttpServlet::mfn_header, 1 },
  { "serverRoot", OPY_HttpServlet::mfn_serverRoot, 1 },
  { "serverPath", OPY_HttpServlet::mfn_serverPath, 1 },
  { "servletPath", OPY_HttpServlet::mfn_servletPath, 1 },
  { "servletInfo", OPY_HttpServlet::mfn_servletInfo, 1 },
  { "sessionId", OPY_HttpServlet::mfn_sessionId, 1 },
  { "sessionContext", OPY_HttpServlet::mfn_sessionContext, 1 },
  { "sendResponse", OPY_HttpServlet::mfn_sendResponse, 1 },
  { "sendHeader", OPY_HttpServlet::mfn_sendHeader, 1 },
  { "endHeaders", OPY_HttpServlet::mfn_endHeaders, 1 },
  { "sendContent", OPY_HttpServlet::mfn_sendContent, 1 },
  { "endContent", OPY_HttpServlet::mfn_endContent, 1 },
  { "sendError", OPY_HttpServlet::mfn_sendError, 1 },
  { "sendComplete", OPY_HttpServlet::mfn_sendComplete, 1 },
  { "shutdown", OPY_HttpServlet::mfn_shutdown, 1 },
  { "statusText", OPY_HttpServlet::mfn_statusText, 1 },
  { "subscribeCongestion", OPY_HttpServlet::mfn_subscribeCongestion, 1 },
  { "unsubscribeCongestion", OPY_HttpServlet::mfn_unsubscribeCongestion, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_HttpServlet::~OPY_HttpServlet()
{
  outer_ = 0;

  endPoint()->removeObserver(id());
}

/* ------------------------------------------------------------------------- */
OPY_HttpServlet::OPY_HttpServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession
)
  : OTC_HttpServlet(theSession), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;

  if (!PyArg_ParseTuple(theArgs,"OO",&theOuter,&theObject))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OPY_HttpServlet* theServlet;
  theServlet = new OPY_HttpServlet(theOuter,theSession);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_HttpServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_HttpServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_HttpServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_contentLength(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("i",theSelf->contentLength());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_contentType(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->contentType().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_requestMethod(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->requestMethod().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_url(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->url().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_protocolVersion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->protocolVersion().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_serverHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_serverPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_requestPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->requestPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_queryString(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->queryString().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_clientHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->clientHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_clientPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->clientPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_remoteUser(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->remoteUser().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_headers(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  PyObject* theDictionary;
  theDictionary = PyDict_New();

  if (theDictionary == 0)
    return 0;

  OTC_PairIterator<OTC_String,OTC_String> theItems;
  theItems = theSelf->headers();
  while (theItems.isValid())
  {
    PyObject* theKey = 0;
    PyObject* theValue = 0;

    theKey = PyString_FromString(theItems.key().string());
    theValue = PyString_FromString(theItems.item().string());

    if (theKey == 0 || theValue == 0)
    {
      Py_XDECREF(theKey);
      Py_XDECREF(theValue);
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

    theItems.next();
  }

  return theDictionary;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_containsHeader(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  if (theSelf->containsHeader(theHeader))
    return Py_BuildValue("i",1);

  return Py_BuildValue("i",0);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_header(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  if (!theSelf->containsHeader(theHeader))
  {
    // PyErr_SetString(PyExc_KeyError,
    //  "no such header present in request");
    PyErr_SetString(PyExc_KeyError,theHeader);

    return 0;
  }

  return Py_BuildValue("s",theSelf->header(theHeader).string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_serverRoot(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverRoot().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_serverPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->serverPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_servletPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->servletPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_servletInfo(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->servletInfo().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sessionId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->sessionId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sessionContext(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->sessionContext().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sendResponse(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theStatus;
  char const* theMessage=0;

  if (!PyArg_ParseTuple(theArgs,"i|z",&theStatus,&theMessage))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendResponse(u_int(theStatus),theMessage);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sendHeader(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theKeyWord=0;
  char const* theValue=0;

  if (!PyArg_ParseTuple(theArgs,"ss",&theKeyWord,&theValue))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendHeader(theKeyWord,theValue);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_endHeaders(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->endHeaders();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sendContent(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theContent=0;
  int theLength;

  if (!PyArg_ParseTuple(theArgs,"s#",&theContent,&theLength))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendContent(OTC_String(theContent,theLength));

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_endContent(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->endContent();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sendError(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theStatus;
  char const* theContent=0;

  if (!PyArg_ParseTuple(theArgs,"i|z",&theStatus,&theContent))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendError(u_int(theStatus),theContent);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_sendComplete(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theResponse=0;
  int theLength;

  if (!PyArg_ParseTuple(theArgs,"s#",&theResponse,&theLength))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->sendComplete(OTC_String(theResponse,theLength));

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_shutdown(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theDelay = -1;

  if (!PyArg_ParseTuple(theArgs,"|i",&theDelay))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  Py_BEGIN_ALLOW_THREADS

  theSelf->shutdown(theDelay);

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_statusText(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  int theStatus;

  if (!PyArg_ParseTuple(theArgs,"i",&theStatus))
    return 0;

  OTC_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  return Py_BuildValue("s",theSelf->statusText(theStatus));
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_subscribeCongestion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  theSelf->endPoint()->addObserver(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServlet::mfn_unsubscribeCongestion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OPY_HttpServlet* theSelf;
  theSelf = (OPY_HttpServlet*)theInstance;

  theSelf->endPoint()->removeObserver(theSelf->id());

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
void OPY_HttpServlet::processRequest()
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_HttpServlet::processRequest()");

  OPY_Dispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_processRequest","");

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
void OPY_HttpServlet::processContent(OTC_String const& theContent)
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_HttpServlet::processContent()");

  OPY_Dispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_processContent","s#",theContent.string(),theContent.length());

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
void OPY_HttpServlet::destroy()
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_HttpServlet::destroy()");

  endPoint()->removeObserver(id());

  OPY_Dispatcher::enterPython();

  PyObject* theOuter;
  theOuter = outer_;

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_destroyServlet","");

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();
  }
  else
    Py_XDECREF(theResult);

  Py_XDECREF(theOuter);

  OPY_Dispatcher::leavePython();
}

/* ------------------------------------------------------------------------- */
void OPY_HttpServlet::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_HttpServlet::handle()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  if (theEvent->type() == OTCEV_Congestion::typeId())
  {
    OTCEV_Congestion* theCongestion;
    theCongestion = (OTCEV_Congestion*)theEvent;

    OPY_Dispatcher::enterPython();

    PyObject* theResult;
    theResult = PyObject_CallMethod(outer_,
     "_handleCongestion","ii",theCongestion->status(),
     theCongestion->pending());

    if (theResult == 0)
    {
      PyErr_Print();
      PyErr_Clear();
    }
    else
      Py_XDECREF(theResult);

    OPY_Dispatcher::leavePython();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
