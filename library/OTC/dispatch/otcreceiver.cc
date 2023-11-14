/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcreceiver.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/receiver.hh"
#endif
#endif

#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/event.hh>
#include <OTC/dispatch/eventjob.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/system/program.hh>
#include <OTC/text/symbol.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Receiver::mutex_;
OTC_NRMutex OTC_Receiver::receiverMutex_;
OTC_HIndex<OTC_String>* OTC_Receiver::gIndex_ = 0;
OTC_Receiver* OTC_Receiver::gLocalReceiverInBox_;
OTC_Receiver* OTC_Receiver::gReceiverInBox_;
OTC_Receiver* OTC_Receiver::gAgentInBox_;

/* ------------------------------------------------------------------------- */
OTC_Receiver::~OTC_Receiver()
{
  unbind();
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver()
  : agentId_(0), callback_(0), prev_(0), next_(0)
{
  init_();
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(OTC_EVAgent* theAgent, char const* theName)
  : agentId_(0), callback_(0), prev_(0), next_(0)
{
  init_();

  bind(theAgent,theName);
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(OTC_EVAgent* theAgent)
  : agentId_(0), callback_(0), prev_(0), next_(0)
{
  init_();

  bind(theAgent);
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(void (*theCallback)(OTC_Event*), char const* theName)
  : agentId_(0), callback_(0), prev_(0), next_(0)
{
  init_();

  bind(theCallback,theName);
}

/* ------------------------------------------------------------------------- */
OTC_Receiver::OTC_Receiver(void (*theCallback)(OTC_Event*))
  : agentId_(0), callback_(0), prev_(0), next_(0)
{
  init_();

  bind(theCallback);
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::unbind()
{
  mutex_.lock();

  if (agentId_ != 0 || callback_ != 0)
  {
    // Unlink ourselves from the chain. If we are
    // first in the list we need to update entry
    // in named index or if we are also the last
    // in the chain delete the entry from the
    // index.

    if (prev_ == 0)
    {
      OTC_HIndexEntry<OTC_String>* theNode;
      theNode = gIndex_->entry(name_);
      OTCLIB_ASSERT(theNode != 0);

      if (next_ != 0)
	theNode->setItem((void*)next_);
      else
	theNode->removeEntry();
    }

    if (prev_ != 0)
      prev_->next_ = next_;
    if (next_ != 0)
      next_->prev_ = prev_;

    agentId_ = 0;
    callback_ = 0;
    name_ = "";
    prev_ = 0;
    next_ = 0;
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(OTC_EVAgent* theAgent, char const* theName)
{
  OTCLIB_ENSURE_FN((theAgent != 0 && theName != 0 && *theName != EOS),
   "OTC_Receiver::bind(OTC_EVAgent*, char const*)",
   "invalid agent/name");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Symbol tmpName;
  tmpName = theName;

  agentId_ = theAgent->id();
  callback_ = 0;
  name_ = tmpName;

  OTC_HIndexEntry<OTC_String>* theNode;
  theNode = gIndex_->insert(name_);

  next_ = (OTC_Receiver*)theNode->item();
  if (next_ != 0)
    next_->prev_ = this;

  theNode->setItem((void*)this);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(OTC_EVAgent* theAgent)
{
  OTCLIB_ENSURE_FN((theAgent != 0),
   "OTC_Receiver::bind(OTC_EVAgent*)",
   "invalid agent");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  // Have a while loop here just in case
  // the ID we use is in use already, in
  // which case we try with another one
  // until we find one that isn't in use.

  while (1)
  {
    name_ = OTC_Symbol(OTC_Program::uniqueId(
     "$O?",OTCLIB_ID_SHORT_FORMAT));

    agentId_ = theAgent->id();
    callback_ = 0;

    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->insert(name_);

    next_ = (OTC_Receiver*)theNode->item();

    if (next_ == 0)
    {
      theNode->setItem((void*)this);

      break;
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(void (*theCallback)(OTC_Event*), char const* theName)
{
  OTCLIB_ENSURE_FN((theCallback != 0 && theName != 0 && *theName != EOS),
   "OTC_Receiver::bind(void (*)(OTC_Event*), char const*)",
   "invalid callback/name");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Symbol tmpName;
  tmpName = theName;

  agentId_ = 0;
  callback_ = theCallback;
  name_ = tmpName;

  OTC_HIndexEntry<OTC_String>* theNode;
  theNode = gIndex_->insert(name_);

  next_ = (OTC_Receiver*)theNode->item();
  if (next_ != 0)
    next_->prev_ = this;

  theNode->setItem((void*)this);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::bind(void (*theCallback)(OTC_Event*))
{
  OTCLIB_ENSURE_FN((theCallback != 0),
   "OTC_Receiver::bind(void (*)(OTC_Event*))",
   "invalid callback");

  unbind();

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  while (1)
  {
    OTC_String theName;
    theName = OTC_Program::uniqueId("$O?",OTCLIB_ID_SHORT_FORMAT);

    OTC_Symbol tmpName;
    tmpName = theName;

    agentId_ = 0;
    callback_ = theCallback;
    name_ = tmpName;

    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->insert(name_);

    next_ = (OTC_Receiver*)theNode->item();

    if (next_ == 0)
    {
      theNode->setItem((void*)this);

      break;
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::deliver(char const* theName, OTC_Event* theEvent)
{
  OTC_JobList theJobList;

  fill_(&theJobList,theName,theEvent);

  OTC_Job* theJob;
  theJob = theJobList.next();

  while (theJob != 0)
  {
    theJob->execute();
    theJob->destroy();

    theJob = theJobList.next();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::queue(char const* theName, OTC_Event* theEvent, int theType)
{
  OTC_JobList theJobList;

  fill_(&theJobList,theName,theEvent);

  OTC_Job* theJob;
  theJob = theJobList.next();

  while (theJob != 0)
  {
    OTC_Dispatcher::schedule(theJob,theType);

    theJob = theJobList.next();
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::fill_(
 OTC_JobList* theJobList,
 char const* theName,
 OTC_Event* theEvent
)
{
  if (theName == 0 || *theName == EOS)
    return;

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;

  // Create special receivers.

  receiverMutex_.lock();

  xxxMutex.grab(receiverMutex_);

  if (gLocalReceiverInBox_ == 0)
  {
    gLocalReceiverInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gLocalReceiverInBox_ != 0);

    gLocalReceiverInBox_->bind(proxy_,"$LOCAL-RECEIVER");
  }

  if (gReceiverInBox_ == 0)
  {
    gReceiverInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gReceiverInBox_ != 0);

    gReceiverInBox_->bind(proxy_,"$RECEIVER");
  }

  if (gAgentInBox_ == 0)
  {
    gAgentInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gAgentInBox_ != 0);

    gAgentInBox_->bind(proxy_,"$AGENT");
  }

  xxxMutex.release();

  receiverMutex_.unlock();

  // Now do real work.

  OTCLIB_ENSURE_FN((theJobList != 0),
   "OTC_Receiver::fill(OTC_JobList*, char const*, OTC_Event*)",
   "invalid job list");

  OTCLIB_ENSURE_FN((theEvent != 0),
   "OTC_Receiver::fill(OTC_JobList*, char const*, OTC_Event*)",
   "invalid event");

  mutex_.lock();

  xxxMutex.grab(mutex_);

  OTC_Receiver const* theReceiver;
  theReceiver = 0;

  if (gIndex_ != 0)
  {
    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->entry(theName);

    if (theNode != 0)
      theReceiver = (OTC_Receiver*)theNode->item();
  }

  while (theReceiver != 0)
  {
    OTCLIB_ASSERT(theReceiver->agentId_ != 0 || theReceiver->callback_ != 0);

    OTC_Job* theJob;
    OTC_Event* theClone;

    theClone = theEvent->clone();
    OTCLIB_ASSERT(theClone != 0);

    if (theReceiver->agentId_ != 0)
    {
      theJob = new OTC_EventJob(theReceiver->agentId_,theClone);
      OTCLIB_ASSERT_M(theJob != 0);
    }
    else
    {
      theJob = new OTC_EventJob(theReceiver->callback_,theClone);
      OTCLIB_ASSERT_M(theJob != 0);
    }

    theJobList->add(theJob);

    theReceiver = theReceiver->next();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_Receiver const* OTC_Receiver::lookup(char const* theName)
{
  if (theName == 0 || *theName == EOS)
    return 0;

  // Create special receivers.

  receiverMutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(receiverMutex_);

  if (gLocalReceiverInBox_ == 0)
  {
    gLocalReceiverInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gLocalReceiverInBox_ != 0);

    gLocalReceiverInBox_->bind(proxy_,"$LOCAL-RECEIVER");
  }

  if (gReceiverInBox_ == 0)
  {
    gReceiverInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gReceiverInBox_ != 0);

    gReceiverInBox_->bind(proxy_,"$RECEIVER");
  }

  if (gAgentInBox_ == 0)
  {
    gAgentInBox_ = new OTC_Receiver;
    OTCLIB_ASSERT_M(gAgentInBox_ != 0);

    gAgentInBox_->bind(proxy_,"$AGENT");
  }

  xxxMutex.release();

  receiverMutex_.unlock();

  // Now do the lookup.

  OTC_Receiver* theResult;
  theResult = 0;

  mutex_.lock();

  if (gIndex_ != 0)
  {
    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->entry(theName);

    if (theNode != 0)
      theResult = (OTC_Receiver*)theNode->item();
  }

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::init_()
{
  mutex_.lock();

  if (gIndex_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gIndex_ = new OTC_HIndex<OTC_String>;
    OTCLIB_ASSERT_M(gIndex_ != 0);

    xxxMutex.release();
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Receiver::proxy_(OTC_Event* theEvent)
{
  if (theEvent == 0)
    return;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (theEnvelope->currentReceiver() == "$RECEIVER" ||
     theEnvelope->currentReceiver() == "$LOCAL-RECEIVER")
    {
      OTCEV_Envelope* tmpEnvelope;
      tmpEnvelope = new OTCEV_Envelope(theEnvelope,
       theEnvelope->nextDestination(),theEnvelope->from());
      OTCLIB_ASSERT_M(tmpEnvelope);

      tmpEnvelope->deliver();
    }
    else if (theEnvelope->currentReceiver() == "$AGENT")
    {
      int theAgentId = 0;

      theAgentId = atoi(theEnvelope->nextDestination());

      if (theAgentId != 0)
	theEvent->clone()->deliver(theAgentId);
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
