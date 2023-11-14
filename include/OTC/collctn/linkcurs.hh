#ifndef OTC_COLLCTN_LINKCURS_HH
#define OTC_COLLCTN_LINKCURS_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/linkcurs.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/linkiter.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/linkcurs.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_LinkList;

class OSE_EXPORT OTC_LinkCursor
    // = TITLE
    //	   Base class for cursors operating over a linked list.
    //
    // = CLASS TYPE
    //	   Abstract
    //
    // = DESCRIPTION
    //	   The <OTC_LinkCursor> class encapsulates most of the operations
    //	   needed when implementing a cursor over an instance of
    //	   <OTC_LinkedList>.
{
  public:

    // = ITERATION

    OTC_Direction	direction() const
				{ return direction_; }
				// Returns the direction in which the
				// iterator is currently moving.

    void		reverse()
				{
				  direction_ = (direction() == OTCLIB_FORWARD)
				   ? OTCLIB_BACKWARD : OTCLIB_FORWARD;
				}
				// Reverses the direction in which the
				// iterator will move on the list.

    void		reset()
				{
				  if (direction() == OTCLIB_FORWARD)
				    iter_->resetFirst();
				  else
				    iter_->resetLast();
				}
				// Resets the iterator back to the start
				// of the list. Note that if the iterator
				// is actually moving in the reverse
				// direction over the list the iterator
				// will be reset to the end of the list.

    void		next()
				{
				  if (direction() == OTCLIB_FORWARD)
				    iter_->next();
				  else
				    iter_->prev();
				}
				// Moves the iterator onto the next
				// live link in the list.

    bool		isValid() const
				{ return iter_->isLink(); }
				// Returns <true> if the iterator is located
				// over a link.

  protected:

    virtual		~OTC_LinkCursor();

    // = INITIALISATION

			OTC_LinkCursor(
			 OTC_LinkList* theList,
			 OTC_Direction theDirection,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// <theList> should be the list which you
				// wish to iterate over. <theDirection>
				// should indicate the direction in which
				// you wish the iterator to initially
				// proceed. If <theDirection> is
				// <OTCLIB_FORWARD> then the direction of
				// traversal will be from the start of the
				// list to the end. If it is
				// <OTCLIB_BACKWARD> then it will be in the
				// opposite direction. <theProtection>
                                // determines whether reference counting is
                                // performed on the links in the list.
                                // Reference counting ensures the integrity of
                                // the iterator when removals and additions
                                // are perfomed. The values for
                                // <theProtection> are <OTCLIB_SAFE> and
                                // <OTCLIB_UNSAFE>. The default argument is
                                // <OTCLIB_UNSAFE>.

			OTC_LinkCursor(
			 OTC_LinkList* theList,
			 OTC_Linkable* theStart,
			 OTC_Linkable* theEnd,
			 OTC_Direction theDirection,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// <theList> should be the list which you
				// wish to iterate over. <theStart> and
				// <theEnd> are the anchor points within that
				// list which define the bounds of the
				// iterator. If <theDirection> is
				// <OTCLIB_FORWARD> then traversal will be
				// from the <theStart> to <theEnd>. If it is
				// <OTCLIB_BACKWARD> then it will be in the
				// opposite direction. Note that this class
				// relies on <theStart> preceeding <theEnd>
				// in the list. <theProtection>
                                // determines whether reference counting is
                                // performed on the links in the list.
                                // Reference counting ensures the integrity of
                                // the iterator when removals and additions
                                // are perfomed. The values for
                                // <theProtection> are <OTCLIB_SAFE> and
                                // <OTCLIB_UNSAFE>. The default argument is
                                // <OTCLIB_UNSAFE>.

			OTC_LinkCursor(
			 OTC_LinkCursor const& theIter
			);
				// <theIter> is an iterator which you
				// wish to clone, ie., this iterator will
				// traverse the same list and will initially
				// be located at the same location and
				// moving in the same direction.

    // = ACCESS TO LINK

    OTC_Link*		link() const
				{ return iter_->link(); }
				// Returns a pointer to the link which
				// the iterator is currently located over.
				// If the iterator is not located over
				// a link but an anchor point then <0>
				// will be returned.

  private:

    OTC_LinkCursor&	operator=(OTC_LinkCursor const&);
				// Do not define an implementation for this.

    OTC_LinkList*	list_;
				// The list being iterated over.

    OTC_LinkIterator*	iter_;
				// The iterator doing the real work.

    OTC_Direction	direction_;
				// The direction through this list which
				// the iterator will move.

    OTC_Protection	protection_;
				// Whether this cursor is safe or not.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LINKCURS_HH */
