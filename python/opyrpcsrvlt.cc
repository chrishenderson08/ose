/*
// ============================================================================
//
// = FILENAME
//     opyrpcsrvlt.cc
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

#include "rpcsrvlt.hh"
#include "servbind.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_RpcServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "RpcServlet",
  sizeof(OPY_RpcServlet),
  0,
  OPY_RpcServlet::sys_dealloc,
  0,
  OPY_RpcServlet::sys_getattr,
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
PyMethodDef OPY_RpcServlet::gMethods_[] = {
  { "instance", OPY_RpcServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_RpcServlet::~OPY_RpcServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_RpcServlet::OPY_RpcServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_RpcServlet(theSession,theBinding), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_RpcServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;
  PyObject* theBindingObject = 0;

  if (!PyArg_ParseTuple(theArgs,"OOO",&theOuter,
   &theObject,&theBindingObject))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OTC_ServiceBinding* theBinding;
  theBinding = ((OPY_ServiceBinding*)theBindingObject)->binding();

  OPY_RpcServlet* theServlet;
  theServlet = new OPY_RpcServlet(theOuter,theSession,theBinding);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_RpcServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_RpcServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_RpcServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_RpcServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_RpcServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_RpcServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_RpcServlet* theSelf;
  theSelf = (OPY_RpcServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OPY_RpcServlet::destroy()
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
