/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/otcdirectry.cc
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
#pragma implementation "OTC/system/directry.hh"
#endif
#endif

#include <OTC/system/directry.hh>
#include <OTC/text/globex.hh>

#include <sys/types.h>

#if defined(OSE_SYS_UNIX)
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#define dirent direct
#ifdef HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#else
#ifdef HAVE_SYS_DIR_H
#include <sys/dir.h>
#else
#ifdef HAVE_NDIR_H
#include <ndir.h>
#else
#undef dirent
#endif
#endif
#endif
#endif
#endif

#if defined(OSE_SYS_WIN32)
#include <windows.h>
#endif

/* ------------------------------------------------------------------------- */
OTC_Directory::OTC_Directory(char const* theName)
  : name_(theName)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Directory::OTC_Directory(OTC_Directory const& theDir)
  : name_(theDir.name_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
bool OTC_Directory::isSearchable() const
{
  if (name_.isEmpty())
    return false;

#if defined(OSE_SYS_WIN32)

  HANDLE theHandle = INVALID_HANDLE_VALUE;

  theHandle = CreateFile(
   (char*)name_.string(),
   GENERIC_READ,
   FILE_SHARE_READ|FILE_SHARE_WRITE,
   0,
   OPEN_EXISTING,
   FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS,
   0);

  if (theHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(theHandle);
    return true;
  }

  return false;

#elif defined(OSE_SYS_UNIX)

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle != 0)
  {
    closedir(theHandle);
    return true;
  }

  return false;

#else
#error "unsupported"
#endif
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Directory::files() const
{
  if (name_.isEmpty())
    return 0;

#if defined(OSE_SYS_WIN32)

  HANDLE theHandle = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA theEntry;

  OTC_String thePath;

  thePath += name_;
  thePath += '/';
  thePath += '*';

  theHandle = FindFirstFile((char*)thePath.string(),&theEntry);

  if (theHandle == INVALID_HANDLE_VALUE)
    return 0;

  OTC_Deque<OTC_String> theNames;
  theNames.addLast((char const*)theEntry.cFileName);

  while (FindNextFile(theHandle,&theEntry))
    theNames.addLast((char const*)theEntry.cFileName);

  FindClose(theHandle);

  return theNames.items();

#elif defined(OSE_SYS_UNIX)

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle == 0)
    return 0;

  OTC_Deque<OTC_String> theNames;
  dirent* theEntry;
  for (theEntry = readdir(theHandle);
   theEntry != 0; theEntry = readdir(theHandle))
  {
    theNames.addLast(theEntry->d_name);
  }
  closedir(theHandle);
  return theNames.items();

#else
#error "unsupported"
#endif
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Directory::files(char const* thePattern) const
{
  if (name_.isEmpty())
    return 0;

#if defined(OSE_SYS_WIN32)

  HANDLE theHandle = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA theEntry;

  OTC_Globex theGlobex(thePattern);
  if (!theGlobex.isValid())
    return 0;

  OTC_String thePath;

  thePath += name_;
  thePath += '/';
  thePath += '*';

  theHandle = FindFirstFile((char*)thePath.string(),&theEntry);

  if (theHandle == INVALID_HANDLE_VALUE)
    return 0;

  OTC_Deque<OTC_String> theNames;

  if (theGlobex.match(theEntry.cFileName))
    theNames.addLast((char const*)theEntry.cFileName);

  while (FindNextFile(theHandle,&theEntry))
  {
    if (theGlobex.match(theEntry.cFileName))
      theNames.addLast((char const*)theEntry.cFileName);
  }

  FindClose(theHandle);

  return theNames.items();

#elif defined(OSE_SYS_UNIX)

  OTC_Globex theGlobex(thePattern);
  if (!theGlobex.isValid())
    return 0;

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle != 0)
  {
    OTC_Deque<OTC_String> theNames;
    dirent* theEntry;
    for (theEntry = readdir(theHandle);
     theEntry != 0; theEntry = readdir(theHandle))
    {
      if (theGlobex.match(theEntry->d_name))
	theNames.addLast(theEntry->d_name);
    }
    closedir(theHandle);
    return theNames.items();
  }

  return 0;

#else
#error "unsupported"
#endif
}

/* ------------------------------------------------------------------------- */
bool OTC_Directory::contains(char const* theName) const
{
  if (name_.isEmpty())
    return false;

#if defined(OSE_SYS_WIN32)

  HANDLE theHandle = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA theEntry;

  OTC_String thePath;

  thePath += name_;
  thePath += '/';
  thePath += '*';

  theHandle = FindFirstFile((char*)thePath.string(),&theEntry);

  if (theHandle == INVALID_HANDLE_VALUE)
    return false;

  if (strcmp(theName,theEntry.cFileName) == 0)
  {
    FindClose(theHandle);
    return true;
  }

  while (FindNextFile(theHandle,&theEntry))
  {
    if (strcmp(theName,theEntry.cFileName) == 0)
    {
      FindClose(theHandle);
      return true;
    }
  }

  FindClose(theHandle);

  return false;

#elif defined(OSE_SYS_UNIX)

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle == 0)
    return false;

  dirent* theEntry;
  for (theEntry = readdir(theHandle);
   theEntry != 0; theEntry = readdir(theHandle))
  {
    if (strcmp(theName,theEntry->d_name) == 0)
    {
      closedir(theHandle);
      return true;
    }
  }
  closedir(theHandle);

  return false;

#else
#error "unsupported"
#endif
}

/* ------------------------------------------------------------------------- */
