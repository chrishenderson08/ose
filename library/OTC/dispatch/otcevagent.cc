/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcevagent.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/evagent.hh"
#endif
#endif

#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/event.hh>
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
    TAG = new OTC_TraceTag("OTC_EVAgent");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_EVAgent::mutex_;
OTC_HashTable* OTC_EVAgent::globAgents_ = 0;
int OTC_EVAgent::globId_ = 0;

/* ------------------------------------------------------------------------- */
OTC_EVAgent::~OTC_EVAgent()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVAgent::~OTC_EVAgent()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  globAgents_->remove((void*)this,id_);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_EVAgent* OTC_EVAgent::lookup(int theAgentId)
{
  OTC_EVAgent* theAgent;
  theAgent = 0;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globAgents_ != 0)
  {
    int tmpRank;
    int tmpHashValue;
    u_int tmpIteration = 0;
    u_int tmpStartIndex = 0;
    u_int tmpEntryIndex = 0;

    OTC_HashEntry* tmpEntry = 0;
    OTC_EVAgent* tmpIndexEntry = 0;

    tmpHashValue = theAgentId;
    tmpEntry = globAgents_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0 && !tmpEntry->isEmpty())
    {
      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_EVAgent*)tmpEntry->item();
	tmpRank = theAgentId - tmpIndexEntry->id();
	if (tmpRank == 0)
	{
	  theAgent = tmpIndexEntry;
	  break;
	}
      }

      tmpEntry = globAgents_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }
  }

  xxxMutex.release();

  mutex_.unlock();

  return theAgent;
}

/* ------------------------------------------------------------------------- */
OTC_EVAgent::OTC_EVAgent()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVAgent::OTC_EVAgent()");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globAgents_ == 0)
  {
    globAgents_ = new OTC_HashTable;
    OTCLIB_ASSERT_M(globAgents_ != 0);
  }

  // It is unlikely it will wrap around, but
  // just in case. Also, never use id 0, use
  // this for a bad agent.

  id_ = globId_++;
  if (id_ == 0)
    id_ = globId_++;

  int tmpHashValue = 0;
  int tmpRank;
  u_int tmpIteration = 0;
  u_int tmpStartIndex = 0;
  u_int tmpEntryIndex = 0;

  int tmpAvailableIndex = -1;

  OTC_HashEntry* tmpEntry = 0;
  OTC_EVAgent* tmpIndexEntry = 0;

  while (tmpHashValue != id_)
  {
    tmpHashValue = id_;
    tmpEntry = globAgents_->searchHash(tmpHashValue,
     tmpIteration,tmpStartIndex,tmpEntryIndex);

    while (tmpEntry != 0)
    {
      if (!tmpEntry->isOccupied())
      {
	if (tmpAvailableIndex == -1)
	  tmpAvailableIndex = tmpEntryIndex;
      }

      if (tmpEntry->isEmpty())
	break;

      if (tmpEntry->isOccupied())
      {
	tmpIndexEntry = (OTC_EVAgent*)tmpEntry->item();

	tmpRank = id_ - tmpIndexEntry->id();
	if (tmpRank == 0)
	{
	  id_ = globId_++;
	  if (id_ == 0)
	    id_ = globId_++;

	  break;
	}
      }

      tmpEntry = globAgents_->searchHash(tmpHashValue,
       tmpIteration,tmpStartIndex,tmpEntryIndex);
    }
  }

  globAgents_->add(tmpAvailableIndex,(void*)this,tmpHashValue);

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_EVAgent::accept(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_EVAgent::accept(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << *theEvent << endl;

  execute(theEvent);
}

/* ------------------------------------------------------------------------- */
void OTC_EVAgent::execute(OTC_Event* theEvent)
{
  filter(theEvent);
}

/* ------------------------------------------------------------------------- */
void OTC_EVAgent::filter(OTC_Event* theEvent)
{
  handle(theEvent);
}

/* ------------------------------------------------------------------------- */
