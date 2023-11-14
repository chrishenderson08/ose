/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcinetclnt.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/inetclnt.hh"
#endif
#endif

#include <OTC/message/inetclnt.hh>

#include <OTC/message/connect.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(OSE_SYS_UNIX)
#include <sys/uio.h>
#include <sys/socket.h>
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

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_InetClient");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_InetClient::~OTC_InetClient()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::~OTC_InetClient()");

  stop(0);
}

/* ------------------------------------------------------------------------- */
OTC_InetClient::OTC_InetClient(
 char const* theInterface,
 char const* theHostname,
 int thePort,
 int theRetryDelay
)
  : OTC_EPClient(theInterface,"INET"),
    handler_(0),
    hostname_(theHostname),
    port_(thePort),
    address_(theHostname,thePort),
    retryDelay_(theRetryDelay),
    retryTimer_(0),
    stopDelay_(0),
    stopTimer_(0),
    blockedFd_(INVALID_SOCKET)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_InetClient::OTC_InetClient(char const*, int,int)");

  if (theInterface != 0)
    attach(OTC_EPISocket::create(theInterface,this));
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetClient::localTransport() const
{
  return localTransport_;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetClient::remoteTransport() const
{
  return remoteTransport_;
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::start()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::start()");

  if (handler_->fd() != INVALID_SOCKET)
    return;

  if (retryTimer_ != 0)
    return;

  int theFd = INVALID_SOCKET;

  void* theFdP = 0;
  theFdP = shutdown(0,0);

  if (theFdP != 0)
    theFd = *(int*)theFdP;

  if (theFd != INVALID_SOCKET)
  {
    OTC_InetAddress theAddress;

    if (getsockname(theFd,theAddress.addr(),
     &theAddress.addrlen()) != SOCKET_ERROR)
    {
      localTransport_ = theAddress.numericAddress();
    }

    if (getpeername(theFd,theAddress.addr(),
     &theAddress.addrlen()) != SOCKET_ERROR)
    {
      remoteTransport_ = theAddress.numericAddress();
    }

    handler_->attach(theFd);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::stop(int theDelay)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::stop(int)");

  if (theDelay == 0)
  {
    if (handler_->fd() != INVALID_SOCKET)
    {
      int theFd = INVALID_SOCKET;
      theFd = handler_->detach();
#if defined(OSE_SYS_WIN32)
      closesocket(theFd);
#else
      ::close(theFd);
#endif
    }

    if (blockedFd_ != INVALID_SOCKET)
    {
      OTCEV_IOEvent::unsubscribe(id(),blockedFd_);

#if defined(OSE_SYS_WIN32)
      closesocket(blockedFd_);
#else
      ::close(blockedFd_);
#endif
      blockedFd_ = INVALID_SOCKET;
    }

    if (retryTimer_ != 0)
    {
      OTCEV_Timeout::cancel(retryTimer_);
      retryTimer_ = 0;
    }

    if (stopTimer_ != 0)
    {
      OTCEV_Timeout::cancel(stopTimer_);

      stopTimer_ = 0;
      stopDelay_ = 0;
    }
  }
  else
  {
    if (stopTimer_ == 0 && stopDelay_ == 0)
    {
      OTCEV_Action::schedule(handler_->id(),OTCLIB_STANDARD_JOB,"CLOSE");

      if (theDelay > 0)
	stopTimer_ = OTCEV_Timeout::start(id(),theDelay,"STOP");

      stopDelay_ = theDelay;
    }
  }
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::suspendInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::suspendInput()");

  handler_->suspendInput();
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::resumeInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::resumeInput()");

  handler_->resumeInput();
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::restart(int theDelay)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::restart(int)");

  stop(0);

  if (theDelay == 0 || (theDelay == -1 && retryDelay_ <= 0))
  {
    start();
  }
  else
  {
    if (theDelay == -1)
      theDelay = retryDelay_;

    retryTimer_ = OTCEV_Timeout::start(id(),theDelay,"RETRY");

    OTCLIB_TRACER(MODULE) << "timer = " << retryTimer_ << endl;
  }
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::attach(OTC_EPISocket* theInterface)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::attach()");

  OTCLIB_ENSURE_FN((handler_ == 0),
   "OTC_InetClient::attach()",
   "endpoint interface already specified");

  OTCLIB_ENSURE_FN((theInterface != 0),
   "OTC_InetClient::attach()",
   "invalid endpoint interface specified");

  handler_ = theInterface;

  OTC_EndPoint::attach(theInterface);
}

/* ------------------------------------------------------------------------- */
void* OTC_InetClient::shutdown(void* theHandle, int theError)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::shutdown(void*,int)");

  OTCLIB_TRACER(MODULE) << "error = " << theError << endl;

  int theFd = INVALID_SOCKET;

  if (theHandle != 0)
    theFd = *(int*)theHandle;

  if (theFd != INVALID_SOCKET)
  {
    OTCLIB_TRACER(MODULE) << "close = " << theFd << endl;

#if defined(OSE_SYS_WIN32)
    closesocket(theFd);
#else
    ::close(theFd);
#endif

    if (retryDelay_ < 0)
      return 0;

    theFd = INVALID_SOCKET;

    if (retryDelay_ >= 0)
    {
      retryTimer_ = OTCEV_Timeout::start(id(),retryDelay_,"RETRY");

      OTCLIB_TRACER(MODULE) << "timer = " << retryTimer_ << endl;
    }

    return 0;
  }

  if (blockedFd_ != INVALID_SOCKET)
  {
    OTCLIB_TRACER(MODULE) << "retry = " << blockedFd_ << endl;

    theFd = blockedFd_;
  }
  else
  {
    theFd = socket(AF_INET,SOCK_STREAM,0);

    if (theFd == INVALID_SOCKET)
    {
      if (retryDelay_ >= 0)
      {
	retryTimer_ = OTCEV_Timeout::start(id(),retryDelay_,"RETRY");

	OTCLIB_TRACER(MODULE) << "timer = " << retryTimer_ << endl;
      }

      connectionFailed();

      return 0;
    }

    OTCLIB_TRACER(MODULE) << "socket = " << theFd << endl;

    // Set file descriptor to be non blocking.

#if defined(OSE_SYS_WIN32)
    int result;
    u_long flags = 1;
    result = ioctlsocket(theFd,FIONBIO,&flags);

    OTCLIB_TRACER(MODULE) << "ioctlsocket() = " << result << endl;
#else
#if defined(O_NONBLOCK)
    int flags = 0;
    flags = fcntl(theFd,F_GETFL,0);
    fcntl(theFd,F_SETFL,flags|O_NONBLOCK);
#else
#if defined(O_NDELAY)
    int flags = 0;
    flags = fcntl(theFd,F_GETFL,0);
    fcntl(theFd,F_SETFL,flags|O_NDELAY);
#endif
#endif
#endif

    // Ensure file descriptor is closed on exec.

#if defined(FD_CLOEXEC)
    fcntl(theFd,F_SETFD,FD_CLOEXEC);
#endif
  }

  int theResult;

  theResult = connect(theFd,address_.addr(),address_.addrlen());

  OTCLIB_TRACER(MODULE) << "connect = " << theResult << endl;

  if (theResult == SOCKET_ERROR)
  {
    int theLastError = socket_errno();

    OTCLIB_TRACER(MODULE) << "errno = " << theLastError << endl;

    switch (theLastError)
    {
#if defined(WSAEISCONN)
      case WSAEISCONN:
#else
      case EISCONN:
#endif
      {
	// Connect has completed.

	break;
      }

#if defined(WSAEINVAL)
      case WSAEINVAL:
#endif
#if defined(WSAEWOULDBLOCK)
      case WSAEWOULDBLOCK:
#endif
#if defined(WSAEINPROGRESS)
      case WSAEINPROGRESS:
#else
      case EINPROGRESS:
#endif
      {
	// Connect is in progress. This should
	// only be returned on first attempt to
	// do the connect.

	OTCLIB_TRACER(MODULE) << "subscribe = " << theFd << endl;

	blockedFd_ = theFd;

	OTCEV_IOEvent::subscribe(id(),
	 blockedFd_,OTCLIB_POLLOUT|OTCLIB_POLLPRI);

	return 0;
      }

#if defined(WSAEALREADY)
      case WSAEALREADY:
#else
      case EALREADY:
#endif
      {
	// Connect hasn't completed even though
	// file descriptor event seems to suggest
	// that it has finished. This should only
	// be returned on subsequent attempts to
	// do the connect.

	return 0;
      }

      default:
      {
	if (blockedFd_ != INVALID_SOCKET)
	{
	  OTCLIB_TRACER(MODULE) << "unsubscribe = " << blockedFd_ << endl;

	  OTCEV_IOEvent::unsubscribe(id(),blockedFd_);
	  blockedFd_ = INVALID_SOCKET;
	}

	OTCLIB_TRACER(MODULE) << "close = " << theFd << endl;

#if defined(OSE_SYS_WIN32)
	closesocket(theFd);
#else
	::close(theFd);
#endif

	if (retryDelay_ >= 0)
	{
	  retryTimer_ = OTCEV_Timeout::start(id(),retryDelay_,"RETRY");

	  OTCLIB_TRACER(MODULE) << "timer = " << retryTimer_ << endl;
	}

	connectionFailed();

	return 0;
      }
    }
  }

  if (blockedFd_ != INVALID_SOCKET)
  {
    OTCLIB_TRACER(MODULE) << "unsubscribe = " << blockedFd_ << endl;

    OTCEV_IOEvent::unsubscribe(id(),blockedFd_);
    blockedFd_ = INVALID_SOCKET;
  }

  restartFd_ = theFd;

  return (void*)&restartFd_;
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::handle(OTC_Event* theEvent)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetClient::handle(OTC_Event*)");

  if (theEvent == 0)
    return;

  OTCLIB_TRACER(MODULE) << "event = " << *theEvent << endl;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeout;
    theTimeout = (OTCEV_Timeout*)theEvent;

    if (theTimeout->description() == "RETRY")
    {
      if (theTimeout->timer() == retryTimer_)
      {
	retryTimer_ = 0;

	// Rebind the address in case it is a FQDN and
	// the IP address it maps to has changed.

	OTC_InetAddress theAddress(hostname_,port_);
	address_ = theAddress;

	start();
      }
    }
    else if (theTimeout->description() == "STOP")
    {
      if (theTimeout->timer() == stopTimer_)
      {
	stopDelay_ = 0;
	stopTimer_ = 0;

	stop(0);
      }
    }
  }
  else if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCEV_IOEvent* theIOEvent;
    theIOEvent = (OTCEV_IOEvent*)theEvent;

    if (retryTimer_ != 0)
    {
      OTCEV_Timeout::cancel(retryTimer_);
      retryTimer_ = 0;
    }

    if (theIOEvent->fd() == blockedFd_)
    {
      if (theIOEvent->events() & OTCLIB_POLLPRI)
      {
        stop(0);

	if (retryDelay_ >= 0)
	  retryTimer_ = OTCEV_Timeout::start(id(),retryDelay_,"RETRY");
      }
      else
	start();
    }
  }

  theEvent->destroy();
}

/* ------------------------------------------------------------------------- */
void OTC_InetClient::connectionFailed()
{
  OTCEV_Connection* theEvent;
  theEvent = new OTCEV_Connection(localAddress(),"",interface(),protocol(),
   localTransport(),remoteTransport(),OTCLIB_CONNECTION_INACTIVE);
  OTCLIB_ASSERT_M(theEvent != 0);

  notifyObservers(theEvent,OTCLIB_STANDARD_JOB);
}

/* ------------------------------------------------------------------------- */
