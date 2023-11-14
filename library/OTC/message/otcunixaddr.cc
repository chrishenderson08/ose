/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     message/otcunixaddr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/unixaddr.hh"
#endif
#endif
 
#include <OTC/message/unixaddr.hh>

#if defined(OSE_SYS_UNIX) && !defined(OSE_SYS_CYGWIN32)

#include <string.h>

/* ------------------------------------------------------------------------- */
char const* const OTC_UnixAddress::ANY = "";

/* ------------------------------------------------------------------------- */
OTC_UnixAddress::OTC_UnixAddress()
{
  resetAddress();
}

/* ------------------------------------------------------------------------- */
OTC_UnixAddress::OTC_UnixAddress(char const* pathname)
{
  OTCLIB_ASSERT(pathname != 0);
  initAddress(pathname);
}

/* ------------------------------------------------------------------------- */
OTC_UnixAddress::OTC_UnixAddress(OTC_UnixAddress const& theAddress){
  length_ = theAddress.length_;
  memcpy(&address_,&theAddress.address_,length_);
}

/* ------------------------------------------------------------------------- */
OTC_UnixAddress& OTC_UnixAddress::operator=(OTC_UnixAddress const& theAddress) 
{
  length_ = theAddress.length_;
  memcpy(&address_,&theAddress.address_,length_);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_UnixAddress::pathname() const
{
  OTC_String aString(address_.sun_path);
  return aString;
}

/* ------------------------------------------------------------------------- */
sockaddr* OTC_UnixAddress::addr()
{
  return (sockaddr*)(void*)&address_;
}

/* ------------------------------------------------------------------------- */
sockaddr const* OTC_UnixAddress::addr() const
{
  return (sockaddr const*)(void const*)&address_;
}

/* ------------------------------------------------------------------------- */
void OTC_UnixAddress::resetAddress()
{
  length_ = sizeof(address_);
  memset(&address_,0,length_);
  address_.sun_family = AF_UNIX;
  address_.sun_path[0] = EOS;
  length_ = sizeof(address_.sun_family);
}

/* ------------------------------------------------------------------------- */
void OTC_UnixAddress::initAddress(char const* pathname)
{
  size_t pathlen = strlen(pathname);

  OTCLIB_ASSERT(pathlen<((sizeof(address_)-sizeof(address_.sun_family))));

  resetAddress();
  if (*pathname == EOS)
    strcpy(address_.sun_path,tmpnam(0));
  else
    strcpy(address_.sun_path,pathname);
  length_ = pathlen + sizeof(address_.sun_family);
}

/* ------------------------------------------------------------------------- */

#endif
