/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onsservice.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "ONS/agent/service.hh"
#endif
#endif

#include <ONS/agent/service.hh>

#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/evreaper.hh>
#include <OTC/system/program.hh>
#include <OTC/text/osstream.hh>
#include <OTC/collctn/deque.hh>
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
    TAG = new OTC_TraceTag("ONS_Service");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}
 
#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
class OSE_EXPORT ONS_ServiceContext
    // = TITLE
    //     Wrapper for cached thread context information.
{
  public:

			~ONS_ServiceContext()
				{
				  if (lastRequest != 0)
				    lastRequest->unReference();
				}

			ONS_ServiceContext()
			  : lastRequest(0),
			    currentRequest(0),
			    currentResponse(0),
			    currentFailure(0),
			    currentMessage(0),
			    currentReport(0)
			    	{}

    ONS_ActiveRequest*	lastRequest;
				// Details of the last request to be sent.

    OTCEV_Request*	currentRequest;
				// Request currently being processed.

    OTCEV_Response*	currentResponse;
				// Response currently being processed.

    OTCEV_Failure*	currentFailure;
				// Failure currently being processed.

    OTCEV_Opaque*	currentMessage;
				// Opaque message currently being processed.

    OTCEV_Report*	currentReport;
				// Report currently being processed.
};

/* ------------------------------------------------------------------------- */
ONS_Service::~ONS_Service()
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::~ONS_Service()");

  delete [] agentContext_;

  delete svbroker_;
}

/* ------------------------------------------------------------------------- */
ONS_Service::ONS_Service(
 char const* theServiceName,
 char const* theServiceAudience
)
  : agentContext_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::ONS_Service()");

  svbroker_ = new OTC_SVBroker(this,theServiceName,theServiceAudience);
  OTCLIB_ASSERT_M(svbroker_ != 0);

  OTCLIB_TRACER(MODULE) << "serviceName = " << theServiceName << endl;
  OTCLIB_TRACER(MODULE) << "serviceAudience = " << theServiceAudience << endl;

  // Only allocate enough space for single
  // context initially. If threading is
  // activated, need to reallocate array
  // so separate context for each thread.

  agentContext_ = new ONS_ServiceContext[1];
  OTCLIB_ASSERT_M(agentContext_ != 0);
}

/* ------------------------------------------------------------------------- */
ONS_Service::ONS_Service(
 char const* theServiceName,
 char const* theServiceAudience,
 OTC_ServiceVisibility theServiceVisibility
)
  : agentContext_(0)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::ONS_Service()");

  svbroker_ = new OTC_SVBroker(this,theServiceName,
   theServiceAudience,theServiceVisibility);
  OTCLIB_ASSERT_M(svbroker_ != 0);

  OTCLIB_TRACER(MODULE) << "serviceName = " << theServiceName << endl;
  OTCLIB_TRACER(MODULE) << "serviceAudience = " << theServiceAudience << endl;

  // Only allocate enough space for single
  // context initially. If threading is
  // activated, need to reallocate array
  // so separate context for each thread.

  agentContext_ = new ONS_ServiceContext[1];
  OTCLIB_ASSERT_M(agentContext_ != 0);
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> ONS_Service::exportedMethods() const
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Deque<OTC_String> theList;

  OTC_Iterator<OTC_String> theMethods;
  theMethods = exportedMethods_.keys();

  while (theMethods.isValid())
  {
    theList.addLast(theMethods.item());

    theMethods.next();
  }

  theMethods = 0;

  xxxMutex.release();

  mutex_.unlock();

  return theList.items();
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Service::conversationId() const
{
  OTC_String theResult;

  acmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(acmutex_);

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  if (theContext->currentRequest != 0)
    theResult = theContext->currentRequest->conversationId();
  else if (theContext->currentResponse != 0)
    theResult = theContext->currentResponse->conversationId();
  else if (theContext->currentFailure != 0)
    theResult = theContext->currentFailure->conversationId();
  else if (theContext->currentMessage != 0)
    theResult = theContext->currentMessage->conversationId();

  xxxMutex.release();

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTCEV_Request* ONS_Service::currentRequest() const
{
  OTCEV_Request* theResult;

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theResult = theContext->currentRequest;

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTCEV_Response* ONS_Service::currentResponse() const
{
  OTCEV_Response* theResult;

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theResult = theContext->currentResponse;

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTCEV_Failure* ONS_Service::currentFailure() const
{
  OTCEV_Failure* theResult;

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theResult = theContext->currentFailure;

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTCEV_Opaque* ONS_Service::currentMessage() const
{
  OTCEV_Opaque* theResult;

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theResult = theContext->currentMessage;

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTCEV_Report* ONS_Service::currentReport() const
{
  OTCEV_Report* theResult;

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theResult = theContext->currentReport;

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
ONS_ActiveRequest* ONS_Service::lastRequest() const
{
  ONS_ActiveRequest* theResult;

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theResult = theContext->lastRequest;

  acmutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Service::sendRequest(
 OTC_ServiceBinding* theServiceBinding,
 char const* theMethodName,
 OTC_ROPayload const& thePayload
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendRequest()");

  OTC_String theConversationId(svbroker_->sendRequest(
   theServiceBinding,theMethodName,thePayload));

  ONS_ActiveRequest* theRequest;
  theRequest = new ONS_ActiveRequest(
   theConversationId,theServiceBinding,theMethodName);
  OTCLIB_ASSERT_M(theRequest != 0);

  lastRequest_(theRequest);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Service::sendRequest(
 OTC_ServiceBinding* theServiceBinding,
 char const* theMethodName,
 OTC_RWPayload const& thePayload
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendRequest()");

  OTC_String theConversationId(svbroker_->sendRequest(
   theServiceBinding,theMethodName,thePayload));

  ONS_ActiveRequest* theRequest;
  theRequest = new ONS_ActiveRequest(
   theConversationId,theServiceBinding,theMethodName);
  OTCLIB_ASSERT_M(theRequest != 0);

  lastRequest_(theRequest);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Service::sendRequest(
 OTC_ServiceBinding* theServiceBinding,
 char const* theMethodName,
 OTC_SVPayload const& thePayload
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendRequest()");

  OTC_String theConversationId(svbroker_->sendRequest(
   theServiceBinding,theMethodName,thePayload));

  ONS_ActiveRequest* theRequest;
  theRequest = new ONS_ActiveRequest(
   theConversationId,theServiceBinding,theMethodName);
  OTCLIB_ASSERT_M(theRequest != 0);

  lastRequest_(theRequest);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
void ONS_Service::sendResponse(
 OTCEV_Request* theRequest,
 OTC_ROPayload const& thePayload
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendResponse()");

  theRequest->sendResponse(thePayload);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::sendResponse(OTCEV_Request* theRequest) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendResponse()");

  theRequest->sendResponse(OTC_ROPayload::nullValue());
}

/* ------------------------------------------------------------------------- */
void ONS_Service::sendFailure(
 OTCEV_Request* theRequest,
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendFailure()");

  theRequest->sendFailure(theError,theDescription,theOrigin,theDetails);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::returnResponse(OTC_ROPayload const& thePayload) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::returnResponse()");

  OTCEV_Request* theRequest;
  theRequest = currentRequest();

  OTCLIB_ENSURE_FN((theRequest != 0),
   "ONS_Service::returnResponse()",
   "there is no active request");

  sendResponse(theRequest,thePayload);

  throw ONS_DelayedResponse();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::returnResponse(OTC_RWPayload const& thePayload) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::returnResponse()");

  OTCEV_Request* theRequest;
  theRequest = currentRequest();

  OTCLIB_ENSURE_FN((theRequest != 0),
   "ONS_Service::returnResponse()",
   "there is no active request");

  sendResponse(theRequest,thePayload);

  throw ONS_DelayedResponse();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::returnResponse(OTC_SVPayload const& thePayload) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::returnResponse()");

  OTCEV_Request* theRequest;
  theRequest = currentRequest();

  OTCLIB_ENSURE_FN((theRequest != 0),
   "ONS_Service::returnResponse()",
   "there is no active request");

  sendResponse(theRequest,thePayload);

  throw ONS_DelayedResponse();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::deliverResponse(
 OTC_String const& theConversationId,
 OTC_ROPayload const& thePayload
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::deliverResponse()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (suspendedResponses_.contains(theConversationId))
  {
    ONS_PendingResponse* tmpDetails;
    tmpDetails = suspendedResponses_.item(theConversationId);

    sendResponse(tmpDetails->request(),thePayload);

    suspendedResponses_.remove(theConversationId);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::deliverResponse(
 OTC_String const& theConversationId,
 OTC_RWPayload const& thePayload
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::deliverResponse()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (suspendedResponses_.contains(theConversationId))
  {
    ONS_PendingResponse* tmpDetails;
    tmpDetails = suspendedResponses_.item(theConversationId);

    sendResponse(tmpDetails->request(),thePayload);

    suspendedResponses_.remove(theConversationId);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::deliverResponse(
 OTC_String const& theConversationId,
 OTC_SVPayload const& thePayload
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::deliverResponse()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (suspendedResponses_.contains(theConversationId))
  {
    ONS_PendingResponse* tmpDetails;
    tmpDetails = suspendedResponses_.item(theConversationId);

    sendResponse(tmpDetails->request(),thePayload);

    suspendedResponses_.remove(theConversationId);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::delayResponse() const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::delayResponse()");

  throw ONS_DelayedResponse();
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Service::sendRequest(
 OTC_ServiceBinding* theServiceBinding,
 char const* theMethodName
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendRequest()");

  OTC_String theConversationId(svbroker_->sendRequest(
   theServiceBinding,theMethodName,OTC_ROPayload::nullArray()));

  ONS_ActiveRequest* theRequest;
  theRequest = new ONS_ActiveRequest(
   theConversationId,theServiceBinding,theMethodName);
  OTCLIB_ASSERT_M(theRequest != 0);

  lastRequest_(theRequest);

  return theConversationId;
}

/* ------------------------------------------------------------------------- */
void ONS_Service::ignoreResponse(OTC_String const& theConversationId)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::ignoreResponse()");

  OTCLIB_TRACER(MODULE) << "conversationId = " << theConversationId << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (activeRequests_.contains(theConversationId))
  {
    OTC_Capacity tmpCapacity(63);
    OTC_String tmpKey(tmpCapacity);

    tmpKey = "RESPONSE";
    tmpKey += EOS;
    tmpKey += theConversationId;

    serviceCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

    tmpKey = "FAILURE";
    tmpKey += EOS;
    tmpKey += theConversationId;

    serviceCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

    tmpKey = "$RESPONSE-TIMEOUT/";
    tmpKey += theConversationId;

    cancelTimer(tmpKey);

    ONS_ActiveRequest* tmpActiveRequest;
    tmpActiveRequest = activeRequests_.item(theConversationId);

    OTC_ServiceBinding* tmpServiceBinding;
    tmpServiceBinding = tmpActiveRequest->serviceBinding();

    tmpKey = "$SERVICE-STATUS/";
    tmpKey += theConversationId;

    unsubscribeServiceAddress(tmpServiceBinding->serviceAddress(),tmpKey);

    activeRequests_.remove(theConversationId);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool ONS_Service::responsePending(OTC_String const& theConversationId) const
{
  bool theResult;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  theResult = activeRequests_.contains(theConversationId);

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void ONS_Service::abortResponse(
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails   
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::abortResponse()");

  OTCLIB_TRACER(MODULE) << "error = " << theError << endl;
  OTCLIB_TRACER(MODULE) << "description = " << theDescription << endl;
  OTCLIB_TRACER(MODULE) << "origin = " << theOrigin << endl;
  OTCLIB_TRACER(MODULE) << "details = " << theDetails << endl;

  throw ONS_ServiceFailure(theError,theDescription,theOrigin,theDetails);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::returnResponse() const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::returnResponse()");

  OTCEV_Request* theRequest;
  theRequest = currentRequest();

  OTCLIB_ENSURE_FN((theRequest != 0),
   "ONS_Service::returnResponse()",
   "there is no active request");

  sendResponse(theRequest,OTC_ROPayload::nullValue());

  throw ONS_DelayedResponse();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::deliverResponse(OTC_String const& theConversationId)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::deliverResponse()");

  deliverResponse(theConversationId,OTC_ROPayload::nullValue());
}

/* ------------------------------------------------------------------------- */
void ONS_Service::resumeResponse(OTC_String const& theConversationId)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::resumeResponse()");

  OTC_Capacity tmpCapacity(63);
  OTC_String theAction(tmpCapacity);

  theAction += "$RESUME-RESPONSE/";
  theAction += theConversationId;

  scheduleAction(&ONS_Service::handleResponseResumption_,
   OTCLIB_PRIORITY_JOB,theAction);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::abandonResponse(OTC_String const& theConversationId)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::abandonResponse()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  suspendedResponses_.remove(theConversationId,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::cancelResponse(
 OTC_String const& theConversationId,
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::cancelResponse()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (suspendedResponses_.contains(theConversationId))
  {
    ONS_PendingResponse* tmpDetails;
    tmpDetails = suspendedResponses_.item(theConversationId);

    sendFailure(tmpDetails->request(),theError,
     theDescription,theOrigin,theDetails);

    suspendedResponses_.remove(theConversationId);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::cancelAllResponses(
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::cancelAllResponses()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<ONS_PendingResponse*> theItems;
  theItems = suspendedResponses_.items();

  while (theItems.isValid())
  {
    ONS_PendingResponse* tmpDetails;
    tmpDetails = theItems.item();

    sendFailure(tmpDetails->request(),theError,
     theDescription,theOrigin,theDetails);

    theItems.next();
  }

  theItems = 0;

  suspendedResponses_.removeAll();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::cancelRequest(
 OTC_String const& theConversationId,  
 int theError,   
 char const* theDescription,  
 char const* theOrigin,  
 char const* theDetails  
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::cancelRequest()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  ONS_PendingCancellation* theCancellation;

  OTC_Capacity tmpCapacity(63);
  OTC_String theAction(tmpCapacity);

  theAction = "$CANCEL-REQUEST/";
  theAction += theConversationId;

  if (!pendingCancellations_.contains(theAction))
  {
    theCancellation = new ONS_PendingCancellation(
     theConversationId,theError,theDescription,theOrigin,theDetails);
    OTCLIB_ASSERT_M(theCancellation != 0);

    scheduleAction(&ONS_Service::handleRequestCancellation_,
     OTCLIB_PRIORITY_JOB,theAction);

    pendingCancellations_.add(theAction,theCancellation);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::cancelAllRequests(
 int theError,
 char const* theDescription,
 char const* theOrigin,
 char const* theDetails
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::cancelAllRequests()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Iterator<OTC_String> tmpItems;
  ONS_PendingCancellation* theCancellation;

  OTC_Capacity tmpCapacity(63);
  OTC_String theAction(tmpCapacity);

  tmpItems = activeRequests_.keys();

  while (tmpItems.isValid())
  {
    theAction = "$CANCEL-REQUEST/";
    theAction += tmpItems.item();

    if (!pendingCancellations_.contains(theAction))
    {
      theCancellation = new ONS_PendingCancellation(
       tmpItems.item(),theError,theDescription,theOrigin,theDetails);
      OTCLIB_ASSERT_M(theCancellation != 0);

      scheduleAction(&ONS_Service::handleRequestCancellation_,
       OTCLIB_PRIORITY_JOB,theAction);

      pendingCancellations_.add(theAction,theCancellation);
    }

    tmpItems.next();
  }

  tmpItems = 0;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_String ONS_Service::sendMessage(
 OTC_ServiceBinding* theServiceAgent,
 char const* theMessageId,  
 char const* theDescription,
 OTC_String const& theContent,
 char const* theContentType,
 int theSequenceNumber
) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::sendMessage()");

  return svbroker_->sendMessage(theServiceAgent,theMessageId,
   theDescription,theContent,theContentType,theSequenceNumber);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::discardMessages(
 char const* theSender,
 char const* theMessageId
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::discardMessages()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(127);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "MESSAGE";
  tmpKey += EOS;
  tmpKey += theSender;
  tmpKey += EOS;
  tmpKey += theMessageId;

  if (theMessageId == 0)
  {
    OTC_Iterator<OTC_String> theKeys;
    theKeys = serviceCallbacks_.keys();

    while (theKeys.isValid())
    {
      if (theKeys.item().index(tmpKey) == 0)
	serviceCallbacks_.remove(tmpKey);

      theKeys.next();
    }
  }
  else
    serviceCallbacks_.remove(tmpKey,OTCLIB_UNSAFE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::unexpectedMessage(
 OTC_String const& theSender,
 OTC_String const& theMessageId,
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType,
 int theSequenceNumber
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::unexpectedMessage()");
}

/* ------------------------------------------------------------------------- */
void ONS_Service::ignoreReports(   
 char const* theServiceName,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::ignoreReports()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey1(tmpCapacity);
  OTC_String tmpKey2(tmpCapacity);

  if (theSubjectPattern == 0)
  {
    OTC_Iterator<OTC_String> theKeys;
    theKeys = serviceCallbacks_.keys();

    tmpKey1 = "ACTIVE";
    tmpKey1 += EOS;
    tmpKey1 += theServiceName;
    tmpKey1 += EOS;
    tmpKey1 += EOS;

    tmpKey2 = "PASSIVE";
    tmpKey2 += EOS;
    tmpKey2 += theServiceName;
    tmpKey2 += EOS;
    tmpKey2 += EOS;

    while (theKeys.isValid())
    {
      if (theKeys.item().index(tmpKey1) == 0)
      {
	theSubjectPattern = theKeys.item().string();
	theSubjectPattern += tmpKey1.length();

	unsubscribeReports(theServiceName,theSubjectPattern);
	serviceCallbacks_.remove(theKeys.item());
      }
      else if (theKeys.item().index(tmpKey2) == 0)
      {
	theSubjectPattern = theKeys.item().string();
	theSubjectPattern += tmpKey2.length();

	serviceCallbacks_.remove(theKeys.item());
      }

      theKeys.next();
    }

    theKeys = 0;
  }
  else
  {
    tmpKey1 = "ACTIVE";
    tmpKey1 += EOS;
    tmpKey1 += theServiceName;
    tmpKey1 += EOS;
    tmpKey1 += EOS;
    tmpKey1 += theSubjectPattern;

    tmpKey2 = "PASSIVE";
    tmpKey2 += EOS;
    tmpKey2 += theServiceName;
    tmpKey2 += EOS;
    tmpKey2 += EOS;
    tmpKey2 += theSubjectPattern;

    if (serviceCallbacks_.contains(tmpKey1))
    {
      unsubscribeReports(theServiceName,theSubjectPattern);
      serviceCallbacks_.remove(tmpKey1);
    }
    else if (serviceCallbacks_.contains(tmpKey2))
    {
      serviceCallbacks_.remove(tmpKey2);
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::ignoreReports(
 OTC_ServiceBinding* theServiceBinding,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::ignoreReports()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey1(tmpCapacity);
  OTC_String tmpKey2(tmpCapacity);

  if (theSubjectPattern == 0)
  {
    OTC_Iterator<OTC_String> theKeys;
    theKeys = serviceCallbacks_.keys();

    tmpKey1 = "ACTIVE";
    tmpKey1 += EOS;
    tmpKey1 += theServiceBinding->serviceName();
    tmpKey1 += EOS;
    tmpKey1 += theServiceBinding->serviceAddress();
    tmpKey1 += EOS;

    tmpKey2 = "PASSIVE";
    tmpKey2 += EOS;
    tmpKey2 += theServiceBinding->serviceName();
    tmpKey2 += EOS;
    tmpKey2 += theServiceBinding->serviceAddress();
    tmpKey2 += EOS;

    while (theKeys.isValid())
    {
      if (theKeys.item().index(tmpKey1) == 0)
      {
	theSubjectPattern = theKeys.item().string();
	theSubjectPattern += tmpKey1.length();

	unsubscribeReports(theServiceBinding,theSubjectPattern);
	serviceCallbacks_.remove(theKeys.item());
      }
      else if (theKeys.item().index(tmpKey2) == 0)
      {
	theSubjectPattern = theKeys.item().string();
	theSubjectPattern += tmpKey2.length();

	serviceCallbacks_.remove(theKeys.item());
      }

      theKeys.next();
    }

    theKeys = 0;
  }
  else
  {
    tmpKey1 = "ACTIVE";
    tmpKey1 += EOS;
    tmpKey1 += theServiceBinding->serviceName();
    tmpKey1 += EOS;
    tmpKey1 += theServiceBinding->serviceAddress();
    tmpKey1 += EOS;
    tmpKey1 += theSubjectPattern;

    tmpKey2 = "PASSIVE";
    tmpKey2 += EOS;
    tmpKey2 += theServiceBinding->serviceName();
    tmpKey2 += EOS;
    tmpKey2 += theServiceBinding->serviceAddress();
    tmpKey2 += EOS;
    tmpKey2 += theSubjectPattern;

    if (!serviceCallbacks_.contains(tmpKey1))
    {
      unsubscribeReports(theServiceBinding,theSubjectPattern);
      serviceCallbacks_.remove(tmpKey1);
    }
    else if (!serviceCallbacks_.contains(tmpKey2))
    {
      serviceCallbacks_.remove(tmpKey2);
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::unexpectedReport(
 OTC_String const& theServiceName,
 OTC_String const& theSubjectName,
 OTC_ROPayload const& theContent
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::unexpectedReport()");
}

/* ------------------------------------------------------------------------- */
void ONS_Service::allocateContexts(u_int theNumThreads)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::allocateContexts()");

  ONS_Monitor::allocateContexts(theNumThreads);

  mutex_.lock();

  // Allocate more context space for each
  // active thread being created. Need to
  // preserve any current context.

  if (!threadingEnabled() && theNumThreads != 0)
  {
    acmutex_.lock();

    ONS_ServiceContext* tmpContext;
    tmpContext = agentContext_;

    agentContext_ = new ONS_ServiceContext[theNumThreads+1];
    OTCLIB_ASSERT_M(agentContext_ != 0);

    agentContext_[0].lastRequest = tmpContext[0].lastRequest;
    agentContext_[0].currentRequest = tmpContext[0].currentRequest;
    agentContext_[0].currentResponse = tmpContext[0].currentResponse;
    agentContext_[0].currentFailure = tmpContext[0].currentFailure;
    agentContext_[0].currentMessage = tmpContext[0].currentMessage;
    agentContext_[0].currentReport = tmpContext[0].currentReport;

    tmpContext[0].lastRequest = 0;

    delete [] tmpContext;

    acmutex_.unlock();
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::filter(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::filter()");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  OTC_EventReaper xxxEvent;

  xxxEvent.grab(theEvent);

  if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    handleRequest(theRequest);
  }
  else if (theEvent->type() == OTCEV_Response::typeId())
  {
    OTCEV_Response* theResponse;
    theResponse = (OTCEV_Response*)theEvent;

    handleResponse(theResponse);
  }
  else if (theEvent->type() == OTCEV_Failure::typeId())
  {
    OTCEV_Failure* theFailure;
    theFailure = (OTCEV_Failure*)theEvent;

    handleFailure(theFailure);
  }
  else if (theEvent->type() == OTCEV_Opaque::typeId())
  {
    OTCEV_Opaque* theMessage;
    theMessage = (OTCEV_Opaque*)theEvent;

    handleMessage(theMessage);
  }
  else if (theEvent->type() == OTCEV_Report::typeId())
  {
    OTCEV_Report* theReport;
    theReport = (OTCEV_Report*)theEvent;

    handleReport(theReport);
  }
  else if (theEvent->type() == OTCEV_Subscription::typeId())
  {
    OTCEV_Subscription* theSubscription;
    theSubscription = (OTCEV_Subscription*)theEvent;

    handleSubscription(theSubscription);
  }
  else if (theEvent->type() == OTCEV_PublisherNotification::typeId())
  {
    OTCEV_PublisherNotification* theNotification;
    theNotification = (OTCEV_PublisherNotification*)theEvent;

    handlePublisherNotification(theNotification);
  }
  else
  {
    xxxEvent.release();

    ONS_Monitor::filter(theEvent);

    return;
  }

  xxxEvent.release();

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleRequest(OTCEV_Request* theRequest)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleRequest()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (exportedMethods_.contains(theRequest->methodName()))
      theCallback = exportedMethods_.item(theRequest->methodName());

    if (theCallback != 0)
      theCallback->reference();

    xxxMutex.release();

    mutex_.unlock();

    if (theCallback != 0)
    {
      OTC_SVPayload theResult;       

      currentRequest_(theRequest);

      executeMethod(theCallback,theRequest,theResult);

      theRequest->sendResponse(theResult);
    }
    else
      invalidMethod(theRequest);
  }
  catch (ONS_PayloadInvalid)
  {
    theRequest->sendFailure(ONSLIB_SERVER_REQUEST_DECODING_ERROR,
     ONSLIB_SERVER_REQUEST_DECODING_ERROR_MESSAGE,"netsvc",
     "Payload element was not an array.");
  }
  catch (ONS_ArgumentMismatch theException)
  {
    char tmpBuffer[128];
    sprintf(tmpBuffer,"Expected %d arguments, received %d.",
     theException.count(),theRequest->content().numChildren());

    theRequest->sendFailure(ONSLIB_SERVER_PARAMETERS_INCORRECT,
     ONSLIB_SERVER_PARAMETERS_INCORRECT_MESSAGE,"netsvc",tmpBuffer);
  }
  catch (ONS_ArgumentInvalid theException)
  {
    char tmpBuffer[128];
    sprintf(tmpBuffer,"Error decoding argument %d.",theException.count());

    theRequest->sendFailure(ONSLIB_SERVER_PARAMETERS_INCORRECT,
     ONSLIB_SERVER_PARAMETERS_INCORRECT_MESSAGE,"netsvc",tmpBuffer);
  }
  catch (ONS_ServiceFailure theException)
  {
    theRequest->sendFailure(theException.error(),
     theException.description(),theException.origin(),
     theException.details());
  }
  catch (ONS_DelayedResponse)
  {
    // Do nothing, response will be sent later somehow.     
  }
  catch (OTC_Exception theException)
  {
    OTC_Capacity tmpCapacity(255);
    OTC_String theBuffer(tmpCapacity);
    OTC_OSStream theStream(theBuffer,OTCLIB_BUFFERED);
    theException.display(theStream);
    theStream << flush;

    theRequest->sendFailure(ONSLIB_SERVER_APPLICATION_ERROR,
     ONSLIB_SERVER_APPLICATION_ERROR_MESSAGE,"netsvc",theBuffer);

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleRequest()" << EOL;
    theException.display(theLogger);
    theLogger << flush;
  }
  catch (...)
  {
    theRequest->sendFailure(ONSLIB_SERVER_APPLICATION_ERROR,
     ONSLIB_SERVER_APPLICATION_ERROR_MESSAGE,"netsvc");

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleRequest()" << EOL;
    theLogger << flush;
  }

  if (theCallback != 0)
  {
    theCallback->unReference();

    currentRequest_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleResponse(OTCEV_Response* theResponse)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleResponse()");

  ONS_EventCallback* theResponseCallback = 0;
  ONS_EventCallback* theFailureCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (activeRequests_.contains(theResponse->conversationId()))
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey = "RESPONSE";
      tmpKey += EOS;
      tmpKey += theResponse->conversationId();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theResponseCallback = serviceCallbacks_.item(tmpKey);
	theResponseCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      tmpKey = "FAILURE";
      tmpKey += EOS;
      tmpKey += theResponse->conversationId();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theFailureCallback = serviceCallbacks_.item(tmpKey);
	theFailureCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      tmpKey = "$RESPONSE-TIMEOUT/";
      tmpKey += theResponse->conversationId();

      cancelTimer(tmpKey);

      ONS_ActiveRequest* tmpActiveRequest;
      tmpActiveRequest = activeRequests_.item(theResponse->conversationId());

      OTC_ServiceBinding* tmpServiceBinding;
      tmpServiceBinding = tmpActiveRequest->serviceBinding();

      tmpKey = "$SERVICE-STATUS/";
      tmpKey += theResponse->conversationId();

      unsubscribeServiceAddress(tmpServiceBinding->serviceAddress(),tmpKey);

      activeRequests_.remove(theResponse->conversationId());
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theResponseCallback != 0)
    {
      currentResponse_(theResponse);

      theResponseCallback->execute(theResponse,0);
    }
  }
  catch (ONS_ArgumentInvalid theException)
  {
    OTCEV_Failure tmpFailure(theResponse->conversationId(),
     ONSLIB_CLIENT_RESPONSE_DECODING_ERROR,
     ONSLIB_CLIENT_RESPONSE_DECODING_ERROR_MESSAGE,
     "netsvc","Error decoding result.");

    try
    {
      if (theFailureCallback != 0)
      {
	currentFailure_(&tmpFailure);

	theFailureCallback->execute(&tmpFailure,0);
      }
    }
    catch (...)
    {
      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_CRITICAL);

      theLogger << "Unexpected exception occurred" << EOL;
      theLogger << "Class: ONS_Service" << EOL;
      theLogger << "Method: handleResponse()" << EOL;
      theLogger << "Description: exception in callback was not caught" << EOL;
      theLogger << "Result: dispatcher will be stopped immediately" << EOL;
      theLogger << flush;

      OTC_Dispatcher::stop();
    }
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleResponse()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theResponseCallback != 0)
  {
    theResponseCallback->unReference();

    currentResponse_(0);
  }

  if (theFailureCallback != 0)
  {
    theFailureCallback->unReference();

    currentFailure_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleFailure(OTCEV_Failure* theFailure)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleFailure()");

  ONS_EventCallback* theResponseCallback = 0;
  ONS_EventCallback* theFailureCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (activeRequests_.contains(theFailure->conversationId()))
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey = "RESPONSE";
      tmpKey += EOS;
      tmpKey += theFailure->conversationId();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theResponseCallback = serviceCallbacks_.item(tmpKey);
	theResponseCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      tmpKey = "FAILURE";
      tmpKey += EOS;
      tmpKey += theFailure->conversationId();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theFailureCallback = serviceCallbacks_.item(tmpKey);
	theFailureCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      tmpKey = "$RESPONSE-TIMEOUT/";
      tmpKey += theFailure->conversationId();

      cancelTimer(tmpKey);

      ONS_ActiveRequest* tmpActiveRequest;
      tmpActiveRequest = activeRequests_.item(theFailure->conversationId());

      OTC_ServiceBinding* tmpServiceBinding;
      tmpServiceBinding = tmpActiveRequest->serviceBinding();

      tmpKey = "$SERVICE-STATUS/";
      tmpKey += theFailure->conversationId();

      unsubscribeServiceAddress(tmpServiceBinding->serviceAddress(),tmpKey);

      activeRequests_.remove(theFailure->conversationId());
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theFailureCallback != 0)
    {
      currentFailure_(theFailure);

      theFailureCallback->execute(theFailure,0);
    }
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleFailure()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theResponseCallback != 0)
    theResponseCallback->unReference();

  if (theFailureCallback != 0)
  {
    theFailureCallback->unReference();

    currentFailure_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleMessage(OTCEV_Opaque* theMessage)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleMessage()");

  ONS_EventCallback* theCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Capacity tmpCapacity(63);
    OTC_String tmpKey(tmpCapacity);

    tmpKey = "MESSAGE";
    tmpKey += EOS;
    tmpKey += theMessage->sender();
    tmpKey += EOS;
    tmpKey += theMessage->messageId();

    if (serviceCallbacks_.contains(tmpKey))
    {
      theCallback = serviceCallbacks_.item(tmpKey);
      theCallback->reference();
    }

    xxxMutex.release();

    mutex_.unlock();

    currentMessage_(theMessage);

    if (theCallback == 0)
    {
      unexpectedMessage(theMessage->sender(),theMessage->messageId(),
       theMessage->description(),theMessage->content(),
       theMessage->contentType(),theMessage->sequenceNumber());
    }
    else
      theCallback->execute(theMessage,0);
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleMessage()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
    theCallback->unReference();

  currentMessage_(0);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleReport(OTCEV_Report* theReport)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleReport()");

  ONS_EventCallback* theCallback = 0;

  OTC_ServiceBinding* thePublisher;
  thePublisher = theReport->publisher();

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Capacity tmpCapacity(255);
    OTC_String tmpKey(tmpCapacity);

    tmpKey = "ACTIVE";
    tmpKey += EOS;
    tmpKey += thePublisher->serviceName();
    tmpKey += EOS;
    tmpKey += thePublisher->serviceAddress();
    tmpKey += EOS;
    tmpKey += theReport->subjectPattern();

    if (serviceCallbacks_.contains(tmpKey))
    {
      theCallback = serviceCallbacks_.item(tmpKey);
      theCallback->reference();
    }

    if (theCallback == 0)
    {
      tmpKey = "ACTIVE";
      tmpKey += EOS;
      tmpKey += thePublisher->serviceName();
      tmpKey += EOS;
      tmpKey += EOS;
      tmpKey += theReport->subjectPattern();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theCallback = serviceCallbacks_.item(tmpKey);
	theCallback->reference();
      }
    }

    if (theCallback == 0)
    {
      tmpKey = "PASSIVE";
      tmpKey += EOS;
      tmpKey += thePublisher->serviceName();
      tmpKey += EOS;
      tmpKey += thePublisher->serviceAddress();
      tmpKey += EOS;
      tmpKey += theReport->subjectName();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theCallback = serviceCallbacks_.item(tmpKey);
	theCallback->reference();
      }
    }

    if (theCallback == 0)
    {
      tmpKey = "PASSIVE";
      tmpKey += EOS;
      tmpKey += thePublisher->serviceName();
      tmpKey += EOS;
      tmpKey += EOS;
      tmpKey += theReport->subjectName();

      if (serviceCallbacks_.contains(tmpKey))
      {
	theCallback = serviceCallbacks_.item(tmpKey);
	theCallback->reference();
      }
    }

    xxxMutex.release();

    mutex_.unlock();

    currentReport_(theReport);

    if (theCallback == 0)
    {
      unexpectedReport(thePublisher->serviceName(),
       theReport->subjectName(),theReport->content());
    }
    else
      theCallback->execute(theReport,0);
  }
  catch (ONS_ArgumentInvalid theException)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Service report cannot be decoded" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleReport()" << EOL;
    theLogger << "ServiceName: " << thePublisher->serviceName() << EOL;
    theLogger << "SubjectName: " << theReport->subjectName() << EOL;
    theLogger << "SubjectPattern: " << theReport->subjectPattern() << EOL;
    theLogger << flush;
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleReport()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theCallback != 0)
  {
    theCallback->unReference();

    currentReport_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleSubscription(OTCEV_Subscription* theSubscription)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleSubscription()");
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handlePublisherNotification(
 OTCEV_PublisherNotification* theNotification
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handlePublisherNotification()");
}

/* ------------------------------------------------------------------------- */
void ONS_Service::executeMethod(
 ONS_EventCallback* theCallback,
 OTCEV_Request* theRequest,
 OTC_SVPayload& theResult
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::executeMethod()");

  theCallback->execute(theRequest,(void*)&theResult);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::invalidMethod(OTCEV_Request* theRequest)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::invalidMethod()");

  abortResponse(ONSLIB_SERVER_METHOD_UNAVAILABLE,
   ONSLIB_SERVER_METHOD_UNAVAILABLE_MESSAGE,"netsvc");
}

/* ------------------------------------------------------------------------- */
void ONS_Service::exportMethod_(
 ONS_EventCallback* theCallback,
 char const* theName
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::exportMethod_()");

  OTCLIB_TRACER(MODULE) << "name = " << theName << endl;

  if (theCallback == 0)
    exportedMethods_.remove(theName,OTCLIB_UNSAFE);
  else
    exportedMethods_.add(theName,theCallback,OTCLIB_REPLACE);
}

/* ------------------------------------------------------------------------- */
void ONS_Service::processResponse_(
 ONS_EventCallback* theCallback,
 OTC_String const& theConversationId
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::processResponse_()");

  OTCLIB_TRACER(MODULE) << "conversationId = " << theConversationId << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  ONS_ActiveRequest* theRequest;
  theRequest = lastRequest();

  OTCLIB_ENSURE_FN_W((theRequest != 0),
   "ONS_Service::processResponse_()",
   "No previous request to monitor.");

  if (theRequest != 0)
  {
    OTCLIB_ENSURE_FN_W((theRequest->conversationId() == theConversationId),
     "ONS_Service::processResponse_()",
     "Does not match the previous request.");

    if (theRequest->conversationId() == theConversationId)
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "RESPONSE";
      tmpKey += EOS;
      tmpKey += theConversationId;

      serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

      activeRequests_.add(theConversationId,theRequest,OTCLIB_REPLACE);
    }
    else
      delete theCallback;
  }
  else
    delete theCallback;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::processFailure_(
 ONS_EventCallback* theCallback,
 OTC_String const& theConversationId,
 double theRequestTimeout
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::processFailure_()");

  OTCLIB_TRACER(MODULE) << "conversationId = " << theConversationId << endl;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  ONS_ActiveRequest* theRequest;
  theRequest = lastRequest();

  OTCLIB_ENSURE_FN_W((theRequest != 0),
   "ONS_Service::processFailure_()",
   "No previous request to monitor.");

  if (theRequest != 0)
  {
    OTCLIB_ENSURE_FN_W((theRequest->conversationId() == theConversationId),
     "ONS_Service::processFailure_()",
     "Does not match the previous request.");

    if (theRequest->conversationId() == theConversationId)
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey += "FAILURE";
      tmpKey += EOS;
      tmpKey += theConversationId;

      serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

      activeRequests_.add(theConversationId,theRequest,OTCLIB_REPLACE);

      if (theRequestTimeout > 0)
      {
	tmpKey = "$RESPONSE-TIMEOUT/";
	tmpKey += theConversationId;

	startTimer(&ONS_Service::handleResponseTimeout_,
	 theRequestTimeout,tmpKey);
      }

      tmpKey = "$SERVICE-STATUS/";
      tmpKey += theConversationId;

      if (subscribeServiceAddress(&ONS_Service::handleServiceWithdrawn_,
       theRequest->serviceBinding()->serviceAddress(),
       tmpKey) == OTCLIB_SERVICE_WITHDRAWN)
      {
        unsubscribeServiceAddress(
	 theRequest->serviceBinding()->serviceAddress(),tmpKey);

	tmpKey = "$SERVICE-UNAVAILABLE/";
	tmpKey += theConversationId;

	scheduleAction(&ONS_Service::handleServiceUnavailable_,
	 OTCLIB_PRIORITY_JOB,tmpKey);
      }
    }
    else
      delete theCallback;
  }
  else
    delete theCallback;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::suspendResponse_(ONS_EventCallback* theCallback) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::suspendResponse_()");

  bool theDelayed;
  theDelayed = true;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTCEV_Request* theRequest;
  theRequest = currentRequest();

  OTCLIB_ENSURE_FN_W((theRequest != 0),
   "ONS_Service::suspendResponse_()",
   "No active response to suspend.");

  if (theRequest != 0)
  {
    ONS_PendingResponse* tmpDetails;
    tmpDetails = new ONS_PendingResponse(theRequest,theCallback);
    OTCLIB_ASSERT_M(tmpDetails != 0);

    ONS_Service* theSelf;
    theSelf = (ONS_Service*)this;

    theSelf->suspendedResponses_.add(theRequest->conversationId(),
     tmpDetails,OTCLIB_REPLACE);
  }
  else
  {
    theDelayed = false;

    delete theCallback;
  }

  xxxMutex.release();

  mutex_.unlock();

  if (theDelayed != false)
    delayResponse();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::processMessages_(
 ONS_EventCallback* theCallback,
 char const* theSender,
 char const* theMessageId
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::processMessages_()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "MESSAGE";
  tmpKey += EOS;
  tmpKey += theSender;
  tmpKey += EOS;
  tmpKey += theMessageId;

  serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::monitorReports_(
 ONS_EventCallback* theCallback,
 char const* theServiceName,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::monitorReports_()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(63);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "ACTIVE";
  tmpKey += EOS;
  tmpKey += theServiceName;
  tmpKey += EOS;
  tmpKey += EOS;
  tmpKey += theSubjectPattern;

  if (!serviceCallbacks_.contains(tmpKey))
    subscribeReports(theServiceName,theSubjectPattern);

  serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::monitorReports_(
 ONS_EventCallback* theCallback,
 OTC_ServiceBinding* theServiceBinding,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::monitorReports_()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(255);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "ACTIVE";
  tmpKey += EOS;
  tmpKey += theServiceBinding->serviceName();
  tmpKey += EOS;
  tmpKey += theServiceBinding->serviceAddress();
  tmpKey += EOS;
  tmpKey += theSubjectPattern;

  if (!serviceCallbacks_.contains(tmpKey))
    subscribeReports(theServiceBinding,theSubjectPattern);

  serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::acceptReports_(
 ONS_EventCallback* theCallback,
 char const* theServiceName,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::acceptReports_()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(127);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "PASSIVE";
  tmpKey += EOS;
  tmpKey += theServiceName;
  tmpKey += EOS;
  tmpKey += EOS;
  tmpKey += theSubjectPattern;

  serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::acceptReports_(
 ONS_EventCallback* theCallback,
 OTC_ServiceBinding* theServiceBinding,
 char const* theSubjectPattern
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::monitorReports_()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Capacity tmpCapacity(255);
  OTC_String tmpKey(tmpCapacity);

  tmpKey = "PASSIVE";
  tmpKey += EOS;
  tmpKey += theServiceBinding->serviceName();
  tmpKey += EOS;
  tmpKey += theServiceBinding->serviceAddress();
  tmpKey += EOS;
  tmpKey += theSubjectPattern;

  serviceCallbacks_.add(tmpKey,theCallback,OTCLIB_REPLACE);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleResponseTimeout_(OTC_String const& theTimer)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleResponseTimeout_()");

  OTCLIB_TRACER(MODULE) << "timer = " << theTimer << endl;

  OTC_String theConversationId;

  int theIndex;
  theIndex = theTimer.index('/');
  if (theIndex != -1)
    theConversationId = theTimer.after(theIndex);

  ONS_EventCallback* theResponseCallback = 0;
  ONS_EventCallback* theFailureCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (activeRequests_.contains(theConversationId))
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey = "RESPONSE";
      tmpKey += EOS;
      tmpKey += theConversationId;

      if (serviceCallbacks_.contains(tmpKey))
      {
	theResponseCallback = serviceCallbacks_.item(tmpKey);
	theResponseCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      tmpKey = "FAILURE";
      tmpKey += EOS;
      tmpKey += theConversationId;

      if (serviceCallbacks_.contains(tmpKey))
      {
	theFailureCallback = serviceCallbacks_.item(tmpKey);
	theFailureCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      ONS_ActiveRequest* tmpActiveRequest;
      tmpActiveRequest = activeRequests_.item(theConversationId);

      OTC_ServiceBinding* tmpServiceBinding;
      tmpServiceBinding = tmpActiveRequest->serviceBinding();

      tmpKey = "$SERVICE-STATUS/";
      tmpKey += theConversationId;

      unsubscribeServiceAddress(tmpServiceBinding->serviceAddress(),tmpKey);

      activeRequests_.remove(theConversationId);
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theFailureCallback != 0)
    {
      OTCEV_Failure tmpFailure(theConversationId,
       ONSLIB_CLIENT_REQUEST_TIMEOUT,
       ONSLIB_CLIENT_REQUEST_TIMEOUT_MESSAGE,
       "netsvc","User specified timeout expired.");

      currentFailure_(&tmpFailure);

      theFailureCallback->execute(&tmpFailure,0);
    }
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleResponseTimeout_()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theResponseCallback != 0)
    theResponseCallback->unReference();

  if (theFailureCallback != 0)
  {
    theFailureCallback->unReference();

    currentFailure_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleServiceWithdrawn_(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus,
 OTC_String const& theSubscription
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleServiceWithdrawn_()");

  if (!theGroup.isEmpty() || theStatus != OTCLIB_SERVICE_WITHDRAWN)
    return;

  OTCLIB_TRACER(MODULE) << "subscription = " << theSubscription << endl;

  OTC_String theConversationId;

  int theIndex;
  theIndex = theSubscription.index('/');
  if (theIndex != -1)
    theConversationId = theSubscription.after(theIndex);

  ONS_EventCallback* theResponseCallback = 0;
  ONS_EventCallback* theFailureCallback = 0;

  try
  {
    mutex_.lock();

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    if (activeRequests_.contains(theConversationId))
    {
      OTC_Capacity tmpCapacity(63);
      OTC_String tmpKey(tmpCapacity);

      tmpKey = "RESPONSE";
      tmpKey += EOS;
      tmpKey += theConversationId;

      if (serviceCallbacks_.contains(tmpKey))
      {
	theResponseCallback = serviceCallbacks_.item(tmpKey);
	theResponseCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      tmpKey = "FAILURE";
      tmpKey += EOS;
      tmpKey += theConversationId;

      if (serviceCallbacks_.contains(tmpKey))
      {
	theFailureCallback = serviceCallbacks_.item(tmpKey);
	theFailureCallback->reference();
	serviceCallbacks_.remove(tmpKey);
      }

      ONS_ActiveRequest* tmpActiveRequest;
      tmpActiveRequest = activeRequests_.item(theConversationId);

      OTC_ServiceBinding* tmpServiceBinding;
      tmpServiceBinding = tmpActiveRequest->serviceBinding();

      tmpKey = "$SERVICE-STATUS/";
      tmpKey += theConversationId;

      unsubscribeServiceAddress(tmpServiceBinding->serviceAddress(),tmpKey);

      activeRequests_.remove(theConversationId);
    }

    xxxMutex.release();

    mutex_.unlock();

    if (theFailureCallback != 0)
    {
      OTCEV_Failure tmpFailure(theConversationId,
       ONSLIB_SERVER_APPLICATION_ERROR,
       ONSLIB_SERVER_APPLICATION_ERROR_MESSAGE,
       "netsvc","Service was withdrawn before response received.");

      currentFailure_(&tmpFailure);

      theFailureCallback->execute(&tmpFailure,0);
    }
  }
  catch (...)
  {
    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_CRITICAL);

    theLogger << "Unexpected exception occurred" << EOL;
    theLogger << "Class: ONS_Service" << EOL;
    theLogger << "Method: handleServiceWithdrawn_()" << EOL;
    theLogger << "Description: exception in callback was not caught" << EOL;
    theLogger << "Result: dispatcher will be stopped immediately" << EOL;
    theLogger << flush;

    OTC_Dispatcher::stop();
  }

  if (theResponseCallback != 0)
    theResponseCallback->unReference();

  if (theFailureCallback != 0)
  {
    theFailureCallback->unReference();

    currentFailure_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleServiceUnavailable_(
 OTC_String const& theAction    
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleServiceUnavailable_()");

  OTCLIB_TRACER(MODULE) << "action = " << theAction << endl;

  OTC_String theConversationId;

  int theIndex;
  theIndex = theAction.index('/');
  if (theIndex != -1)
    theConversationId = theAction.after(theIndex);

  OTC_ServiceBinding* theServiceBinding = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (activeRequests_.contains(theConversationId))
  {
    ONS_ActiveRequest* tmpActiveRequest;
    tmpActiveRequest = activeRequests_.item(theConversationId);

    theServiceBinding = tmpActiveRequest->serviceBinding()->clone();
  }

  xxxMutex.release();

  mutex_.unlock();

  if (theServiceBinding != 0)
  {
    OTC_Capacity tmpCapacity(63);
    OTC_String tmpKey(tmpCapacity);

    tmpKey = "$SERVICE-STATUS/";
    tmpKey += theConversationId;

    handleServiceWithdrawn_(theServiceBinding,"",
     OTCLIB_SERVICE_WITHDRAWN,tmpKey);

    theServiceBinding->destroy();
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleRequestCancellation_(
 OTC_String const& theAction
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleRequestCancellation_()");

  OTCLIB_TRACER(MODULE) << "action = " << theAction << endl;

  ONS_PendingCancellation* theCancellation = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (pendingCancellations_.contains(theAction))
  {
    theCancellation = pendingCancellations_.item(theAction);
    theCancellation->reference();

    pendingCancellations_.remove(theAction);
  }

  xxxMutex.release();

  mutex_.unlock();

  if (theCancellation != 0)
  {
    OTCEV_Failure tmpFailure(theCancellation->conversationId(),
     theCancellation->error(),theCancellation->description(),
     theCancellation->origin(),theCancellation->details());

    handleFailure(&tmpFailure);

    theCancellation->unReference();
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::handleResponseResumption_(
 OTC_String const& theAction
)
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::handleResponseResumption_()");

  OTCLIB_TRACER(MODULE) << "action = " << theAction << endl;

  OTC_String theConversationId;

  int theIndex;
  theIndex = theAction.index('/');
  if (theIndex != -1)
    theConversationId = theAction.after(theIndex);

  ONS_PendingResponse* tmpDetails = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (suspendedResponses_.contains(theConversationId))
  {
    tmpDetails = suspendedResponses_.item(theConversationId);
    tmpDetails->reference();
    suspendedResponses_.remove(theConversationId);
  }

  xxxMutex.release();

  mutex_.unlock();

  if (tmpDetails != 0)
  {
    try
    {
      OTC_SVPayload thePayload;

      currentRequest_(tmpDetails->request());

      tmpDetails->callback()->execute(
       tmpDetails->request(),(void*)&thePayload);

      tmpDetails->request()->sendResponse(thePayload);
    }
    catch (ONS_ServiceFailure theException)
    {
      tmpDetails->request()->sendFailure(theException.error(),
       theException.description(),theException.origin(),
       theException.details());
    }
    catch (ONS_DelayedResponse)
    {
      // Do nothing, response will be sent later somehow.     
    }
    catch (...)
    {
      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_CRITICAL);

      theLogger << "Unexpected exception occurred" << EOL;
      theLogger << "Class: ONS_Service" << EOL;
      theLogger << "Method: handleResponseResumption_()" << EOL;
      theLogger << "Description: exception in callback was not caught" << EOL;
      theLogger << "Result: dispatcher will be stopped immediately" << EOL;
      theLogger << flush;

      OTC_Dispatcher::stop();
    }

    tmpDetails->unReference();

    currentRequest_(0);
  }
}

/* ------------------------------------------------------------------------- */
void ONS_Service::lastRequest_(ONS_ActiveRequest* theRequest) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::lastRequest_()");

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  if (theContext->lastRequest != 0)
    theContext->lastRequest->unReference();

  theContext->lastRequest = theRequest;

  if (theContext->lastRequest != 0)
    theContext->lastRequest->reference();

  acmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::currentRequest_(OTCEV_Request* theRequest) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::currentRequest_()");

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theContext->currentRequest = theRequest;

  acmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::currentResponse_(OTCEV_Response* theResponse) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::currentResponse_()");

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theContext->currentResponse = theResponse;

  acmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::currentFailure_(OTCEV_Failure* theFailure) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::currentFailure_()");

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theContext->currentFailure = theFailure;

  acmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::currentMessage_(OTCEV_Opaque* theMessage) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::currentMessage_()");

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theContext->currentMessage = theMessage;

  acmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Service::currentReport_(OTCEV_Report* theReport) const
{
  OTCLIB_MARKBLOCK(MODULE,"ONS_Service::currentReport_()");

  acmutex_.lock();

  ONS_ServiceContext* theContext;
  theContext = &agentContext_[threadingContext()];

  theContext->currentReport = theReport;

  acmutex_.unlock();
}

/* ------------------------------------------------------------------------- */
