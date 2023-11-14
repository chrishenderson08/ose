/*
// ============================================================================
//
// = FILENAME
//     opyechosrvl.cc
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

#include "echosrvl.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_EchoServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "EchoServlet",
  sizeof(OPY_EchoServlet),
  0,
  OPY_EchoServlet::sys_dealloc,
  0,
  OPY_EchoServlet::sys_getattr,
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
PyMethodDef OPY_EchoServlet::gMethods_[] = {
  { "instance", OPY_EchoServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_EchoServlet::~OPY_EchoServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_EchoServlet::OPY_EchoServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession
)
  : OTC_EchoServlet(theSession),
    outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_EchoServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;

  if (!PyArg_ParseTuple(theArgs,"OO",&theOuter,&theObject))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OPY_EchoServlet* theServlet;
  theServlet = new OPY_EchoServlet(theOuter,theSession);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_EchoServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_EchoServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_EchoServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_EchoServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_EchoServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_EchoServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_EchoServlet* theSelf;
  theSelf = (OPY_EchoServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OPY_EchoServlet::destroy()
{
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
