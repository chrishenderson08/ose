#ifndef OTC_REFCNT_COUNT_HH
#define OTC_REFCNT_COUNT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/count.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/mpobject.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/refcnt/count.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Count : public OTC_MPObject
    // = TITLE
    //     An integer count.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_Count> contains an integer count. It has a lower
    //     bound of <0> and an upper bound of the largest number which
    //     can be held in a <u_int>. When created, the count is set
    //     to the lower bound of <0>.
    //
    // = NOTES
    //     No check is made against the upper bound, if the count was
    //     incremented that far, it would quitely wrap around to <0>.
{
  public:

			~OTC_Count();

    // = CONSTRUCTION

			OTC_Count()
			  : count_(0) {}
				// Initialises the count to the lower bound
				// of <0>.

			OTC_Count(OTC_Count const& theCtr)
			  : OTC_MPObject(), count_(theCtr.count_) {}
				// Initialises the count to the same
				// value as <theCtr>.

    // = ASSIGNMENT

    OTC_Count&		operator=(OTC_Count const& theCtr)
				{ count_ = theCtr.count_; return *this; }
				// Sets the count to the same value as
				// <theCtr>.

    // = MODIFIERS

    void		inc()
				{ count_++; }
				// Increments the current count by one.

    void		dec()
				{ if (count_ != 0) count_--; }
				// Decrements the current count by one. If
				// the counter is already at <0>, the
				// value is unchanged.

    void		set(u_int theCount)
				{ if (count_ != theCount) count_ = theCount; }
				// Sets the count to <theCount>.

    // = QUERY

    u_int		count() const
				{ return count_; }
				// Returns the current count.

  private:

    u_int		count_;
				// The current value of the count.

};

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_COUNT_HH */
