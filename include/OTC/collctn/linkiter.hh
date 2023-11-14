#ifndef OTC_COLLCTN_LINKITER_HH
#define OTC_COLLCTN_LINKITER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/linkiter.hh
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

#include <OTC/collctn/linkable.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/link.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/linkiter.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_LinkIterator : public OTC_MPObject
    // = TITLE
    //	   Iterator over a range of objects in a doubly linked list.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   The class <OTC_LinkIterator>, implements an iterator over a range
    //	   of objects in a doubly linked list. The iterator provides the
    //	   ability to move back and forth along the list of objects, and
    //	   provides the ability to know when the iterator is located over a
    //     valid link, or on an end anchor point. By default, the iterator
    //     also transparently increments and decrements reference counts on
    //     the objects, to ensure that removal of a link from the list will
    //     not corrupt the iterator. This feature can be disabled if
    //     necessary.
    //     
    // = SEE ALSO
    //	   <OTC_Link>, <OTC_Anchor>, <OTC_Linkable>
{
  public:

			~OTC_LinkIterator();

    // = INITIALISATION

			OTC_LinkIterator(
			 OTC_Linkable* theStart,
			 OTC_Linkable* theEnd,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// <theStart> and <theEnd> should be anchor
				// points inserted into the list to mark
				// the boundaries of the range over which
				// the iterator can traverse. Both anchor
				// points should be in the same list, and
				// <theStart> should appear prior to <theEnd>
				// in the list. If <theStart> and <theEnd>
				// are not correctly placed into the list,
				// an exception will be raised at the time
				// that the condition is detected. Link
				// objects in the list, may also be used
				// as end points of iteration. In this
				// case, the endpoints are not included in
				// the iteration, and references are
				// maintained on the link objects for the
				// whole life of the iterator, meaning that
				// deletion of items will be delayed.
				// The argument <theProtection> determines
				// if reference counting will actually be
				// performed on object in the list. The
				// default of <OTCLIB_SAFE> will result in
				// reference counting being performed. A
				// value of <OTCLIB_UNSAFE> will turn of
				// reference counting. In this later case,
				// the list should not be modified while
				// the iterator exists as the state of the
				// iterator will be corrupted.

			OTC_LinkIterator(OTC_LinkIterator const& theIter);
				// Creates an iterator which operates in the
				// same range as <theIter>, that is
				// initially located over the same object as
				// <theIter>.

    OTC_LinkIterator&	operator=(OTC_LinkIterator const& theIter);
				// Initialises this iterator to operate in
				// the same range as <theIter>, and to be
				// initially located over the same object as
				// <theIter>.

    // = LOCATION

    bool		isStart() const
				{ return link_ == start_; }
				// Returns <true> if the iterator is situated
				// on the start anchor point, of the valid
				// range for this iterator.

    bool		isEnd() const
				{ return link_ == end_; }
				// Returns <true> if the iterator is situated
				// on the end anchor point, of the valid
				// range for this iterator.

    bool		isLink() const
				{ return !isStart() && !isEnd(); }
				// Returns <true> if the iterator is located
				// on a real link, ie., a link for which a
				// derived class may contain actual data.
				// Note though, that the link may be
				// dead; thus it is the user's responsibility
				// to only access data in a derived class if
				// it is still valid. If <false> is returned
				// then it implies that we are located over
				// either the start or end anchor points.

    // = ITERATION

    void		resetStart()
				{ locate(start_); }
				// Reset the iterator back to the start
				// anchor point.

    void		resetEnd()
				{ locate(end_); }
				// Reset the iterator to point at the end
				// anchor point.

    void		resetFirst()
				{ locate(start_); next(); }
				// Resets the iterator to the first real
				// link. If there are no links, the iterator
				// is reset to the end anchor point.

    void		resetLast()
				{ locate(end_); prev(); }
				// Resets the iterator to the last real
				// link. If there are no links, the iterator
				// is reset to the start anchor point.

    void		next();
				// Moves the iterator to the next real link
				// occurring after the current link, or if
				// there are no real links between the
				// current link and the end link for this
				// range, to the end of the list.

    void		prev();
				// Moves the iterator to the next real
				// link occurring before the current link,
				// or if there are no real links between the
				// current link and the start link for this
				// range, to the start of the list.

    // = RETRIEVAL

    OTC_Link*		link() const
				{ return isLink() ? (OTC_Link*)link_ : 0; }
				// Returns the link on which the iterator is
				// located, if it is a real link. If the
				// iterator is not located on a real link,
				// then <0> is returned. Note that although a
				// non zero value may be returned, it does
				// not mean that the link is alive. To find
				// out if a link is alive, it should be
				// queried directly using the <isDead()>
				// member function.

  private:

    void		locate(OTC_Linkable* theLink);
				// Locate this iterator on <theLink>.

    OTC_Linkable*	start_;
				// Marker for the start of the range over
				// which the iterator is to operate.

    OTC_Linkable*	end_;
				// Marker for the end of the range over
				// which the iterator is to operate.

    OTC_Linkable*	link_;
				// The link in the linked list at which
				// the iterator is currently located.

    OTC_Protection	protection_;
				// Has value of <OTCLIB_SAFE> if internal
				// reference counting should be done.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LINKITER_HH */
