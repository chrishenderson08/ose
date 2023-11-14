/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcenvelope.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/envelope.hh"
#endif
#endif

#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/job.hh>
#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/strstream.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_EnvelopeContent : public OTC_Resource
{
  public:

			~OTC_EnvelopeContent();

			OTC_EnvelopeContent(
			 OTC_Event* theEvent,
			 OTCEV_Message* theMessage
			)
			  : event_(theEvent), message_(theMessage) {}

			OTC_EnvelopeContent(OTC_Event* theEvent)
			  : event_(theEvent), message_(0) {}

			OTC_EnvelopeContent(OTCEV_Message* theMessage)
			  : event_(0), message_(theMessage) {}

    OTC_Event*		event() const
				{ return event_; }

    OTCEV_Message*	message() const
				{ return message_; }

    OTC_Event*		inflate()
				{
				  if (event_ == 0 && message_ != 0)
				    event_ = message_->inflate();
				  return event_;
				}

    OTCEV_Message*	deflate()
				{
				  if (message_ == 0 && event_ != 0)
				    message_ = event_->deflate();
				  return message_;
				}

  private:

			OTC_EnvelopeContent(OTC_EnvelopeContent const&);

    OTC_EnvelopeContent&	operator=(OTC_EnvelopeContent const&);

    OTC_Event*		event_;

    OTCEV_Message*	message_;
};

OTC_EnvelopeContent::~OTC_EnvelopeContent()
{
  if (event_ != 0)
    event_->destroy();

  if (message_ != 0)
    message_->destroy();
}

/* ------------------------------------------------------------------------- */
class OSE_EXPORT OTC_EnvelopeJob : public OTC_Job
{
  public:

			~OTC_EnvelopeJob();

			OTC_EnvelopeJob(OTCEV_Envelope* theEnvelope)
			  : envelope_(theEnvelope) {}

    void		execute();

  private:

    OTCEV_Envelope*	envelope_;
};

OTC_EnvelopeJob::~OTC_EnvelopeJob()
{
  if (envelope_ != 0)
    envelope_->destroy();
}

void OTC_EnvelopeJob::execute()
{
  if (envelope_ != 0)
    envelope_->deliver();

  envelope_ = 0;
}

/* ------------------------------------------------------------------------- */
OTC_Mutex OTCEV_Envelope::mutex_;
int OTCEV_Envelope::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Envelope::OTCEV_Envelope(
 OTC_Event* theEvent,
 char const* theToAddress,
 char const* theFromAddress,
 char const* theConversationId,
 int theSequenceNumber
)
  : content_(0),
    to_(theToAddress),
    from_(theFromAddress),
    conversationId_(theConversationId),
    sequenceNumber_(theSequenceNumber)
{
  OTCLIB_ENSURE_FN((theEvent != 0),
   "OTCEV_Envelope::OTCEV_Envelope(OTC_Event*, ....)",
   "invalid message");

  if (theEvent->type() == OTCEV_Message::typeId())
  {
    content_ = new OTC_EnvelopeContent((OTCEV_Message*)theEvent);
    OTCLIB_ASSERT_M(content_ != 0);
  }
  else
  {
    content_ = new OTC_EnvelopeContent(theEvent);
    OTCLIB_ASSERT_M(content_ != 0);
  }

  content_->reference();

  if (theConversationId == 0)
    conversationId_ = OTC_Program::uniqueId("$ID?",OTCLIB_ID_LONG_FORMAT);
}

/* ------------------------------------------------------------------------- */
OTCEV_Envelope::OTCEV_Envelope(
 OTC_Event* theEvent,
 char const* theToAddress,
 char const* theFromAddress,
 char const* theSubject,
 char const* theHeader,
 char const* theConversationId,
 int theSequenceNumber
)
  : content_(0),
    to_(theToAddress),
    from_(theFromAddress),
    subject_(theSubject),
    header_(theHeader),
    conversationId_(theConversationId),
    sequenceNumber_(theSequenceNumber)
{
  OTCLIB_ENSURE_FN((theEvent != 0),
   "OTCEV_Envelope::OTCEV_Envelope(OTC_Event*, ....)",
   "invalid message");

  if (theEvent->type() == OTCEV_Message::typeId())
  {
    content_ = new OTC_EnvelopeContent((OTCEV_Message*)theEvent);
    OTCLIB_ASSERT_M(content_ != 0);
  }
  else
  {
    content_ = new OTC_EnvelopeContent(theEvent);
    OTCLIB_ASSERT_M(content_ != 0);
  }

  content_->reference();

  if (theConversationId == 0)
    conversationId_ = OTC_Program::uniqueId("$ID?",OTCLIB_ID_LONG_FORMAT);
}

/* ------------------------------------------------------------------------- */
OTCEV_Envelope::OTCEV_Envelope(
 OTCEV_Envelope* theEnvelope,
 char const* theToAddress,
 char const* theFromAddress
)
  : content_(0),
    to_(theToAddress),
    from_(theFromAddress),
    subject_(theEnvelope->subject_),
    header_(theEnvelope->header_),
    conversationId_(theEnvelope->conversationId_),
    sequenceNumber_(theEnvelope->sequenceNumber_)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Envelope::OTCEV_Envelope(OTC_Envelope*, ....)",
   "invalid envelope");

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;

  xxxMutex.grab(mutex_);

  OTC_Event* theEvent = 0;
  OTCEV_Message* theMessage = 0;

  if (theEnvelope->content_->message() != 0)
    theMessage = (OTCEV_Message*)theEnvelope->content_->message()->clone();

  if (theEnvelope->content_->event() != 0)
  {
    theEvent = theEnvelope->content_->event()->clone();

    if (theEvent != 0 && theEvent != theEnvelope->content_->event())
    {

      content_ = new OTC_EnvelopeContent(theEvent,theMessage);
      OTCLIB_ASSERT_M(content_ != 0);

      content_->reference();
    }
    else
    {
      if (theMessage != 0)
	theMessage->destroy();

      if (theEvent != 0)
	theEvent->destroy();

      content_ = theEnvelope->content_;
      content_->reference();
    }
  }
  else
  {
    if (theMessage != 0)
      theMessage->destroy();

    content_ = theEnvelope->content_;
    content_->reference();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTCEV_Envelope::OTCEV_Envelope(
 OTCEV_Envelope* theEnvelope,
 char const* theToAddress,
 char const* theFromAddress,
 char const* theSubject,
 char const* theHeader,
 char const* theConversationId,
 int theSequenceNumber
)
  : content_(0),
    to_(theToAddress),
    from_(theFromAddress),
    subject_(theSubject),
    header_(theHeader),
    conversationId_(theConversationId),
    sequenceNumber_(theSequenceNumber)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Envelope::OTCEV_Envelope(OTC_Envelope*, ....)",
   "invalid envelope");

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;

  xxxMutex.grab(mutex_);

  OTC_Event* theEvent = 0;
  OTCEV_Message* theMessage = 0;

  if (theEnvelope->content_->message() != 0)
    theMessage = (OTCEV_Message*)theEnvelope->content_->message()->clone();

  if (theEnvelope->content_->event() != 0)
  {
    theEvent = theEnvelope->content_->event()->clone();

    if (theEvent != 0 && theEvent != theEnvelope->content_->event())
    {

      content_ = new OTC_EnvelopeContent(theEvent,theMessage);
      OTCLIB_ASSERT_M(content_ != 0);

      content_->reference();
    }
    else
    {
      if (theMessage != 0)
	theMessage->destroy();

      if (theEvent != 0)
	theEvent->destroy();

      content_ = theEnvelope->content_;
      content_->reference();
    }
  }
  else
  {
    if (theMessage != 0)
      theMessage->destroy();

    content_ = theEnvelope->content_;
    content_->reference();
  }

  xxxMutex.release();

  mutex_.unlock();

  if (theConversationId == 0)
    conversationId_ = OTC_Program::uniqueId("$ID?",OTCLIB_ID_LONG_FORMAT);
}

/* ------------------------------------------------------------------------- */
OTCEV_Message* OTCEV_Envelope::message() const
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;

  xxxMutex.grab(mutex_);

  OTCEV_Message* theMessage;

  theMessage = content_->deflate();

  xxxMutex.release();

  mutex_.unlock();

  return theMessage;
}

/* ------------------------------------------------------------------------- */
OTC_Event* OTCEV_Envelope::event() const
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;

  xxxMutex.grab(mutex_);

  OTC_Event* theEvent;

  theEvent = content_->inflate();

  if (theEvent == 0)
    theEvent = content_->message();

  xxxMutex.release();

  mutex_.unlock();

  return theEvent;
}

/* ------------------------------------------------------------------------- */
OTC_String OTCEV_Envelope::originalSender() const
{
  int theIndex;

  theIndex = from_.rindex('!');
  if (theIndex == -1)
    return from_;

  return from_.after(theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_String OTCEV_Envelope::previousSender() const
{
  int theIndex;

  theIndex = from_.index('!');
  if (theIndex == -1)
    return from_;

  return from_.before(theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_String OTCEV_Envelope::currentReceiver() const
{
  int theIndex;

  theIndex = to_.index('!');
  if (theIndex == -1)
    return to_;

  return to_.before(theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_String OTCEV_Envelope::nextReceiver() const
{
  int theIndex;

  theIndex = to_.index('!');
  if (theIndex == -1)
    return OTC_String::nullString();

  OTC_String tmpName(to_.after(theIndex));

  theIndex = tmpName.index('!');
  if (theIndex == -1)
    return tmpName;

  return tmpName.before(theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_String OTCEV_Envelope::finalReceiver() const
{
  int theIndex;

  theIndex = to_.rindex('!');
  if (theIndex == -1)
    return to_;

  return to_.after(theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_String OTCEV_Envelope::nextDestination() const
{
  int theIndex;

  theIndex = to_.index('!');
  if (theIndex == -1)
    return OTC_String::nullString();

  return to_.after(theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTCEV_Envelope::eventType() const
{
  OTCEV_Message* theMessage;
  theMessage = message();

  if (theMessage == 0)
    return OTC_String::nullString();

  return theMessage->eventType();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::deliver()
{
  OTC_String theReceiverName(currentReceiver());

  if (theReceiverName != "*")
  {
    OTC_Receiver::deliver(theReceiverName,this);
  }
  else
  {
    // XXX Check "To: " fields in header".

    OTC_String tmpLine;
    OTC_String tmpHeader;
    OTC_String tmpAddresses;

    // Separate "To: " fields. Remember others fields.

    istrstream ins1((char*)header_.string(),header_.length());

    while (ins1.good())
    {
      tmpLine.readLine(ins1);
      if (!ins1.fail())
      {
	if (tmpLine.compare(0,"To: ",4))
	{
	  tmpLine.remove(0,4);

	  tmpAddresses += tmpLine;
	  tmpAddresses += EOL;
	}
	else
	{
	  tmpHeader += tmpLine;
	  tmpHeader += EOL;
	}
      }
    }

    // Interpret "To: " fields.

    int theIndex;
    int theNumAddresses = 0;

    OTC_String tmpCurrentPrefix;
    OTC_String tmpNewPrefix;
    OTC_String tmpNewHeader;
    OTC_String tmpLastAddress;

    istrstream ins2((char*)tmpAddresses.string(),tmpAddresses.length());

    while (ins2.good())
    {
      tmpLine.readLine(ins2);
      if (!ins2.fail())
      {
	theIndex = tmpLine.index('!');
	if (theIndex == -1)
	{
	  // Last recipient in address, send this
	  // separately rather than mixed in with
	  // any other addresses with the same
	  // receiver so as to make processing by
	  // by receiver easier.

	  OTCEV_Envelope* theEnvelope;
	  theEnvelope = new OTCEV_Envelope(this,tmpLine,from_,subject_,
	   tmpHeader,conversationId_,sequenceNumber_);
	  OTCLIB_ASSERT_M(theEnvelope != 0);

	  theEnvelope->deliver();
	}
	else
	{
	  tmpNewPrefix = tmpLine.before(theIndex);
	  tmpLine.remove(0,theIndex+1);

	  if (tmpCurrentPrefix.isEmpty())
	  {
	    // First address.

	    theNumAddresses = 1;
	    tmpCurrentPrefix = tmpNewPrefix;

	    tmpNewHeader = tmpHeader;
	    tmpNewHeader += "To: ";
	    tmpNewHeader += tmpLine;
	    tmpNewHeader += EOL;

	    tmpLastAddress = tmpLine;
	  }
	  else if (tmpNewPrefix != tmpCurrentPrefix)
	  {
	    OTCEV_Envelope* theEnvelope;

	    if (theNumAddresses == 1)
	    {
	      // Only one in batch.

	      tmpCurrentPrefix += '!';
	      tmpCurrentPrefix += tmpLastAddress;

	      theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	       from_,subject_,tmpHeader,conversationId_,sequenceNumber_);
	      OTCLIB_ASSERT_M(theEnvelope != 0);
	    }
	    else
	    {
	      // More than one in batch.

	      tmpCurrentPrefix += '!';
	      tmpCurrentPrefix += '*';

	      theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	       from_,subject_,tmpNewHeader,conversationId_,sequenceNumber_);
	      OTCLIB_ASSERT_M(theEnvelope != 0);
	    }

	    theEnvelope->deliver();

	    theNumAddresses = 1;
	    tmpCurrentPrefix =  tmpNewPrefix;

	    tmpNewHeader = tmpHeader;
	    tmpNewHeader += "To: ";
	    tmpNewHeader += tmpLine;
	    tmpNewHeader += EOL;

	    tmpLastAddress = tmpLine;
	  }
	  else
	  {
	    theNumAddresses += 1;
	    tmpNewHeader += "To: ";
	    tmpNewHeader += tmpLine;
	    tmpNewHeader += EOL;
	  }
	}
      }
    }

    if (theNumAddresses != 0)
    {
      OTCEV_Envelope* theEnvelope;

      if (theNumAddresses == 1)
      {
	// Only one in batch.

	tmpCurrentPrefix += '!';
	tmpCurrentPrefix += tmpLastAddress;

	theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	 from_,subject_,tmpHeader,conversationId_,sequenceNumber_);
	OTCLIB_ASSERT_M(theEnvelope != 0);
      }
      else
      {
	// More than one in batch.

	tmpCurrentPrefix += '!';
	tmpCurrentPrefix += '*';

	theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	 from_,subject_,tmpNewHeader,conversationId_,sequenceNumber_);
	OTCLIB_ASSERT_M(theEnvelope != 0);
      }

      theEnvelope->deliver();

      theNumAddresses = 1;
      tmpCurrentPrefix =  tmpNewPrefix;

      tmpNewHeader = tmpHeader;
      tmpNewHeader += "To: ";
      tmpNewHeader += tmpLine;
      tmpNewHeader += EOL;

      tmpLastAddress = tmpLine;
    }

    destroy();
  }
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::queue(int theType)
{
  OTC_EnvelopeJob* theJob;
  theJob = new OTC_EnvelopeJob(this);
  OTCLIB_ASSERT_M(theJob != 0);

  OTC_Dispatcher::schedule(theJob,theType);

#if 0
  OTC_String theReceiverName;
  theReceiverName = currentReceiver();

  if (theReceiverName != "*")
  {
    OTC_Receiver::queue(theReceiverName,this,theType);
  }
  else
  {
    // XXX Check "To: " fields in header".

    OTC_String tmpLine;
    OTC_String tmpHeader;
    OTC_String tmpAddresses;

    // Separate "To: " fields. Remember others fields.

    istrstream ins1((char*)header_.string(),header_.length());

    while (ins1.good())
    {
      tmpLine.readLine(ins1);
      if (!ins1.fail())
      {
	if (tmpLine.compare(0,"To: ",4))
	{
	  tmpLine.remove(0,3);

	  tmpAddresses += tmpLine;
	  tmpAddresses += EOL;
	}
	else
	{
	  tmpHeader += tmpLine;
	  tmpHeader += EOL;
	}
      }
    }

    // Interpret "To: " fields.

    int theIndex;
    int theNumAddresses = 0;

    OTC_String tmpCurrentPrefix;
    OTC_String tmpNewPrefix;
    OTC_String tmpNewHeader;
    OTC_String tmpLastAddress;

    istrstream ins2((char*)tmpAddresses.string(),tmpAddresses.length());

    while (ins2.good())
    {
      tmpLine.readLine(ins2);
      if (!ins2.fail())
      {
	theIndex = tmpLine.index('!');
	if (theIndex == -1)
	{
	  // Last recipient in address, send this
	  // separately rather than mixed in with
	  // any other addresses with the same
	  // receiver so as to make processing by
	  // by receiver easier.

	  OTCEV_Envelope* theEnvelope;
	  theEnvelope = new OTCEV_Envelope(this,tmpLine,from_,subject_,
	   tmpHeader,conversationId_,sequenceNumber_);
	  OTCLIB_ASSERT_M(theEnvelope != 0);

	  theEnvelope->queue(theType);
	}
	else
	{
	  tmpNewPrefix = tmpLine.before(theIndex);
	  tmpLine.remove(0,theIndex+1);

	  if (tmpCurrentPrefix.isEmpty())
	  {
	    // First address.

	    theNumAddresses = 1;
	    tmpCurrentPrefix = tmpNewPrefix;

	    tmpNewHeader = tmpHeader;
	    tmpNewHeader += "To: ";
	    tmpNewHeader += tmpLine;
	    tmpNewHeader += EOL;

	    tmpLastAddress = tmpLine;
	  }
	  else if (tmpNewPrefix != tmpCurrentPrefix)
	  {
	    OTCEV_Envelope* theEnvelope;

	    if (theNumAddresses == 1)
	    {
	      // Only one in batch.

	      tmpCurrentPrefix += '!';
	      tmpCurrentPrefix += tmpLastAddress;

	      theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	       from_,subject_,tmpHeader,conversationId_,sequenceNumber_);
	      OTCLIB_ASSERT_M(theEnvelope != 0);
	    }
	    else
	    {
	      // More than one in batch.

	      tmpCurrentPrefix += '!';
	      tmpCurrentPrefix += '*';

	      theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	       from_,subject_,tmpNewHeader,conversationId_,sequenceNumber_);
	      OTCLIB_ASSERT_M(theEnvelope != 0);
	    }

	    theEnvelope->queue(theType);

	    theNumAddresses = 1;
	    tmpCurrentPrefix =  tmpNewPrefix;

	    tmpNewHeader = tmpHeader;
	    tmpNewHeader += "To: ";
	    tmpNewHeader += tmpLine;
	    tmpNewHeader += EOL;

	    tmpLastAddress = tmpLine;
	  }
	  else
	  {
	    theNumAddresses += 1;
	    tmpNewHeader += "To: ";
	    tmpNewHeader += tmpLine;
	    tmpNewHeader += EOL;
	  }
	}
      }
    }

    if (theNumAddresses != 0)
    {
      OTCEV_Envelope* theEnvelope;

      if (theNumAddresses == 1)
      {
	// Only one in batch.

	tmpCurrentPrefix += '!';
	tmpCurrentPrefix += tmpLastAddress;

	theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	 from_,subject_,tmpHeader,conversationId_,sequenceNumber_);
	OTCLIB_ASSERT_M(theEnvelope != 0);
      }
      else
      {
	// More than one in batch.

	tmpCurrentPrefix += '!';
	tmpCurrentPrefix += '*';

	theEnvelope = new OTCEV_Envelope(this,tmpCurrentPrefix,
	 from_,subject_,tmpNewHeader,conversationId_,sequenceNumber_);
	OTCLIB_ASSERT_M(theEnvelope != 0);
      }

      theEnvelope->queue(theType);

      theNumAddresses = 1;
      tmpCurrentPrefix =  tmpNewPrefix;

      tmpNewHeader = tmpHeader;
      tmpNewHeader += "To: ";
      tmpNewHeader += tmpLine;
      tmpNewHeader += EOL;

      tmpLastAddress = tmpLine;
    }

    destroy();
  }
#endif
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::bounce(char const* theAddress, int theType)
{
  OTC_String theToAddress;

  if (theAddress == 0 || *theAddress == EOS)
    theToAddress = nextDestination();
  else
    theToAddress = theAddress;

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(this,theToAddress,from());
  OTCLIB_ASSERT_M(theEnvelope);

  theEnvelope->queue(theType);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::forward(char const* theAddress, int theType)
{
  OTC_String theToAddress;

  if (theAddress == 0 || *theAddress == EOS)
    theToAddress = nextDestination();
  else
    theToAddress = theAddress;

  OTC_String theFromAddress(currentReceiver());

  if (!theFromAddress.isEmpty())
    theFromAddress += '!';

  theFromAddress += from();

  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(this,theToAddress,theFromAddress);
  OTCLIB_ASSERT_M(theEnvelope);

  theEnvelope->queue(theType);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::reply(OTC_Event* theEvent, int theType)
{
  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(theEvent,from_,to_,
   subject_,header_,conversationId_,sequenceNumber_+1);
  OTCLIB_ASSERT_M(theEnvelope);

  theEnvelope->queue(theType);
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::reply(
 OTC_Event* theEvent,
 char const* theSubject,
 char const* theHeader,
 int theType
)
{
  OTCEV_Envelope* theEnvelope;
  theEnvelope = new OTCEV_Envelope(theEvent,from_,to_,
   theSubject,theHeader,conversationId_,sequenceNumber_+1);
  OTCLIB_ASSERT_M(theEnvelope);

  theEnvelope->queue(theType);
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Envelope::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Envelope::dump(ostream& outs) const
{
  outs << "<OTC> ENVELOPE - to = " << to_ << ", from = " << from_;
  outs << ", subject = " << subject_ << ", header = " << header_;
  outs << ", id = " << conversationId_ << ", seq = " << sequenceNumber_;
  if (content_->event() != 0)
    outs << ", event = " << *content_->event();
  else
    outs << ", message = " << *content_->message();
}

/* ------------------------------------------------------------------------- */
OTC_Event* OTCEV_Envelope::clone()
{
  // If the clone is the same object, destroy clone so as to put things
  // back as they were and return this object by calling base class clone()
  // function. The base class clone() function will use reference counting
  // to ensure that the event is not deleted before its time.

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;

  xxxMutex.grab(mutex_);

  OTC_Event* theEvent = 0;
  OTC_Event* theClone = 0;

  if (content_->event() != 0)
  {
    theEvent = (OTC_Event*)content_->event()->clone();
    if (theEvent == 0)
    {
      theClone = OTC_Event::clone();
    }
    else if (theEvent == content_->event())
    {
      theEvent->destroy();
      theClone = OTC_Event::clone();
    }
  }
  else
  {
    theEvent = (OTC_Event*)content_->message()->clone();
    if (theEvent == 0)
    {
      theClone = OTC_Event::clone();
    }
    else if (theEvent == content_->message())
    {
      theEvent->destroy();
      theClone = OTC_Event::clone();
    }
  }

  if (theClone == 0)
  {
    theEvent = new OTCEV_Envelope(theEvent,to_,from_,
     subject_,header_,conversationId_,sequenceNumber_);
    OTCLIB_ASSERT_M(theClone != 0);
  }

  xxxMutex.release();

  mutex_.unlock();

  return theClone;
}

/* ------------------------------------------------------------------------- */
OTCEV_Envelope::~OTCEV_Envelope()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;

  xxxMutex.grab(mutex_);

  content_->unReference();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
