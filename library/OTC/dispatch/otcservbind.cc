/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcservbind.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/servbind.hh"
#endif
#endif

#include <OTC/dispatch/servbind.hh>

#include <OTC/text/symbol.hh>
#include <OTC/thread/mxreaper.hh>

#if defined(OSE_SYS_UNIX)
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

#include <OTC/stdlib/strstream.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_ServiceBinding::mutex_;

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding::OTC_ServiceBinding(
 char const* theServiceName,
 char const* theServiceAudience,
 char const* theReceiverName,
 char const* theProcessAddress,
 char const* theNetworkGroup
)
  : refCount_(1),
    serviceName_(OTC_Symbol(theServiceName)),
    serviceAudience_(OTC_Symbol(theServiceAudience)),
    receiverName_(OTC_Symbol(theReceiverName)),
    processAddress_(OTC_Symbol(theProcessAddress)),
    networkGroup_(OTC_Symbol(theNetworkGroup))
{
  OTC_String tmpString(serviceName_);

  agentAddress_ += processAddress_;
  if (!agentAddress_.isEmpty())
    agentAddress_ += '!';

  agentAddress_ += receiverName_;
  serviceAddress_ = agentAddress_;

  // if (!tmpString.isEmpty())
  // {
    serviceAddress_ += '!';
    serviceAddress_ += tmpString;
  // }

  if (processAddress_.isEmpty())
    serviceLocation_ = OTCLIB_SERVICE_LOCAL;
  else
    serviceLocation_ = OTCLIB_SERVICE_REMOTE;

  tmpString = receiverName_.after(2);

  u_long theHostId;
  u_long theProcessId;
  u_long theCount;

  istrstream ins1((char*)tmpString.string());

  ins1 >> hex >> theHostId;
  ins1.ignore(1);
  ins1 >> hex >> theProcessId;
  ins1.ignore(1);
  ins1 >> hex >> theCount;

  if (!ins1.fail())
  {
    char buf[512];

    int d1, d2, d3, d4;

    d1 = (theHostId & 0xFF000000) >> 24;
    d2 = (theHostId & 0x00FF0000) >> 16;
    d3 = (theHostId & 0x0000FF00) >> 8;
    d4 = (theHostId & 0x000000FF);

    sprintf(buf,"%d.%d.%d.%d",d1,d2,d3,d4);

    tmpString = buf;

#if 0
    hostent* h;
    u_long addr;

    addr = inet_addr(buf);

    if ((int)addr != -1)
    {
#if defined(_REENTRANT)
      struct hostent hostEntBuf;
      char stringBuf[2048];
      int result;

      h = gethostbyaddr_r((char*)&addr,sizeof(addr),AF_INET,
       &hostEntBuf,stringBuf,sizeof(stringBuf),&result);
#else
      h = gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
#endif

      if (h)
      {
	if (tmpString != h->h_name)
	{
	  tmpString = h->h_name;
	}
	else
	{
	  while (*h->h_aliases != 0)
	  {
	    if (tmpString != *h->h_aliases)
	    {
	      tmpString = *h->h_aliases;
	      break;
	    }

	    h->h_aliases++;
	  }
	}
      }
    }
#endif

    sprintf(buf,"%s:%lu",tmpString.string(),theProcessId);
    processIdentity_ = OTC_Symbol(buf);

    sprintf(buf,"%lu@%s:%lu",theCount,tmpString.string(),theProcessId);
    agentIdentity_ = buf;
  }
  else
  {
    istrstream ins2((char*)tmpString.string());

    OTC_String tmpHostName;
    tmpHostName.readToDelim(ins2,':');

    ins2 >> hex >> theProcessId;
    ins2.ignore(1);
    ins2 >> hex >> theCount;

    if (!ins2.fail())
    {
      char buf[512];

      sprintf(buf,"%s:%lu",tmpHostName.string(),theProcessId);
      processIdentity_ = OTC_Symbol(buf);

      sprintf(buf,"%lu@%s:%lu",theCount,tmpHostName.string(),theProcessId);
      agentIdentity_ = buf;
    }
    else
    {
      // Leave them empty if they are blank.
      // This is not good, but means something
      // is screwed up if we get here.
    }
  }
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding::~OTC_ServiceBinding()
{
// Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_ServiceBinding* OTC_ServiceBinding::clone()
{
  otclib_atomic_inc(refCount_,mutex_);

  return this;
}

/* ------------------------------------------------------------------------- */
void OTC_ServiceBinding::destroy()
{
  if (otclib_atomic_dec(refCount_,mutex_) < 1)
    delete this;
}

/* ------------------------------------------------------------------------- */
