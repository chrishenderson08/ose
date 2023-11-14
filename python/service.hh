#ifndef OPY_SERVICE_HH
#define OPY_SERVICE_HH
/*
// ============================================================================
//
// = FILENAME
//     service.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "monitor.hh"

#include <OTC/dispatch/svbroker.hh>

/* ------------------------------------------------------------------------- */

class OPY_Service : public OPY_Monitor
{
  public:

  			OPY_Service(
			 PyObject* theOuter,
			 char const* theServiceName,
			 char const* theServiceAudience,
			 OTC_ServiceVisibility theServiceVisibility
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_serviceBinding(PyObject*, PyObject*);

    static PyObject*	mfn_serviceVisibility(PyObject*, PyObject*);

    static PyObject*	mfn_announceAvailability(PyObject*, PyObject*);

    static PyObject*	mfn_joinGroup(PyObject*, PyObject*);

    static PyObject*	mfn_leaveGroup(PyObject*, PyObject*);

    static PyObject*	mfn_sendRequest(PyObject*, PyObject*);

    static PyObject*	mfn_sendMessage(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeReports(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeReports(PyObject*, PyObject*);

    static PyObject*	mfn_publishReport(PyObject*, PyObject*);

    static PyObject*	mfn_sendReport(PyObject*, PyObject*);

    static PyObject*	mfn_expireReports(PyObject*, PyObject*);

    static PyObject*	mfn_expireAllReports(PyObject*, PyObject*);

    static PyObject*	mfn_purgeReports(PyObject*, PyObject*);

    static PyObject*	mfn_matchSubject(PyObject*, PyObject*);

  public:

    static PyObject*	payloadToObject(
			 OTC_ROPayload const& thePayload,
			 PyObject* theCallback
			);

    static bool	objectToPayload(
			 PyObject* theObject,
			 OTC_RWPayload thePayload,
			 PyObject* theCallback
			);

  protected:

    void		handle(OTC_Event* theEvent);

  protected:

			~OPY_Service();

  private:

			OPY_Service(OPY_Service const&);

    OPY_Service&	operator=(OPY_Service const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];

    OTC_SVBroker	broker_;
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_SERVICE_HH */
