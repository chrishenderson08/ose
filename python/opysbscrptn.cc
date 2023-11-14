/*
// ============================================================================
//
// = FILENAME
//     opysbscrptn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "sbscrptn.hh"
#include "servbind.hh"

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Subscription::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Subscription",
  sizeof(OPY_Subscription),
  0,
  OPY_Subscription::sys_dealloc,
  0,
  OPY_Subscription::sys_getattr,
  0,
  OPY_Subscription::sys_compare,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

/* ------------------------------------------------------------------------- */
PyMethodDef OPY_Subscription::gMethods_[] = {
  { "publisher", OPY_Subscription::mfn_publisher, 1 },
  { "subscriber", OPY_Subscription::mfn_subscriber, 1 },
  { "status", OPY_Subscription::mfn_status, 1 },
  { "subject", OPY_Subscription::mfn_subject, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Subscription::~OPY_Subscription()
{
  subscription_->destroy();
}

/* ------------------------------------------------------------------------- */
OPY_Subscription::OPY_Subscription(
 OTCEV_Subscription* theSubscription
)
  : subscription_(0)
{
  OTCLIB_ENSURE_FN((theSubscription != 0),
   "OPY_Subscription::OPY_Subscription(...)",
   "invalid subscription event supplied");

  ob_type = &gType_;

  _Py_NewReference(this);

  subscription_ = (OTCEV_Subscription*)theSubscription->clone();
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Subscription::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
int OPY_Subscription::obj_compare(
 OPY_Subscription const* theObject
) const
{
  return subscription_->subject().rank(
   ((OPY_Subscription*)theObject)->subscription_->subject());
}

/* ------------------------------------------------------------------------- */
void OPY_Subscription::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Subscription*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Subscription::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Subscription*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
int OPY_Subscription::sys_compare(
 PyObject* theInstance,
 PyObject* theObject
)
{
  return ((OPY_Subscription*)theInstance)->obj_compare(
   (OPY_Subscription*)theObject);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Subscription::mfn_publisher(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OPY_Subscription*)theInstance)->subscription_;

  OPY_ServiceBinding* theObject;
  theObject = new OPY_ServiceBinding(theSubscription->publisher());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Subscription::mfn_subscriber(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OPY_Subscription*)theInstance)->subscription_;

  OPY_ServiceBinding* theObject;
  theObject = new OPY_ServiceBinding(theSubscription->subscriber());
  OTCLIB_ASSERT_M(theObject != 0);

  return theObject;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Subscription::mfn_status(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OPY_Subscription*)theInstance)->subscription_;

  return Py_BuildValue("i",theSubscription->status());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Subscription::mfn_subject(
 PyObject* theInstance,
 PyObject* theArgs
)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTCEV_Subscription const* theSubscription;
  theSubscription = ((OPY_Subscription*)theInstance)->subscription_;

  return Py_BuildValue("s",theSubscription->subject().string());
}

/* ------------------------------------------------------------------------- */
