#ifndef OPY_AGENT_HH
#define OPY_AGENT_HH
/*
// ============================================================================
//
// = FILENAME
//     agent.hh
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

#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/thread/mutex.hh>

/* ------------------------------------------------------------------------- */

class OPY_Agent : public PyObject, public OTC_EVAgent
{
  public:

  			OPY_Agent(PyObject* theOuter);

    static PyObject*	create(PyObject* theModule, PyObject* theArgs);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_suspendEvents(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_resumeEvents(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_activateThreading(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_suspendThreading(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_resumeThreading(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_shutdownThreading(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_threadingEnabled(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_id(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_startTimer(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelTimer(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAllTimers(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_setAlarm(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAlarm(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAllAlarms(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_scheduleAction(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_scheduleCronAction(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAction(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_cancelAllActions(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscribeSignal(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeSignal(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeAllSignals(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_scheduleShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_suspendShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_resumeShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscribeShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeShutdown(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscribeSocket(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeSocket(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_unsubscribeAllSockets(PyObject*, PyObject* theArgs);

  protected:

    void		execute(OTC_Event* theEvent);

    void		filter(OTC_Event* theEvent);

    void		handle(OTC_Event* theEvent);

    void		enterPython()
				{
#ifdef WITH_THREAD
				  if (threadState_ != 0)
				    PyEval_AcquireThread(threadState_);
				  else
				    OPY_Dispatcher::enterPython();
#endif
				  PyErr_Clear();
				}

    void		leavePython()
				{
#ifdef WITH_THREAD
				  if (threadState_ != 0)
				    PyEval_ReleaseThread(threadState_);
				  else
				    OPY_Dispatcher::leavePython();
#endif
				}

  protected:

			~OPY_Agent();

    PyObject*		outer_;

  private:

			OPY_Agent(OPY_Agent const&);

    OPY_Agent&	operator=(OPY_Agent const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_Mutex		mutex_;

    OTC_EVHandler*	threadPool_;

    PyInterpreterState*	interpreterState_;

    PyThreadState*	threadState_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_AGENT_HH */
