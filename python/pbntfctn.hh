#ifndef OPY_PBNTFCTN_HH
#define OPY_PBNTFCTN_HH
/*
// ============================================================================
//
// = FILENAME
//     pbntfctn.hh
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

#include <OTC/dispatch/pbntfctn.hh>

/* ------------------------------------------------------------------------- */

class OPY_PublisherNotification : public PyObject
{
  public:

  			OPY_PublisherNotification(
			 OTCEV_PublisherNotification* theNotification
			);

    OTCEV_PublisherNotification*	notification() const
    				{ return notification_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OPY_PublisherNotification const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_publisher(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_subscriber(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_status(PyObject*, PyObject* theArgs);

  protected:

			~OPY_PublisherNotification();

  private:

			OPY_PublisherNotification(
			 OPY_PublisherNotification const&
			);

    OPY_PublisherNotification&	operator=(
			 OPY_PublisherNotification const&
			);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_PublisherNotification*	notification_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_PBNTFCTN_HH */
