/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onscbobject.cc
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
#pragma implementation "ONS/agent/cbobject.hh"
#endif
#endif

#include <ONS/agent/cbobject.hh>
#include <ONS/agent/callback.hh>

/* ------------------------------------------------------------------------- */
ONS_CBObject::~ONS_CBObject()
{
  detachAllCallbacks();

  if (sources_ != 0)
    delete sources_;
}

/* ------------------------------------------------------------------------- */
ONS_CBObject::ONS_CBObject()
  : sources_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void ONS_CBObject::detachAllCallbacks()
{
  ONS_Callback::mutex_.lock();

  if (sources_ != 0)
  {
    OTC_Iterator<ONS_Callback*> theCallbacks;
    theCallbacks = sources_->items();

    while (theCallbacks.isValid())
    {
      ONS_Callback* theCallback;
      OTC_Iterator<OTC_String> theNames;

      theCallback = theCallbacks.item();
      theNames = theCallback->targets_.items(this);

      while (theNames.isValid())
      {
	theCallback->callbacks_.item(theNames.item())->kill();
	theCallback->callbacks_.remove(theNames.item());

	theNames.next();
      }

      theCallback->targets_.removeKey(this);

      theCallbacks.next();
    }

    theCallbacks = 0;

    sources_->removeAll();
  }

  ONS_Callback::mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
