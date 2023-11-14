#ifndef OTC_COLLCTN_RANGE_HH
#define OTC_COLLCTN_RANGE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/range.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/dirction.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/range.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Range
    // = TITLE
    //     Class to represent a range of integers.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is for representing a range of integers. That set of
    //     integers is any integer <i> such that the condition <lower() \<=
    //     i \< upper()> is satisfied. If <lower()> is equal to <upper()>
    //     then it is a null range. In defining the range, the lower bound
    //     and a length is expected. The property which relates the bounds
    //     and the length is <upper() == lower() + length()>.
{
  public:

    // = CONSTRUCTION

			OTC_Range(u_int theLength=0)
			  : lower_(0), length_(theLength) {}
				// Initialises the class to represent
				// a range with a length <theLength> and
				// lower bound of <0>.

			OTC_Range(int theLower, u_int theLength)
			  : lower_(theLower), length_(theLength) {}
				// Initialises the class to represent a
				// range with a lower bound <theLower> and
				// length <theLength>.

			OTC_Range(OTC_Range const& theRange)
			  : lower_(theRange.lower_),
			    length_(theRange.length_) {}
				// Initialises the class to represent
				// the same range as <theRange>.

    // = ASSIGNMENT

    OTC_Range&		operator=(OTC_Range const& theRange)
				{
				  lower_ = theRange.lower_;
				  length_ = theRange.length_;
				  return *this;
				}
				// Changes this class to represent the
				// same range as <theRange>.

    // = COMPARISON

    bool		operator==(OTC_Range const& theRange) const
				{
				  return lower_ == theRange.lower_ &&
				   length_ == theRange.length_;
				}
				// Returns <false> if this range isn't
				// equivalent to <theRange>.

    bool		operator!=(OTC_Range const& theRange) const
				{ return !(*this == theRange); }
				// Returns <false> if this range is
				// equivalent to <theRange>.

    // = QUERY

    int			lower() const
				{ return lower_; }
				// Returns the integer which is the lower
				// bound of the range. Note that this value
				// is regarded as being in the range.

    int			upper() const
				{ return lower_ + length_; }
				// Returns the integer which is the upper
				// bound of the range. Note that this value
				// is not regarded as being in the range, but
				// is one more than the highest value in the
				// range.

    u_int		length() const
				{ return length_; }
				// Returns the length of this range, ie., the
				// number of integers in the range.

    bool		contains(int theValue) const
				{
				  return lower() <= theValue &&
				   theValue < upper();
				}
				// Returns <false> if <theValue> is not
				// contained in this range.

    bool		isNullRange() const
				{ return length_ == 0; }
				// Returns <false> if this is not a null
				// range.

    bool		isSubRange(OTC_Range const& theRange) const;
				// Returns <false> if this range is not
				// contained within <theRange>. If this
				// range is a null range then <true> will
				// always be returned.

    OTC_Iterator<int>	indices(
			 OTC_Direction theDirection=OTCLIB_FORWARD
			) const;
				// Returns an iterator for the indices in the
				// range. <theDirection> indicates the
				// direction of traversal, namely, if
				// <theDirection> is <OTCLIB_FORWARD>,
				// traversal will be in the direction of
				// lower to upper. For <OTCLIB_BACKWARD> it
				// will be the reverse.

  private:

    int			lower_;
				// Lower bound of the range.

    u_int		length_;
				// The length of the range.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_RANGE_HH */
