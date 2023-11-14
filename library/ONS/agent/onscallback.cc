/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onscallback.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "ONS/agent/callback.hh"
#endif
#endif

#include <ONS/agent/callback.hh>
#include <ONS/agent/cbobject.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex ONS_Callback::mutex_;

/* ------------------------------------------------------------------------- */
ONS_Callback::~ONS_Callback()
{
  disconnect();
}

/* ------------------------------------------------------------------------- */
void ONS_Callback::disconnect()
{
  mutex_.lock();

  OTC_Iterator<ONS_ExecuteCallback*> theItems;
  theItems = callbacks_.items();

  ONS_ExecuteCallback* theCallback;
  ONS_CBObject* theTarget;

  while (theItems.isValid())
  {
    theCallback = theItems.item();
    theTarget = theCallback->target();

    theCallback->kill();

    if (theTarget->sources_ != 0)
      theTarget->sources_->remove(this,OTCLIB_UNSAFE);

    theItems.next();
  }

  theItems = 0;

  targets_.removeAll();
  callbacks_.removeAll();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void ONS_Callback::disconnect(char const* theName)
{
  mutex_.lock();

  OTC_String tmpName;
  tmpName = theName;

  if (callbacks_.contains(tmpName))
  {
    ONS_ExecuteCallback* theCallback;
    ONS_CBObject* theTarget;

    theCallback = callbacks_.item(tmpName);
    theTarget = theCallback->target();

    targets_.remove(theTarget,tmpName);

    if (!targets_.containsKey(theTarget))
    {
      if (theTarget->sources_ != 0)
	theTarget->sources_->remove(this,OTCLIB_UNSAFE);
    }

    callbacks_.remove(tmpName);
  }

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
ONS_Callback::ONS_Callback()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_String const& ONS_Callback::connect(ONS_ExecuteCallback* theCallback)
{
  OTCLIB_ENSURE_FN_W((theCallback != 0), 
   "ONS_Callback::connect()",
   "invalid callback was been supplied");

  if (theCallback == 0)
    return OTC_String::undefinedString();

  mutex_.lock();

  ONS_CBObject* theTarget;
  theTarget = theCallback->target();

  if (theTarget->sources_ == 0)
  {
    theTarget->sources_ = new OTC_HSet<ONS_Callback*>;
    OTCLIB_ASSERT_M(theTarget->sources_ != 0);
  }

  theTarget->sources_->add(this,OTCLIB_REPLACE);

  callbacks_.add(theCallback->name(),theCallback,OTCLIB_REPLACE);

  if (!targets_.contains(theTarget,theCallback->name()))
    targets_.add(theTarget,theCallback->name());

  mutex_.unlock();

  return theCallback->name();
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<ONS_ExecuteCallback*> ONS_Callback::callbacks() const
{
#if defined(OSE_WITH_THREADS)
  OTC_Deque<ONS_ExecuteCallback*> theResult;

  mutex_.lock();

  theResult.addLast(callbacks_.items());

  mutex_.unlock();

  return theResult.items();
#else
  return callbacks_.items();
#endif
}

/* ------------------------------------------------------------------------- */
