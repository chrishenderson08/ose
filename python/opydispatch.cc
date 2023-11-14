/*
// ============================================================================
//
// = FILENAME
//     opydispatch.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "dispatch.hh"
#include "job.hh"
#include "module.hh"

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/shutdown.hh>
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
    TAG = new OTC_TraceTag("OPY_Dispatcher");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()
#endif

/* ------------------------------------------------------------------------- */
OPY_Dispatcher* OPY_Dispatcher::gInstance_ = 0;
PyInterpreterState* OPY_Dispatcher::gInterpreterState_ = 0;
PyThreadState* OPY_Dispatcher::gThreadState_ = 0;

/* ------------------------------------------------------------------------- */
PyTypeObject OPY_Dispatcher::gType_ = {
  PyObject_HEAD_INIT(&PyType_Type)
  0,
  "Dispatcher",
  sizeof(OPY_Dispatcher),
  0,
  OPY_Dispatcher::sys_dealloc,
  0,
  OPY_Dispatcher::sys_getattr,
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
PyMethodDef OPY_Dispatcher::gMethods_[] = {
  { "run", OPY_Dispatcher::mfn_run, 1 },
  { "stop", OPY_Dispatcher::mfn_stop, 1 },
  { "schedule", OPY_Dispatcher::mfn_schedule, 1 },
  { "monitor", OPY_Dispatcher::mfn_monitor, 1 },
  { "shutdown", OPY_Dispatcher::mfn_shutdown, 1 },
  { "reset", OPY_Dispatcher::mfn_reset, 1 },
  { "wakeup", OPY_Dispatcher::mfn_wakeup, 1 },
  { "isWaiting", OPY_Dispatcher::mfn_isWaiting, 1 },
  { "isRunning", OPY_Dispatcher::mfn_isRunning, 1 },
  { "isStopped", OPY_Dispatcher::mfn_isStopped, 1 },
  { "task_start", OPY_Dispatcher::mfn_task_start, 1 },
  { "task_stop", OPY_Dispatcher::mfn_task_stop, 1 },
  { "task_wait", OPY_Dispatcher::mfn_task_wait, 1 },
  { "warningsEnabled", OPY_Dispatcher::mfn_warningsEnabled, 1 },
  { "enableWarnings", OPY_Dispatcher::mfn_enableWarnings, 1 },
  { "disableWarnings", OPY_Dispatcher::mfn_disableWarnings, 1 },
  { 0, 0 }
};

/* ------------------------------------------------------------------------- */
OPY_Dispatcher::~OPY_Dispatcher()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OPY_Dispatcher::OPY_Dispatcher()
{
  ob_type = &gType_;

  _Py_NewReference(this);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::instance(PyObject* theModule, PyObject* theArgs)
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_Dispatcher::instance()");

  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  if (gInstance_ == 0)
  {
    OTC_Dispatcher::initialise();

    gInstance_ = new OPY_Dispatcher;
    OTCLIB_ASSERT_M(gInstance_ != 0);

#ifdef WITH_THREAD
    PyEval_InitThreads();
    gInterpreterState_ = PyThreadState_Get()->interp;
    gThreadState_ = PyThreadState_New(gInterpreterState_);
#endif
  }

  Py_XINCREF(gInstance_);

  return (PyObject*)gInstance_;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::obj_getattr(char const* theName)
{
  return Py_FindMethod(gMethods_,this,(char*)theName);
}

/* ------------------------------------------------------------------------- */
void OPY_Dispatcher::sys_dealloc(PyObject* theInstance)
{
  delete (OPY_Dispatcher*)theInstance;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::sys_getattr(
 PyObject* theInstance,
 char* theName
)
{
  return ((OPY_Dispatcher*)theInstance)->obj_getattr(theName);
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_run(PyObject*, PyObject* theArgs)
{
  OTCLIB_MARKBLOCK(MODULE,"OPY_Dispatcher::run()");

  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Dispatcher::run();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_stop(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Dispatcher::stop();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_schedule(PyObject*, PyObject* theArgs)
{
  PyObject* theCallback;
  int theOptions = OTCLIB_STANDARD_JOB;

  if (!PyArg_ParseTuple(theArgs,"O|i",&theCallback,&theOptions))
    return 0;

  OTC_Job* theJob;
  theJob = new OPY_Job(theCallback);
  OTCLIB_ASSERT(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theOptions);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_wakeup(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Dispatcher::wakeup();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_monitor(PyObject*, PyObject* theArgs)
{
  int theSignal;

  if (!PyArg_ParseTuple(theArgs,"i",&theSignal))
    return 0;

  OTCEV_Shutdown::monitor(theSignal);

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_shutdown(PyObject*, PyObject* theArgs)
{
  double theDelay = 0;

  if (!PyArg_ParseTuple(theArgs,"|d",&theDelay))
    return 0;

  OTCEV_Shutdown::schedule(int(theDelay*1000));

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_reset(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Dispatcher::reset();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_isWaiting(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  bool theResult;

  theResult = OTC_Dispatcher::isWaiting();

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_isRunning(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  bool theResult;

  theResult = OTC_Dispatcher::isRunning();

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_isStopped(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  bool theResult;

  theResult = OTC_Dispatcher::isStopped();

  return Py_BuildValue("i",int(theResult));
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_task_start(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Dispatcher::task()->start();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_task_stop(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Dispatcher::task()->stop();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_task_wait(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  Py_BEGIN_ALLOW_THREADS

  OTC_Dispatcher::task()->wait();

  Py_END_ALLOW_THREADS

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_warningsEnabled(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  return Py_BuildValue("i",OTC_Dispatcher::warningsEnabled());
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_enableWarnings(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Dispatcher::enableWarnings();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
PyObject* OPY_Dispatcher::mfn_disableWarnings(PyObject*, PyObject* theArgs)
{
  if (!PyArg_ParseTuple(theArgs,""))
    return 0;

  OTC_Dispatcher::disableWarnings();

  Py_XINCREF(Py_None);

  return Py_None;
}

/* ------------------------------------------------------------------------- */
