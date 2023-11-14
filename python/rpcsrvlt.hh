#ifndef OPY_RPCSRVLT_HH
#define OPY_RPCSRVLT_HH
/*
// ============================================================================
//
// = FILENAME
//     rpcsrvlt.hh
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

#include <OTC/internet/rpcsrvlt.hh>

/* ------------------------------------------------------------------------- */

class OPY_RpcServlet : public PyObject, public OTC_RpcServlet
{
  public:

  			OPY_RpcServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession,
			 OTC_ServiceBinding* theBinding=0
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    void		destroy();

  protected:

			~OPY_RpcServlet();

    PyObject*		outer_;

  private:

			OPY_RpcServlet(OPY_RpcServlet const&);

    OPY_RpcServlet&	operator=(OPY_RpcServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_RPCSRVLT_HH */
