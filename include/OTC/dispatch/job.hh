#ifndef OTC_DISPATCH_JOB_HH
#define OTC_DISPATCH_JOB_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/job.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/cmmnpool.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/job.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_JobType
{
  OTCLIB_IDLE_JOB=0,
  OTCLIB_STANDARD_JOB=1,
  OTCLIB_PRIORITY_JOB=2
};

enum OTC_JobActions
{
  OTCLIB_DONT_WAIT = 0x0001,
  OTCLIB_THREADS_DONT_WAIT = 0x0002,
  OTCLIB_ALL_DONT_WAIT = 0x0003,
  OTCLIB_SIGNAL_JOBS = 0x0004,
  OTCLIB_ALARM_JOBS = 0x0008,
  OTCLIB_TIMEOUT_JOBS = 0x0010,
  OTCLIB_IO_JOBS = 0x0020,
  OTCLIB_IDLE_JOBS = 0x0040,
  OTCLIB_STANDARD_JOBS = 0x0080,
  OTCLIB_PRIORITY_JOBS = 0x0100,
  OTCLIB_ALL_JOBS = 0x01fc
};

class OSE_EXPORT OTC_Job
    // = TITLE
    //     Base class for jobs to be executed by the dispatcher.
    //
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     <OTC_Job> is the base class for any jobs to be executed by the
    //     dispatcher. Derived classes must override the <execute()> function
    //     to perform the actual work. The dispatcher will call <destroy()>
    //     once the job has executed. The default action of <destroy()> is to
    //     delete the object; this can be overridden in a derived class if
    //     necessary.
    //     
    //     Space for any derived classes is allocated from <OTC_CommonPool>.
    //     
    // = SEE ALSO
    //     <OTC_JobQueue>, <OTC_Dispatcher>, <OTC_CommonPool>
{
  public:

    virtual		~OTC_Job();

    void*		operator new(size_t theSize)
				{ return OTC_CommonPool::allocate(theSize); }

    void		operator delete(void* theMemory, size_t theSize)
				{ OTC_CommonPool::release(theMemory,theSize); }

    // = EXECUTION

    virtual void	execute() = 0;
				// Must be redefined in a derived class to
				// perform the actual work associated with
				// the job.

    // = DESTRUCTION

    virtual void	destroy();
				// Called by the dispatcher once the job has
				// been executed. The default action is to
				// delete the object. This can be overridden
				// in a derived class if necessary.

    // = IDENTIFICATION

    virtual void*       type() const;
                                // Could be redefined in a derived class
                                // to return a <void*> which uniquely
                                // identifies the job type. Normally,
                                // a pointer to a static member variable
                                // of a derived class would be used as the
                                // return value. By default, the job type
				// will be that for this class, ie.,
				// <OTC_Job>. You need only override this
				// function if you need to identify a new
				// class of jobs.

    static void*	typeId()
				{ return &globTypeId_; }
				// Returns a value which uniquely identifies
				// this type of event.

    // = SCHEDULING

    void		schedule(OTC_JobType theType);
				// Schedules this job with the dispatcher
				// as a job of <theType>.

  protected:

			OTC_Job()
				{}

  private:

			OTC_Job(OTC_Job const&) {}
				// Do not define an implementation for this.

    OTC_Job&		operator=(OTC_Job const&);
				// Do not define an implementation for this.

    static int		globTypeId_;
				// Tag for mini rtti.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_JOB_HH */
