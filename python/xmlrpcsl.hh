#ifndef OPY_XMLRPCSL_HH
#define OPY_XMLRPCSL_HH
/*
// ============================================================================
//
// = FILENAME
//     xmlrpcsl.hh
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

#include <OTC/internet/xmlrpcsl.hh>

/* ------------------------------------------------------------------------- */

class OPY_XmlRpcServlet : public PyObject, public OTC_XmlRpcServlet
{
  public:

  			OPY_XmlRpcServlet(
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

			~OPY_XmlRpcServlet();

    PyObject*		outer_;

  private:

			OPY_XmlRpcServlet(OPY_XmlRpcServlet const&);

    OPY_XmlRpcServlet&	operator=(OPY_XmlRpcServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_XMLRPCSL_HH */
