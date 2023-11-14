/*
// ============================================================================
//
// = FILENAME
//     opyresponse.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "response.hh"
#include "servbind.hh"
#include "service.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Response::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Response",
  sizeof(OPY_Response),
  0,
  OPY_Response::sys_dealloc,
  0,
  OPY_Response::sys_getattr,
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
PyMethodDef OPY_Response::gMethods_[] = {
  { "sender", OPY_Response::mfn_sender, 1 },
  { "conversationId", OPY_Response::mfn_conversationId, 1 },
  { "sequenceNumber", OPY_Response::mfn_sequenceNumber, 1 },
  { "content", OPY_Response::mfn_content, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Response::~OPY_Response()
{
  response_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_Response::OPY_Response(OTCEV_Response* theResponse)
  : response_(0)
{
  OTCLIB_ENSURE_FN((theResponse != 0),
   "OPY_Response::OPY_Response(OTCEV_Response*)",
   "invalid response supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  response_ = (OTCEV_Response*)theResponse->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Response::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Response::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Response*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Response::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Response*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Response::mfn_sender(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OPY_Response*)theInstance)->response_;

  return Py_BuildValue("s",
   theResponse->sender().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Response::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OPY_Response*)theInstance)->response_;

  return Py_BuildValue("s",
   theResponse->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Response::mfn_sequenceNumber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OPY_Response*)theInstance)->response_;

  return Py_BuildValue("i",
   theResponse->sequenceNumber());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Response::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  PyObject* theDecoder = 0;

  if (!PyArg_ParseTuple(theArgs,"O",&theDecoder))
    return 0;

  OTCEV_Response* theResponse;
  theResponse = ((OPY_Response*)theInstance)->response_;

  return OPY_Service::payloadToObject(theResponse->content(),theDecoder);
}

/* ------------------------------------------------------------------------- */
