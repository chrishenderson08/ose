/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcinetsrvr.cc
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
#pragma implementation "OTC/message/inetsrvr.hh"
#endif
#endif

#include <OTC/message/inetsrvr.hh>
#include <OTC/message/inetlstn.hh>
#include <OTC/message/episockt.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <string.h>

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

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_InetServer");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_InetServer::OTC_InetServer(
 char const* theInterface,
 OTC_EPListener* theListener,
 int theFd
)
  : OTC_EPServer(theInterface,"INET",theListener),
    fd_(theFd)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_InetServer::OTC_InetServer(...)");

  OTCLIB_ASSERT(theFd > 0);

  handler_ = OTC_EPISocket::create(theInterface,this);

  attach(handler_);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetServer::localTransport() const
{
  return localTransport_;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetServer::remoteTransport() const
{
  return remoteTransport_;
}

/* ------------------------------------------------------------------------- */
void OTC_InetServer::start()
{
  if (fd_ != INVALID_SOCKET)
  {
    OTC_InetAddress theAddress;

    if (getsockname(fd_,theAddress.addr(),
     &theAddress.addrlen()) != SOCKET_ERROR)
    {
      localTransport_ = theAddress.numericAddress();
    }

    if (getpeername(fd_,theAddress.addr(),
     &theAddress.addrlen()) != SOCKET_ERROR)
    {
      remoteTransport_ = theAddress.numericAddress();
    }
  }

  handler_->attach(fd_);
}

/* ------------------------------------------------------------------------- */
void OTC_InetServer::suspendInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetServer::suspendInput()");

  handler_->suspendInput();
}

/* ------------------------------------------------------------------------- */
void OTC_InetServer::resumeInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetServer::resumeInput()");

  handler_->resumeInput();
}

/* ------------------------------------------------------------------------- */
void OTC_InetServer::close()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetServer::close()");

  if (handler_->fd() != INVALID_SOCKET)
  {
    int theFd;
    theFd = handler_->detach();
    shutdown((void*)&theFd,0);
  }
}

/* ------------------------------------------------------------------------- */
void* OTC_InetServer::shutdown(void* theHandle, int theError)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetServer::shutdown(void*,int)");

  OTCLIB_TRACER(MODULE) << "error " << theError << endl;

  int theFd = INVALID_SOCKET;

  if (theHandle != 0)
    theFd = *(int*)theHandle;

  if (theFd != INVALID_SOCKET)
  {
#if defined(OSE_SYS_WIN32)
    closesocket(theFd);
#else
#if 0
#if defined(OSE_SYS_CYGWIN32)
    // If we don't do this, it doesn't
    // actually close connection down
    // when close() is called.
    ::shutdown(theFd,2);
#endif
#endif
    ::close(theFd);
#endif
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_InetServer::~OTC_InetServer()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_InetServer::~OTC_InetServer()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
