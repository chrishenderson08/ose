/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otctracetag.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/tracetag.hh"
#endif
#endif

#include <OTC/debug/tracetag.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/strstream.h>

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_TraceTag::mutex_;
char const* OTC_TraceTag::globTagVariable_ = 0;
OTC_TagInfo* OTC_TraceTag::globTagInfo_ = 0;

/* ------------------------------------------------------------------------- */
OTC_TraceTag::~OTC_TraceTag()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_TraceTag::OTC_TraceTag(char const* theName)
  : tagInfo_(0)
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_TraceTag::OTC_TraceTag(char const*)",
   "invalid tag name");

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_TagInfo* theInfo;

  if (globTagVariable_ == 0)
  {
    globTagVariable_ = getenv("OTCLIB_TRACETAGS");
    if (globTagVariable_ == 0)
      globTagVariable_ = "";

    if (*globTagVariable_ != 0)
    {
      istrstream ins((char*)globTagVariable_);
      char buf[64];
      while (ins.good())
      {
	char* theIndex = 0;
	int theLevel = 0;

	ins >> ws;
	ins.width(64);
	ins >> buf;

	if (!ins.fail())
	{
	  theIndex = strchr(buf,'=');
	  if (theIndex != 0)
	  {
	    *theIndex = EOS;
	    theLevel = atoi(theIndex+1);
	  }

	  char* tmpString = new char[strlen(buf)+1];
	  OTCLIB_ASSERT_M(tmpString != 0);
	  strcpy(tmpString,buf);

	  theInfo = new OTC_TagInfo(tmpString,theLevel,globTagInfo_);
	  OTCLIB_ASSERT_M(theInfo != 0);

	  globTagInfo_ = theInfo;
	}
      }
    }
  }

  theInfo = globTagInfo_;
  while (theInfo != 0)
  {
    if (strcmp(theInfo->name(),theName) == 0)
    {
      tagInfo_ = theInfo;
      break;
    }
    theInfo = theInfo->next();
  }

  if (tagInfo_ == 0)
  {
    char* tmpString = new char[strlen(theName)+1];
    OTCLIB_ASSERT_M(tmpString != 0);
    strcpy(tmpString,theName);

    theInfo = new OTC_TagInfo(tmpString,-1,globTagInfo_);
    OTCLIB_ASSERT_M(theInfo != 0);

    globTagInfo_ = theInfo;
    tagInfo_ = theInfo;
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_TraceTag::enabled() const
{
  return (tagInfo_->level() >= 0) ? true : false;
}

/* ------------------------------------------------------------------------- */
int OTC_TraceTag::set(char const* theName, int theLevel)
{
  OTC_TagInfo* theInfo;
  int oldLevel = -1;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  theInfo = globTagInfo_;
  while (theInfo != 0)
  {
    if (strcmp(theInfo->name(),theName) == 0)
    {
      oldLevel = theInfo->level();
      theInfo->setLevel(theLevel);
      break;
    }
    theInfo = theInfo->next();
  }

  if (theInfo == 0)
  {
    char* tmpString = new char[strlen(theName)+1];
    OTCLIB_ASSERT_M(tmpString != 0);
    strcpy(tmpString,theName);

    theInfo = new OTC_TagInfo(tmpString,theLevel,globTagInfo_);
    OTCLIB_ASSERT_M(theInfo != 0);

    globTagInfo_ = theInfo;
  }

  xxxMutex.release();

  mutex_.unlock();

  return oldLevel;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceTag::operator==(int theLevel) const
{
  return ((level() >= 0) && (level() == theLevel)) ? true : false;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceTag::operator!=(int theLevel) const
{
  return ((level() >= 0) && (level() != theLevel)) ? true : false;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceTag::operator<(int theLevel) const
{
  return ((level() >= 0) && (level() < theLevel)) ? true : false;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceTag::operator<=(int theLevel) const
{
  return ((level() >= 0) && (level() <= theLevel)) ? true : false;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceTag::operator>(int theLevel) const
{
  return ((level() >= 0) && (level() > theLevel)) ? true : false;
}

/* ------------------------------------------------------------------------- */
OTC_TraceSwitch OTC_TraceTag::operator>=(int theLevel) const
{
  return ((level() >= 0) && (level() >= theLevel)) ? true : false;
}

/* ------------------------------------------------------------------------- */
