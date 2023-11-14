#ifndef OPY_MONITOR_HH
#define OPY_MONITOR_HH
/*
// ============================================================================
//
// = FILENAME
//     monitor.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "agent.hh"

#include <OTC/dispatch/svbroker.hh>

/* ------------------------------------------------------------------------- */

class OPY_Monitor : public OPY_Agent
{
  public:

  			OPY_Monitor(PyObject* theOuter);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_serviceAgents(PyObject*, PyObject*);

    static PyObject*	mfn_serviceGroups(PyObject*, PyObject*);

    static PyObject*	mfn_lookupServiceName(PyObject*, PyObject*);

    static PyObject*	mfn_lookupServiceAddress(PyObject*, PyObject*);

    static PyObject*	mfn_lookupServiceGroup(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeAllRegistry(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeRegistry(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeRegistry(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeServiceName(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeServiceName(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeServiceAddress(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeServiceAddress(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeServiceGroup(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeServiceGroup(PyObject*, PyObject*);

  protected:

    void		handle(OTC_Event* theEvent);

  protected:

			~OPY_Monitor();

  private:

			OPY_Monitor(OPY_Monitor const&);

    OPY_Monitor&	operator=(OPY_Monitor const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_MONITOR_HH */
