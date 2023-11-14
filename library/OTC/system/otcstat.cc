/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/otcstat.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/stat.hh"
#endif
#endif

#include <OTC/system/stat.hh>

#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

/* ------------------------------------------------------------------------- */
OTC_Stat::OTC_Stat()
{
  memset(&stat_,0,sizeof(stat_));
}

/* ------------------------------------------------------------------------- */
OTC_Stat::OTC_Stat(OTC_Stat const& theStat)
{
  memcpy(&stat_,&theStat.stat_,sizeof(stat_));
}

/* ------------------------------------------------------------------------- */
OTC_Stat& OTC_Stat::operator=(OTC_Stat const& theStat)
{
  memcpy(&stat_,&theStat.stat_,sizeof(stat_));
  return *this;
}

/* ------------------------------------------------------------------------- */
