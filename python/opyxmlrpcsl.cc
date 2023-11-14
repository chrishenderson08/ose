/*
// ============================================================================
//
// = FILENAME
//     opyxmlrpcsl.cc
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

#include "xmlrpcsl.hh"
#include "servbind.hh"
#include "dispatch.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_XmlRpcServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "XmlRpcServlet",
  sizeof(OPY_XmlRpcServlet),
  0,
  OPY_XmlRpcServlet::sys_dealloc,
  0,
  OPY_XmlRpcServlet::sys_getattr,
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
PyMethodDef OPY_XmlRpcServlet::gMethods_[] = {
  { "instance", OPY_XmlRpcServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_XmlRpcServlet::~OPY_XmlRpcServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_XmlRpcServlet::OPY_XmlRpcServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 OTC_ServiceBinding* theBinding
)
  : OTC_XmlRpcServlet(theSession,theBinding), outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcServlet::create(PyObject* theModule, PyObject* theArgs)
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

  OPY_XmlRpcServlet* theServlet;
  theServlet = new OPY_XmlRpcServlet(theOuter,theSession,theBinding);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_XmlRpcServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_XmlRpcServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_XmlRpcServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_XmlRpcServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_XmlRpcServlet* theSelf;
  theSelf = (OPY_XmlRpcServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OPY_XmlRpcServlet::destroy()
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
