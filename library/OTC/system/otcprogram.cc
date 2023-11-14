/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/otcprogram.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/program.hh"
#endif
#endif

#include <OTC/system/pathname.hh>
#include <OTC/system/program.hh>
#include <OTC/text/globex.hh>
#include <OTC/text/regexp.hh>
#include <OTC/thread/mxreaper.hh>

#include <stdlib.h>
#include <time.h>

#if defined(OSE_SYS_UNIX)
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#if defined(HAVE_UNAME)
#include <sys/utsname.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <io.h>
#ifndef getpid
#define getpid _getpid
#endif
#define MAXHOSTNAMELEN MAX_COMPUTERNAME_LENGTH
#ifndef WINSOCK_VERSION
#define WINSOCK_VERSION MAKEWORD(1,0)
#endif 
#endif

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 80
#endif

/* ------------------------------------------------------------------------- */
#if defined(OSE_SYS_WIN32)
extern "C" void otclib_winsock_cleanup() { WSACleanup(); }
#endif

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Program::mutex_;
OTC_NRMutex OTC_Program::idmutex_;
OTC_String* OTC_Program::gName_ = 0;
OTC_Options* OTC_Program::gOptions_ = 0;
OTC_String* OTC_Program::gBasename_ = 0;
OTC_String* OTC_Program::gDirname_ = 0;
OTC_String* OTC_Program::gHostname_ = 0;
OTC_String* OTC_Program::gHostnameIP_ = 0;
u_long OTC_Program::gInitTime_ = 0;
u_long OTC_Program::gProcessId_ = 0;
u_int OTC_Program::gCounter_ = 0;
OTC_Properties* OTC_Program::gConfig_ = 0;
OTC_HMap<OTC_String,OTC_String>* OTC_Program::gEnviron_ = 0;

/* ------------------------------------------------------------------------- */
void OTC_Program::initialise(int argc, char* argv[])
{
  OTCLIB_ENSURE_FN((argv != 0 && argc > 0),
   "OTC_Program::initialise(int, char*[])",
   "invalid arguments");

  OTC_String* tmpName = 0;
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  if (gName_ == 0 && gOptions_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gName_ = new OTC_String;
    OTCLIB_ASSERT_M(gName_ != 0);
    gOptions_ = new OTC_Options;
    OTCLIB_ASSERT_M(gOptions_ != 0);

    gName_->assign(argv[0]);
    if (argc > 1)
      gOptions_->initialise(argc-1,argv+1);

    tmpName = gName_;
    tmpOptions = gOptions_;

    xxxMutex.release();
  }

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpName != 0 && tmpOptions != 0),
   "OTC_Program::initialise(int, char*[])",
   "class already initialised");
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::name()
{
  OTC_String* tmpName = 0;

  mutex_.lock();

  tmpName = gName_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpName != 0),
   "OTC_Program::name()",
   "class not initialised");

  return *tmpName;
}

/* ------------------------------------------------------------------------- */
int OTC_Program::numOptions()
{
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpOptions = gOptions_;

  mutex_.unlock();

  if (tmpOptions == 0)
    return -1;

  return tmpOptions->numOptions();
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::option(u_int theNum)
{
  OTC_String* tmpName = 0;
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpName = gName_;
  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpName != 0 && tmpOptions != 0),
   "OTC_Program::option(u_int)",
   "class not initialised");
  OTCLIB_ENSURE_FN((theNum <= tmpOptions->numOptions()),
   "OTC_Program::option(u_int)",
   "invalid option number");

  if (theNum == 0)
    return *tmpName;

  return tmpOptions->option(theNum);
}

/* ------------------------------------------------------------------------- */
bool OTC_Program::optionMatches(u_int theNum, char const* thePattern)
{
  OTC_String* tmpName = 0;
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpName = gName_;
  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpName != 0 && tmpOptions != 0),
   "OTC_Program::optionMatches(u_int, char const*)",
   "class not initialised");
  OTCLIB_ENSURE_FN((theNum <= tmpOptions->numOptions()),
   "OTC_Program::optionMatches(u_int, char const*)",
   "invalid option number");

  OTC_Globex tmpPattern(thePattern);

  if (theNum == 0)
    return tmpPattern.match(*tmpName);

  return tmpPattern.match(tmpOptions->option(theNum));
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Program::options(OTC_Direction theDirection)
{
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpOptions != 0),
   "OTC_Program::options(OTC_Direction)",
   "class not initialised");

  return tmpOptions->options(theDirection,OTCLIB_UNSAFE);
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Program::options(
 u_int theStart,
 u_int theLength,
 OTC_Direction theDirection
)
{
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpOptions != 0),
   "OTC_Program::options(u_int, u_int, OTC_Direction)",
   "class not initialised");

  return tmpOptions->options(theStart,theLength,theDirection,OTCLIB_UNSAFE);
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Program::options(
 OTC_Range const& theRange,
 OTC_Direction theDirection
)
{
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpOptions != 0),
   "OTC_Program::options(OTC_Range const&, OTC_Direction)",
   "class not initialised");

  return tmpOptions->options(theRange,theDirection,OTCLIB_UNSAFE);
}

/* ------------------------------------------------------------------------- */
void OTC_Program::shift(u_int theNum)
{
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpOptions != 0),
   "OTC_Program::shift(u_int)",
   "class not initialised");

  tmpOptions->shift(theNum);
}

/* ------------------------------------------------------------------------- */
void OTC_Program::restore()
{
  OTC_Options* tmpOptions = 0;

  mutex_.lock();

  tmpOptions = gOptions_;

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpOptions != 0),
   "OTC_Program::restore()",
   "class not initialised");

  tmpOptions->restore();
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::basename()
{
  OTC_String* tmpName = 0;

  mutex_.lock();

  tmpName = gName_;

  if (tmpName != 0 && gBasename_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Pathname path = tmpName->string();

    gBasename_ = new OTC_String(path.basename());
    OTCLIB_ASSERT_M(gBasename_ != 0);

    xxxMutex.release();
  }

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpName != 0),
   "OTC_Program::basename()",
   "class not initialised");

  return *gBasename_;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::dirname()
{
  OTC_String* tmpName = 0;

  mutex_.lock();

  tmpName = gName_;

  if (tmpName != 0 && gDirname_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    OTC_Pathname path = tmpName->string();

    gDirname_ = new OTC_String(path.dirname());
    OTCLIB_ASSERT_M(gDirname_ != 0);

    xxxMutex.release();
  }

  mutex_.unlock();

  OTCLIB_ENSURE_FN((tmpName != 0),
   "OTC_Program::dirname()",
   "class not initialised");

  return *gDirname_;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::hostname()
{
  mutex_.lock();

  if (gHostname_ == 0)
  {
    char tmpHostName[MAXHOSTNAMELEN+1];

#if defined(OSE_SYS_WIN32)
    DWORD tmpLength = MAXHOSTNAMELEN+1;
    GetComputerName(&tmpHostName[0],&tmpLength);
#else
#if defined(HAVE_UNAME)
    static struct utsname hostinfo;
    uname(&hostinfo);
    int theLength = strlen(hostinfo.nodename);
    tmpHostName[MAXHOSTNAMELEN] = EOS;
    if (theLength < MAXHOSTNAMELEN)
      strcpy(&tmpHostName[0],hostinfo.nodename);
    else
      strncpy(&tmpHostName[0],hostinfo.nodename,MAXHOSTNAMELEN);
#else
    gethostname(&tmpHostName[0],MAXHOSTNAMELEN+1);
#endif
#endif

    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gHostname_ = new OTC_String(tmpHostName);
    OTCLIB_ASSERT_M(gHostname_ != 0);

    xxxMutex.release();
  }

  mutex_.unlock();

  return *gHostname_;
}

/* ------------------------------------------------------------------------- */
long OTC_Program::processId()
{
  return getpid();
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Program::processIdentity()
{
  OTC_String theString(hostname());

  char buf[128];

  sprintf(buf,"%lu",u_long(processId()));

  theString += ":";
  theString += buf;

  return theString;
}

/* ------------------------------------------------------------------------- */
void OTC_Program::removeAllConfig()
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->removeAll();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Program::removeConfig(char const* theKey)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->remove(theKey);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Program::loadConfig(OTC_Properties const& theDatabase)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->load(theDatabase);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Program::loadConfig(istream& theStream, char const* theChannel)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->load(theStream,theChannel);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Program::mergeConfig(OTC_Properties const& theDatabase)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->merge(theDatabase);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Program::mergeConfig(istream& theStream, char const* theChannel)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->merge(theStream,theChannel);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Program::mergeConfig(char const* theKey, char const* theValue)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->merge(theKey,theValue);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::lookupConfig(char const* theKey)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  OTC_String const& theResult = gConfig_->lookup(theKey);

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_String const& OTC_Program::lookupConfig(
 char const* theKeyPrefix,
 char const* theKey
)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  OTC_String const& theResult = gConfig_->lookup(theKeyPrefix,theKey);

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Program::lookupEnviron(char const* theName)
{
  if (theName == 0)
    return OTC_String::undefinedString();

  OTC_String theResult;

  mutex_.lock();

  char const* theValue = 0;

  theValue = getenv(theName);

  if (theValue != 0)
  {
    if (gEnviron_ != 0)
    {
      OTC_String theCachedName;
      theCachedName = theName;

      if (gEnviron_->contains(theCachedName))
      {
	OTC_String theCachedString;
	theCachedString = gEnviron_->item(theCachedName);

	if (theValue != theCachedString.string()+theCachedName.length()+1)
	{
	  gEnviron_->remove(theCachedName);
	}
      }
    }

    theResult = theValue;
  }

  mutex_.unlock();

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Program::mergeEnviron(char const* theName, char const* theValue)
{
  if (theName == 0)
    return;

  mutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (gEnviron_ == 0)
  {
    gEnviron_ = new OTC_HMap<OTC_String,OTC_String>;
    OTCLIB_ASSERT_M(gEnviron_ != 0);
  }

  OTC_String theCachedString;
  theCachedString += theName;
  theCachedString += '=';
  theCachedString += theValue;

  putenv((char*)theCachedString.string());

  char const* theNewValue = 0;

  theNewValue = getenv(theName);

  if (theNewValue != 0)
  {
    OTC_String theCachedName;
    theCachedName = theName;

    if (gEnviron_->contains(theCachedName))
    {
      OTC_String theOldCachedString;
      theOldCachedString = gEnviron_->item(theCachedName);

      if (theNewValue != theOldCachedString.string()+theCachedName.length()+1)
      {
	gEnviron_->remove(theCachedName);
	gEnviron_->add(theCachedName,theCachedString);
      }
    }
    else
    {
      if (theNewValue == theCachedString.string()+theCachedName.length()+1)
      {
	gEnviron_->add(theCachedName,theCachedString);
      }
    }
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Program::expandEnviron(char const* theString)
{
  if (theString == 0)
    return OTC_String::undefinedString();

  OTC_String theResult;
  theResult = theString;

  u_int theOffset = 0;

  OTC_Regexp thePattern(
   "\\$(([a-zA-Z][a-zA-Z0-9_]*)|{([a-zA-Z][a-zA-Z0-9_]*)})");

  OTC_String theName;
  OTC_String theValue;

  char const* theStart;
  theStart = theResult.string()+theOffset;

  while (thePattern.match(theStart))
  {
    u_int theIndex = 0;

    if (thePattern.length(2) != 0)
      theIndex = 2;
    else if (thePattern.length(3) != 0)
      theIndex = 3;
    else
      break;

    theName.assign(theStart+thePattern.start(theIndex),
     thePattern.length(theIndex));

    theValue = lookupEnviron(theName);

    theResult.replace(theOffset+thePattern.start(0),
     thePattern.length(0),theValue);

    theOffset += thePattern.start(0)+theValue.length();

    theStart = theResult.string()+theOffset;
  }

  return theResult;
}

/* ------------------------------------------------------------------------- */
void OTC_Program::saveConfig(ostream& theStream)
{
  mutex_.lock();

  if (gConfig_ == 0)
  {
    OTC_MutexReaper<OTC_NRMutex> xxxMutex;
    xxxMutex.grab(mutex_);

    gConfig_ = new OTC_Properties;
    OTCLIB_ASSERT_M(gConfig_ != 0);

    xxxMutex.release();
  }

  gConfig_->save(theStream);

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Program::uniqueId(
 char const* thePrefix,
 OTC_UniqueIdType theType
)
{
  // If the prefix is extra long we will
  // exceed 63 bytes and need to resize.
  // This should be the exception. Using
  // a value of 63 ensures that in the
  // normal environment memory will come
  // from the common memory pool.

  OTC_Capacity theCapacity(63);
  OTC_String theResult(theCapacity);

  theResult += thePrefix;

  char buf[32];
  u_long tmpProcessId;
  tmpProcessId = getpid();

  u_long nowTime;
  nowTime = time(0);

  idmutex_.lock();

  OTC_MutexReaper<OTC_NRMutex> xxxMutex;
  xxxMutex.grab(idmutex_);

#if defined(__linux__) && defined(OSE_WITH_THREADS)
  if (gInitTime_ == 0)
    gInitTime_ = nowTime;
#else
  if (gInitTime_ == 0 || gProcessId_ != tmpProcessId)
  {
    gInitTime_ = nowTime;
    gProcessId_ = tmpProcessId;
    gCounter_ = 0;
  }
#endif

  if (++gCounter_ == 0)
    gCounter_++;

  if (gHostnameIP_ == 0)
  {
    struct hostent *hp;

#if defined(OSE_SYS_WIN32)
  WSAData wsaData;
  WSAStartup(WINSOCK_VERSION,&wsaData);
  atexit(otclib_winsock_cleanup);
#endif

#ifdef _REENTRANT
    int err;
    char hostbuf[2048];
    struct hostent shp;
    hp = gethostbyname_r(hostname(),&shp,(char*)&hostbuf,sizeof(hostbuf),&err);
#else
    hp = gethostbyname(hostname());
#endif

    if (hp == 0)
    {
      gHostnameIP_ = new OTC_String(hostname());
      OTCLIB_ASSERT_M(gHostnameIP_ != 0);
    }
    else
    {
      struct in_addr in;

      memcpy(&in.s_addr,*hp->h_addr_list,sizeof(in.s_addr));

      sprintf(buf,"%08lX",u_long(ntohl(in.s_addr)));

      gHostnameIP_ = new OTC_String(buf);
      OTCLIB_ASSERT_M(gHostnameIP_ != 0);
    }
  }

  xxxMutex.release();

  idmutex_.unlock();

  theResult += *gHostnameIP_;

  theResult += ':';

  //sprintf(buf,"%04lX",tmpProcessId);
  sprintf(buf,"%lX",tmpProcessId);

  theResult += buf;
  theResult += ':';

  if (theType == OTCLIB_ID_LONG_FORMAT)
  {
    //sprintf(buf,"%08lX",gInitTime_);
    sprintf(buf,"%lX",gInitTime_);

    theResult += buf;
    theResult += ':';

    //sprintf(buf,"%06lX",nowTime-gInitTime_);
    sprintf(buf,"%lX",nowTime-gInitTime_);

    theResult += buf;
    theResult += ':';
  }

  //sprintf(buf,"%02X",gCounter_);
  sprintf(buf,"%X",gCounter_);

  theResult += buf;

  return theResult;
}

/* ------------------------------------------------------------------------- */
bool OTC_Program::threadingEnabled()
{
#if defined(OSE_WITH_THREADS)
  return true;
#else
  return false;
#endif
}

/* ------------------------------------------------------------------------- */
