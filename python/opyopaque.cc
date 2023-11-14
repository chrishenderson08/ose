/*
// ============================================================================
//
// = FILENAME
//     opyopaque.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2002-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "opaque.hh"
#include "servbind.hh"
#include "service.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Opaque::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Opaque",
  sizeof(OPY_Opaque),
  0,
  OPY_Opaque::sys_dealloc,
  0,
  OPY_Opaque::sys_getattr,
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
PyMethodDef OPY_Opaque::gMethods_[] = {
  { "sender", OPY_Opaque::mfn_sender, 1 },
  { "conversationId", OPY_Opaque::mfn_conversationId, 1 },
  { "sequenceNumber", OPY_Opaque::mfn_sequenceNumber, 1 },
  { "messageId", OPY_Opaque::mfn_messageId, 1 },
  { "description", OPY_Opaque::mfn_description, 1 },
  { "content", OPY_Opaque::mfn_content, 1 },
  { "contentType", OPY_Opaque::mfn_contentType, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Opaque::~OPY_Opaque()
{
  message_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_Opaque::OPY_Opaque(OTCEV_Opaque* theMessage)
  : message_(0)
{
  OTCLIB_ENSURE_FN((theMessage != 0),
   "OPY_Opaque::OPY_Opaque(OTCEV_Opaque*)",
   "invalid request supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  message_ = (OTCEV_Opaque*)theMessage->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Opaque::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Opaque*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Opaque*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_sender(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->sender().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_conversationId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->conversationId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_sequenceNumber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("i",
   theMessage->sequenceNumber());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_messageId(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->messageId().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_description(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->description().string());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_content(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("s#",
   theMessage->content().string(),theMessage->content().length());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Opaque::mfn_contentType(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Opaque* theMessage;
  theMessage = ((OPY_Opaque*)theInstance)->message_;

  return Py_BuildValue("s",
   theMessage->contentType().string());
}

/* ------------------------------------------------------------------------- */
