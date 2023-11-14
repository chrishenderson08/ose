/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/otcpathname.cc
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
#pragma implementation "OTC/system/pathname.hh"
#endif
#endif

#include <OTC/system/pathname.hh>

#include <stdlib.h>
#include <string.h>

#if defined(OSE_SYS_UNIX)
#include <unistd.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <io.h>
#include <direct.h>
#ifdef __STDC__
#ifndef access
#define access access_
#endif
#endif
#endif

#ifndef F_OK
#define F_OK 00
#define X_OK 01
#define W_OK 02
#define R_OK 04
#endif

/* ------------------------------------------------------------------------- */
OTC_Pathname::~OTC_Pathname()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Pathname::OTC_Pathname()
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Pathname::OTC_Pathname(char const* thePath)
  : path_(thePath)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Pathname::OTC_Pathname(OTC_Pathname const& thePath)
  : path_(thePath.path_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
bool OTC_Pathname::isAccessable() const
{
  return !access((char*)path_.string(),F_OK);
}

/* ------------------------------------------------------------------------- */
bool OTC_Pathname::isWritable() const
{
  return !access((char*)path_.string(),W_OK);
}

/* ------------------------------------------------------------------------- */
bool OTC_Pathname::isReadable() const
{
  return !access((char*)path_.string(),R_OK);
}

/* ------------------------------------------------------------------------- */
bool OTC_Pathname::isExecutable() const
{
  return !access((char*)path_.string(),X_OK);
}

/* ------------------------------------------------------------------------- */
OTC_Pathname OTC_Pathname::absolute() const
{
  if (isAbsolute())
    return *this;

  else if (path_.isEmpty())
    return "";

  char buf[MAXPATHLEN];
  char const* res;
#ifdef HAVE_GETCWD
  res = getcwd(buf,MAXPATHLEN);
#else
  res = (char*)getwd((char*)buf);
#endif
  if (res == 0 || *buf == EOS)
    OTCLIB_EXCEPTION("Can't get name of current directory");

#if defined(OSE_SYS_WIN32)
  char* p = buf;
  while (*p != EOS)
  {
    if (*p == '\\')
      *p = '/';
    p++;
  }
#endif

  OTC_String theString = buf;

  if (!path_.isEmpty() && path_ != ".")
  {
    theString.append('/');
    theString.append(path_);
  }

  return OTC_Pathname(theString);
}

/* ------------------------------------------------------------------------- */
OTC_Pathname OTC_Pathname::dirname() const
{
  if (path_.isEmpty())
    return "";

  else
  {
    OTC_String theString = path_;

    // Remove trailing slashes.

    if (theString[theString.length()-1] == '/')
    {
      u_int i=0;
      while (i<theString.length() && theString[theString.length()-1-i] == '/')
	i++;
      theString.truncate(theString.length()-i);
    }

    // Check for empty string, meaning that original path represented
    // root directory.

    if (theString.isEmpty())
    {
      theString = "/";
      return OTC_Pathname(theString);
    }

    // Remove last pathname component.

    u_int j=0;
    while (j<theString.length() && theString[theString.length()-1-j] != '/')
      j++;
    if (j > 0)
      theString.truncate(theString.length()-j);

    // Check for empty string, meaning that original path was name of
    // file in current directory.

    if (theString.isEmpty())
    {
      theString = ".";
      return OTC_Pathname(theString);
    }

    // Remove trailing slashes again.

    u_int k=0;
    while (k<theString.length() && theString[theString.length()-1-k] == '/')
      k++;
    if (k > 0)
      theString.truncate(theString.length()-k);

    // Check for empty string, meaning that original path was name of
    // file in root directory.

    if (theString.isEmpty())
    {
      theString = "/";
      return OTC_Pathname(theString);
    }

    // Whats left is the directory portion.

    return OTC_Pathname(theString);
  }
}

/* ------------------------------------------------------------------------- */
OTC_Pathname OTC_Pathname::basename() const
{
  if (path_.isEmpty())
    return "";

  else
  {
    OTC_Record record(path_,'/');
    return OTC_Pathname(record.field(record.numFields()));
  }
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_Pathname::basename(char const* theSuffix) const
{
  if (path_.isEmpty())
    return "";
  
  if (theSuffix == 0 || *theSuffix == EOS)
    return basename().path_;

  else
  {
    OTC_String theString(basename().path_);

    if (theString.isEmpty())
      return "";

    u_int theLength = strlen(theSuffix);

    if (theLength > theString.length())
      return basename().path_;

    u_int theStart = theString.length() - theLength;
    OTC_String aSuffix(theString.section(theStart,theLength));

    if (aSuffix == theSuffix)
      theString.truncate(theStart);

    return theString;
  }
}

/* ------------------------------------------------------------------------- */
bool OTC_Pathname::isAbsolute() const
{
  if (path_.isEmpty())
    return false;

  if (path_[(u_int)0] == '/')
    return true;

#if defined(OSE_SYS_WIN32)
  if (path_.length() >= 3 && path_[(u_int)1] == ':')
  {
    char theDrive = path_[(u_int)0];
    if (theDrive >= 'a' && theDrive <= 'z')
      return true;
    if (theDrive >= 'A' && theDrive <= 'Z')
      return true;
  }
#endif

  return false;
}

/* ------------------------------------------------------------------------- */
bool OTC_Pathname::isRelative() const
{
  return path_.isEmpty() ? false : (path_[(u_int)0] != '/');
}

/* ------------------------------------------------------------------------- */
