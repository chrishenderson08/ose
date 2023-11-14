/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcjoblist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/joblist.hh"
#endif
#endif

#include <OTC/dispatch/joblist.hh>

/* ------------------------------------------------------------------------- */
OTC_JobList::~OTC_JobList()
{
  OTC_LinkIterator theJobs = jobs_.items();
  theJobs.resetFirst();
  while (theJobs.isLink())
  {
    OTC_Job* theJob;
    theJob = (OTC_Job*)((OTC_VLink*)theJobs.link())->item();
    theJob->destroy();
  }
}

/* ------------------------------------------------------------------------- */
bool OTC_JobList::isEmpty() const
{
  return jobs_.isEmpty();
}

/* ------------------------------------------------------------------------- */
void OTC_JobList::add(OTC_Job* theJob)
{
  OTCLIB_ENSURE_FN((theJob != 0),
   "OTC_JobList::add(OTC_Job*)",
   "invalid job");

  OTC_VLink* theLink;
  theLink = new OTC_VLink((void*)theJob);
  OTCLIB_ASSERT_M(theLink != 0);
  jobs_.addLast(theLink);
}

/* ------------------------------------------------------------------------- */
OTC_Job* OTC_JobList::next()
{
  if (jobs_.isEmpty())
    return 0;

  OTC_Job* theJob;
  theJob = (OTC_Job*)((OTC_VLink*)jobs_.first())->item();
  jobs_.removeFirst();

  return theJob;
}

/* ------------------------------------------------------------------------- */
