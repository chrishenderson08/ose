/*
// ============================================================================
//
// = FILENAME
//     opyfilesrvl.cc
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

#include "filesrvl.hh"
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
    TAG = new OTC_TraceTag("OPY_FileServlet");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif


/* ------------------------------------------------------------------------- */
PyTypeObject OPY_FileServlet::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "FileServlet",
  sizeof(OPY_FileServlet),
  0,
  OPY_FileServlet::sys_dealloc,
  0,
  OPY_FileServlet::sys_getattr,
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
PyMethodDef OPY_FileServlet::gMethods_[] = {
  { "instance", OPY_FileServlet::mfn_instance, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_FileServlet::~OPY_FileServlet()
{
  outer_ = 0;
}

/* ------------------------------------------------------------------------- */
OPY_FileServlet::OPY_FileServlet(
 PyObject* theOuter,
 OTC_HttpSession* theSession,
 char const* theFileName,
 char const* theFileType
)
  : OTC_FileServlet(theSession,theFileName,theFileType),
    outer_(theOuter)
{
  ob_type = &gType_;

  _Py_NewReference(this);

  Py_XINCREF(outer_);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServlet::create(PyObject* theModule, PyObject* theArgs)
{
  PyObject* theOuter = 0;
  PyObject* theObject = 0;
  char const* theFileName = 0;
  char const* theFileType = 0;

  if (!PyArg_ParseTuple(theArgs,"OOsz",&theOuter,
   &theObject,&theFileName,&theFileType))
    return 0;

  OTC_HttpSession* theSession;
  theSession = (OTC_HttpSession*)PyCObject_AsVoidPtr(theObject);

  OPY_FileServlet* theServlet;
  theServlet = new OPY_FileServlet(theOuter,
   theSession,theFileName,theFileType);
  OTCLIB_ASSERT_M(theServlet != 0);

  return (PyObject*)theServlet;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServlet::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_FileServlet::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_FileServlet*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServlet::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_FileServlet*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_FileServlet::mfn_instance(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_FileServlet* theSelf;
  theSelf = (OPY_FileServlet*)theInstance;

  return PyCObject_FromVoidPtr((void*)theSelf,0);
}

/* ------------------------------------------------------------------------- */
void OPY_FileServlet::destroy()
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_FileServlet::destroy()");

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
