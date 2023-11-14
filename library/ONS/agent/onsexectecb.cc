/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/onsexectecb.cc
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
#pragma implementation "ONS/agent/exectecb.hh"
#endif
#endif

#include <ONS/agent/exectecb.hh>
#include <OTC/system/program.hh>

/* ------------------------------------------------------------------------- */
OTC_NRMutex ONS_ExecuteCallback::mutex_;

/* ------------------------------------------------------------------------- */
ONS_ExecuteCallback::ONS_ExecuteCallback(
 ONS_CBObject const* theTarget,
 char const* theName
)
  : target_((ONS_CBObject*)theTarget), dead_(false)
{
  if (theName == 0)
    name_ = OTC_Program::uniqueId("$CALLBACK?",OTCLIB_ID_SHORT_FORMAT);
  else
    name_ = theName;
} 

/* ------------------------------------------------------------------------- */
void ONS_ExecuteCallback::kill()  
{
  mutex_.lock();

  dead_ = true;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool ONS_ExecuteCallback::dead() const
{
  bool theResult;

  mutex_.lock();

  theResult = dead_;

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
