#ifndef OPY_XMLRPCGW_HH
#define OPY_XMLRPCGW_HH
/*
// ============================================================================
//
// = FILENAME
//     xmlrpcgw.hh
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

#include <OTC/internet/xmlrpcgw.hh>

/* ------------------------------------------------------------------------- */

class OPY_XmlRpcGateway : public PyObject, public OTC_XmlRpcGateway
{
  public:

  			OPY_XmlRpcGateway(
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

			~OPY_XmlRpcGateway();

    PyObject*		outer_;

  private:

			OPY_XmlRpcGateway(OPY_XmlRpcGateway const&);

    OPY_XmlRpcGateway&	operator=(OPY_XmlRpcGateway const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_XMLRPCGW_HH */
