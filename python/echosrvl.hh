#ifndef OPY_ECHOSRVL_HH
#define OPY_ECHOSRVL_HH
/*
// ============================================================================
//
// = FILENAME
//     echosrvl.hh
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

#include <OTC/internet/echosrvl.hh>

/* ------------------------------------------------------------------------- */

class OPY_EchoServlet : public PyObject, public OTC_EchoServlet
{
  public:

  			OPY_EchoServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    void		destroy();

  protected:

			~OPY_EchoServlet();

    PyObject*		outer_;

  private:

			OPY_EchoServlet(OPY_EchoServlet const&);

    OPY_EchoServlet&	operator=(OPY_EchoServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_ECHOSRVL_HH */
