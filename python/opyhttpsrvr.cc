/*
// ============================================================================
//
// = FILENAME
//     opyhttpsrvr.cc
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

#include "httpsrvr.hh"
#include "httpsssn.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_HttpServer::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpServer",
  sizeof(OPY_HttpServer),
  0,
  OPY_HttpServer::sys_dealloc,
  0,
  OPY_HttpServer::sys_getattr,
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
PyMethodDef OPY_HttpServer::gMethods_[] = {
  { "instance", OPY_HttpServer::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_HttpServer::~OPY_HttpServer()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_HttpServer::OPY_HttpServer(PyObject* theOuter)
  : outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServer::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theOuter))
    return 0;

  OPY_HttpServer* theDaemon;
  theDaemon = new OPY_HttpServer(theOuter);
  OTCLIB_ASSERT_M(theDaemon != 0);

  return (PyObject*)theDaemon;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServer::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_HttpServer::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_HttpServer*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServer::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_HttpServer*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpServer::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpServer* theSelf;
  theSelf = (OPY_HttpServer*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
bool OPY_HttpServer::authorise(
 char const* theLogin,
 char const* thePassword
)
{
  OPY_Dispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","ss",theLogin,thePassword);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OPY_Dispatcher::leavePython();

    return false;
  }

  bool theAnswer;
  theAnswer = true;

  if (theResult == Py_None)
    theAnswer = false;

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();

  return theAnswer;
}

/* ------------------------------------------------------------------------- */
OTC_HttpServlet* OPY_HttpServer::servlet(OTC_HttpSession* theSession)
{
  OPY_Dispatcher::enterPython();

  PyObject* theObject;
  theObject = new OPY_HttpSession(theSession);
  OTCLIB_ASSERT_M(theObject != 0);

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,"_servlet","O",theObject);

  Py_XDECREF(theObject);

  if (theResult == 0)
  {
    PyErr_Print();
    PyErr_Clear();

    OPY_Dispatcher::leavePython();

    return 0;
  }

  OTC_HttpServlet* theServlet = 0;

  if (theResult != Py_None)
    theServlet = (OTC_HttpServlet*)PyCObject_AsVoidPtr(theResult);

  Py_XDECREF(theResult);

  OPY_Dispatcher::leavePython();

  return theServlet;
}

/* ------------------------------------------------------------------------- */
