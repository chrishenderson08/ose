/*
// ============================================================================
//
// = FILENAME
//     opyhttpdaem.cc
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

#include "httpdaem.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_HttpDaemon::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "HttpDaemon",
  sizeof(OPY_HttpDaemon),
  0,
  OPY_HttpDaemon::sys_dealloc,
  0,
  OPY_HttpDaemon::sys_getattr,
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
PyMethodDef OPY_HttpDaemon::gMethods_[] = {
  { "port", OPY_HttpDaemon::mfn_port, 1 },
  { "start", OPY_HttpDaemon::mfn_start, 1 },
  { "stop", OPY_HttpDaemon::mfn_stop, 1 },
  { "attach", OPY_HttpDaemon::mfn_attach, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_HttpDaemon::~OPY_HttpDaemon()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_HttpDaemon::OPY_HttpDaemon(PyObject* theOuter, int thePort)
  : OTC_HttpDaemon(thePort), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  int thePort;

  if (!PyArg_ParseTuple(theArgs,"Oi",&theOuter,&thePort))
    return 0;

  OPY_HttpDaemon* theDaemon;
  theDaemon = new OPY_HttpDaemon(theOuter,thePort);
  OTCLIB_ASSERT_M(theDaemon != 0);

  return (PyObject*)theDaemon;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_HttpDaemon::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_HttpDaemon*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_HttpDaemon*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::mfn_port(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OPY_HttpDaemon*)theInstance;

  return Py_BuildValue("i",theSelf->port());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::mfn_start(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OPY_HttpDaemon*)theInstance;

  theSelf->start();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::mfn_stop(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OPY_HttpDaemon*)theInstance;

  theSelf->stop();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_HttpDaemon::mfn_attach(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  char const* theServerPath;
  PyObject* theObject;

  if (!PyArg_ParseTuple(theArgs,"sO",&theServerPath,&theObject))
    return 0;

  OTC_HttpDaemon* theSelf;
  theSelf = (OPY_HttpDaemon*)theInstance;

  OTC_HttpServer* theServer;
  theServer = (OTC_HttpServer*)PyCObject_AsVoidPtr(theObject);

  theSelf->attach(theServerPath,theServer);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
bool OPY_HttpDaemon::authorise(
 OTC_String const& theClientHost
)
{
  OPY_Dispatcher::enterPython();

  PyObject* theResult;
  theResult = PyObject_CallMethod(outer_,
   "_authorise","s",theClientHost.string());

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
