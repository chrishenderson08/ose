/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     internet/otchttpdaem.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/internet/httpdaem.hh"
#endif
#endif

#include <OTC/internet/httpdaem.hh>

#include <OTC/internet/echosrvl.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/evreaper.hh>
#include <OTC/thread/mxreaper.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_HttpDaemon");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_HttpDaemon::~OTC_HttpDaemon()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_HttpDaemon::OTC_HttpDaemon(int thePort)
  : listener_(this,thePort)
{
  listener_.addObserver(id());
}

/* ------------------------------------------------------------------------- */
void OTC_HttpDaemon::attach(
 char const* theServerPath,
 OTC_HttpServer* theServer
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_HttpDaemon::attach()");

  OTCLIB_TRACER(MODULE) << "server = " << (void*)theServer << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_HIndexEntry<OTC_String>* theNode;

  if (theServer == 0)
  {
    theNode = servers_.entry(theServerPath);
    if (theNode != 0)
      theNode->removeEntry();
  }
  else
  {
    theNode = servers_.insert(theServerPath);
    theNode->setItem((void*)theServer);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_HttpDaemon::authorise(OTC_String const& theClientHost)
{
  return true;
}

/* ------------------------------------------------------------------------- */
OTC_HttpServer* OTC_HttpDaemon::server_(
 OTC_String const& theServerPath
) const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_HttpDaemon::server_()");

  OTC_HttpServer* theServer = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_HIndexEntry<OTC_String> const* theNode;

  theNode = servers_.entry(theServerPath);

  OTCLIB_TRACER(MODULE) << "found = " << (theNode != 0) << endl;

  if (theNode != 0)
    theServer = (OTC_HttpServer*)theNode->item();

  OTCLIB_TRACER(MODULE) << "okay = " << (theServer != 0) << endl;
  OTCLIB_TRACER(MODULE) << "server = " << (void*)theServer << endl;

  xxxMutex.release();

  mutex_.unlock();

  return theServer;
}

/* ------------------------------------------------------------------------- */
void OTC_HttpDaemon::process(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
    {
      OTC_EndPoint* theEndPoint;
      theEndPoint = OTC_EndPoint::remote(theConnection->remoteAddress());

      if (theEndPoint != 0)
      {
	OTC_String theClientHost;
	theClientHost = theConnection->remoteTransport();

	int theIndex;
	theIndex = theClientHost.index(':');
	
	if (theIndex != -1)
	  theClientHost = theClientHost.before(theIndex);

	OTC_HIndexEntry<OTC_String>* theNode;
	theNode = sessions_.insert(theConnection->localAddress());

	if (theNode->item() == 0)
	{
	  OTC_HttpSession* theSession;
	  theSession = new OTC_HttpSession(this,theEndPoint);
	  OTCLIB_ASSERT_M(theSession != 0);

	  theSession->reference();
	  theNode->setItem((void*)theSession);
	}
      }
    }
    else
    {
      OTC_HIndexEntry<OTC_String>* theNode;
      theNode = sessions_.entry(theConnection->localAddress());

      if (theNode != 0)
      {
        if (theNode->item() != 0)
	{
	  OTC_HttpSession* theSession;
	  theSession = (OTC_HttpSession*)theNode->item();

	  theSession->terminate();
	  theSession->unReference();
	}

	theNode->removeEntry();
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_HttpDaemon::handle(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  OTC_EventReaper xxxEvent;
  xxxEvent.grab(theEvent);

  process(theEvent);
}

/* ------------------------------------------------------------------------- */
