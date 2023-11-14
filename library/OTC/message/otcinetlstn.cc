/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcinetlstn.cc
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
#pragma implementation "OTC/message/inetlstn.hh"
#endif
#endif

#include <OTC/message/inetlstn.hh>

#include <OTC/message/inetaddr.hh>
#include <OTC/message/connect.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/logstrm.hh>

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

#if defined(OSE_SYS_UNIX)
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#if defined(shutdown)
#undef shutdown
#endif
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#if defined(OSE_SYS_WIN32)
inline int socket_errno() { return WSAGetLastError(); }
#else
inline int socket_errno() { return errno; }
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
    TAG = new OTC_TraceTag("OTC_InetListener");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG() 

/* ------------------------------------------------------------------------- */
OTC_InetListener::~OTC_InetListener()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::~OTC_InetListener()");

  stop();
}

/* ------------------------------------------------------------------------- */
OTC_InetListener::OTC_InetListener(char const* theInterface, int thePort)
  : interface_(theInterface), address_(thePort), fd_(INVALID_SOCKET)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::OTC_InetListener()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_InetListener::OTC_InetListener(
 char const* theInterface,
 char const* theHostname,
 int thePort
)
  : interface_(theInterface),
    address_(theHostname,thePort),
    fd_(INVALID_SOCKET)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::OTC_InetListener()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
void OTC_InetListener::start()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::start()");

  if (fd_ != INVALID_SOCKET)
    return;

  int theFd;

  OTCLIB_TRACER(MODULE) << "port = " << address_.port() << endl;

  theFd = socket(AF_INET,SOCK_STREAM,0);

  if (theFd == INVALID_SOCKET)
  {
    int result = socket_errno();

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_InetListener" << EOL;
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
    int result = socket_errno();

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_InetListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: can't set socket option" << EOL;
    theLogger << "SystemFunction: setsockopt()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << flush;

#if defined(OSE_SYS_WIN32)
    closesocket(theFd);
#else
    ::close(theFd);
#endif
    return;
  }

  if (bind(theFd,address_.addr(),address_.addrlen()) == SOCKET_ERROR)
  {
    int result = socket_errno();

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_InetListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: can't bind socket address" << EOL;
    theLogger << "SystemFunction: bind()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << "Port: " << address_.port() << EOL;
    theLogger << flush;

#if defined(OSE_SYS_WIN32)
    closesocket(theFd);
#else
    ::close(theFd);
#endif
    return;
  }

  if (address_.port() == 0)
  {
    if (getsockname(theFd,address_.addr(),
     &address_.addrlen()) == SOCKET_ERROR)
    {
      int result = socket_errno();

      OTC_LogStream theLogger;
      theLogger.setLevel(OTCLIB_LOG_WARNING);

      theLogger << "Operation failed" << EOL;
      theLogger << "Class: OTC_InetListener" << EOL;
      theLogger << "Method: start()" << EOL;
      theLogger << "Description: can't get socket name" << EOL;
      theLogger << "SystemFunction: getsockname()" << EOL;
      theLogger << "SystemError: " << result << EOL;
      theLogger << "Port: " << address_.port() << EOL;
      theLogger << flush;

#if defined(OSE_SYS_WIN32)
      closesocket(theFd);
#else
      ::close(theFd);
#endif
      return;
    }
  }

  if (listen(theFd,SOMAXCONN) == SOCKET_ERROR)
  {
    int result = socket_errno();

    OTC_LogStream theLogger;
    theLogger.setLevel(OTCLIB_LOG_WARNING);

    theLogger << "Operation failed" << EOL;
    theLogger << "Class: OTC_InetListener" << EOL;
    theLogger << "Method: start()" << EOL;
    theLogger << "Description: listen on socket failed" << EOL;
    theLogger << "SystemFunction: listen()" << EOL;
    theLogger << "SystemError: " << result << EOL;
    theLogger << "Port: " << address_.port() << EOL;
    theLogger << flush;

#if defined(OSE_SYS_WIN32)
    closesocket(theFd);
#else
    ::close(theFd);
#endif
    return;
  }

  // Ensure file descriptor is closed on exec.

#if defined(FD_CLOEXEC)
  fcntl(theFd,F_SETFD,FD_CLOEXEC);
#endif

  attach(theFd);
}

/* ------------------------------------------------------------------------- */
void OTC_InetListener::stop()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::stop()");

  int theFd;

  theFd = detach();
  if (theFd != INVALID_SOCKET)
  {
#if defined(OSE_SYS_WIN32)
    closesocket(theFd);
#else
    ::close(theFd);
#endif
  }
}

/* ------------------------------------------------------------------------- */
void OTC_InetListener::suspendListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::suspendListening()");

  if (fd_ != INVALID_SOCKET)
    OTCEV_IOEvent::unsubscribe(id(),fd_);
}

/* ------------------------------------------------------------------------- */
void OTC_InetListener::resumeListening()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::resumeListening()");

  if (fd_ != INVALID_SOCKET)
    OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);
}

/* ------------------------------------------------------------------------- */
void OTC_InetListener::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::handle(OTC_Event*)");

  if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCEV_IOEvent* theIOEvent;
    theIOEvent = (OTCEV_IOEvent*)theEvent;
 
    if (theIOEvent->fd() == fd_ && theIOEvent->events() & OTCLIB_POLLIN)
    {
      int theResult = 0;

      OTC_InetAddress theAddress;

      theResult = ::accept(fd_,theAddress,&theAddress.addrlen());

      if (theResult != INVALID_SOCKET)
      {
	// Authorise connection request.

	if (authorise(theAddress.numericHostname()))
	{
	  // Set file descriptor to be non blocking.

#if defined(OSE_SYS_WIN32)
	  int result;
	  u_long flags = 1;
	  result = ioctlsocket(theResult,FIONBIO,&flags);

	  OTCLIB_TRACER(MODULE) << "ioctlsocket() = " << result << endl;
#else
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
	else
	{
#if defined(OSE_SYS_WIN32)
	  closesocket(theResult);
#else
	  ::close(theResult);
#endif
	}
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
void OTC_InetListener::attach(int theFd)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::attach(int)");

  OTCLIB_ENSURE_FN((fd_ == INVALID_SOCKET),
   "OTC_InetListener::attach(int)",
   "already attached to file descriptor");

  fd_ = theFd;

  OTCEV_IOEvent::subscribe(id(),fd_,OTCLIB_POLLIN);

  // Set file descriptor to be non blocking.
  // If we don't, might block on accept() if
  // connection was aborted. Non blocking mode
  // isn't inherited across accept() so must
  // apply it later.

#if defined(OSE_SYS_WIN32)
  int result;
  u_long flags = 1;
  result = ioctlsocket(fd_,FIONBIO,&flags);

  OTCLIB_TRACER(MODULE) << "ioctlsocket() = " << result << endl;
#else
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
#endif
}

/* ------------------------------------------------------------------------- */
int OTC_InetListener::detach()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetListener::detach()");

  int theOldFd = fd_;

  if (fd_ != INVALID_SOCKET)
  {
    OTCEV_IOEvent::unsubscribe(id(),fd_);
    fd_ = INVALID_SOCKET;
  }

  return theOldFd;
}

/* ------------------------------------------------------------------------- */
OTC_EPServer* OTC_InetListener::create(
 OTC_EPListener* theListener,
 int theFd
)
{
  OTC_InetServer* theServer;
  theServer = new OTC_InetServer(interface_,theListener,theFd);
  OTCLIB_ASSERT_M(theServer != 0);

  return theServer;
}

/* ------------------------------------------------------------------------- */
bool OTC_InetListener::authorise(OTC_String const&)
{
  return true;
}

/* ------------------------------------------------------------------------- */
