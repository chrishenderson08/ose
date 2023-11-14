/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcsubject.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/subject.hh"
#endif
#endif

#include <OTC/dispatch/subject.hh>

#include <OTC/thread/mxreaper.hh>
#include <OTC/text/silink.hh>
#include <OTC/text/globex.hh>

/* ------------------------------------------------------------------------- */
OTC_Subject::~OTC_Subject()
{
  observers_->unReference();
}

/* ------------------------------------------------------------------------- */
OTC_Subject::OTC_Subject()
  : observers_(0)
{
  observers_ = new OTC_LinkList;
  OTCLIB_ASSERT_M(observers_ != 0);

  observers_->reference();
}

/* ------------------------------------------------------------------------- */
void OTC_Subject::addObserver(int theAgentId, char const* theSubject)
{
  if (theSubject == 0 || *theSubject == EOS)
    theSubject = "*";

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  bool theExists;
  theExists = false;

  OTC_SILink* theHolder;
  OTC_LinkIterator theIter = observers_->items();
  theIter.resetFirst();
  while (theIter.isLink())
  {
    theHolder = (OTC_SILink*)theIter.link();

    if (theHolder->key() == theSubject &&
     theHolder->item() == theAgentId)
    {
      theExists = true;

      break;
    }

    theIter.next();
  }

  if (theExists == false)
  {
    theHolder = new OTC_SILink(theSubject,theAgentId);
    OTCLIB_ASSERT_M(theHolder);

    observers_->addLast(theHolder);
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Subject::removeObserver(int theAgentId, char const* theSubject)
{
  if (theSubject == 0 || *theSubject == EOS)
    theSubject = "*";

  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!observers_->isEmpty())
  {
    OTC_SILink* theHolder;
    OTC_LinkIterator theIter = observers_->items();
    theIter.resetFirst();
    while (theIter.isLink())
    {
      theHolder = (OTC_SILink*)theIter.link();

      if (theHolder->key() == theSubject &&
       theHolder->item() == theAgentId)
      {
	theHolder->kill();

	break;
      }

      theIter.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Subject::removeAllObservers()
{
  mutex_.writeLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  observers_->removeAll();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_Subject::observersExist() const
{
  bool theResult;

  mutex_.readLock();

  theResult = !observers_->isEmpty();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTC_Subject::observersExist(char const* theSubject) const
{
  bool theResult;
  theResult = false;

  if (theSubject == 0)
    theSubject = "";

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!observers_->isEmpty())
  {
    OTC_SILink* theHolder;
    OTC_LinkIterator theIter = observers_->items();
    theIter.resetFirst();
    while (theIter.isLink())
    {
      theHolder = (OTC_SILink*)theIter.link();

      OTC_Globex thePattern(theHolder->key());

      if (thePattern.match(theSubject))
      {
	theResult = true;

	break;
      }

      theIter.next();
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Subject::notifyObservers(
 OTC_Event* theEvent,
 char const* theSubject,
 int theType
) const
{
  if (theSubject == 0)
    theSubject = "";

  mutex_.readLock();

  OTC_MutexReaper<OTC_RWMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (!observers_->isEmpty())
  {
    OTC_SILink* theHolder;
    OTC_LinkIterator theIter = observers_->items();
    theIter.resetFirst();
    while (theIter.isLink())
    {
      theHolder = (OTC_SILink*)theIter.link();

      OTC_Globex thePattern(theHolder->key());

      if (thePattern.match(theSubject))
      {
	OTC_Event* theClone;
	theClone = theEvent->clone();
	theClone->queue(theHolder->item(),theType);
      }

      theIter.next();
    }
  }

  theEvent->destroy();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
