#ifndef OPY_SERVBIND_HH
#define OPY_SERVBIND_HH
/*
// ============================================================================
//
// = FILENAME
//     servbind.hh
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

#include <OTC/dispatch/servbind.hh>

/* ------------------------------------------------------------------------- */

class OPY_ServiceBinding : public PyObject
{
  public:

  			OPY_ServiceBinding(
			 OTC_ServiceBinding* theServiceBinding
			);

    OTC_ServiceBinding*	binding() const
    				{ return serviceBinding_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OPY_ServiceBinding const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_serviceName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_serviceAudience(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_receiverName(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_agentIdentity(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_processAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_processIdentity(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_networkGroup(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_agentAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_serviceAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_serviceLocation(PyObject*, PyObject* theArgs);

  protected:

			~OPY_ServiceBinding();

  private:

			OPY_ServiceBinding(OPY_ServiceBinding const&);

    OPY_ServiceBinding&	operator=(OPY_ServiceBinding const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_ServiceBinding*	serviceBinding_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_SERVBIND_HH */
