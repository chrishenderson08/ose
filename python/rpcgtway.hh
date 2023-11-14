#ifndef OPY_RPCGTWAY_HH
#define OPY_RPCGTWAY_HH
/*
// ============================================================================
//
// = FILENAME
//     rpcgtway.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/internet/rpcgtway.hh>

/* ------------------------------------------------------------------------- */

class OPY_RpcGateway : public PyObject, public OTC_RpcGateway
{
  public:

  			OPY_RpcGateway(
			 PyObject* theOuter,
			 char const* theSelection,
			 OTC_ServiceSubscription theType
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    bool		authorise(
			 char const* theLogin,
			 char const* thePassword
			);

  protected:

			~OPY_RpcGateway();

    PyObject*		outer_;

  private:

			OPY_RpcGateway(OPY_RpcGateway const&);

    OPY_RpcGateway&	operator=(OPY_RpcGateway const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_RPCGTWAY_HH */
