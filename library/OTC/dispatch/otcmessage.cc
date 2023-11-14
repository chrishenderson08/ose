/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcmessage.cc
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
#pragma implementation "OTC/dispatch/message.hh"
#endif
#endif

#include <OTC/dispatch/message.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTCEV_Message::mutex_;
int OTCEV_Message::gTypeId_ = 0;
OTC_HIndex<OTC_String>* OTCEV_Message::gIndex_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Message::OTCEV_Message(
 OTC_String const& theContent,
 char const* theContentType,
 char const* theEventType
)
  : content_(theContent),
    contentType_(theContentType),
    eventType_(theEventType)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTCEV_Message::callback(
 OTC_String const& theType,
 OTC_Event* (*theCallback)(OTCEV_Message*)
)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gIndex_ == 0)
  {
    gIndex_ = new OTC_HIndex<OTC_String>;
    OTCLIB_ASSERT_M(gIndex_ != 0);
  }

  if (theCallback == 0)
  {
    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->entry(theType);
    if (theNode != 0)
      theNode->removeEntry();
  }
  else
  {
    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->insert(theType);
    theNode->setItem((void*)theCallback);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTCEV_Message* OTCEV_Message::deflate() const
{
  OTC_Event* theSelf;
  theSelf = (OTC_Event*)this;

  OTC_Event* theEvent;
  theEvent = theSelf->clone();

  return (OTCEV_Message*)theEvent;
}

/* ------------------------------------------------------------------------- */
OTC_Event* OTCEV_Message::inflate(char const* theType) const
{
  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gIndex_ == 0)
  {
    gIndex_ = new OTC_HIndex<OTC_String>;
    OTCLIB_ASSERT_M(gIndex_ != 0);
  }

  OTC_String theLookupType;

  if (theType == 0)
    theLookupType = eventType_;
  else
    theLookupType = theType;

  OTC_HIndexEntry<OTC_String>* theNode;
  theNode = gIndex_->entry(theLookupType);

  OTC_Event* (*theCallback)(OTCEV_Message*) = 0;

  if (theNode != 0)
    theCallback = (OTC_Event* (*)(OTCEV_Message*))theNode->item();

  xxxMutex.release();

  mutex_.unlock();

  if (theCallback != 0)
    return (*theCallback)((OTCEV_Message*)this);

  return 0;
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Message::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Message::dump(ostream& outs) const
{
  outs << "<OTC> MESSAGE ";
  outs << "- contentType = " << contentType_;
  outs << ", eventType = " << eventType_;
  outs << ", contentLength = " << content_.length();
  outs << ", content = " << content_;
}

/* ------------------------------------------------------------------------- */
OTCEV_Message::~OTCEV_Message()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
