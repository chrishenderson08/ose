/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/otcresource.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/refcnt/resource.hh"
#endif
#endif

#include <OTC/refcnt/resource.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_Resource::mutex_;

/* ------------------------------------------------------------------------- */
OTC_Resource::~OTC_Resource()
{
  OTCLIB_ENSURE_FN((numRefs() == 0),
   "OTC_Resource::~OTC_Resource()",
   "object still referenced");
}

/* ------------------------------------------------------------------------- */
u_int OTC_Resource::numRefs() const
{
  OTC_Resource* theSelf;
  theSelf = (OTC_Resource*)this;

  return otclib_atomic_get(theSelf->refCount_,mutex_);
}

/* ------------------------------------------------------------------------- */
u_int OTC_Resource::reference() const
{
  OTC_Resource* theSelf;
  theSelf = (OTC_Resource*)this;

  return otclib_atomic_inc(theSelf->refCount_,mutex_);
}

/* ------------------------------------------------------------------------- */
u_int OTC_Resource::unReference() const
{
  OTCLIB_ENSURE_FN((numRefs() != 0),
   "OTC_Resource::unReference()",
   "object not referenced");

  u_int theResult;

  OTC_Resource* theSelf;
  theSelf = (OTC_Resource*)this;

  theResult = otclib_atomic_dec(theSelf->refCount_,mutex_);

  if (theResult == 0)
    delete (OTC_Resource*)this;

  return theResult;
}

/* ------------------------------------------------------------------------- */
