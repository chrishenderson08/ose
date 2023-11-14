/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcexchange.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/exchange.hh"
#endif
#endif

#include <OTC/message/exchange.hh>

#include <OTC/message/inetclnt.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/jobqueue.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/system/program.hh>
#include <OTC/text/vrecord.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_Exchange");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Exchange::mutex_;
OTC_NRMutex OTC_Exchange::rmutex_;
OTC_EXRegistry* OTC_Exchange::gRegistry_ = 0;

/* ------------------------------------------------------------------------- */
OTC_Exchange::~OTC_Exchange()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Exchange::~OTC_Exchange()");

  OTC_SVBroker::registry()->unsubscribeRegistry(id());

  registry()->remove(group_);
}

/* ------------------------------------------------------------------------- */
OTC_Exchange::OTC_Exchange(OTC_ExchangeType theType, char const* theGroup)
  : group_(theGroup), type_(theType)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Exchange::OTC_Exchange(char const*)");

  OTCLIB_ENSURE_FN((theType != OTCLIB_EXCHANGE_UNKNOWN),
   "OTC_Exchange::OTC_Exchange(OTC_ExchangeType, char const*)",
   "invalid exchange type");
  OTCLIB_ENSURE_FN((theType != OTCLIB_EXCHANGE_SHUTDOWN),
   "OTC_Exchange::OTC_Exchange(OTC_ExchangeType, char const*)",
   "invalid exchange type");

  OTC_String tmpName(fullName(theGroup));

  // Bind name.

  inBox_.bind(this,tmpName);

  // Bind special receivers.

  receiverInBox_.bind(this,"$RECEIVER");
  remoteReceiverInBox_.bind(this,"$REMOTE-RECEIVER");

  // Save local process name.

#if 0
  char theLocalProcess[512];
  sprintf(theLocalProcess,"%ld@%s",
   OTC_Program::processId(),
   OTC_Program::hostname().string());
  localProcess_ = theLocalProcess;
#endif

  localProcess_ = OTC_Program::processIdentity();

  // Add to registry.

  registry()->add(this);

  // Subscribe to service announcements.

  OTC_SVBroker::registry()->subscribeRegistry(id());
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Exchange::fullName(char const* theGroup)
{
  OTC_String theResult = "$E";

  if (theGroup != 0 && *theGroup != EOS)
  {
    theResult += '?';
    theResult += theGroup;
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Exchange::groupName(OTC_String const& theFullName)
{
  int theIndex;
  theIndex = theFullName.index('?');

  if (theIndex == -1)
    return OTC_String::nullString();

  return theFullName.after(theIndex);
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::connect(OTC_EPClient* theClient)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::connect_(OTC_EPClient*)");

  OTCLIB_ENSURE_FN((theClient != 0),
   "OTC_Exchange::connect(OTC_EPClient*)",
   "invalid client");

  OTC_EPRegistry* theRegistry;
  theRegistry = new OTC_EPRegistry(theClient,group_);
  OTCLIB_ASSERT_M(theRegistry != 0);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  endPoints_.add(theClient->localAddress(),theRegistry);

  xxxMutex.release();

  mutex_.unlock();

  theClient->addObserver(id());
  theClient->start();
}

/* ------------------------------------------------------------------------- */
OTC_EPClient* OTC_Exchange::connect(
 char const* theHostname,
 int thePort,
 double theRetryDelay
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Exchange::connect(char const*,int,double)");

  int theDelay = int(1000*theRetryDelay);

  OTC_InetClient* theClient;
  theClient = new OTC_InetClient("MESSAGE",theHostname,thePort,theDelay);
  OTCLIB_ASSERT_M(theClient);

  connect(theClient);

  return theClient;
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::listen(OTC_EPListener* theListener)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::listen(OTC_EPListener*)");

  OTCLIB_ENSURE_FN((theListener != 0),
   "OTC_Exchange::listen(OTC_EPListener*)",
   "invalid listener");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  listeners_.add(theListener);

  xxxMutex.release();

  mutex_.unlock();

  theListener->addObserver(id());
  theListener->start();
}

/* ------------------------------------------------------------------------- */
OTC_EPListener* OTC_Exchange::listen(int thePort)
{
  OTC_InetListener* theListener;
  theListener = new OTC_InetListener("MESSAGE",thePort);
  OTCLIB_ASSERT_M(theListener);

  listen(theListener);

  return theListener;
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::shutdown(OTC_EPClient* theClient)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::shutdown(OTC_EPClient*)");

  OTCLIB_ENSURE_FN((theClient != 0),
   "OTC_Exchange::shutdown(OTC_EPClient*)",
   "invalid client");
  OTCLIB_ENSURE_FN((endPoints_.contains(theClient->localAddress())),
   "OTC_Exchange::shutdown(OTC_EPClient*)",
   "invalid client");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_EPRegistry* theRegistry;
  theRegistry = endPoints_.item(theClient->localAddress());

  if (theRegistry->exchangeType() == OTCLIB_EXCHANGE_UNKNOWN)
  {
    theRegistry->shutdown();
    endPoints_.remove(theClient->localAddress());
  }
  else
  {
    theRegistry->shutdown();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::shutdown(OTC_EPListener* theListener)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::shutdown(OTC_EPListener*)");

  OTCLIB_ENSURE_FN((theListener != 0),
   "OTC_Exchange::shutdown(OTC_EPListener*)",
   "invalid listener");
  OTCLIB_ENSURE_FN((listeners_.contains(theListener)),
   "OTC_Exchange::shutdown(OTC_EPListener*)",
   "listener not bound to this exchange");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  theListener->stop();

  listeners_.remove(theListener);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_EXRegistry* OTC_Exchange::registry()
{
  rmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(rmutex_);

  if (gRegistry_ == 0)
  {
    gRegistry_ = new OTC_EXRegistry;
    OTCLIB_ASSERT_M(gRegistry_ != 0);
  }

  xxxMutex.release();

  rmutex_.unlock();

  return gRegistry_;
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Exchange::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Connection::typeId())
  {
    OTCEV_Connection* theConnection;
    theConnection = (OTCEV_Connection*)theEvent;

    // Status from an endpoint.

    handleConnection(theConnection);
  }
  else if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*)theEvent;

    // Local service announcement.

    handleServiceAnnouncement(theAnnouncement);
  }
  else if (theEvent->type() == OTCEV_GroupAnnouncement::typeId())
  {
    OTCEV_GroupAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_GroupAnnouncement*)theEvent;

    // Local service group announcement.

    handleGroupAnnouncement(theAnnouncement);
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->currentReceiver() == name() &&
     theEnvelope->nextDestination().isEmpty())
    {
      // Message to be interpreted here.

      handleExchangeMessage(theEnvelope);
    }
    else if (theEnvelope->currentReceiver() == "$RECEIVER" ||
     theEnvelope->currentReceiver() == "$REMOTE-RECEIVER")
    {
      // Broadcast message.

      handleBroadcastMessage(theEnvelope);
    }
    else
    {
      // Pass onto next recipient.

      theEnvelope->forward(OTCLIB_STANDARD_JOB);
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::handleConnection(OTCEV_Connection* theConnection)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::handleConnection(OTCEV_Connection*)");

  if (theConnection->state() == OTCLIB_CONNECTION_ACTIVE)
  {
    OTC_EndPoint* theEndPoint;
    theEndPoint = OTC_EndPoint::local(theConnection->localAddress());

    if (theEndPoint->type() == OTCLIB_ENDPOINT_CLIENT)
    {
      // We should already have a record of client
      // endpoints as they were supplied explicitly.

      if (endPoints_.contains(theConnection->localAddress()))
      {
	OTC_EPRegistry* theRegistry;
	theRegistry = endPoints_.item(theConnection->localAddress());

	if (theRegistry->endPoint()->type() == OTCLIB_ENDPOINT_CLIENT)
	{
	  // Send out query to find out if there is a
	  // message exchange at the other end of the
	  // connection. Tell the other end whether we
	  // are a client or server. Use the group we
	  // were told to use which may not be our own.

          char const* tmpType = "$E::ANNOUNCE-SERVER";
          if (type() == OTCLIB_EXCHANGE_CLIENT)
            tmpType = "$E::ANNOUNCE-CLIENT";

	  OTCEV_Message* tmpMessage;
	  tmpMessage = new OTCEV_Message(localProcess_);
	  OTCLIB_ASSERT_M(tmpMessage != 0);

	  OTC_Capacity tmpCapacity(127);
	  OTC_String tmpAddress(tmpCapacity);

	  tmpAddress += theConnection->remoteAddress();
	  tmpAddress += '!';
	  tmpAddress += theRegistry->exchangeName();

	  tmpMessage->queue(tmpAddress,name(),
	   tmpType,"",OTCLIB_STANDARD_JOB);
	}
      }
      else
      {
        OTC_LogStream theLogger;
        theLogger.setLevel(OTCLIB_LOG_WARNING);

        theLogger << "Sanity check failed" << EOL;
        theLogger << "Class: OTC_Exchange" << EOL;
	theLogger << "Method: handleConnection()" << EOL;
        theLogger << "Description: don't know about client endpoint" << EOL;
	theLogger << "Result: notification will be ignored" << EOL;
	theLogger << "Comment: possibly an implementation problem" << EOL;
        theLogger << "LocalAddress: " << theConnection->localAddress() << EOL;
        theLogger << "RemoteAddress: " << theConnection->remoteAddress() << EOL;
        theLogger << flush;
      }
    }
    else
    {
      // We shouldn't know about a server endpoint
      // as they are created by listeners on demand.

      if (!endPoints_.contains(theEndPoint->localAddress()))
      {
        OTC_EPRegistry* theEPRegistry;
        theEPRegistry = new OTC_EPRegistry(theEndPoint,group_);
        OTCLIB_ASSERT_M(theEPRegistry != 0);

        endPoints_.add(theEndPoint->localAddress(),theEPRegistry);
      }
      else
      {
        OTC_LogStream theLogger;
        theLogger.setLevel(OTCLIB_LOG_WARNING);

	theLogger << "Sanity check failed" << EOL;
        theLogger << "Class: OTC_Exchange" << EOL;
	theLogger << "Method: handleConnection()";
        theLogger << "Description: already know about server endpoint" << EOL;
	theLogger << "Result: notification will be ignored" << EOL;
	theLogger << "Comment: possibly an implementation problem" << EOL;
        theLogger << "LocalAddress: " << theConnection->localAddress() << EOL;
        theLogger << "RemoteAddress: " << theConnection->remoteAddress() << EOL;
        theLogger << flush;
      }
    }
  }
  else
  {
    if (endPoints_.contains(theConnection->localAddress()))
    {
      OTC_EPRegistry* theRegistry;
      theRegistry = endPoints_.item(theConnection->localAddress());

      // Notify of withdrawl of all services
      // accessable via the endpoint.

      OTC_Iterator<OTC_ServiceBinding*> services;
      services = theRegistry->services(OTCLIB_BACKWARD);
      while (services.isValid())
      {
	OTC_ServiceBinding* theBinding;
	theBinding = services.item();

	// Remove from service registry.

	OTC_SVBroker::registry()->removeServiceBinding(
	 theBinding->serviceAddress());

	// Now broadcast to other exchanges.

	OTCEV_Message* tmpMessage;
	tmpMessage = new OTCEV_Message(theBinding->serviceAddress());
	OTCLIB_ASSERT_M(tmpMessage != 0);

	broadcastMessage(theRegistry,
	 inBox_.name(),"$E::SERVICE-WITHDRAWN","",tmpMessage);

	services.next();
      }

      // Remove all services recorded for
      // endpoint as for client we will reuse
      // the registry.

      theRegistry->removeAllServices();

      // Remove remote address from set of those
      // for which there was an exchange at the
      // other end of the connection. Note that
      // we don't remove a client connection from
      // set of client endpoints as it may restart
      // itself.

      if (theRegistry->endPoint()->type() == OTCLIB_ENDPOINT_CLIENT)
      {
	OTCLIB_TRACER(MODULE) << "reset client = ";
	OTCLIB_TRACER(MODULE) << theConnection->remoteAddress() << endl;

	if (theRegistry->exchangeType() != OTCLIB_EXCHANGE_UNKNOWN)
	{
	  OTCEV_ExchangeAnnouncement* tmpEvent;
	  tmpEvent = new OTCEV_ExchangeAnnouncement(group_,
	   theConnection->localAddress(),theConnection->remoteAddress(),
	   theRegistry->remoteProcess(),OTCLIB_CONNECTION_INACTIVE);
	  OTCLIB_ASSERT_M(tmpEvent != 0);

	  registry()->notifyObservers(tmpEvent,OTCLIB_STANDARD_JOB);
	}

	if (theRegistry->exchangeType() == OTCLIB_EXCHANGE_SHUTDOWN)
	{
	  endPoints_.remove(theConnection->localAddress());

	  OTCLIB_TRACER(MODULE) << "shutdown = ";
	  OTCLIB_TRACER(MODULE) << theConnection->localAddress() << endl;
	}

	processes_.remove(theRegistry->remoteProcess(),OTCLIB_UNSAFE);

	theRegistry->reset();
      }
      else if (theRegistry->endPoint()->type() == OTCLIB_ENDPOINT_SERVER)
      {
	OTCLIB_TRACER(MODULE) << "delete server = ";
	OTCLIB_TRACER(MODULE) << theConnection->remoteAddress() << endl;

	if (theRegistry->exchangeType() != OTCLIB_EXCHANGE_UNKNOWN)
	{
	  OTCEV_ExchangeAnnouncement* tmpEvent;
	  tmpEvent = new OTCEV_ExchangeAnnouncement(group_,
	   theConnection->localAddress(),theConnection->remoteAddress(),
	   theRegistry->remoteProcess(),OTCLIB_CONNECTION_INACTIVE);
	  OTCLIB_ASSERT_M(tmpEvent != 0);

	  registry()->notifyObservers(tmpEvent,OTCLIB_STANDARD_JOB);
	}

	processes_.remove(theRegistry->remoteProcess(),OTCLIB_UNSAFE);

	theRegistry->reset();

	endPoints_.remove(theConnection->localAddress());
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::handleServiceAnnouncement(
 OTCEV_ServiceAnnouncement* theAnnouncement
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::handleServiceAnnouncement(OTCEV_ServiceAnnouncement*)");

  // Accept only registry announcements.

  if (theAnnouncement->subscriptionType() != OTCLIB_SERVICE_REGISTRY)
    return;

  OTC_ServiceBinding* theBinding;
  theBinding = theAnnouncement->serviceBinding();

  // Eliminate any services from a remote
  // process with the same network group as we
  // imported them in the first place. Only
  // remote services we are interested in are
  // those from different network groups.

  if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE &&
   theBinding->networkGroup() == group_)
  {
    return;
  }

  // Only worry about services if audience is
  // "*", or if audience is non empty and
  // matches our group. Once we have eliminated
  // services we don't want, treat the
  // remainder as if they are a local service.

  if (theBinding->serviceAudience() == "*" ||
   (!theBinding->serviceAudience().isEmpty() &&
   theBinding->serviceAudience() == group_))
  {
    if (theAnnouncement->status() == OTCLIB_SERVICE_AVAILABLE)
    {
      // New service created in this process.

      if (services_.contains(theBinding->serviceAddress()))
      {
	// We already know about this service.
	// Ignore this subsequent announcement.
	// If anything about the service had
	// changed this will be a problem, but
	// we shouldn't see a second announcement
	// anyway.

	OTC_LogStream theLogger;
	theLogger.setLevel(OTCLIB_LOG_WARNING);

	theLogger << "Sanity check failed" << EOL;
        theLogger << "Class: OTC_Exchange" << EOL;
	theLogger << "Method: handle()" << EOL;
	theLogger << "Description: already knew about this service" << EOL;
	theLogger << "Result: notification will be ignored" << EOL;
	theLogger << "Comment: possibly an implementation problem" << EOL;
	theLogger << "Service: " << theBinding->serviceAddress() << EOL;
	theLogger << flush;
      }
      else
      {
	services_.add(theBinding->serviceAddress(),
	 theBinding,OTCLIB_REPLACE);

	// Broadcast service announcement to
	// other connected exchanges.

	OTC_Capacity tmpCapacity(127);
	OTC_String theContent(tmpCapacity);

	theContent += theBinding->serviceName();
	theContent += EOS;
	theContent += theBinding->receiverName();
	theContent += EOS;

	if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE)
	{
	  theContent += theBinding->processAddress();
	}
	else
	{
	  theContent += name();

	  if (!theBinding->processAddress().isEmpty())
	  {
	    theContent += '!';
	    theContent += theBinding->processAddress();
	  }
	}

	theContent += EOS;
	theContent += theBinding->serviceAudience();

	OTCEV_Message* tmpMessage;
	tmpMessage = new OTCEV_Message(theContent);
	OTCLIB_ASSERT_M(tmpMessage != 0);

	broadcastMessage(0,inBox_.name(),
	 "$E::SERVICE-AVAILABLE","",tmpMessage);
      }
    }
    else
    {
      // Service deleted in this process.

      if (!services_.contains(theBinding->serviceAddress()))
      {
	// We did't know about this service.
	// Ignore the event and don't pass
	// anything on.

	OTC_LogStream theLogger;
	theLogger.setLevel(OTCLIB_LOG_WARNING);

	theLogger << "Sanity check failed" << EOL;
        theLogger << "Class: OTC_Exchange" << EOL;
	theLogger << "Method: handleServiceAnnouncement()" << EOL;
	theLogger << "Description: don't know about this service" << EOL;
	theLogger << "Result: notification will be ignored" << EOL;
	theLogger << "Comment: possibly an implementation problem" << EOL;
	theLogger << "Address: " << theBinding->serviceAddress() << EOL;
	theLogger << flush;
      }
      else
      {
	theBinding = services_.item(theBinding->serviceAddress());

	// Broadcast service announcement to
	// other connected exchanges.

	OTC_Capacity tmpCapacity(127);
	OTC_String theContent(tmpCapacity);

	if (theBinding->serviceLocation() == OTCLIB_SERVICE_LOCAL)
	{
	  theContent += name();
	  theContent += '!';
	}

	theContent += theBinding->serviceAddress();

	OTCEV_Message* tmpMessage;
	// tmpMessage = new OTCEV_Message(theBinding->serviceAddress());
	tmpMessage = new OTCEV_Message(theContent);
	OTCLIB_ASSERT_M(tmpMessage != 0);

	broadcastMessage(0,inBox_.name(),
	 "$E::SERVICE-WITHDRAWN","",tmpMessage);

	// Remove service from local table.

	services_.remove(theBinding->serviceAddress());
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::handleGroupAnnouncement(
 OTCEV_GroupAnnouncement* theAnnouncement
)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::handleGroupAnnouncement(OTCEV_GroupAnnouncement*)");

  // Accept only registry announcements.

  if (theAnnouncement->subscriptionType() != OTCLIB_SERVICE_REGISTRY)
    return;

  OTC_ServiceBinding* theBinding;
  theBinding = theAnnouncement->serviceBinding();

  // Eliminate any services from a remote
  // process with the same network group as we
  // imported them in the first place. Only
  // remote services we are interested in are
  // those from different network groups.

  if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE &&
   theBinding->networkGroup() == group_)
  {
    return;
  }

  // Only worry about services if audience is
  // "*", or if audience is non empty and
  // matches our group. Once we have eliminated
  // services we don't want, treat the
  // remainder as if they are a local service.

  if (theBinding->serviceAudience() == "*" ||
   (!theBinding->serviceAudience().isEmpty() &&
   theBinding->serviceAudience() == group_))
  {
    // Whether the service has joined or left
    // a service group, we need to know about
    // it.

    if (!services_.contains(theBinding->serviceAddress()))
    {
      // We don't know about this service.
      // Ignore this group announcement.

      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_WARNING);

      theLogger << "Sanity check failed" << EOL;
      theLogger << "Class: OTC_Exchange" << EOL;
      theLogger << "Method: handleGroupAnnouncement()" << EOL;
      theLogger << "Description: don't know about this service" << EOL;
      theLogger << "Result: notification will be ignored" << EOL;
      theLogger << "Comment: possibly an implementation problem" << EOL;
      theLogger << "Service: " << theBinding->serviceAddress() << EOL;
      theLogger << flush;
    }
    else
    {
      if ((theAnnouncement->status() == OTCLIB_SERVICE_AVAILABLE &&
       !mapServiceGroup_.contains(theAnnouncement->serviceGroup(),
        theBinding->serviceAddress())) ||
       (theAnnouncement->status() != OTCLIB_SERVICE_AVAILABLE &&
       mapServiceGroup_.contains(theAnnouncement->serviceGroup(),
	theBinding->serviceAddress())))
      {
	// Broadcast group announcement to
	// other connected exchanges.

	OTC_Capacity tmpCapacity(127);
	OTC_String theContent(tmpCapacity);

	theContent += theBinding->serviceName();
	theContent += EOS;
	theContent += theBinding->receiverName();
	theContent += EOS;

	if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE)
	{
	  theContent += theBinding->processAddress();
	}
	else
	{
	  theContent += name();

	  if (!theBinding->processAddress().isEmpty())
	  {
	    theContent += '!';
	    theContent += theBinding->processAddress();
	  }
	}

	theContent += EOS;
	theContent += theAnnouncement->serviceGroup();

	OTCEV_Message* tmpMessage;
	tmpMessage = new OTCEV_Message(theContent);
	OTCLIB_ASSERT_M(tmpMessage != 0);

	if (theAnnouncement->status() == OTCLIB_SERVICE_AVAILABLE)
	{
	  mapServiceGroup_.add(theAnnouncement->serviceGroup(),
	   theBinding->serviceAddress());

	  broadcastMessage(0,inBox_.name(),
	   "$E::GROUP-AVAILABLE","",tmpMessage);
	}
	else
	{
	  broadcastMessage(0,inBox_.name(),
	   "$E::GROUP-WITHDRAWN","",tmpMessage);

	  mapServiceGroup_.remove(theAnnouncement->serviceGroup(),
	   theBinding->serviceAddress());
	}
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::handleExchangeMessage(OTCEV_Envelope* theEnvelope)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::handleExchangeMessage(OTCEV_Envelope*)");

  if (theEnvelope->subject() == "$E::ANNOUNCE-CLIENT")
    messageExchangeAnnounce(theEnvelope);
  else if (theEnvelope->subject() == "$E::ANNOUNCE-SERVER")
    messageExchangeAnnounce(theEnvelope);
  else if (theEnvelope->subject() == "$E::SERVICE-AVAILABLE")
    messageServiceAvailable(theEnvelope);
  else if (theEnvelope->subject() == "$E::SERVICE-WITHDRAWN")
    messageServiceWithdrawn(theEnvelope);
  else if (theEnvelope->subject() == "$E::GROUP-AVAILABLE")
    messageGroupAnnouncement(theEnvelope);
  else if (theEnvelope->subject() == "$E::GROUP-WITHDRAWN")
    messageGroupAnnouncement(theEnvelope);
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::messageExchangeAnnounce(OTCEV_Envelope* theEnvelope)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::messageExchangeAnnounce(OTCEV_Envelope*)");

  OTC_String theRemoteAddress(theEnvelope->previousSender());

  OTC_EndPoint* theEndPoint;
  theEndPoint = OTC_EndPoint::remote(theRemoteAddress);

  if (theEndPoint != 0)
  {
    OTC_EPRegistry* theEPRegistry = 0;
    if (!endPoints_.contains(theEndPoint->localAddress()))
    {
      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_WARNING);

      theLogger << "Sanity check failed" << EOL;
      theLogger << "Class: OTC_Exchange" << EOL;
      theLogger << "Method: messageExchangeAnnounce()" << EOL;
      theLogger << "Description: no connection for this exchange" << EOL;
      theLogger << "Result: exchange will be ignored" << EOL;
      theLogger << "Comment: wrong listener for network group used" << EOL;
      theLogger << "NetworkGroup: " << group_ << EOL;
      theLogger << flush;
    }
    else
    {
      theEPRegistry = endPoints_.item(theEndPoint->localAddress());

      OTC_String theRemoteProcess(theEnvelope->message()->content());

      // Check to see whether we already have
      // a connection to this process for this
      // exchange group.

      if (processes_.contains(theRemoteProcess))
      {
	OTC_LogStream theLogger;
	theLogger.setLevel(OTCLIB_LOG_WARNING);

	theLogger << "Sanity check failed" << EOL;
	theLogger << "Class: OTC_Exchange" << EOL;
	theLogger << "Method: messageExchangeAnnounce()" << EOL;
	theLogger << "Description: already connected to process" << EOL;
	theLogger << "Comment: mutually connected processes" << EOL;
	theLogger << "Action: abort the new connection" << EOL;
	theLogger << "RemoteProcess: " << theRemoteProcess << EOL;
	theLogger << flush;

	theEndPoint->stop();

	return;
      }

      if (theEndPoint->type() == OTCLIB_ENDPOINT_SERVER)
      {
	// Send back response saying we are here.
	// Tell other end whether we are a client
	// or server.

	char const* tmpType = "$E::ANNOUNCE-SERVER";
	if (type() == OTCLIB_EXCHANGE_CLIENT)
	  tmpType = "$E::ANNOUNCE-CLIENT";

	OTCEV_Message* tmpMessage;
	tmpMessage = new OTCEV_Message(localProcess_);
	OTCLIB_ASSERT_M(tmpMessage != 0);

	theEnvelope->reply(tmpMessage,
	 tmpType,"",OTCLIB_STANDARD_JOB);
      }

      processes_.add(theRemoteProcess);

      if (theEnvelope->subject() == "$E::ANNOUNCE-CLIENT")
      {
	OTCLIB_TRACER(MODULE) << "new client = ";
	OTCLIB_TRACER(MODULE) << theRemoteAddress << endl;

	theEPRegistry->setup(OTCLIB_EXCHANGE_CLIENT,theRemoteProcess);
      }
      else
      {
	OTCLIB_TRACER(MODULE) << "new server = ";
	OTCLIB_TRACER(MODULE) << theRemoteAddress << endl;

	theEPRegistry->setup(OTCLIB_EXCHANGE_SERVER,theRemoteProcess);
      }

      OTCEV_ExchangeAnnouncement* tmpEvent;
      tmpEvent = new OTCEV_ExchangeAnnouncement(group_,
       theEndPoint->localAddress(),theRemoteAddress,
       theRemoteProcess,OTCLIB_CONNECTION_ACTIVE);
      OTCLIB_ASSERT_M(tmpEvent != 0);

      registry()->notifyObservers(tmpEvent,OTCLIB_PRIORITY_JOB);

      // Inform new exchange of what services we
      // have access to. Do local services first.
      // Remember only to propogate information
      // about those we are supposed to.

      OTC_Iterator<OTC_ServiceBinding*> services;

      services = services_.items();
      while (services.isValid())
      {
	OTC_ServiceBinding* theBinding;
	theBinding = services.item();

	OTC_Capacity tmpCapacity(127);
	OTC_String theContent(tmpCapacity);

	theContent += theBinding->serviceName();
	theContent += EOS;
	theContent += theBinding->receiverName();
	theContent += EOS;

	if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE)
	{
	  theContent += theBinding->processAddress();
	}
	else
	{
	  theContent += name();

	  if (!theBinding->processAddress().isEmpty())
	  {
	    theContent += '!';
	    theContent += theBinding->processAddress();
	  }
	}

	theContent += EOS;
	theContent += theBinding->serviceAudience();

	OTCEV_Message* tmpMessage;
	tmpMessage = new OTCEV_Message(theContent);
	OTCLIB_ASSERT_M(tmpMessage != 0);

	theEnvelope->reply(tmpMessage,"$E::SERVICE-AVAILABLE",
	 "",OTCLIB_STANDARD_JOB);

	OTC_Iterator<OTC_String> theGroups;
	theGroups = mapServiceGroup_.keys(theBinding->serviceAddress());

	while (theGroups.isValid())
	{
	  OTC_Capacity tmpCapacity(127);
	  OTC_String tmpContent(tmpCapacity);

	  tmpContent += theBinding->serviceName();
	  tmpContent += EOS;
	  tmpContent += theBinding->receiverName();
	  tmpContent += EOS;

	  if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE)
	  {
	    tmpContent += theBinding->processAddress();
	  }
	  else
	  {
	    tmpContent += name();

	    if (!theBinding->processAddress().isEmpty())
	    {
	      tmpContent += '!';
	      tmpContent += theBinding->processAddress();
	    }
	  }

	  tmpContent += EOS;
	  tmpContent += theGroups.item();

	  tmpMessage = new OTCEV_Message(tmpContent);
	  OTCLIB_ASSERT_M(tmpMessage != 0);

	  theEnvelope->reply(tmpMessage,"$E::GROUP-AVAILABLE",
	   "",OTCLIB_STANDARD_JOB);

	  theGroups.next();
	}

	services.next();
      }

      // Now go through each endpoint we already
      // know about. If an endpoint is a client
      // exchange, send notifications back to new
      // endpoint. If an endpoint is a server
      // exchange, only send notifications if new
      // endpoint is a client.

      OTC_Iterator<OTC_EPRegistry*> registries;
      registries = endPoints_.items();
      while (registries.isValid())
      {
	OTC_EPRegistry* tmpEPRegistry;
	tmpEPRegistry = registries.item();

        if (theEPRegistry != tmpEPRegistry &&
	 (tmpEPRegistry->exchangeType() == OTCLIB_EXCHANGE_CLIENT ||
	 theEPRegistry->exchangeType() == OTCLIB_EXCHANGE_CLIENT))
	{
	  services = tmpEPRegistry->services(OTCLIB_FORWARD);
	  while (services.isValid())
	  {
	    OTC_ServiceBinding* theBinding;
	    theBinding = services.item();

	    OTC_Capacity tmpCapacity(255);
	    OTC_String theContent(tmpCapacity);

	    theContent += theBinding->serviceName();
	    theContent += EOS;
	    theContent += theBinding->receiverName();
	    theContent += EOS;
	    theContent += theBinding->processAddress();
	    theContent += EOS;
	    theContent += theBinding->serviceAudience();

	    OTCEV_Message* tmpMessage;
	    tmpMessage = new OTCEV_Message(theContent);
	    OTCLIB_ASSERT_M(tmpMessage != 0);

	    theEnvelope->reply(tmpMessage,"$E::SERVICE-AVAILABLE",
	     "",OTCLIB_STANDARD_JOB);

	    OTC_Iterator<OTC_String> theGroups;
	    theGroups = tmpEPRegistry->serviceGroupsByAddress(
	     theBinding->serviceAddress());

	    while (theGroups.isValid())
	    {
	      OTC_Capacity tmpCapacity(127);
	      OTC_String tmpContent(tmpCapacity);

	      tmpContent += theBinding->serviceName();
	      tmpContent += EOS;
	      tmpContent += theBinding->receiverName();
	      tmpContent += EOS;

	      if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE)
	      {
		tmpContent += theBinding->processAddress();
	      }
	      else
	      {
		tmpContent += name();

		if (!theBinding->processAddress().isEmpty())
		{
		  tmpContent += '!';
		  tmpContent += theBinding->processAddress();
		}
	      }

	      tmpContent += EOS;
	      tmpContent += theGroups.item();

	      tmpMessage = new OTCEV_Message(tmpContent);
	      OTCLIB_ASSERT_M(tmpMessage != 0);

	      theEnvelope->reply(tmpMessage,"$E::GROUP-AVAILABLE",
	       "",OTCLIB_STANDARD_JOB);

	      theGroups.next();
	    }

	    services.next();
	  }
	}

	registries.next();
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::messageServiceAvailable(OTCEV_Envelope* theEnvelope)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::messageServiceAvailable(OTCEV_Envelope*)");

  OTCEV_Message* theMessage;
  theMessage = theEnvelope->message();

  OTC_String theRemoteAddress(theEnvelope->previousSender());

  OTC_EndPoint* theEndPoint;
  theEndPoint = OTC_EndPoint::remote(theRemoteAddress);

  if (theEndPoint != 0)
  {
    if (theMessage != 0)
    {
      OTCLIB_TRACER(MODULE) << "localAddress = "
       << theEndPoint->localAddress() << endl;

      if (endPoints_.contains(theEndPoint->localAddress()))
      {
	OTC_EPRegistry* theEPRegistry = 0;
	theEPRegistry = endPoints_.item(theEndPoint->localAddress());

	OTC_VRecord theFields(theMessage->content(),EOS);

	if (theFields.numFields() == 4)
	{
	  OTC_Capacity tmpCapacity(127);
	  OTC_String theProcessAddress(tmpCapacity);

	  theProcessAddress += name();
	  theProcessAddress += '!';

	  theProcessAddress += theRemoteAddress;
	  if (*theFields[3] != EOS)
	  {
	    theProcessAddress += '!';
	    theProcessAddress += theFields[3];
	  }

	  OTC_String theServiceAddress(tmpCapacity);

	  theServiceAddress += theProcessAddress;
	  theServiceAddress += '!';
	  theServiceAddress += theFields[2];
	  theServiceAddress += '!';
	  theServiceAddress += theFields[1];

	  OTCLIB_TRACER(MODULE) << "address = " << theServiceAddress << endl;

	  if (!theEPRegistry->containsService(theServiceAddress))
	  {
	    // Create service binding.

	    OTC_ServiceBinding* theBinding;
	    theBinding = OTC_SVBroker::registry()->addServiceBinding(
	     theFields[1],theFields[4],theFields[2],
	     theProcessAddress,group_);

	    // Add to endpoint registry.

	    theEPRegistry->addService(theBinding);

	    // Broadcast service announcement to
	    // other connected exchanges.

	    OTC_Capacity tmpCapacity(255);
	    OTC_String theContent(tmpCapacity);

	    theContent += theBinding->serviceName();
	    theContent += EOS;
	    theContent += theBinding->receiverName();
	    theContent += EOS;
	    theContent += theBinding->processAddress();
	    theContent += EOS;
	    theContent += theBinding->serviceAudience();

	    OTCEV_Message* tmpMessage;
	    tmpMessage = new OTCEV_Message(theContent);
	    OTCLIB_ASSERT_M(tmpMessage != 0);

	    broadcastMessage(theEPRegistry,inBox_.name(),
	     "$E::SERVICE-AVAILABLE","",tmpMessage);
	  }
	  else
	  {
	    OTC_LogStream theLogger;
	    theLogger.setLevel(OTCLIB_LOG_WARNING);

	    theLogger << "Sanity check failed" << EOL;
	    theLogger << "Class: OTC_Exchange" << EOL;
	    theLogger << "Method: messageServiceAvailable()" << EOL;
	    theLogger << "Description: already know about this service" << EOL;
	    theLogger << "Result: notification will be ignored" << EOL;
	    theLogger << "Comment: possibly an implementation problem" << EOL;
	    theLogger << "RemoteAddress: " << theRemoteAddress << EOL;
	    theLogger << "ServiceAddress: " << theServiceAddress << EOL;
	    theLogger << flush;
	  }
	}
	else
	{
	  OTC_LogStream theLogger;
	  theLogger.setLevel(OTCLIB_LOG_WARNING);

	  theLogger << "Sanity check failed" << EOL;
	  theLogger << "Class: OTC_Exchange" << EOL;
	  theLogger << "Method: messageServiceAvailable()" << EOL;
	  theLogger << "Description: invalid number of fields" << EOL;
	  theLogger << "Comment: possibly an implementation problem" << EOL;
	  theLogger << "NumFields: " << theFields.numFields() << EOL;
	  theLogger << flush;
	}
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::messageServiceWithdrawn(OTCEV_Envelope* theEnvelope)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::messageServiceWithdrawn(OTCEV_Envelope*)");

  OTCEV_Message* theMessage;
  theMessage = theEnvelope->message();

  OTC_String theRemoteAddress(theEnvelope->previousSender());

  OTC_EndPoint* theEndPoint;
  theEndPoint = OTC_EndPoint::remote(theRemoteAddress);

  if (theEndPoint != 0)
  {
    if (theMessage != 0)
    {
      if (endPoints_.contains(theEndPoint->localAddress()))
      {
	OTC_EPRegistry* theEPRegistry = 0;
	theEPRegistry = endPoints_.item(theEndPoint->localAddress());

	OTC_Capacity tmpCapacity(127);
	OTC_String theServiceAddress(tmpCapacity);

	theServiceAddress += name();
	theServiceAddress += '!';
	theServiceAddress += theRemoteAddress;
	theServiceAddress += '!';
	theServiceAddress += theMessage->content();

	if (theEPRegistry->containsService(theServiceAddress))
	{
	  theEPRegistry->removeService(theServiceAddress);

	  // Broadcast service announcement to
	  // other connected exchanges.

	  OTCEV_Message* tmpMessage;
	  tmpMessage = new OTCEV_Message(theServiceAddress);
	  OTCLIB_ASSERT_M(tmpMessage != 0);

	  broadcastMessage(theEPRegistry,inBox_.name(),
	   "$E::SERVICE-WITHDRAWN","",tmpMessage);

	  // Remove from service registry.

	  OTC_SVBroker::registry()->removeServiceBinding(
	   theServiceAddress);
	}
	else
	{
	  OTC_LogStream theLogger;
	  theLogger.setLevel(OTCLIB_LOG_WARNING);

	  theLogger << "Sanity check failed" << EOL;
	  theLogger << "Class: OTC_Exchange" << EOL;
	  theLogger << "Method: messageServiceWithdrawn()" << EOL;
	  theLogger << "Description: don't know about this service" << EOL;
	  theLogger << "Result: notification will be ignored" << EOL;
	  theLogger << "Comment: possibly an implementation problem" << EOL;
	  theLogger << "RemoteAddress: " << theRemoteAddress << EOL;
	  theLogger << "Service: " << theServiceAddress << EOL;
	  theLogger << flush;
	}
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::messageGroupAnnouncement(OTCEV_Envelope* theEnvelope)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::messageGroupAnnouncement(OTCEV_Envelope*)");

  OTCEV_Message* theMessage;
  theMessage = theEnvelope->message();

  OTC_String theRemoteAddress(theEnvelope->previousSender());

  OTC_EndPoint* theEndPoint;
  theEndPoint = OTC_EndPoint::remote(theRemoteAddress);

  if (theEndPoint != 0)
  {
    if (theMessage != 0)
    {
      OTCLIB_TRACER(MODULE) << "localAddress = "
       << theEndPoint->localAddress() << endl;

      if (endPoints_.contains(theEndPoint->localAddress()))
      {
	OTC_EPRegistry* theEPRegistry = 0;
	theEPRegistry = endPoints_.item(theEndPoint->localAddress());

	OTC_VRecord theFields(theMessage->content(),EOS);

	if (theFields.numFields() == 4)
	{
	  OTC_Capacity tmpCapacity(127);
	  OTC_String theProcessAddress(tmpCapacity);

	  theProcessAddress += name();
	  theProcessAddress += '!';

	  theProcessAddress += theRemoteAddress;
	  if (*theFields[3] != EOS)
	  {
	    theProcessAddress += '!';
	    theProcessAddress += theFields[3];
	  }

	  OTC_String theServiceAddress(tmpCapacity);

	  theServiceAddress += theProcessAddress;
	  theServiceAddress += '!';
	  theServiceAddress += theFields[2];
	  theServiceAddress += '!';
	  theServiceAddress += theFields[1];

	  OTCLIB_TRACER(MODULE) << "address = " << theServiceAddress << endl;

	  if (theEPRegistry->containsService(theServiceAddress))
	  {
	    if ((theEnvelope->subject() == "$E::GROUP-AVAILABLE" &&
	     !theEPRegistry->isServiceGroupMember(theFields[4],
	      theServiceAddress)) ||
	     (theEnvelope->subject() != "$E::GROUP-AVAILABLE" &&
	     theEPRegistry->isServiceGroupMember(theFields[4],
	      theServiceAddress)))
	    {
	      OTC_ServiceBinding* theBinding;
	      theBinding = theEPRegistry->service(theServiceAddress);

	      // Add or remove from service group.

	      if (theEnvelope->subject() == "$E::GROUP-AVAILABLE")
	      {
		theEPRegistry->addToServiceGroup(
		 theFields[4],theServiceAddress);

		OTC_SVBroker::registry()->addToServiceGroup(
		 theFields[4],theServiceAddress);
	      }
	      else
	      {
		OTC_SVBroker::registry()->removeFromServiceGroup(
		 theFields[4],theServiceAddress);

		theEPRegistry->removeFromServiceGroup(
		 theFields[4],theServiceAddress);
	      }

	      // Broadcast group announcement to
	      // other connected exchanges.

	      OTC_Capacity tmpCapacity(255);
	      OTC_String theContent(tmpCapacity);

	      theContent += theBinding->serviceName();
	      theContent += EOS;
	      theContent += theBinding->receiverName();
	      theContent += EOS;

	      if (theBinding->serviceLocation() == OTCLIB_SERVICE_REMOTE)
	      {
		theContent += theBinding->processAddress();
	      }
	      else
	      {
		theContent += name();

		if (!theBinding->processAddress().isEmpty())
		{
		  theContent += '!';
		  theContent += theBinding->processAddress();
		}
	      }

	      theContent += EOS;
	      theContent += theFields[4];

	      OTCEV_Message* tmpMessage;
	      tmpMessage = new OTCEV_Message(theContent);
	      OTCLIB_ASSERT_M(tmpMessage != 0);

	      broadcastMessage(theEPRegistry,inBox_.name(),
	       theEnvelope->subject(),"",tmpMessage);
	    }
	  }
	  else
	  {
	    OTC_LogStream theLogger;
	    theLogger.setLevel(OTCLIB_LOG_WARNING);

	    theLogger << "Sanity check failed" << EOL;
	    theLogger << "Class: OTC_Exchange" << EOL;
	    theLogger << "Method: messageGroupAnnouncement()" << EOL;
	    theLogger << "Description: don't know about this service" << EOL;
	    theLogger << "Result: notification will be ignored" << EOL;
	    theLogger << "Comment: possibly an implementation problem" << EOL;
	    theLogger << "RemoteAddress: " << theRemoteAddress << EOL;
	    theLogger << "ServiceAddress: " << theServiceAddress << EOL;
	    theLogger << flush;
	  }
	}
	else
	{
	  OTC_LogStream theLogger;
	  theLogger.setLevel(OTCLIB_LOG_WARNING);

	  theLogger << "Sanity check failed" << EOL;
	  theLogger << "Class: OTC_Exchange" << EOL;
	  theLogger << "Method: messageGroupAnnouncement()" << EOL;
	  theLogger << "Description: invalid number of fields" << EOL;
	  theLogger << "Comment: possibly an implementation problem" << EOL;
	  theLogger << "NumFields: " << theFields.numFields() << EOL;
	  theLogger << flush;
	}
      }
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::handleBroadcastMessage(OTCEV_Envelope* theEnvelope)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_Exchange::handleBroadcastMessage(OTCEV_Envelope*)");

  OTCLIB_TRACER(MODULE) << "last = "
   << theEnvelope->previousSender() << endl;

  // Make sure we do not send messages out via
  // link from which we just received it. If we
  // are a client exchange, send it out to all
  // exchanges. If we are a server exchange,
  // send it out to client exchanges but only
  // send it out to server exchanges if we
  // received it from a client exchange.

  OTC_EndPoint* theEndPoint = 0;
  OTC_EPRegistry* theRegistry = 0;

  theEndPoint = OTC_EndPoint::remote(theEnvelope->previousSender());

  if (theEndPoint != 0)
  {
    if (endPoints_.contains(theEndPoint->localAddress()))
      theRegistry = endPoints_.item(theEndPoint->localAddress());
  }

  bool tmpServerBounce;
  tmpServerBounce = false;

  if (theRegistry != 0)
  {
    if (theRegistry->exchangeType() == OTCLIB_EXCHANGE_CLIENT)
      tmpServerBounce = true;
  }

  OTC_Iterator<OTC_EPRegistry*> registries;

  registries = endPoints_.items();
  while (registries.isValid())
  {
    theRegistry = registries.item();
    theEndPoint = theRegistry->endPoint();

    if (theEnvelope->previousSender() != theEndPoint->remoteAddress())
    {
      if (type() == OTCLIB_EXCHANGE_CLIENT ||
       theRegistry->exchangeType() == OTCLIB_EXCHANGE_CLIENT ||
       tmpServerBounce != false)
      {
        OTC_Capacity tmpCapacity(255);
	OTC_String theAddress(tmpCapacity);

	theAddress += theEndPoint->remoteAddress();
	theAddress += '!';
	theAddress += theEnvelope->to();

	theEnvelope->bounce(theAddress,OTCLIB_STANDARD_JOB);
      }
    }

    registries.next();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Exchange::broadcastMessage(
 OTC_EPRegistry* theEPRegistry,
 OTC_String const& theFromAddress,
 OTC_String const& theSubject,
 OTC_String const& theHeader,
 OTC_Event* theEvent
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_Exchange::broadcastMessage(...)");

  OTCLIB_TRACER(MODULE) << "registry = " << (theEPRegistry!=0) << endl;
  OTCLIB_TRACER(MODULE) << "subject = " << theSubject << endl;
  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  // Make sure we do not send messages out via
  // link from which we just received it. If we
  // are a client exchange, send it out to all
  // exchanges. If we are a server exchange,
  // send it out to client exchanges but only
  // send it out to server exchanges if we
  // received it from a client exchange.

  bool tmpServerBounce;
  tmpServerBounce = false;

  OTC_String theRemoteAddress;

  if (theEPRegistry != 0)
  {
    theRemoteAddress = theEPRegistry->remoteAddress();
    if (theEPRegistry->exchangeType() == OTCLIB_EXCHANGE_CLIENT)
      tmpServerBounce = true;
  }

  OTCLIB_TRACER(MODULE) << "remote = " << theRemoteAddress << endl;
  OTCLIB_TRACER(MODULE) << "bounce = " << tmpServerBounce << endl;

  OTC_Iterator<OTC_EPRegistry*> registries;
  OTC_EPRegistry* tmpEPRegistry = 0;

  registries = endPoints_.items();
  while (registries.isValid())
  {
    tmpEPRegistry = registries.item();

    OTCLIB_TRACER(MODULE) << "nextRecipient = " <<
     tmpEPRegistry->remoteAddress() << endl;

    if (theRemoteAddress != tmpEPRegistry->remoteAddress())
    {
      OTCLIB_TRACER(MODULE) << "type = " <<
       int(tmpEPRegistry->exchangeType()) << endl;

      if (
       tmpEPRegistry->exchangeType() != OTCLIB_EXCHANGE_UNKNOWN &&
       (type() == OTCLIB_EXCHANGE_CLIENT ||
       tmpEPRegistry->exchangeType() == OTCLIB_EXCHANGE_CLIENT ||
       tmpServerBounce != false))
      {
        OTC_Capacity tmpCapacity(255);
	OTC_String theAddress(tmpCapacity);

	theAddress += tmpEPRegistry->remoteAddress();
	theAddress += '!';
	theAddress += tmpEPRegistry->exchangeName();

	OTCLIB_TRACER(MODULE) << "to address = " << theAddress << endl;

	OTCEV_Envelope* theEnvelope;

	theEnvelope = new OTCEV_Envelope(theEvent->clone(),
	 theAddress,theFromAddress,theSubject,theHeader);
	OTCLIB_ASSERT_M(theEnvelope != 0);

	theEnvelope->queue(OTCLIB_STANDARD_JOB);
      }
    }

    registries.next();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
