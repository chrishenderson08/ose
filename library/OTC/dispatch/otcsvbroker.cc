/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsvbroker.cc
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
#pragma implementation "OTC/dispatch/svbroker.hh"
#endif
#endif

#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/report.hh>
#include <OTC/dispatch/request.hh>
#include <OTC/dispatch/response.hh>
#include <OTC/dispatch/failure.hh>
#include <OTC/dispatch/opaque.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/servannc.hh>
#include <OTC/dispatch/pbntfctn.hh>
#include <OTC/dispatch/sbscrptn.hh>
#include <OTC/text/symbol.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/strstream.h>

#include <time.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_SVBroker");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_SVBroker::mutex_;
OTC_NRMutex OTC_SVBroker::sdmutex_;
OTC_SVRegistry* OTC_SVBroker::gRegistry_ = 0;

/* ------------------------------------------------------------------------- */
class OTC_ReportCache : public OTC_MPObject
{
  public:

                        ~OTC_ReportCache() {}

                        OTC_ReportCache(
                         OTC_String const& theContent,
                         char const* theContentType,
                         time_t thePostingTime,
                         time_t theExpirationTime
                        )
                          : content_(theContent),
                            contentType_(theContentType),
                            postingTime_(thePostingTime),
                            expirationTime_(theExpirationTime) {}

    OTC_String const&	content() const
				{ return content_; }

    OTC_String const&	contentType() const
				{ return contentType_; }

    time_t              postingTime() const
                                { return postingTime_; }

    time_t              expirationTime() const
                                { return expirationTime_; }

  private:

    OTC_String		content_;

    OTC_String		contentType_;

    time_t              postingTime_;

    time_t              expirationTime_;
};

/* ------------------------------------------------------------------------- */
OTC_SVRegistry* OTC_SVBroker::registry()
{
  sdmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(sdmutex_);

  if (gRegistry_ == 0)
  {
    gRegistry_ = new OTC_SVRegistry;
    OTCLIB_ASSERT_M(gRegistry_ != 0);
  }

  xxxMutex.release();

  sdmutex_.unlock();

  return gRegistry_;
}

/* ------------------------------------------------------------------------- */
OTC_SVBroker::~OTC_SVBroker()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::~OTC_SVBroker()");

  // Remove all cached reports.

  expireReports(0);

  // Remove all registry subscriptions.

  registry()->unsubscribeAgent(id());

  // Remove service binding.

  registry()->removeServiceBinding(serviceBinding_->serviceAddress());

  serviceBinding_->destroy();
}

/* ------------------------------------------------------------------------- */
OTC_SVBroker::OTC_SVBroker(
 OTC_EVAgent* theAgent,
 char const* theServiceName,
 char const* theServiceAudience
)
  : agent_(theAgent),
    serviceBinding_(0),
    serviceVisibility_(OTCLIB_SERVICE_VISIBLE)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::OTC_SVBroker()");

  OTCLIB_ENSURE_FN((theAgent != 0),
   "OTC_SVBroker::OTC_SVBroker()",
   "invalid service agent");

  // Create anonymous receiver.

  inBox_.bind(this);

  // Create binding.

  serviceBinding_ = new OTC_ServiceBinding(theServiceName,
   theServiceAudience,inBox_.name(),"","");
  OTCLIB_ASSERT_M(serviceBinding_ != 0);

  registry()->addServiceBinding(serviceBinding_);

  OTCLIB_TRACER(MODULE) << "serviceName = "
   << serviceBinding_->serviceName() << endl;
  OTCLIB_TRACER(MODULE) << "serviceAddress = "
   << serviceBinding_->serviceAddress() << endl;
  OTCLIB_TRACER(MODULE) << "serviceAudience = "
   << serviceBinding_->serviceAudience() << endl;
}

/* ------------------------------------------------------------------------- */
OTC_SVBroker::OTC_SVBroker(
 OTC_EVAgent* theAgent,
 char const* theServiceName,
 char const* theServiceAudience,
 OTC_ServiceVisibility theServiceVisibility
)
  : agent_(theAgent),
    serviceBinding_(0),
    serviceVisibility_(theServiceVisibility)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::OTC_SVBroker()");

  OTCLIB_ENSURE_FN((theAgent != 0),
   "OTC_SVBroker::OTC_SVBroker()",
   "invalid service agent");

  // Create anonymous receiver.

  inBox_.bind(this);

  // Create binding.

  serviceBinding_ = new OTC_ServiceBinding(theServiceName,
   theServiceAudience,inBox_.name(),"","");
  OTCLIB_ASSERT_M(serviceBinding_ != 0);

  // Register binding if visible immediately.

  if (theServiceVisibility == OTCLIB_SERVICE_VISIBLE)
    registry()->addServiceBinding(serviceBinding_);

  OTCLIB_TRACER(MODULE) << "serviceName = "
   << serviceBinding_->serviceName() << endl;
  OTCLIB_TRACER(MODULE) << "serviceAddress = "
   << serviceBinding_->serviceAddress() << endl;
  OTCLIB_TRACER(MODULE) << "serviceAudience = "
   << serviceBinding_->serviceAudience() << endl;
  OTCLIB_TRACER(MODULE) << "serviceVisibility = "
   << int(theServiceVisibility) << endl;
}

/* ------------------------------------------------------------------------- */
OTC_ServiceVisibility OTC_SVBroker::serviceVisibility() const
{
  OTC_ServiceVisibility theResult;

  mutex_.lock();

  theResult = serviceVisibility_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::announceAvailability()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::announceAvailability()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (serviceVisibility_ == OTCLIB_SERVICE_HIDDEN)
  {
    serviceVisibility_ = OTCLIB_SERVICE_VISIBLE;

    registry()->addServiceBinding(serviceBinding_);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::handle()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_ServiceAnnouncement::typeId())
  {
    OTCEV_ServiceAnnouncement* theAnnouncement;
    theAnnouncement = (OTCEV_ServiceAnnouncement*) theEvent;

    if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_NAME)
    {
      OTC_ServiceBinding* theBinding;
      theBinding = theAnnouncement->serviceBinding();

      OTC_String theKey(theBinding->serviceName());

      mutex_.lock();

      OTC_MutexReaper<OTC_NRMutex> xxxMutex;
      xxxMutex.grab(mutex_);

      if (theAnnouncement->status() == OTCLIB_SERVICE_AVAILABLE)
      {
	if (!publishers_.contains(theKey,theBinding))
	{
	  publishers_.add(theKey,theBinding);

	  OTC_Iterator<OTC_String> theSubjectPatterns;
	  theSubjectPatterns = subscriptions_.items(theKey);

	  while (theSubjectPatterns.isValid())
	  {
	    // Send subscription request.

	    OTC_Capacity tmpCapacity(63);
	    OTC_String theHeader(tmpCapacity);

	    theHeader += "SubjectPattern: ";
	    theHeader += theSubjectPatterns.item();
	    theHeader += EOL;

	    OTC_Event* theMessage;
	    theMessage = new OTCEV_Message("");
	    OTCLIB_ASSERT_M(theMessage != 0);

	    OTCEV_Envelope* theEnvelope;
	    theEnvelope = new OTCEV_Envelope(theMessage,
	     theBinding->serviceAddress(),
	     serviceBinding_->serviceAddress(),
	     "$S::SUBSCRIPTION-REQUESTED",theHeader);
	    OTCLIB_ASSERT_M(theEnvelope != 0);

	    theEnvelope->queue(OTCLIB_STANDARD_JOB);

	    theSubjectPatterns.next();
	  }

	  OTCEV_PublisherNotification* theNotification;

	  theNotification = new OTCEV_PublisherNotification(
	   theBinding,serviceBinding_,theAnnouncement->status());
	  OTCLIB_ASSERT_M(theNotification != 0);

	  theNotification->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
	}
      }
      else
      {
	if (publishers_.containsItem(theBinding))
	{
	  publishers_.removeItem(theBinding);

	  OTCEV_PublisherNotification* theNotification;

	  theNotification = new OTCEV_PublisherNotification(
	   theBinding,serviceBinding_,theAnnouncement->status());
	  OTCLIB_ASSERT_M(theNotification != 0);

	  theNotification->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
	}
      }

      xxxMutex.release();

      mutex_.unlock();
    }
    else if (theAnnouncement->subscriptionType() == OTCLIB_SERVICE_ADDRESS)
    {
      OTC_ServiceBinding* theBinding;
      theBinding = theAnnouncement->serviceBinding();

      mutex_.lock();

      OTC_MutexReaper<OTC_NRMutex> xxxMutex;
      xxxMutex.grab(mutex_);

      if (theAnnouncement->status() == OTCLIB_SERVICE_AVAILABLE)
      {
	if (!serviceAgents_.contains(theBinding->serviceAddress()))
	{
	  serviceAgents_.add(theBinding->serviceAddress(),theBinding);

	  OTCLIB_TRACER(MODULE) << "serviceAgent = "
	   << theBinding->serviceAddress() << endl;

          if (!agentPublishers_.containsKey(theBinding->serviceAddress()))
	    agentPublishers_.add(theBinding->serviceAddress(),theBinding);

	  // Now need to see if we are wanting to
	  // subscribe to the specific service agent.

	  OTC_Iterator<OTC_String> theSubjectPatterns;
	  theSubjectPatterns = agentSubscriptions_.items(
	   theBinding->serviceAddress());

	  while (theSubjectPatterns.isValid())
	  {
	    OTCLIB_TRACER(MODULE) << "subject = "
	     << theSubjectPatterns.item() << endl;

	    OTC_Capacity tmpCapacity(63);
	    OTC_String theHeader(tmpCapacity);

	    theHeader += "SubjectPattern: ";
	    theHeader += theSubjectPatterns.item();
	    theHeader += EOL;

	    OTC_Event* theMessage;
	    theMessage = new OTCEV_Message("");
	    OTCLIB_ASSERT_M(theMessage != 0);

	    OTCEV_Envelope* theEnvelope;
	    theEnvelope = new OTCEV_Envelope(theMessage,
	     theBinding->serviceAddress(),
	     serviceBinding_->serviceAddress(),
	     "$S::SUBSCRIPTION-REQUESTED",theHeader);
	    OTCLIB_ASSERT_M(theEnvelope != 0);

	    theEnvelope->queue(OTCLIB_STANDARD_JOB);

	    theSubjectPatterns.next();
	  }

	  OTCEV_PublisherNotification* theNotification;

	  theNotification = new OTCEV_PublisherNotification(
	   theBinding,serviceBinding_,theAnnouncement->status());
	  OTCLIB_ASSERT_M(theNotification != 0);

	  theNotification->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
	}
      }
      else
      {
	if (serviceAgents_.contains(theBinding->serviceAddress()))
	{
	  serviceAgents_.remove(theBinding->serviceAddress());

	  OTCLIB_TRACER(MODULE) << "serviceAgent = "
	   << theBinding->serviceAddress() << endl;

	  if (subscribers_.containsItem(theBinding))
	  {
	    OTC_Iterator<OTC_String> theSubjectPatterns;
	    theSubjectPatterns = subscribers_.keys(theBinding);

	    while (theSubjectPatterns.isValid())
	    {
	      OTCEV_Subscription* theSubscription;

	      OTCLIB_TRACER(MODULE) << "subject = "
	       << theSubjectPatterns.item() << endl;

	      theSubscription = new OTCEV_Subscription(
	       serviceBinding_,theAnnouncement->serviceBinding(),
	       OTCLIB_SUBSCRIPTION_WITHDRAWN,theSubjectPatterns.item());
	      OTCLIB_ASSERT_M(theSubscription != 0);

	      theSubscription->queue(agent_->id(),OTCLIB_PRIORITY_JOB);

	      theSubjectPatterns.next();
	    }

	    subscribers_.removeItem(theBinding);
	  }

	  if (agentSubscriptions_.containsKey(theBinding->serviceAddress()))
	  {
	    OTCLIB_TRACER(MODULE) << "subscriber" << endl;

	    if (agentPublishers_.containsItem(theBinding))
	    {
	      OTCLIB_TRACER(MODULE) << "publisher" << endl;

	      agentPublishers_.removeItem(theBinding);

	      OTCEV_PublisherNotification* theNotification;

	      theNotification = new OTCEV_PublisherNotification(
	       theBinding,serviceBinding_,theAnnouncement->status());
	      OTCLIB_ASSERT_M(theNotification != 0);

	      theNotification->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
	    }
	  }
	  else
	  {
	    // We aren't subscribed by service address
	    // and subject so can unsubscribe from the
	    // existance of the service agent.

	    registry()->unsubscribeServiceAddress(
	     id(),theBinding->serviceAddress());
	  }
	}
      }

      xxxMutex.release();

      mutex_.unlock();
    }
  }
  else if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->subject() == "$S::REPORT")
    {
      OTC_Iterator<OTC_ServiceBinding*> thePublisherHandle;
      thePublisherHandle = registry()->lookupServiceAddress(
       theEnvelope->from());

      if (thePublisherHandle.isValid())
      {
	OTCEV_Report* thePublication;

	thePublication = new OTCEV_Report(theEnvelope,
	 thePublisherHandle.item(),serviceBinding_);
	OTCLIB_ASSERT_M(thePublication != 0);

	thePublication->queue(agent_->id(),OTCLIB_PRIORITY_JOB);

	OTCLIB_TRACER(MODULE) << "serviceName = " <<
	 thePublisherHandle.item()->serviceName() << endl;
	OTCLIB_TRACER(MODULE) << "subjectName = " <<
	 thePublication->subjectName() << endl;
	OTCLIB_TRACER(MODULE) << "subjectPattern = " <<
	 thePublication->subjectPattern() << endl;
      }
    }
    else if (theEnvelope->subject() == "$S::REQUEST")
    {
      if (serviceBinding_->serviceAddress() == theEnvelope->to())
      {
	OTCEV_Request* theRequest;

	theRequest = new OTCEV_Request(theEnvelope,serviceBinding_);
	OTCLIB_ASSERT_M(theRequest != 0);

	if (!builtinRequest_(theRequest))
	  theRequest->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
      }
    }
    else if (theEnvelope->subject() == "$S::RESPONSE")
    {
      if (serviceBinding_->serviceAddress() == theEnvelope->to())
      {
	OTCEV_Response* theResponse;

	theResponse = new OTCEV_Response(theEnvelope);
	OTCLIB_ASSERT_M(theResponse != 0);

	theResponse->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
      }
    }
    else if (theEnvelope->subject() == "$S::FAILURE")
    {
      if (serviceBinding_->serviceAddress() == theEnvelope->to())
      {
	OTCEV_Failure* theFailure;

	theFailure = new OTCEV_Failure(theEnvelope);
	OTCLIB_ASSERT_M(theFailure != 0);

	theFailure->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
      }
    }
    else if (theEnvelope->subject() == "$S::MESSAGE")
    {
      if (serviceBinding_->serviceAddress() == theEnvelope->to())
      {
	OTCEV_Opaque* theMessage;

	theMessage = new OTCEV_Opaque(theEnvelope,serviceBinding_);
	OTCLIB_ASSERT_M(theMessage != 0);

	theMessage->queue(agent_->id(),OTCLIB_PRIORITY_JOB);
      }
    }
    else if (theEnvelope->subject() == "$S::SUBSCRIPTION-REQUESTED" ||
     theEnvelope->subject() == "$S::SUBSCRIPTION-WITHDRAWN")
    {
      mutex_.lock();

      OTC_MutexReaper<OTC_NRMutex> xxxMutex;
      xxxMutex.grab(mutex_);

      if (serviceBinding_->serviceAddress() == theEnvelope->to())
      {
	OTC_String theHeader(theEnvelope->header());

	istrstream ins1((char*)theHeader.string(),theHeader.length());

	OTC_Capacity tmpCapacity(63);
	OTC_String tmpLine(tmpCapacity);
	OTC_String theSubjectPattern(tmpCapacity);

	while (ins1.good())
	{
	  tmpLine.readToDelim(ins1,EOL);
	  if (!ins1.fail())
	  {
	    if (tmpLine.compare(0,"SubjectPattern: ",16))
	      theSubjectPattern = tmpLine.after(15);
	  }
	}

	OTC_Iterator<OTC_ServiceBinding*> theSubscriberHandle;
	theSubscriberHandle = registry()->lookupServiceAddress(
	 theEnvelope->from());

        if (!theSubjectPattern.isUndefined() && theSubscriberHandle.isValid())
	{
	  if (theEnvelope->subject() == "$S::SUBSCRIPTION-REQUESTED")
	  {
	    OTCEV_Subscription* theSubscription;

	    theSubscription = new OTCEV_Subscription(
	     serviceBinding_,theSubscriberHandle.item(),
	     OTCLIB_SUBSCRIPTION_REQUESTED,theSubjectPattern);
	    OTCLIB_ASSERT_M(theSubscription != 0);

	    theSubscription->queue(agent_->id(),OTCLIB_PRIORITY_JOB);

	    if (!subscribers_.containsItem(theSubscriberHandle.item()))
	    {
	      // Check first that we haven't already
	      // subscribed to the service agents existance
	      // because of a subscription based on service
	      // address and subject.

	      if (!agentSubscriptions_.containsKey(
	       theSubscriberHandle.item()->serviceAddress()))
	      {
		registry()->subscribeServiceAddress(id(),
		 theSubscriberHandle.item()->serviceAddress());
	      }
	    }

	    if (!subscribers_.contains(theSubjectPattern,
	     theSubscriberHandle.item()))
	    {
	      subscribers_.add(theSubjectPattern,theSubscriberHandle.item());

	      // Send MATCHING cached events which haven't expired.

	      OTC_HIndexEntry<OTC_String>* theEntry;
	      theEntry = subjects_.firstEntry();

	      time_t theCurrentTime;
	      theCurrentTime = time(0);

	      while (theEntry != 0)
	      {
		if (matchSubject(theSubjectPattern,theEntry->key()))
		{
		  OTC_ReportCache* theCacheEntry;
		  theCacheEntry = (OTC_ReportCache*)theEntry->item();

		  if (theCacheEntry->expirationTime() != -1 &&
		   theCurrentTime > theCacheEntry->expirationTime())
		  {
		    delete theCacheEntry;

		    OTC_HIndexEntry<OTC_String>* theOldEntry;
		    theOldEntry = theEntry;

		    theEntry = theEntry->nextEntry();

		    theOldEntry->removeEntry();
		  }
		  else
		  {
		    // Publish event.

		    OTC_Capacity tmpCapacity(127);
		    OTC_String tmpHeader(tmpCapacity);

		    tmpHeader += "SubjectName: ";
		    tmpHeader += theEntry->key();
		    tmpHeader += EOL;
		    tmpHeader += "SubjectPattern: ";
		    tmpHeader += theSubjectPattern;
		    tmpHeader += EOL;

		    OTCEV_Message* theMessage;
		    theMessage = new OTCEV_Message(
		     theCacheEntry->content(),theCacheEntry->contentType());
		    OTCLIB_ASSERT_M(theMessage != 0);

		    OTCEV_Envelope* tmpEnvelope;
		    tmpEnvelope = new OTCEV_Envelope(theMessage,
		     theSubscriberHandle.item()->serviceAddress(),
		     serviceBinding_->serviceAddress(),
		     "$S::REPORT",tmpHeader);
		    OTCLIB_ASSERT_M(tmpEnvelope != 0);

		    tmpEnvelope->queue(OTCLIB_STANDARD_JOB);

		    theEntry = theEntry->nextEntry();
		  }
		}
		else
		{
		  theEntry = theEntry->nextEntry();
		}
	      }
	    }
	  }
	  else
	  {
	    if (subscribers_.contains(theSubjectPattern,
	     theSubscriberHandle.item()))
	    {
	      OTCEV_Subscription* theSubscription;

	      theSubscription = new OTCEV_Subscription(
	       serviceBinding_,theSubscriberHandle.item(),
	       OTCLIB_SUBSCRIPTION_WITHDRAWN,theSubjectPattern);
	      OTCLIB_ASSERT_M(theSubscription != 0);

	      theSubscription->queue(agent_->id(),OTCLIB_PRIORITY_JOB);

	      subscribers_.remove(theSubjectPattern,
	       theSubscriberHandle.item());

	      if (!subscribers_.containsItem(theSubscriberHandle.item()))
	      {
		// Check first that we don't still need to be
		// subscribed to the service agents existance
		// because of a subscription based on service
		// address and subject.

		if (!agentSubscriptions_.containsKey(
		 theSubscriberHandle.item()->serviceAddress()))
		{
		  registry()->unsubscribeServiceAddress(id(),
		   theSubscriberHandle.item()->serviceAddress());
		}
	      }
	    }
	  }
	}
      }

      xxxMutex.release();

      mutex_.unlock();
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::joinGroup(char const* theServiceGroup)
{
  registry()->addToServiceGroup(theServiceGroup,
   serviceBinding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::leaveGroup(char const* theServiceGroup)
{
  registry()->removeFromServiceGroup(theServiceGroup,
   serviceBinding_->serviceAddress());
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_SVBroker::sendRequest(
 OTC_ServiceBinding* theServiceAgent,
 char const* theMethodName,
 OTC_ROPayload const& theContent
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendRequest()");

  if (theMethodName == 0)
    theMethodName = "";

  if (theServiceAgent == 0)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Precondition failed" << EOL;
    theLogger << "Class: OTC_SVBroker" << EOL;
    theLogger << "Method: sendRequest()" << EOL;
    theLogger << "Description: service is a null pointer" << EOL;
    theLogger << "Result: the request will be ignored" << EOL;
    theLogger << "MethodName: " << theMethodName << EOL;
    theLogger << flush;

    return OTC_String::undefinedString();
  }

  // Send the request.

  OTC_String tmpContent(theContent.encode());
  char const* tmpContentType = "text/xml";

  OTC_Capacity tmpCapacity(63);
  OTC_String theHeader(tmpCapacity);

  theHeader += "MethodName: ";
  theHeader += theMethodName;
  theHeader += EOL;

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(tmpContent,tmpContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(
   theMessage,theServiceAgent->serviceAddress(),
   serviceBinding_->serviceAddress(),"$S::REQUEST",theHeader);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  OTCLIB_TRACER(MODULE) << "sendRequest =" << *theEnvelope << endl;
  OTCLIB_TRACER(MODULE) << "conversationId =" << theConversationId << endl;

  theEnvelope->queue(OTCLIB_STANDARD_JOB);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_SVBroker::sendRequest(
 char const* theServiceAddress,
 char const* theMethodName,
 OTC_ROPayload const& theContent
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendRequest()");

  if (theMethodName == 0)
    theMethodName = "";

  if (theServiceAddress == 0 || *theServiceAddress == EOS)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Precondition failed" << EOL;
    theLogger << "Class: OTC_SVBroker" << EOL;
    theLogger << "Method: sendRequest()" << EOL;
    theLogger << "Description: service address is empty" << EOL;
    theLogger << "Result: the request will be ignored" << EOL;
    theLogger << "MethodName: " << theMethodName << EOL;
    theLogger << flush;

    return OTC_String::undefinedString();
  }

  // Send the request.

  OTC_String tmpContent(theContent.encode());
  char const* tmpContentType = "text/xml";

  OTC_Capacity tmpCapacity(63);
  OTC_String theHeader(tmpCapacity);

  theHeader += "MethodName: ";
  theHeader += theMethodName;
  theHeader += EOL;

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(tmpContent,tmpContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(
   theMessage,theServiceAddress,
   serviceBinding_->serviceAddress(),"$S::REQUEST",theHeader);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  OTCLIB_TRACER(MODULE) << "sendRequest =" << *theEnvelope << endl;
  OTCLIB_TRACER(MODULE) << "conversationId =" << theConversationId << endl;

  theEnvelope->queue(OTCLIB_STANDARD_JOB);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_SVBroker::sendMessage(
 OTC_ServiceBinding* theServiceAgent,
 char const* theMessageId,
 char const* theDescription,
 OTC_String const& theContent,
 char const* theContentType,
 int theSequenceNumber
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendMessage()");

  if (theMessageId == 0)
    theMessageId = "";

  if (theDescription == 0)
    theDescription = "";

  if (theServiceAgent == 0)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Precondition failed" << EOL;
    theLogger << "Class: OTC_SVBroker" << EOL;
    theLogger << "Method: sendMessage()" << EOL;
    theLogger << "Description: service is a null pointer" << EOL;
    theLogger << "Result: the request will be ignored" << EOL;
    theLogger << "MessageId: " << theMessageId << EOL;
    theLogger << flush;

    return OTC_String::undefinedString();
  }

  // Send the message.

  OTC_Capacity tmpCapacity(127);
  OTC_String theHeader(tmpCapacity);

  theHeader += "MessageId: ";
  theHeader += theMessageId;
  theHeader += EOL;

  theHeader += "Description: ";
  theHeader += theDescription;
  theHeader += EOL;

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(theContent,theContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(
   theMessage,theServiceAgent->serviceAddress(),
   serviceBinding_->serviceAddress(),"$S::MESSAGE",theHeader,
   0,theSequenceNumber);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  OTCLIB_TRACER(MODULE) << "sendMessage =" << *theEnvelope << endl;
  OTCLIB_TRACER(MODULE) << "conversationId =" << theConversationId << endl;

  theEnvelope->queue(OTCLIB_STANDARD_JOB);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_SVBroker::sendMessage(
 OTC_ServiceBinding* theServiceAgent,
 char const* theMessageId,
 char const* theDescription,
 char const* theContent,
 size_t theContentLength,
 char const* theContentType,
 int theSequenceNumber
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendMessage()");

  if (theMessageId == 0)
    theMessageId = "";

  if (theDescription == 0)
    theDescription = "";

  if (theServiceAgent == 0)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Precondition failed" << EOL;
    theLogger << "Class: OTC_SVBroker" << EOL;
    theLogger << "Method: sendMessage()" << EOL;
    theLogger << "Description: service is a null pointer" << EOL;
    theLogger << "Result: the request will be ignored" << EOL;
    theLogger << "MessageId: " << theMessageId << EOL;
    theLogger << flush;

    return OTC_String::undefinedString();
  }

  // Send the message.

  OTC_Capacity tmpCapacity(127);
  OTC_String theHeader(tmpCapacity);

  theHeader += "MessageId: ";
  theHeader += theMessageId;
  theHeader += EOL;

  theHeader += "Description: ";
  theHeader += theDescription;
  theHeader += EOL;

  OTC_String tmpContent(theContent,theContentLength);

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(tmpContent,theContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(
   theMessage,theServiceAgent->serviceAddress(),
   serviceBinding_->serviceAddress(),"$S::MESSAGE",theHeader,
   0,theSequenceNumber);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  OTCLIB_TRACER(MODULE) << "sendMessage =" << *theEnvelope << endl;
  OTCLIB_TRACER(MODULE) << "conversationId =" << theConversationId << endl;

  theEnvelope->queue(OTCLIB_STANDARD_JOB);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_SVBroker::sendMessage(
 char const* theServiceAddress,
 char const* theMessageId,
 char const* theDescription,
 OTC_String const& theContent,
 char const* theContentType,
 int theSequenceNumber
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendMessage()");

  if (theMessageId == 0)
    theMessageId = "";

  if (theDescription == 0)
    theDescription = "";

  if (theServiceAddress == 0 || *theServiceAddress == EOS)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Precondition failed" << EOL;
    theLogger << "Class: OTC_SVBroker" << EOL;
    theLogger << "Method: sendMessage()" << EOL;
    theLogger << "Description: service address is empty" << EOL;
    theLogger << "Result: the request will be ignored" << EOL;
    theLogger << "MessageId: " << theMessageId << EOL;
    theLogger << flush;

    return OTC_String::undefinedString();
  }

  // Send the message.

  OTC_Capacity tmpCapacity(127);
  OTC_String theHeader(tmpCapacity);

  theHeader += "MessageId: ";
  theHeader += theMessageId;
  theHeader += EOL;

  theHeader += "Description: ";
  theHeader += theDescription;
  theHeader += EOL;

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(theContent,theContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(
   theMessage,theServiceAddress,
   serviceBinding_->serviceAddress(),"$S::MESSAGE",theHeader,
   0,theSequenceNumber);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  OTCLIB_TRACER(MODULE) << "sendMessage =" << *theEnvelope << endl;
  OTCLIB_TRACER(MODULE) << "conversationId =" << theConversationId << endl;

  theEnvelope->queue(OTCLIB_STANDARD_JOB);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_SVBroker::sendMessage(
 char const* theServiceAddress,
 char const* theMessageId,
 char const* theDescription,
 char const* theContent,
 size_t theContentLength,
 char const* theContentType,
 int theSequenceNumber
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendMessage()");

  if (theMessageId == 0)
    theMessageId = "";

  if (theDescription == 0)
    theDescription = "";

  if (theServiceAddress == 0 || *theServiceAddress == EOS)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Precondition failed" << EOL;
    theLogger << "Class: OTC_SVBroker" << EOL;
    theLogger << "Method: sendMessage()" << EOL;
    theLogger << "Description: service address is empty" << EOL;
    theLogger << "Result: the request will be ignored" << EOL;
    theLogger << "MessageId: " << theMessageId << EOL;
    theLogger << flush;

    return OTC_String::undefinedString();
  }

  // Send the message.

  OTC_Capacity tmpCapacity(127);
  OTC_String theHeader(tmpCapacity);

  theHeader += "MessageId: ";
  theHeader += theMessageId;
  theHeader += EOL;

  theHeader += "Description: ";
  theHeader += theDescription;
  theHeader += EOL;

  OTC_String tmpContent(theContent,theContentLength);

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(tmpContent,theContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(
   theMessage,theServiceAddress,
   serviceBinding_->serviceAddress(),"$S::MESSAGE",theHeader,
   0,theSequenceNumber);
  OTCLIB_ASSERT_M(theEnvelope != 0);

  OTC_String theConversationId(theEnvelope->conversationId());

  OTCLIB_TRACER(MODULE) << "sendMessage =" << *theEnvelope << endl;
  OTCLIB_TRACER(MODULE) << "conversationId =" << theConversationId << endl;

  theEnvelope->queue(OTCLIB_STANDARD_JOB);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::subscribeReports(
 char const* theServiceName,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::subscribeReports()");

  if (theServiceName == 0 || *theServiceName == EOS)
    return;

  OTC_String tmpServiceName(theServiceName);
  OTC_String tmpSubjectPattern(theSubjectPattern);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!subscriptions_.contains(tmpServiceName,tmpSubjectPattern))
  {
    if (!subscriptions_.containsKey(tmpServiceName))
      registry()->subscribeServiceName(id(),theServiceName);

    subscriptions_.add(tmpServiceName,tmpSubjectPattern);

    OTC_Iterator<OTC_ServiceBinding*> thePublishers;
    thePublishers = publishers_.items(tmpServiceName);

    while (thePublishers.isValid())
    {
      // Send subscription request.

      OTCLIB_TRACER(MODULE) << "publisher = "
       << thePublishers.item()->serviceAddress() << endl;

      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectPattern: ";
      theHeader += tmpSubjectPattern;
      theHeader += EOL;

      OTC_Event* theMessage;
      theMessage = new OTCEV_Message("");
      OTCLIB_ASSERT_M(theMessage != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage,
       thePublishers.item()->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::SUBSCRIPTION-REQUESTED",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);

      thePublishers.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::unsubscribeReports(
 char const* theServiceName,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::unsubscribeReports()");

  if (theServiceName == 0 || *theServiceName == EOS)
    return;

  OTC_String tmpServiceName(theServiceName);
  OTC_String tmpSubjectPattern(theSubjectPattern);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (subscriptions_.contains(tmpServiceName,tmpSubjectPattern))
  {
    subscriptions_.remove(tmpServiceName,tmpSubjectPattern);

    OTC_Iterator<OTC_ServiceBinding*> thePublishers;
    thePublishers = publishers_.items(tmpServiceName);

    while (thePublishers.isValid())
    {
      // Send the subscription cancellation.

      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectPattern: ";
      theHeader += tmpSubjectPattern;
      theHeader += EOL;

      OTC_Event* theMessage;
      theMessage = new OTCEV_Message("");
      OTCLIB_ASSERT_M(theMessage != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage,
       thePublishers.item()->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::SUBSCRIPTION-WITHDRAWN",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);

      thePublishers.next();
    }

    if (!subscriptions_.containsKey(tmpServiceName))
    {
      publishers_.removeKey(tmpServiceName,OTCLIB_UNSAFE);

      registry()->unsubscribeServiceName(id(),theServiceName);
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::unsubscribeReports(char const* theServiceName)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::unsubscribeReports()");

  if (theServiceName == 0 || *theServiceName == EOS)
    return;

  OTC_String tmpServiceName(theServiceName);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<OTC_String> theSubjectPatterns;
  theSubjectPatterns = subscriptions_.items(tmpServiceName);

  while (theSubjectPatterns.isValid())
  {
    OTC_String tmpSubjectPattern(theSubjectPatterns.item());

    subscriptions_.remove(tmpServiceName,tmpSubjectPattern);

    OTC_Iterator<OTC_ServiceBinding*> thePublishers;
    thePublishers = publishers_.items(tmpServiceName);

    while (thePublishers.isValid())
    {
      // Send the subscription cancellation.

      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectPattern: ";
      theHeader += tmpSubjectPattern;
      theHeader += EOL;

      OTC_Event* theMessage;
      theMessage = new OTCEV_Message("");
      OTCLIB_ASSERT_M(theMessage != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage,
       thePublishers.item()->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::SUBSCRIPTION-WITHDRAWN",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);

      thePublishers.next();
    }

    if (!subscriptions_.containsKey(tmpServiceName))
    {
      publishers_.removeKey(tmpServiceName,OTCLIB_UNSAFE);

      registry()->unsubscribeServiceName(id(),theServiceName);
    }

    theSubjectPatterns.next();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::subscribeReports(
 OTC_ServiceBinding* theServiceAgent,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::subscribeReports()");

  if (theServiceAgent == 0)
    return;

  OTC_String tmpServiceAddress(theServiceAgent->serviceAddress());
  OTC_String tmpSubjectPattern(theSubjectPattern);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!agentSubscriptions_.contains(tmpServiceAddress,tmpSubjectPattern))
  {
    if (!agentSubscriptions_.containsKey(tmpServiceAddress))
    {
      // Check first that the service agent hasn't
      // subscribed to us and therefore that we are
      // already subscribed to its existance.

      if (!subscribers_.containsItem(theServiceAgent))
	registry()->subscribeServiceAddress(id(),tmpServiceAddress);
    }

    agentSubscriptions_.add(tmpServiceAddress,tmpSubjectPattern);

    if (serviceAgents_.contains(tmpServiceAddress))
    {
      // We already know about this service agent.
      // Send a subscription request straight away.

      OTCLIB_TRACER(MODULE) << "publisher = "
       << tmpServiceAddress << endl;

      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectPattern: ";
      theHeader += tmpSubjectPattern;
      theHeader += EOL;

      OTC_Event* theMessage;
      theMessage = new OTCEV_Message("");
      OTCLIB_ASSERT_M(theMessage != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage,
       tmpServiceAddress,serviceBinding_->serviceAddress(),
       "$S::SUBSCRIPTION-REQUESTED",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::unsubscribeReports(
 OTC_ServiceBinding* theServiceAgent,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::unsubscribeReports()");

  if (theServiceAgent == 0)
    return;

  OTC_String tmpServiceAddress(theServiceAgent->serviceAddress());
  OTC_String tmpSubjectPattern(theSubjectPattern);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (agentSubscriptions_.contains(tmpServiceAddress,tmpSubjectPattern))
  {
    agentSubscriptions_.remove(tmpServiceAddress,tmpSubjectPattern);

    OTC_Iterator<OTC_ServiceBinding*> thePublishers;
    thePublishers = agentPublishers_.items(tmpServiceAddress);

    while (thePublishers.isValid())
    {
      // Send the subscription cancellation.

      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectPattern: ";
      theHeader += tmpSubjectPattern;
      theHeader += EOL;

      OTC_Event* theMessage;
      theMessage = new OTCEV_Message("");
      OTCLIB_ASSERT_M(theMessage != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage,
       thePublishers.item()->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::SUBSCRIPTION-WITHDRAWN",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);

      thePublishers.next();
    }

    if (!agentSubscriptions_.containsKey(tmpServiceAddress))
    {
      // This was the last subscription against
      // that particular service agent.

      agentPublishers_.removeKey(tmpServiceAddress,OTCLIB_UNSAFE);

      // If the service agent isn't still also
      // subscribed to us, we can unsubscribe
      // to its existance.

      if (!subscribers_.containsItem(theServiceAgent))
	registry()->unsubscribeServiceAddress(id(),tmpServiceAddress);
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::unsubscribeReports(
 OTC_ServiceBinding* theServiceAgent
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::unsubscribeReports()");

  if (theServiceAgent == 0)
    return;

  OTC_String tmpServiceAddress(theServiceAgent->serviceAddress());

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<OTC_String> theSubjectPatterns;
  theSubjectPatterns = agentSubscriptions_.items(tmpServiceAddress);

  while (theSubjectPatterns.isValid())
  {
    OTC_String tmpSubjectPattern(theSubjectPatterns.item());

    agentSubscriptions_.remove(tmpServiceAddress,tmpSubjectPattern);

    OTC_Iterator<OTC_ServiceBinding*> thePublishers;
    thePublishers = agentPublishers_.items(tmpServiceAddress);

    while (thePublishers.isValid())
    {
      // Send the subscription cancellation.

      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectPattern: ";
      theHeader += tmpSubjectPattern;
      theHeader += EOL;

      OTC_Event* theMessage;
      theMessage = new OTCEV_Message("");
      OTCLIB_ASSERT_M(theMessage != 0);

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage,
       thePublishers.item()->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::SUBSCRIPTION-WITHDRAWN",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);

      thePublishers.next();
    }

    if (!agentSubscriptions_.containsKey(tmpServiceAddress))
    {
      // This was the last subscription against
      // that particular service agent.

      agentPublishers_.removeKey(tmpServiceAddress,OTCLIB_UNSAFE);

      // If the service agent isn't still also
      // subscribed to us, we can unsubscribe
      // to its existance.

      if (!subscribers_.containsItem(theServiceAgent))
	registry()->unsubscribeServiceAddress(id(),tmpServiceAddress);
    }

    theSubjectPatterns.next();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::publishReport(
 char const* theSubjectName,
 OTC_ROPayload const& theContent,
 int theLifeTime
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::publishReport()");

  OTC_String tmpContent(theContent.encode());
  char const* tmpContentType = "text/xml";

  // Remove cache entry or update it.

  OTC_HIndexEntry<OTC_String>* theEntry;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (theLifeTime != 0)
  {
    theEntry = subjects_.insert(theSubjectName);

    OTC_ReportCache* theCacheEntry;
    theCacheEntry = (OTC_ReportCache*)theEntry->item();

    if (theCacheEntry != 0)
      delete theCacheEntry;

    time_t thePostingTime;
    time_t theExpirationTime;

    thePostingTime = time(0);

    if (theLifeTime > 0)
      theExpirationTime = thePostingTime + theLifeTime;
    else
      theExpirationTime = (time_t)-1;

    theCacheEntry = new OTC_ReportCache(tmpContent,
     tmpContentType,thePostingTime,theExpirationTime);
    OTCLIB_ASSERT_M(theCacheEntry != 0);

    theEntry->setItem((void*)theCacheEntry);
  }
  else
  {
    theEntry = subjects_.entry(theSubjectName);

    if (theEntry != 0)
    {
      OTC_ReportCache* theCacheEntry;
      theCacheEntry = (OTC_ReportCache*)theEntry->item();

      delete theCacheEntry;

      theEntry->removeEntry();
    }
  }

  // Publish to subscribers with matching subject pattern.

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message(tmpContent,tmpContentType);
  OTCLIB_ASSERT_M(theMessage != 0);

  OTC_PairIterator<OTC_String,OTC_ServiceBinding*> theSubscriptions;
  theSubscriptions = subscribers_.pairs();

  while (theSubscriptions.isValid())
  {
    if (matchSubject(theSubscriptions.key(),theSubjectName))
    {
      // Publish event.

      OTC_Capacity tmpCapacity(127);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectName: ";
      theHeader += theSubjectName;
      theHeader += EOL;
      theHeader += "SubjectPattern: ";
      theHeader += theSubscriptions.key();
      theHeader += EOL;

      OTCEV_Envelope* theEnvelope;
      theEnvelope = new OTCEV_Envelope(theMessage->clone(),
       theSubscriptions.item()->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::REPORT",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);
    }

    theSubscriptions.next();
  }

  theMessage->destroy();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::sendReport(
 OTC_ServiceBinding* theServiceAgent,
 char const* theSubjectName,
 OTC_ROPayload const& theContent
)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_SVBroker::sendReport()");

  if (theServiceAgent == 0)
    return;

  OTC_PairIterator<OTC_String,OTC_ServiceBinding*> theSubscriptions;
  theSubscriptions = subscribers_.pairsItem(theServiceAgent);

  if (theSubscriptions.isValid())
  {
    OTC_String tmpContent(theContent.encode());
    char const* tmpContentType = "text/xml";

    OTCEV_Message* theMessage = 0;
    theMessage = new OTCEV_Message(tmpContent,tmpContentType);
    OTCLIB_ASSERT_M(theMessage != 0);

    OTCEV_Envelope* theEnvelope = 0;

    while (theSubscriptions.isValid())
    {
      if (matchSubject(theSubscriptions.key(),theSubjectName))
      {
        OTC_Capacity tmpCapacity(127);
	OTC_String theHeader(tmpCapacity);

	theHeader += "SubjectName: ";
	theHeader += theSubjectName;
	theHeader += EOL;
	theHeader += "SubjectPattern: ";
	theHeader += theSubscriptions.key();
	theHeader += EOL;

	theEnvelope = new OTCEV_Envelope(theMessage->clone(),
	 theServiceAgent->serviceAddress(),
	 serviceBinding_->serviceAddress(),
	 "$S::REPORT",theHeader);
	OTCLIB_ASSERT_M(theEnvelope != 0);

	theEnvelope->queue(OTCLIB_STANDARD_JOB);
      }

      theSubscriptions.next();
    }

    if (theEnvelope == 0)
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String theHeader(tmpCapacity);

      theHeader += "SubjectName: ";
      theHeader += theSubjectName;
      theHeader += EOL;

      theEnvelope = new OTCEV_Envelope(theMessage->clone(),
       theServiceAgent->serviceAddress(),
       serviceBinding_->serviceAddress(),
       "$S::REPORT",theHeader);
      OTCLIB_ASSERT_M(theEnvelope != 0);

      theEnvelope->queue(OTCLIB_STANDARD_JOB);
    }

    theMessage->destroy();
  }
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::expireReports(char const* theSubjectPattern, u_int theAge)
{
  time_t theCurrentTime;
  theCurrentTime = time(0);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_HIndexEntry<OTC_String>* theEntry;
  theEntry = subjects_.firstEntry();

  while (theEntry != 0)
  {
    OTC_ReportCache* theCacheEntry;
    theCacheEntry = (OTC_ReportCache*)theEntry->item();

    if (matchSubject(theSubjectPattern,theEntry->key()))
    {
      if (theCurrentTime > time_t(theCacheEntry->postingTime()+theAge))
      {
	delete theCacheEntry;

	OTC_HIndexEntry<OTC_String>* theOldEntry;
	theOldEntry = theEntry;

	theEntry = theEntry->nextEntry();

	theOldEntry->removeEntry();
      }
      else
      {
	theEntry = theEntry->nextEntry();
      }
    }
    else
    {
      theEntry = theEntry->nextEntry();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::expireReports(u_int theAge)
{
  time_t theCurrentTime;
  theCurrentTime = time(0);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_HIndexEntry<OTC_String>* theEntry;
  theEntry = subjects_.firstEntry();

  while (theEntry != 0)
  {
    OTC_ReportCache* theCacheEntry;
    theCacheEntry = (OTC_ReportCache*)theEntry->item();

    if (theCurrentTime > time_t(theCacheEntry->postingTime()+theAge))
    {
      delete theCacheEntry;

      OTC_HIndexEntry<OTC_String>* theOldEntry;
      theOldEntry = theEntry;

      theEntry = theEntry->nextEntry();

      theOldEntry->removeEntry();
    }
    else
    {
      theEntry = theEntry->nextEntry();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_SVBroker::purgeReports()
{
  time_t theCurrentTime;
  theCurrentTime = time(0);

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_HIndexEntry<OTC_String>* theEntry;
  theEntry = subjects_.firstEntry();

  while (theEntry != 0)
  {
    OTC_ReportCache* theCacheEntry;
    theCacheEntry = (OTC_ReportCache*)theEntry->item();

    if (theCacheEntry->expirationTime() != -1 &&
     theCurrentTime > theCacheEntry->expirationTime())
    {
      delete theCacheEntry;

      OTC_HIndexEntry<OTC_String>* theOldEntry;
      theOldEntry = theEntry;

      theEntry = theEntry->nextEntry();

      theOldEntry->removeEntry();
    }
    else
    {
      theEntry = theEntry->nextEntry();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_SVBroker::matchSubject(
 const char* theSubjectPattern,
 const char* theSubjectName
) const
{
  char c;

  while (1)
  {
    switch (c = *theSubjectPattern++)
    {
      case EOS:
      {
	// Must be end of string.

	return *theSubjectName == EOS;

	break;
      }

      case '?':
      {
	// Can't be end of string.

	if (*theSubjectName == EOS)
	  return false;

	++theSubjectName;

	break;
      }

      case '*':
      {
	// Use character following '*'.

	c = *theSubjectPattern;

	// Skip further instances of '*'.

	while (c == '*')
	  c = *++theSubjectPattern;

	// At end.

	if (c == EOS)
	  return true;

	// Use recursion for all other cases.

	while (*theSubjectName != EOS)
	{
	  if (matchSubject(theSubjectPattern,theSubjectName))
	    return true;

	  ++theSubjectName;
	}

	return false;

	break;
      }

      default:
      {
	if (c != *theSubjectName)
	  return false;

	theSubjectName++;

	break;
      }
    }
  }

  return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_SVBroker::builtinRequest_(OTCEV_Request* theRequest)
{
  return false;

#if 0
  OTC_String theMethodName;
  theMethodName = theRequest->methodName();

  if (theMethodName.length() == 0)
    return false;

  if (theMethodName[0] != '_')
    return false;

  if (theMethodName == "_listReports")
  {
    OTC_SVPayload theResult;
    theResult <<= OTC_ROPayload::nullArray();

    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_HIndexEntry<OTC_String>* theEntry;
    theEntry = subjects_.firstEntry();

    while (theEntry != 0)
    {
      OTC_ReportCache* theCacheEntry;
      theCacheEntry = (OTC_ReportCache*)theEntry->item();

      OTC_RWPayload theDetails;
      theDetails = theResult[-1];

      theDetails[-1] <<= theEntry->key();

      OTC_Time thePostingTime(
       theCacheEntry->postingTime(),OTCLIB_GMTIME);
      theDetails[-1] <<= thePostingTime;

      if (theCacheEntry->expirationTime() != -1)
      {
	OTC_Time theExpirationTime(
	 theCacheEntry->expirationTime(),OTCLIB_GMTIME);
	theDetails[-1] <<= theExpirationTime;
      }
      else
	theDetails[-1] <<= OTC_ROPayload::nullValue();

      theEntry = theEntry->nextEntry();
    }

    xxxMutex.release();

    mutex_.unlock();

    theRequest->sendResponse(theResult);
  }
  else
    return false;

  theRequest->destroy();

  return true;
#endif
}

/* ------------------------------------------------------------------------- */
