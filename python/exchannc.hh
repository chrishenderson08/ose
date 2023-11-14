#ifndef OPY_EXCHANNC_HH
#define OPY_EXCHANNC_HH
/*
// ============================================================================
//
// = FILENAME
//     exchannc.hh
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

#include <OTC/message/exchannc.hh>

/* ------------------------------------------------------------------------- */

class OPY_ExchangeAnnouncement : public PyObject
{
  public:

  			OPY_ExchangeAnnouncement(
			 OTCEV_ExchangeAnnouncement* theAnnouncement
			);

    OTCEV_ExchangeAnnouncement*	announcement() const
    				{ return announcement_; }

    static int		isType(PyObject* theObject)
    				{ return theObject->ob_type == &gType_; }

    PyObject*		obj_getattr(char const* theName);

    int			obj_compare(OPY_ExchangeAnnouncement const*) const;

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static int          sys_compare(PyObject*, PyObject*);

    static PyObject*	mfn_group(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_localAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_remoteAddress(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_remoteProcess(PyObject*, PyObject* theArgs);

    static PyObject*	mfn_state(PyObject*, PyObject* theArgs);

  protected:

			~OPY_ExchangeAnnouncement();

  private:

			OPY_ExchangeAnnouncement(
			 OPY_ExchangeAnnouncement const&
			);

    OPY_ExchangeAnnouncement&	operator=(OPY_ExchangeAnnouncement const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTCEV_ExchangeAnnouncement*	announcement_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_EXCHANNC_HH */
