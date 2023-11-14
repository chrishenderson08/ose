/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     message/otcunixsrvr.cc
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
#pragma implementation "OTC/message/unixsrvr.hh"
#endif
#endif

#include <OTC/message/unixsrvr.hh>
#include <OTC/message/unixlstn.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#if defined(OSE_SYS_UNIX) && !defined(OSE_SYS_CYGWIN32)

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
    TAG = new OTC_TraceTag("OTC_UnixServer");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_UnixServer::OTC_UnixServer(
 char const* theInterface,
 OTC_EPListener* theListener,
 int theFd
)
  : OTC_EPServer(theInterface,"UNIX",theListener),
    fd_(theFd)
{
  OTCLIB_MARKBLOCK(MODULE,
   "OTC_UnixServer::OTC_UnixServer(...)");

  OTCLIB_ASSERT(theFd > 0);

  handler_ = OTC_EPISocket::create(theInterface,this);

  attach(handler_);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_UnixServer::localTransport() const
{
  return localTransport_;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_UnixServer::remoteTransport() const
{
  return remoteTransport_;
}

/* ------------------------------------------------------------------------- */
void OTC_UnixServer::start()
{
  if (fd_ != INVALID_SOCKET)
  {
    OTC_UnixAddress theAddress;

    if (getsockname(fd_,theAddress.addr(),
     &theAddress.addrlen()) != SOCKET_ERROR)
    {
      localTransport_ = theAddress.address();
    }

    if (getpeername(fd_,theAddress.addr(),
     &theAddress.addrlen()) != SOCKET_ERROR)
    {
      remoteTransport_ = theAddress.address();
    }
  }

  handler_->attach(fd_);
}

/* ------------------------------------------------------------------------- */
void OTC_UnixServer::suspendInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixServer::suspendInput()");

  handler_->suspendInput();
}

/* ------------------------------------------------------------------------- */
void OTC_UnixServer::resumeInput()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixServer::resumeInput()");

  handler_->resumeInput();
}

/* ------------------------------------------------------------------------- */
void OTC_UnixServer::close()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixServer::close()");

  if (handler_->fd() != INVALID_SOCKET)
  {
    int theFd;
    theFd = handler_->detach();
    shutdown((void*)&theFd,0);
  }
}

/* ------------------------------------------------------------------------- */
void* OTC_UnixServer::shutdown(void* theHandle, int)
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixServer::shutdown(void*,int)");

  int theFd = INVALID_SOCKET;

  if (theHandle != 0)
    theFd = *(int*)theHandle;

  ::close(theFd);

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_UnixServer::~OTC_UnixServer()
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_UnixServer::~OTC_UnixServer()");

  // Nothing to do.
}

/* ------------------------------------------------------------------------- */

#endif
