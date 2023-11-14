/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/otclogger.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/logger.hh"
#endif
#endif

#include <OTC/debug/logger.hh>
#include <OTC/thread/mxreaper.hh>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <windows.h>
#include <process.h>
#ifndef getpid
#define getpid _getpid
#endif
#define MAXHOSTNAMELEN MAX_COMPUTERNAME_LENGTH
#endif

#if defined(HAVE_UNAME)
#include <sys/utsname.h>
#endif

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 80
#endif

/* ------------------------------------------------------------------------- */
bool OTC_Logger::initialised_ = false;
bool OTC_Logger::useStderr_ = false;
char const* OTC_Logger::logFile_ = 0;
char* OTC_Logger::lastLogFile_ = 0;
bool OTC_Logger::appendLogFile_ = false;
OTC_Logger* OTC_Logger::head_ = 0;
FILE* OTC_Logger::stream_ = 0;
bool OTC_Logger::longFormat_ = false;
OTC_Mutex OTC_Logger::mutex_;

char OTC_Logger::hostName_[MAXHOSTNAMELEN+1] = { 0 };

/* ------------------------------------------------------------------------- */
char const* const OTCLIB_LOGLEVELMESSAGES[] =
{
  "EMERGENCY",
  "ALERT",
  "CRITICAL",
  "ERROR",
  "WARNING",
  "NOTICE",
  "INFO",
  "DEBUG"
};

/* ------------------------------------------------------------------------- */
static char const* const OTCLIB_SHORTMONTHS[] =
{
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
};

/* ------------------------------------------------------------------------- */
OTC_Logger::~OTC_Logger()
{
  mutex_.lock();

  unlink();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::initialise()
{
  // Assumed that this function is called from inside locked region.

  // If not already initialised, do it.

  if (initialised_ == false)
  {
#if defined(OSE_SYS_WIN32)
    DWORD tmpLength = MAXHOSTNAMELEN+1;
    GetComputerName(&hostName_[0],&tmpLength);
#else
#if defined(HAVE_UNAME)
    static struct utsname hostinfo;
    uname(&hostinfo);
    int theLength = strlen(hostinfo.nodename);
    if (theLength < MAXHOSTNAMELEN)
      strcpy(&hostName_[0],hostinfo.nodename);
    else
      strncpy(&hostName_[0],hostinfo.nodename,MAXHOSTNAMELEN);
#else
    gethostname(&hostName_[0],MAXHOSTNAMELEN+1);
#endif
#endif

    logFile_ = getenv("OTCLIB_LOGFILE");
    if (getenv("OTCLIB_APPENDLOGFILE") != 0)
      appendLogFile_ = true;
    lastLogFile_ = new char[MAXPATHLEN+1];
    lastLogFile_[0] = EOS;

#ifdef NDEBUG
    useStderr_ = false;
    if (getenv("OTCLIB_LOGSTDERR") != 0)
      useStderr_ = true;
#else
    useStderr_ = true;
    if (getenv("OTCLIB_NOLOGSTDERR") != 0)
      useStderr_ = false;
#endif

    if (stream_ == 0)
    {
      stream_ = stderr;
      errno = 0;
    }

    if (getenv("OTCLIB_LOGLONGFORMAT") != 0)
      longFormat_ = true;

    initialised_ = true;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::notify(
 char const* theChannel,
 OTC_LogLevel theLevel,
 char const* theMessage,
 int theLength
)
{
  mutex_.lock();

  initialise();

  if (theMessage == 0)
    theMessage = "";

  if (theLength < 0)
    theLength = strlen(theMessage);

  if (theChannel == 0 || *theChannel == EOS || isalnum(*theChannel))
  {
    int const BUFSIZE = 128;
    char buffer[BUFSIZE];

    time_t secs = 0;
    secs = time(0);
#if defined(_REENTRANT)
    tm timetmstruct;
    localtime_r(&secs,&timetmstruct);
    tm* timetm = &timetmstruct;
#else
    tm* timetm = localtime(&secs);
#endif

    if (longFormat_ != false)
    {
      int theProcessId = 0;
      theProcessId = getpid();

      int theThreadId = 0;

#if defined(OSE_WITH_THREADS)
      pthread_t theThread;
      theThread = pthread_self();
      theThreadId = (int)theThread;
#endif

#if defined(OSE_WITH_THREADS)
#if defined(OSE_SYS_WIN32)
      sprintf(
       buffer,
       "%s %2d %.2d:%.2d:%.2d [%lu/%lu] %s: ",
       OTCLIB_SHORTMONTHS[timetm->tm_mon],
       timetm->tm_mday,
       timetm->tm_hour,
       timetm->tm_min,
       timetm->tm_sec,
       u_long(theProcessId),
       u_long(theThreadId),
       OTCLIB_LOGLEVELMESSAGES[theLevel]
      );
#else
      snprintf(
       buffer,
       BUFSIZE,
       "%s %2d %.2d:%.2d:%.2d [%lu/%lu] %s: ",
       OTCLIB_SHORTMONTHS[timetm->tm_mon],
       timetm->tm_mday,
       timetm->tm_hour,
       timetm->tm_min,
       timetm->tm_sec,
       u_long(theProcessId),
       u_long(theThreadId),
       OTCLIB_LOGLEVELMESSAGES[theLevel]
      );
#endif
#else
#if defined(OSE_SYS_WIN32)
      sprintf(
       buffer,
       "%s %2d %.2d:%.2d:%.2d [%lu] %s: ",
       OTCLIB_SHORTMONTHS[timetm->tm_mon],
       timetm->tm_mday,
       timetm->tm_hour,
       timetm->tm_min,
       timetm->tm_sec,
       u_long(theProcessId),
       OTCLIB_LOGLEVELMESSAGES[theLevel]
      );
#else
      snprintf(
       buffer,
       BUFSIZE,
       "%s %2d %.2d:%.2d:%.2d [%lu] %s: ",
       OTCLIB_SHORTMONTHS[timetm->tm_mon],
       timetm->tm_mday,
       timetm->tm_hour,
       timetm->tm_min,
       timetm->tm_sec,
       u_long(theProcessId),
       OTCLIB_LOGLEVELMESSAGES[theLevel]
      );
#endif
#endif
    }
    else
    {
#if defined(OSE_SYS_WIN32)
      sprintf(buffer,"%s: ",OTCLIB_LOGLEVELMESSAGES[theLevel]);
#else
      snprintf(buffer,BUFSIZE,"%s: ",OTCLIB_LOGLEVELMESSAGES[theLevel]);
#endif
    }

    // Log message to standard error.

    if (stream_ != stderr || useStderr_ != false)
    {
      char const* theMatch;
      char const* theText;

      theText = theMessage;

      theMatch = strchr(theText,EOL);
      while (theMatch != 0)
      {
	fwrite(buffer,1,strlen(buffer),stream_);
	fwrite((char*)theText,1,theMatch-theText+1,stream_);
	theText = theMatch + 1;
	theMatch = strchr(theText,EOL);
      }

      fwrite(buffer,1,strlen(buffer),stream_);
      fwrite((char*)theText,1,theMessage+theLength-theText,stream_);
      fwrite("\n",1,1,stream_);
      fflush(stream_);
    }

    // Log message to log file.

    if (logFile_ != 0)
    {
      char const* theFile = 0;
      char buf[MAXPATHLEN+1];
      char const* end = &buf[MAXPATHLEN+1];
      char const* p = logFile_;
      char* q = buf;
      while (*p)
      {
	if (*p != '%')
	{
	  *q++ = *p++;
	}
	else
	{
	  p++;
	  if (*p != EOS)
	  {
	    if (*p == 'p')
	    {
#if defined(OSE_SYS_WIN32)
	      sprintf(q,"%d",(int)getpid());
#else
	      snprintf(q,end-q,"%d",(int)getpid());
#endif
	      while (*q != EOS)
		q++;
	    }
	    else if (*p == 'h')
	    {
	      strcpy(q,&hostName_[0]);
	      while (*q != EOS)
		q++;
	    }
	    else if (*p == 'd')
	    {
#if defined(OSE_SYS_WIN32)
	      sprintf(q,"%02d",timetm->tm_mday);
#else
	      snprintf(q,end-q,"%02d",timetm->tm_mday);
#endif
	      while (*q != EOS)
		q++;
	    }
	    else if (*p == 'm')
	    {
#if defined(OSE_SYS_WIN32)
	      sprintf(q,"%02d",timetm->tm_mon+1);
#else
	      snprintf(q,end-q,"%02d",timetm->tm_mon+1);
#endif
	      while (*q != EOS)
		q++;
	    }
	    else if (*p == 'y')
	    {
#if defined(OSE_SYS_WIN32)
	      sprintf(q,"%02d",(timetm->tm_year%100));
#else
	      snprintf(q,end-q,"%02d",(timetm->tm_year%100));
#endif
	      while (*q != EOS)
		q++;
	    }
	    else if (*p == 'Y')
	    {
#if defined(OSE_SYS_WIN32)
	      sprintf(q,"%d",1900+timetm->tm_year);
#else
	      snprintf(q,end-q,"%d",1900+timetm->tm_year);
#endif
	      while (*q != EOS)
		q++;
	    }
	    else if (*p == '%')
	    {
	      *q++ = '%';
	    }

	    p++;
	  }
	}
      }
      *q = EOS;

      theFile = buf;

      FILE* fp = 0;

      if (appendLogFile_ == false &&
       (lastLogFile_[0] == EOS || strcmp(theFile,lastLogFile_) != 0))
      {
	strcpy(lastLogFile_,theFile);
	fp = fopen((char*)theFile,"w");
      }
      else
      {
	fp = fopen((char*)theFile,"a+");
      }

      if (fp != 0)
      {
	char const* theMatch;
	char const* theText;

	theText = theMessage;

	theMatch = strchr(theText,EOL);
	while (theMatch != 0)
	{
	  fwrite(buffer,1,strlen(buffer),fp);
	  fwrite((char*)theText,1,theMatch-theText+1,fp);
	  theText = theMatch + 1;
	  theMatch = strchr(theText,EOL);
	}

	fwrite(buffer,1,strlen(buffer),fp);
	fwrite((char*)theText,1,theMessage+theLength-theText,fp);
	fwrite("\n",1,1,fp);
	fclose(fp);
      }
    }
  }

  // Now send to all user defined loggers. Make sure we are not in a loop.
  // If we are in a loop then don't call user loggers again. An exception
  // could occur in one of these loggers so we need to ensure that mutex
  // will be unlocked.

  static int loop = 0;
  if (loop == 0)
  {
    OTC_MutexReaper<OTC_Mutex> xxxMutex;
    xxxMutex.grab(mutex_);

    loop++;
    OTC_Logger* theLogger = head_;
    while (theLogger)
    {
      theLogger->log(theChannel,theLevel,theMessage,theLength);
      theLogger = theLogger->next_;
    }
    loop--;

    xxxMutex.release();
  }


  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_Logger::stderrOutputEnabled()
{
  bool theState;

  mutex_.lock();

  initialise();

  theState = useStderr_;

  mutex_.unlock();

  return theState;
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::enableStderrOutput()
{
  mutex_.lock();

  initialise();

  useStderr_ = true;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::disableStderrOutput()
{
  mutex_.lock();

  initialise();

  useStderr_ = false;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_Logger::longFormatEnabled()
{
  bool theState;

  mutex_.lock();

  initialise();

  theState = longFormat_;

  mutex_.unlock();

  return theState;
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::enableLongFormat()
{
  mutex_.lock();

  initialise();

  longFormat_ = true;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::disableLongFormat()
{
  mutex_.lock();

  initialise();

  longFormat_ = false;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
char const* OTC_Logger::logFile()
{
  char const* theFile;

  mutex_.lock();

  initialise();

  theFile = logFile_;

  mutex_.unlock();

  return theFile;
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::setLogFile(char const* theFile)
{
  mutex_.lock();

  initialise();

  logFile_ = theFile;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_Logger::OTC_Logger()
 : next_(0), prev_(0)
{
  mutex_.lock();

  initialise();

  link();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::link()
{
  // Assumed that this function is called from inside lock.

  if (!head_)
  {
    head_ = this;
  }
  else
  {
    next_ = head_;
    next_->prev_ = this;
    head_ = this;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_Logger::unlink()
{
  // Assumed that this function is called from inside lock.

  if (head_ == this)
  {
    head_ = next_;
  }
  else
  {
    prev_->next_ = next_;
    if (next_)
      next_->prev_ = prev_;
  }
}

/* ------------------------------------------------------------------------- */
