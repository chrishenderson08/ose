/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/otccluster.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/memory/cluster.hh"
#endif
#endif

#include <OTC/memory/cluster.hh>
#include <OTC/memory/mcobject.hh>
#include <OTC/memory/arena.hh>

/* ------------------------------------------------------------------------- */
OTC_Cluster::OTC_Cluster()
  : objects_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Cluster::OTC_Cluster(size_t theBlockSize, size_t theSlop)
  : arena_(theBlockSize,theSlop), objects_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Cluster::~OTC_Cluster()
{
  while (objects_ != 0)
  {
    OTC_MCObject* theNext = objects_->next_;
    delete objects_;
    objects_ = theNext;
  }
}

/* ------------------------------------------------------------------------- */
