#ifndef OPY_HTTPSRVL_HH
#define OPY_HTTPSRVL_HH
/*
// ============================================================================
//
// = FILENAME
//     httpsrvl.hh
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

#include <OTC/dispatch/evagent.hh>
#include <OTC/internet/httpsrvl.hh>

/* ------------------------------------------------------------------------- */

class OPY_HttpServlet : public PyObject,
 public OTC_HttpServlet, public OTC_EVAgent
{
  public:

  			OPY_HttpServlet(
			 PyObject* theOuter,
			 OTC_HttpSession* theSession
			);

    static PyObject*	create(PyObject*, PyObject*);

    PyObject*		obj_getattr(char const* theName);

    static void         sys_dealloc(PyObject*);

    static PyObject*    sys_getattr(PyObject*, char*);

    static PyObject*	mfn_instance(PyObject*, PyObject*);

    static PyObject*	mfn_contentLength(PyObject*, PyObject*);

    static PyObject*	mfn_contentType(PyObject*, PyObject*);

    static PyObject*	mfn_requestMethod(PyObject*, PyObject*);

    static PyObject*	mfn_url(PyObject*, PyObject*);

    static PyObject*	mfn_protocolVersion(PyObject*, PyObject*);

    static PyObject*	mfn_serverHost(PyObject*, PyObject*);

    static PyObject*	mfn_serverPort(PyObject*, PyObject*);

    static PyObject*	mfn_requestPath(PyObject*, PyObject*);

    static PyObject*	mfn_queryString(PyObject*, PyObject*);

    static PyObject*	mfn_clientHost(PyObject*, PyObject*);

    static PyObject*	mfn_clientPort(PyObject*, PyObject*);

    static PyObject*	mfn_remoteUser(PyObject*, PyObject*);

    static PyObject*	mfn_headers(PyObject*, PyObject*);

    static PyObject*	mfn_containsHeader(PyObject*, PyObject*);

    static PyObject*	mfn_header(PyObject*, PyObject*);

    static PyObject*	mfn_serverRoot(PyObject*, PyObject*);

    static PyObject*	mfn_serverPath(PyObject*, PyObject*);

    static PyObject*	mfn_servletPath(PyObject*, PyObject*);

    static PyObject*	mfn_servletInfo(PyObject*, PyObject*);

    static PyObject*	mfn_sessionId(PyObject*, PyObject*);

    static PyObject*	mfn_sessionContext(PyObject*, PyObject*);

    static PyObject*	mfn_sendResponse(PyObject*, PyObject*);

    static PyObject*	mfn_sendHeader(PyObject*, PyObject*);

    static PyObject*	mfn_endHeaders(PyObject*, PyObject*);

    static PyObject*	mfn_sendContent(PyObject*, PyObject*);

    static PyObject*	mfn_endContent(PyObject*, PyObject*);

    static PyObject*	mfn_sendError(PyObject*, PyObject*);

    static PyObject*	mfn_sendComplete(PyObject*, PyObject*);

    static PyObject*	mfn_shutdown(PyObject*, PyObject*);

    static PyObject*	mfn_statusText(PyObject*, PyObject*);

    static PyObject*	mfn_subscribeCongestion(PyObject*, PyObject*);

    static PyObject*	mfn_unsubscribeCongestion(PyObject*, PyObject*);

    void		processRequest();

    void		processContent(OTC_String const& theContent);

    void		destroy();

  protected:

    void		handle(OTC_Event* theEvent);

			~OPY_HttpServlet();

    PyObject*		outer_;

  private:

			OPY_HttpServlet(OPY_HttpServlet const&);

    OPY_HttpServlet&	operator=(OPY_HttpServlet const&);

    static PyTypeObject gType_;

    static PyMethodDef	gMethods_[];
};

/* ------------------------------------------------------------------------- */

#endif /* OPY_HTTPSRVL_HH */
