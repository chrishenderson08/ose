/*
// ============================================================================
//
// = FILENAME
//     opyhttpsssn.cc
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

#include "httpsssn.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_HttpSession::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpSession",
  sizeof(OPY_HttpSession),
  0,
  OPY_HttpSession::sys_dealloc,
  0,
  OPY_HttpSession::sys_getattr,
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
PyMethodDef OPY_HttpSession::gMethods_[] = {
  { "instance", OPY_HttpSession::mfn_instance, 1 },
  { "contentLength", OPY_HttpSession::mfn_contentLength, 1 },
  { "contentType", OPY_HttpSession::mfn_contentType, 1 },
  { "requestMethod", OPY_HttpSession::mfn_requestMethod, 1 },
  { "url", OPY_HttpSession::mfn_url, 1 },
  { "protocolVersion", OPY_HttpSession::mfn_protocolVersion, 1 },
  { "serverHost", OPY_HttpSession::mfn_serverHost, 1 },
  { "serverPort", OPY_HttpSession::mfn_serverPort, 1 },
  { "requestPath", OPY_HttpSession::mfn_requestPath, 1 },
  { "queryString", OPY_HttpSession::mfn_queryString, 1 },
  { "clientHost", OPY_HttpSession::mfn_clientHost, 1 },
  { "clientPort", OPY_HttpSession::mfn_clientPort, 1 },
  { "remoteUser", OPY_HttpSession::mfn_remoteUser, 1 },
  { "headers", OPY_HttpSession::mfn_headers, 1 },
  { "containsHeader", OPY_HttpSession::mfn_containsHeader, 1 },
  { "header", OPY_HttpSession::mfn_header, 1 },
  { "serverRoot", OPY_HttpSession::mfn_serverRoot, 1 },
  { "serverPath", OPY_HttpSession::mfn_serverPath, 1 },
  { "servletPath", OPY_HttpSession::mfn_servletPath, 1 },
  { "servletInfo", OPY_HttpSession::mfn_servletInfo, 1 },
  { "bindServlet", OPY_HttpSession::mfn_bindServlet, 1 },
  { "sessionId", OPY_HttpSession::mfn_sessionId, 1 },
  { "sessionContext", OPY_HttpSession::mfn_sessionContext, 1 },
  { "bindSession", OPY_HttpSession::mfn_bindSession, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_HttpSession::~OPY_HttpSession()
{
  session_->unReference();
}

/* ------------------------------------------------------------------------- */
OPY_HttpSession::OPY_HttpSession(OTC_HttpSession* theSession)
  : session_(theSession)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  session_->reference();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_HttpSession::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_HttpSession*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_HttpSession*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_contentLength(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("i",theSelf->contentLength());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_contentType(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->contentType().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_requestMethod(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->requestMethod().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_url(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->url().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_protocolVersion(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->protocolVersion().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_serverHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_serverPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_requestPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->requestPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_queryString(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->queryString().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_clientHost(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->clientHost().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_clientPort(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->clientPort().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_remoteUser(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->remoteUser().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_headers(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

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
PyObject* OPY_HttpSession::mfn_containsHeader(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  if (theSelf->containsHeader(theHeader))
    return Py_BuildValue("i",1);

  return Py_BuildValue("i",0);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_header(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theHeader;

  if (!PyArg_ParseTuple(theArgs,"s",&theHeader))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

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
PyObject* OPY_HttpSession::mfn_serverRoot(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverRoot().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_serverPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->serverPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_servletPath(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->servletPath().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_servletInfo(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->servletInfo().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_bindServlet(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theServletPath;
  char const* theServletInfo;

  if (!PyArg_ParseTuple(theArgs,"ss",&theServletPath,&theServletInfo))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  theSelf->bindServlet(theServletPath,theServletInfo);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_sessionId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->sessionId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_sessionContext(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  return Py_BuildValue("s",theSelf->sessionContext().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpSession::mfn_bindSession(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theSessionId;
  char const* theSessionContext = 0;

  if (!PyArg_ParseTuple(theArgs,"s|z",&theSessionId,&theSessionContext))
    return 0;

  OTC_HttpSession* theSelf;
  theSelf = ((OPY_HttpSession*)theInstance)->session_;

  theSelf->bindSession(theSessionId,theSessionContext);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
