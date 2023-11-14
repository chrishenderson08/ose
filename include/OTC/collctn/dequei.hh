#ifndef OTC_COLLCTN_DEQUEI_HH
#define OTC_COLLCTN_DEQUEI_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/dequei.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/refcnt/rcount.hh>
#include <OTC/collctn/dequehi.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/dequei.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_DequeI
    // = TITLE
    //     Core for implementing a deque.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_DequeI> class factors out details necessary for
    //     implementing deque and list structures which do not require
    //     indexes. Factoring these details into this class means
    //     that they do not have to be part of the template interface
    //     which would be wrapped around this class. This class still
    //     provides access to the underlying list and also to the links
    //     in the list so that iteration mechanisms can be provided. You
    //     should however, always use the member functions of this class when
    //     wishing to manipulate the list. In other words you should not add
    //     or remove links directly to or from the list as that will
    //     invalidate the cached population maintained by this class.
{
  public:

			~OTC_DequeI();

    // = INITIALISATION

			OTC_DequeI();
				// Creates an empty deque.

			OTC_DequeI(OTC_DequeI const& theDeque);
				// Creates a deque which is an alias for
				// <theDeque>. Ie., each shares and access
				// the same implementation.

    // = QUERY

    OTC_LinkList*	list() const
				{ return list_; }
				// Returns the underlying list.

    u_int		population() const
				{ return count_->count(); }
				// Returns the number of live links in the
				// deque.

    bool		isEmpty() const
				{ return count_->count() == 0; }
				// Returns <true> if there are no live links
				// in the deque.

    // = REMOVAL

    void		remove(OTC_Link* theLink);
				// Removes <theLink> from the deque. If
				// <theLink> is not in this deque, the result
				// is undefined. Using this function to
				// delete a link not in this deque, will at
				// the minimum invalidate the population
				// for this deque.

    void		removeAll()
				{ list_->removeAll(); count_->set(0); }
				// Removes all links from the deque.

    void		removeFirst(OTC_Protection theProtection=OTCLIB_SAFE);
				// Removes the first link in the deque. If
				// the deque is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeLast(OTC_Protection theProtection=OTCLIB_SAFE);
				// Removes the last link in the deque. If
				// the deque is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    // = ACCESS

    OTC_Link*		first() const;
				// Returns the first link in the deque.
				// Raises an exception if the deque is empty.

    OTC_Link*		last() const;
				// Returns the last link in the deque. Raises
				// an exception if the deque is empty.

    // = ADDITION

    void		addFirst(OTC_Link* theLink);
				// Adds <theLink> at the start of the
				// the deque.

    void		addLast(OTC_Link* theLink);
				// Adds <theLink> at the end of the
				// the deque.

    void		addBeforeLink(
			 OTC_Link* theNewLink,
			 OTC_Link* theOldLink
			);
				// Adds <theNewLink> before <theOldLink>
				// in this list. If <theOldLink> is not
				// in this list, the result is undefined.
				// If <theOldLink> is the start anchor,
				// <theNewLink> will be added as the
				// first link in the list. If <theNewLink>
				// is already in a list, the result
				// is undefined.

    void		addAfterLink(
			 OTC_Link* theNewLink,
			 OTC_Link* theOldLink
			);
				// Adds <theNewLink> after <theOldLink>
				// in this list. If <theOldLink> is not
				// in this list, the result is undefined.
				// If <theOldLink> is the end anchor,
				// <theNewLink> will be added as the
				// last link in the list. If <theNewLink>
				// is already in a list, the result
				// is undefined.

  private:

    OTC_DequeI&		operator=(OTC_DequeI const&);
				// Do not define an implementation for this.

    OTC_LinkList*	list_;
				// The actual list.

    OTC_RCount*		count_;
				// The cached population count.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_DEQUEI_HH */
