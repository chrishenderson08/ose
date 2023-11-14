/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcunixlstn.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/unixlstn.hh"
#endif
#endif

#include <OTC/message/unixlstn.hh>

#include <OTC/message/unixaddr.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

#if defined(OSE_SYS_UNIX) && !defined(OSE_SYS_CYGWIN32)

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

#if defined(HAVE_MEMORY_H)
#include <memory.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>

#if defined(shutdown)
#undef shutdown
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#ifndef SOMAXCONN
#define SOMAXCONN 5
#endif

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()  
{ 
  static OTC_TraceTag* TAG = 0; 
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_UnixListener");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG() 

/* ------------------------------------------------------------------------- */
OTC_UnixListener::~OTC_UnixListener()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::~OTC_UnixListener()");

  stop();
}

/* ------------------------------------------------------------------------- */
OTC_UnixListener::OTC_UnixListener(
 char const* theInterface,
 char const* thePathName
)
  : interface_(theInterface), address_(thePathName), fd_(INVALID_SOCKET)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::OTC_UnixListener(char const*)");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_UnixListener::start()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::start()");

  if (fd_ != INVALID_SOCKET)
    return;

  int theFd;

  OTCLIB_TRACER(MODULE) << "pathname = " << address_.pathname() << endl;

  theFd = socket(AF_UNIX,SOCK_STREAM,0);

  if (theFd == INVALID_SOCKET)
  {
    int result = errno;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_UnixListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: can't create socket" << EOL;
    theLogger << "SystemFunction: socket()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << flush;

    return;
  }

  OTCLIB_TRACER(MODULE) << "socket = " << theFd << endl;

  int const YES = 1;
  if (setsockopt(theFd,SOL_SOCKET,SO_REUSEADDR,
   (char const*)&YES,sizeof(YES)) == SOCKET_ERROR)
  {
    int result = errno;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_UnixListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: can't set socket option" << EOL;
    theLogger << "SystemFunction: setsockopt()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << flush;

    ::close(theFd);

    return;
  }

  if (bind(theFd,address_.addr(),address_.addrlen()) == SOCKET_ERROR)
  {
    int result = errno;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_UnixListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: can't bind socket address" << EOL;
    theLogger << "SystemFunction: bind()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << "PathName: " << address_.pathname() << EOL;
    theLogger << flush;

    ::close(theFd);

    return;
  }

  if (listen(theFd,SOMAXCONN) == SOCKET_ERROR)
  {
    int result = errno;

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_UnixListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: listen on socket failed" << EOL;
    theLogger << "SystemFunction: listen()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << "PathName: " << address_.pathname() << EOL;
    theLogger << flush;

    ::close(theFd);
    ::unlink(address_.pathname());

    return;
  }

  // Ensure file descriptor is closed on exec.

#if defined(FD_CLOEXEC)
  fcntl(theFd,F_SETFD,FD_CLOEXEC);
#endif

  attach(theFd);
}

/* ------------------------------------------------------------------------- */
void OTC_UnixListener::stop()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::stop()");

  int theFd;

  theFd = detach();
  if (theFd != INVALID_SOCKET)
  {
    ::close(theFd);
    ::unlink(address_.pathname());
  }
}

/* ------------------------------------------------------------------------- */
void OTC_UnixListener::suspendListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::suspendListening()");

  if (fd_ != INVALID_SOCKET)
    OTCEV_IOEvent::unsubscribe(id(),fd_);
}

/* ------------------------------------------------------------------------- */
void OTC_UnixListener::resumeListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::resumeListening()");

  if (fd_ != INVALID_SOCKET)
    OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);
}

/* ------------------------------------------------------------------------- */
void OTC_UnixListener::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::handle(OTC_Event*)");

  if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCEV_IOEvent* theIOEvent;
    theIOEvent = (OTCEV_IOEvent*)theEvent;
 
    if (theIOEvent->fd() == fd_ && theIOEvent->events() & OTCLIB_POLLIN)
    {
      int theResult = 0;

      OTC_UnixAddress theAddress;

      theResult = ::accept(fd_,theAddress,&theAddress.addrlen());

      if (theResult != INVALID_SOCKET)
      {
	// Set file descriptor to be non blocking.

#if defined(O_NONBLOCK)
	int flags = 0;
	flags = fcntl(theResult,F_GETFL,0);
	fcntl(theResult,F_SETFL,flags|O_NONBLOCK);
#else
#if defined(O_NDELAY)
	int flags = 0;
	flags = fcntl(theResult,F_GETFL,0);
	fcntl(theResult,F_SETFL,flags|O_NDELAY);
#endif
#endif

	// Ensure file descriptor is closed on exec.

#if defined(FD_CLOEXEC)
	fcntl(theResult,F_SETFD,FD_CLOEXEC);
#endif

	OTC_EPServer* theServer;
	theServer = create(this,theResult);
	theServer->addObserver(id());
	theServer->start();
      }
    }
  }
  else
  {
    OTC_EPListener::handle(theEvent->clone());
  }
 
  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_UnixListener::attach(int theFd)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::attach(int)");

  OTCLIB_ENSURE_FN((fd_ == INVALID_SOCKET),
   "OTC_UnixListener::attach(int)",
   "already attached to file descriptor");

  fd_ = theFd;

  OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);

  // Set file descriptor to be non blocking.
  // If we don't, might block on accept() if
  // connection was aborted. Non blocking mode
  // isn't inherited across accept() so must
  // apply it later.

#if defined(O_NONBLOCK)
  int flags = 0;
  flags = fcntl(fd_,F_GETFL,0);
  fcntl(fd_,F_SETFL,flags|O_NONBLOCK);
#else
#if defined(O_NDELAY)
  int flags = 0;
  flags = fcntl(fd_,F_GETFL,0);
  fcntl(fd_,F_SETFL,flags|O_NDELAY);
#endif
#endif
}

/* ------------------------------------------------------------------------- */
int OTC_UnixListener::detach()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixListener::detach()");

  int theOldFd = fd_;

  if (fd_ != INVALID_SOCKET)
  {
    OTCEV_IOEvent::unsubscribe(id(),fd_);
    fd_ = INVALID_SOCKET;
  }

  return theOldFd;
}

/* ------------------------------------------------------------------------- */
OTC_EPServer* OTC_UnixListener::create(OTC_EPListener* theListener, int theFd)
{
  OTC_UnixServer* theServer;
  theServer = new OTC_UnixServer(interface_,theListener,theFd);
  OTCLIB_ASSERT_M(theServer != 0);

  return theServer;
}

/* ------------------------------------------------------------------------- */

#endif
