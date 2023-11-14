/*
// ============================================================================
//
// = FILENAME
//     opyfailure.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "failure.hh"
#include "servbind.hh"
#include "service.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Failure::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Failure",
  sizeof(OPY_Failure),
  0,
  OPY_Failure::sys_dealloc,
  0,
  OPY_Failure::sys_getattr,
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
PyMethodDef OPY_Failure::gMethods_[] = {
  { "conversationId", OPY_Failure::mfn_conversationId, 1 },
  { "error", OPY_Failure::mfn_error, 1 },
  { "description", OPY_Failure::mfn_description, 1 },
  { "origin", OPY_Failure::mfn_origin, 1 },
  { "details", OPY_Failure::mfn_details, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Failure::~OPY_Failure()
{
  failure_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_Failure::OPY_Failure(OTCEV_Failure* theFailure)
  : failure_(0)
{
  OTCLIB_ENSURE_FN((theFailure != 0),
   "OPY_Failure::OPY_Failure(OTCEV_Failure*)",
   "invalid failure supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  failure_ = (OTCEV_Failure*)theFailure->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Failure::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Failure*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Failure*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OPY_Failure*)theInstance)->failure_;

  return Py_BuildValue("s",
   theFailure->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::mfn_error(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OPY_Failure*)theInstance)->failure_;

  return Py_BuildValue("i",theFailure->error());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::mfn_description(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OPY_Failure*)theInstance)->failure_;

  return Py_BuildValue("s",theFailure->description().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::mfn_origin(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OPY_Failure*)theInstance)->failure_;

  return Py_BuildValue("s",theFailure->origin().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Failure::mfn_details(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Failure* theFailure;
  theFailure = ((OPY_Failure*)theInstance)->failure_;

  return Py_BuildValue("s",theFailure->details().string());
}

/* ------------------------------------------------------------------------- */
