/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcepisockt.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/episockt.hh"
#endif
#endif

#include <OTC/message/episockt.hh>

#include <OTC/message/episstrm.hh>
#include <OTC/message/epismesg.hh>
#include <OTC/message/episxxxx.hh>
#include <OTC/thread/mxreaper.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EPISocket::mutex_;
OTC_HIndex<OTC_String>* OTC_EPISocket::gIndex_ = 0;

/* ------------------------------------------------------------------------- */
OTC_EPISocket::~OTC_EPISocket()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_EPISocket::OTC_EPISocket(OTC_EndPoint* theEndPoint)
  : OTC_EPInterface(theEndPoint)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_EPISocket::install(
 OTC_String const& theInterface,
 OTC_EPISocket* (*theCallback)(OTC_EndPoint*)
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
    theNode = gIndex_->entry(theInterface);
    if (theNode != 0)
      theNode->removeEntry();
  }
  else
  {
    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->insert(theInterface);
    theNode->setItem((void*)theCallback);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_EPISocket* OTC_EPISocket::create(
 OTC_String const& theInterface,
 OTC_EndPoint* theEndPoint
)
{
  OTC_EPISocket* (*theCallback)(OTC_EndPoint*) = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gIndex_ != 0)
  {
    OTC_HIndexEntry<OTC_String>* theNode;
    theNode = gIndex_->entry(theInterface);
    if (theNode != 0)
      theCallback = (OTC_EPISocket* (*)(OTC_EndPoint*))theNode->item();
  }

  xxxMutex.release();

  mutex_.unlock();

  OTC_EPISocket* theResult = 0;

  if (theCallback != 0)
    theResult = theCallback(theEndPoint);

  if (theResult != 0)
    return theResult;

  if (theInterface == "MESSAGE")
  {
    theResult = new OTC_EPISMessage(theEndPoint);
    OTCLIB_ASSERT_M(theResult != 0);
  }
  else if (theInterface == "MeSsAgE")
  {
    theResult = new OTC_EPISMeSsAgE(theEndPoint);
    OTCLIB_ASSERT_M(theResult != 0);
  }
  else
  {
    theResult = new OTC_EPISStream(theEndPoint);
    OTCLIB_ASSERT_M(theResult != 0);
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
