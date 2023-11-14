#ifndef OPY_SBSCRPTN_HH
#define OPY_SBSCRPTN_HH
/*
// ============================================================================
//
// = FILENAME
//     sbscrptn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <Python.h>

#include <OTC/dispatch/sbscrptn.hh>

/* ------------------------------------------------------------------------- */

class OPY_Subscription : public PyObject
{
  public:

  			OPY_Subscription(
			 OTCEV_Subscription* theSubscription
			);

    OTCEV_Subscription*	subscription() const
    				{ return subscription_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OPY_Subscription const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_publisher(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscriber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_status(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subject(PyObject*, PyObject* theArgs);

  protected:

			~OPY_Subscription();

  private:

			OPY_Subscription(
			 OPY_Subscription const&
			);

    OPY_Subscription&	operator=(OPY_Subscription const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_Subscription*	subscription_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_SBSCRPTN_HH */
