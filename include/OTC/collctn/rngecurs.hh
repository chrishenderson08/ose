#ifndef OTC_COLLCTN_RNGECURS_HH
#define OTC_COLLCTN_RNGECURS_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/rngecurs.hh
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

#include <OTC/collctn/cursor.hh>
#include <OTC/collctn/dirction.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/rngecurs.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_RangeCursor : public OTC_Cursor<int>
    // = TITLE
    //     A cursor over a range of integers.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class is a cursor over a range of integers. Note that the
    //     cursor is not over real data and should only be used with the
    //     <OTC_Iterator> class, which will not allow modification of values
    //     returned.
    //     
    // = SEE ALSO
    //     <OTC_Cursor>, <OTC_Iterator>
{
  public:

			~OTC_RangeCursor();

    // = CONSTRUCTION

			OTC_RangeCursor(
			 int theLower,
			 u_int theLength,
			 OTC_Direction theDirection=OTCLIB_FORWARD
			);
				// Creates a cursor which will return values
				// from the range with lower bound <theLower>
				// and length <theLength>. <theDirection>
				// indicates in which direction the cursor
				// will traverse, ie., <OTCLIB_FORWARD> will
				// result in traversal from <theLower> to
				// <theUpper>; <OTCLIB_BACKWARD> will result
				// in traversal in the opposite direction.

    // = MOVEMENT

    void		reset();
				// Resets the iterator back to the start.

    void		next();
				// Moves the iterator to the next value.

    // = RETRIEVAL

    int&		item();
				// Returns the item under the cursor. It
				// is a precondition, that the cursor be
				// located over a valid value. If this is not
				// the case, an exception will be raised.

    bool		isValid() const;
				// Returns <true> while there is a valid
				// value under the cursor.

    // = CLONING

    OTC_Cursor<int>*	clone();
				// Returns a new cursor which is a clone
				// of this one.

  private:

			OTC_RangeCursor(OTC_RangeCursor const& theCursor);
				// Creates a clone of <theCursor>.

    OTC_RangeCursor&	operator=(OTC_RangeCursor const&);
				// Do not define an implementation for this.

    int			lower_;
				// The lower bound of the range. Note that
				// this value is a member of the values
				// in the range.

    int			upper_;
				// The upper bound of the range. Note that
				// this value is not a member of the values
				// in the range but is one more that the
				// largest value in the range.

    OTC_Direction	direction_;
				// The direction in which the cursor is
				// supposed to be traversing.

    int			current_;
				// The current value in the range at which
				// the cursor is located.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_RNGECURS_HH */
