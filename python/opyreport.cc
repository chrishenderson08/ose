/*
// ============================================================================
//
// = FILENAME
//     opyreport.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "report.hh"
#include "servbind.hh"
#include "service.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Report::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Report",
  sizeof(OPY_Report),
  0,
  OPY_Report::sys_dealloc,
  0,
  OPY_Report::sys_getattr,
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
PyMethodDef OPY_Report::gMethods_[] = {
  { "publisher", OPY_Report::mfn_publisher, 1 },
  { "subjectName", OPY_Report::mfn_subjectName, 1 },
  { "subjectPattern", OPY_Report::mfn_subjectPattern, 1 },
  { "content", OPY_Report::mfn_content, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Report::~OPY_Report()
{
  report_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_Report::OPY_Report(OTCEV_Report* theReport)
  : report_(0)
{
  OTCLIB_ENSURE_FN((theReport != 0),
   "OPY_Report::OPY_Report(OTCEV_Report*)",
   "invalid report supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  report_ = (OTCEV_Report*)theReport->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Report::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Report::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Report*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Report::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Report*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Report::mfn_publisher(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_ServiceBinding* thePublisher;
  thePublisher = ((OPY_Report*)theInstance)->report_->publisher();

  OPY_ServiceBinding* theBinding;
  theBinding = new OPY_ServiceBinding(thePublisher);
  OTCLIB_ASSERT_M(theBinding != 0);

  return theBinding;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Report::mfn_subjectName(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Report* theReport;
  theReport = ((OPY_Report*)theInstance)->report_;

  return Py_BuildValue("s",
   theReport->subjectName().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Report::mfn_subjectPattern(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Report* theReport;
  theReport = ((OPY_Report*)theInstance)->report_;

  return Py_BuildValue("s",
   theReport->subjectPattern().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Report::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theDecoder = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theDecoder))
    return 0;

  OTCEV_Report* theReport;
  theReport = ((OPY_Report*)theInstance)->report_;

  return OPY_Service::payloadToObject(theReport->content(),theDecoder);
}

/* ------------------------------------------------------------------------- */
