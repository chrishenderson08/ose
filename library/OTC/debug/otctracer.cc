/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/otctracer.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/debug/tracer.hh"
#endif
#endif

#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/thread/mxreaper.hh>

#include <OTC/stdlib/fstream.h>

#include <string.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_Mutex OTC_Tracer::mutex_;
int OTC_Tracer::globInit_ = 0;
bool OTC_Tracer::globTraceAll_ = false;
int OTC_Tracer::globLevel_ = 0;
bool OTC_Tracer::globInfo_ = false;
bool OTC_Tracer::globIndent_ = false;
bool OTC_Tracer::globLogTrace_ = false;
ostream* OTC_Tracer::globStream_ = 0;
char* OTC_Tracer::globLogChannel_ = 0;
OTC_LogStream* OTC_Tracer::globLogStream_ = 0;
OTC_TraceStream* OTC_Tracer::globTraceStream_ = 0;
OTC_FuncStream* OTC_Tracer::globAnonymousStream_ = 0;
OTC_Tracer* OTC_Tracer::globLast_ = 0;

/* ------------------------------------------------------------------------- */
OTC_Tracer::OTC_Tracer(
 char const* thePrototype,
 char const* theFile,
 u_int theLine,
 int theEnable
)
  : prototype_(thePrototype), enable_(true), stream_(notify), prev_(0)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

#if !defined(OSE_WITH_THREADS)
  prev_ = globLast_;
  globLast_ = this;
#endif

  if (theEnable < 0 || theEnable > globLevel_)
    enable_ = false;

  if (globTraceAll_ || enable_)
  {
    if (prototype_ != 0 && globInfo_ && theFile != 0)
    {
      *globTraceStream_ << "@location - \"" <<
       theFile << "\", line " << theLine << endl;
    }

    if (prototype_ != 0)
      *globTraceStream_ << "@enter - " << prototype() << endl;
  }

  *globTraceStream_ << OTCLIB_TRACEIN;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_Tracer::OTC_Tracer(char const* thePrototype)
  : prototype_(thePrototype), enable_(true), stream_(notify), prev_(0)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

#if !defined(OSE_WITH_THREADS)
  prev_ = globLast_;
  globLast_ = this;
#endif

  if (globTraceAll_ || enable_)
  {
    if (prototype_ != 0)
      *globTraceStream_ << "@enter - " << prototype() << endl;
  }

  *globTraceStream_ << OTCLIB_TRACEIN;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::initialise()
{
  // This function does not lock the mutex as it is expected that the
  // calling function has locked it.

  if (globInit_ == 0)
  {
    globInit_ = 1;

    char const* s;

    globTraceStream_ = new OTC_TraceStream;
    OTCLIB_ASSERT_M(globTraceStream_ != 0);

    globLogStream_ = new OTC_LogStream;
    OTCLIB_ASSERT_M(globLogStream_ != 0);

    globLogStream_->setLevel(OTCLIB_LOG_DEBUG);

    if (globLogChannel_ == 0)
    {
      s = getenv("OTCLIB_TRACECHANNEL");

      if (s == 0)
	s = "";

      globLogChannel_ = new char[strlen(s)+1];
      OTCLIB_ASSERT_M(globLogChannel_ != 0);

      strcpy(globLogChannel_,s);
    }
    
    globLogStream_->setTarget(globLogChannel_);

    globAnonymousStream_ = new OTC_FuncStream(notify);
    OTCLIB_ASSERT_M(globAnonymousStream_ != 0);

    globIndent_ = true;
    s = getenv("OTCLIB_NOTRACEINDENT");
    if (s != 0)
      globIndent_ = false;

    if (globLevel_ == 0)
    {
      s = getenv("OTCLIB_TRACELEVEL");
      if (s == 0)
	globLevel_ = 0;
      else
	globLevel_ = atoi(s);
    }

    s = getenv("OTCLIB_TRACEINFO");
    if (s != 0)
      globInfo_ = true;
    else
      globInfo_ = false;

    s = getenv("OTCLIB_GLOBALTRACE");
    if (s != 0)
      globTraceAll_ = true;
    else
      globTraceAll_ = false;
  }
}

/* ------------------------------------------------------------------------- */
OTC_Tracer::~OTC_Tracer()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  *globTraceStream_ << OTCLIB_TRACEOUT;

#if !defined(OSE_WITH_THREADS)
  globLast_ = prev_;
#endif

  if (globTraceAll_ || enable_)
  {
    if (prototype_ != 0)
      *globTraceStream_ << "@exit - " << prototype() << endl;
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_Tracer::enabled(int theLevel)
{
  bool theState;
  theState = true;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  OTC_Tracer::initialise();

  if (((theLevel < 0) || ((theLevel >= 0) && (theLevel > globLevel_)))
   && (globTraceAll_ == false))
  {
    theState = false;
  }

  xxxMutex.release();

  mutex_.unlock();

  return theState;
}

/* ------------------------------------------------------------------------- */
bool OTC_Tracer::globalTrace()
{
  bool theState;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  theState = globTraceAll_;

  xxxMutex.release();

  mutex_.unlock();

  return theState;
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::enableGlobalTrace()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  globTraceAll_ = true;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::disableGlobalTrace()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  globTraceAll_ = false;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
int OTC_Tracer::level()
{
  int theLevel;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  theLevel = globLevel_;

  xxxMutex.release();

  mutex_.unlock();

  return theLevel;
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::setLevel(int theLevel)
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  globLevel_ = theLevel;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_Tracer::traceIndent()
{
  bool theState;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  theState = globIndent_;

  xxxMutex.release();

  mutex_.unlock();

  return theState;
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::enableTraceIndent()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  globIndent_ = true;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::disableTraceIndent()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  globIndent_ = false;

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OTC_Tracer const* OTC_Tracer::last()
{
#if !defined(OSE_WITH_THREADS)
  return globLast_;
#else
  return 0;
#endif
}

/* ------------------------------------------------------------------------- */
ostream* OTC_Tracer::stream()
{
  ostream* theStream;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  char const* s;

  if (globStream_ == 0)
  {
    s = getenv("OTCLIB_LOGTRACE");
    if (s != 0)
      globLogTrace_ = true;
  }

  if (globStream_ == 0)
  {
    s = getenv("OTCLIB_TRACEFILE");
    if (s != 0)
    {
      ofstream* outs = new ofstream(s,ios::out|ios::trunc);
      OTCLIB_ASSERT_M(outs != 0);
      if (outs->fail())
	delete outs;
      else
	globStream_ = outs;
    }
  }

  if (globStream_ == 0)
    globStream_ = &clog;

  if (globLogTrace_ == false)
    theStream = globStream_;
  else
    theStream = globLogStream_;

  xxxMutex.release();

  mutex_.unlock();

  return theStream;
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::setStream(ostream* theStream)
{
  mutex_.lock();

  globStream_ = theStream;

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
bool OTC_Tracer::logTrace()
{
  bool theState;

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  theState = globLogTrace_;

  xxxMutex.release();

  mutex_.unlock();

  return theState;
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::enableLogTrace()
{
  mutex_.lock();

  initialise();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  if (globLogTrace_ == false)
  {
    globLogTrace_ = true;
    if (globStream_ != 0)
      globStream_->flush();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::disableLogTrace()
{
  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  initialise();

  if (globLogTrace_ != false)
  {
    globLogTrace_ = false;
    if (globLogStream_ != 0)
      globLogStream_->flush();
  }

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::setLogChannel(char const* theChannel)
{
  if (theChannel == 0)
    theChannel = "";

  mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(mutex_);

  char* theOldChannel = 0;

  if (globLogChannel_ != 0)
    theOldChannel = globLogChannel_;

  globLogChannel_ = new char[strlen(theChannel)+1];
  OTCLIB_ASSERT_M(globLogChannel_ != 0);

  strcpy(globLogChannel_,theChannel);

  if (globLogStream_ != 0)
    globLogStream_->setChannel(globLogChannel_);

  if (theOldChannel != 0)
    delete [] theOldChannel;

  initialise();

  xxxMutex.release();

  mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
OSE_EXPORT ostream& otclib_tracer()
{
  ostream* theStream;

  OTC_Tracer::mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(OTC_Tracer::mutex_);

  OTC_Tracer::initialise();

  theStream = OTC_Tracer::globAnonymousStream_;

  xxxMutex.release();

  OTC_Tracer::mutex_.unlock();

  return *theStream;
}

/* ------------------------------------------------------------------------- */
void OTC_Tracer::notify(char const* theBuffer, size_t theSize)
{
  OTC_Tracer::mutex_.lock();

  OTC_MutexReaper<OTC_Mutex> xxxMutex;
  xxxMutex.grab(OTC_Tracer::mutex_);

  OTC_Tracer::initialise();

  globTraceStream_->write(theBuffer,theSize);

  globTraceStream_->flush();

  xxxMutex.release();

  OTC_Tracer::mutex_.unlock();
}

/* ------------------------------------------------------------------------- */
