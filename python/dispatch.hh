#ifndef OPY_DISPATCH_HH
#define OPY_DISPATCH_HH
/*
// ============================================================================
//
// = FILENAME
//     dispatch.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/OTC.h>

/* ------------------------------------------------------------------------- */

class OPY_JobQueue;

class OPY_Dispatcher : public PyObject
{
  public:

			~OPY_Dispatcher();

  			OPY_Dispatcher();

    static PyObject*	instance(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_run(PyObject*, PyObject*);

    static PyObject*	mfn_stop(PyObject*, PyObject*);

    static PyObject*	mfn_schedule(PyObject*, PyObject*);

    static PyObject*	mfn_wakeup(PyObject*, PyObject*);

    static PyObject*	mfn_monitor(PyObject*, PyObject*);

    static PyObject*	mfn_shutdown(PyObject*, PyObject*);

    static PyObject*	mfn_reset(PyObject*, PyObject*);

    static PyObject*	mfn_isWaiting(PyObject*, PyObject*);

    static PyObject*	mfn_isRunning(PyObject*, PyObject*);

    static PyObject*	mfn_isStopped(PyObject*, PyObject*);

    static PyObject*	mfn_task_start(PyObject*, PyObject*);

    static PyObject*	mfn_task_stop(PyObject*, PyObject*);

    static PyObject*	mfn_task_wait(PyObject*, PyObject*);

    static PyObject*	mfn_warningsEnabled(PyObject*, PyObject*);

    static PyObject*	mfn_enableWarnings(PyObject*, PyObject*);

    static PyObject*	mfn_disableWarnings(PyObject*, PyObject*);

    static void		enterPython()
				{
#ifdef WITH_THREAD
				  PyEval_AcquireThread(gThreadState_);
#endif
				  PyErr_Clear();
				}

    static void		leavePython()
				{
#ifdef WITH_THREAD
				  PyEval_ReleaseThread(gThreadState_);
#endif
				}

    static PyInterpreterState*	interpreterState()
    				{ return gInterpreterState_; }

  private:

			OPY_Dispatcher(OPY_Dispatcher const&);

    OPY_Dispatcher&	operator=(OPY_Dispatcher const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    static OPY_Dispatcher*	gInstance_;

    static PyInterpreterState*	gInterpreterState_;

    static PyThreadState*	gThreadState_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_DISPATCH_HH */
