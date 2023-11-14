#ifndef OTC_COLLCTN_ILIST_HH
#define OTC_COLLCTN_ILIST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ilist.hh
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

#include <OTC/collctn/dequei.hh>
#include <OTC/collctn/linkiter.hh>
#include <OTC/collctn/worker.hh>
#include <OTC/collctn/visitor.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/shlwcopy.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/ilist.hh"
#endif
#pragma interface "OTC/collctn/ilist.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_IList
    // = TITLE
    //	   Linked list with inbuilt iterator.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class is a templated, minimal linked list, incorporating an
    //	   inbuilt iterator. This would be used where the extra functionality
    //	   of <OTC_Deque> or <OTC_List> is not required. This class
    //	   replaces the <OTC_SimpleList> class.
    //	   
    //	   Items may be inserted at any position in the list by moving the
    //	   inbuilt iterator to the required location and then adding the
    //	   item, either before, or after the iterator position. It is also
    //	   possible to add items directly onto the head or tail of the list.
    //
    //	   The class uses <OTC_Bucket> to hold items internally. Therefore,
    //	   <OTC_BaseActions> can be used to define what occurs when items
    //	   are added and removed from the list.
    //	   
    // = SEE ALSO
    //	   <OTC_Bucket>, <OTC_BaseActions>
{
  public:

    // = CONSTRUCTION

			OTC_IList();
				// Creates an empty list.

			OTC_IList(OTC_IList<T> const& theList);
				// Creates a list which is a copy of
				// <theList>. If the list holds pointers,
				// only the pointer is copied and not
				// what the pointer points at.

			OTC_IList(OTC_IList<T> const& theList, OTC_ShallowCopy);
				// Creates an alias for <theList>.
				// The same items will be referenced
				// by each list, however, the iterators
				// will be independent.

    // = DESTRUCTION

			~OTC_IList();
				// Invokes <removeAll()> to kill all buckets
				// in the list.

    // = ASSIGNMENT

    OTC_IList<T>&	operator=(OTC_IList<T> const& theList);
				// Replaces this list with a copy of
				// <theList>. The iterator is reset
				// to being off the start of the list.

    // = POPULATION

    u_int		population() const
				{ return list_.population(); }
				// Returns the number of items in the list.

    bool		isEmpty() const
				{ return list_.population() == 0; }
				// Returns <true> if the list is empty.

    // = ITERATION

    T&			item()
				{ return item_(); }
				// If the iterator is located over a valid
				// item, a reference to the item is returned.
				// If there is not a valid item under the
				// iterator, an exception is raised.

    T const&		item() const
				{ return item_(); }
				// If the iterator is located over a valid
				// item, a reference to the item is returned.
				// If there is not a valid item under the
				// iterator, an exception is raised.

    bool		isValid() const
				{
				  return iter_->isLink() &&
				   !iter_->link()->isDead();
				}
				// Returns <true> if the iterator is
				// located over a live item.

    bool		isStart() const
				{ return iter_->isStart(); }
				// Returns <true> if the iterator is
				// located off the start of the list.

    bool		isEnd() const
				{ return iter_->isEnd(); }
				// Returns <true> if the iterator is
				// located off the end of the list.

    void		next() const
				{ iter_->next(); }
				// Moves the iterator onto the next item.

    void		prev() const
				{ iter_->prev(); }
				// Moves the iterator onto the previous item.

    void		resetFirst() const
				{ iter_->resetFirst(); }
				// Moves the iterator to the first item in
				// the list. If the list is empty, the
				// iterator will be placed off the end of
				// the list.

    void		resetLast() const
				{ iter_->resetLast(); }
				// Moves the iterator to the last item in
				// the list. If the list is empty, the
				// iterator will be placed off the start of
				// the list.

    // = APPLICATORS

    void		apply(
			 OTC_Visitor<T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Applies <theApplicator> to each of the
				// items in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

    void		apply(
			 OTC_Worker<T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Applies <theApplicator> to each of the
				// items in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

    // = ADDITION

    void		addBefore(T const& theItem);
				// Adds <theItem> before the item where the
				// iterator is currently located. If the
				// iterator is located off the end of the
				// list, the effect is that <theItem> is
				// appended to the list. If the iterator is
				// off the start of the list, the effect
				// is that <theItem> is prepended to the
				// list. After the operation, the iterator
				// will be located on the newly inserted
				// item.

    void		addAfter(T const& theItem);
				// Adds <theItem> after the item where the
				// iterator is currently located. If the
				// iterator is located off the end of the
				// list, the effect is that <theItem> is
				// appended to the list. If the iterator is
				// off the start of the list, the effect
				// is that <theItem> is prepended to the
				// list. After the operation, the iterator
				// will be located on the newly inserted
				// item.

    void		addFirst(T const& theItem)
				{ list_.addFirst(link_(theItem)); }
				// Adds <theItem> at the head of the list.
				// The location of the iterator is unchanged.

    void		addLast(T const& theItem)
				{ list_.addLast(link_(theItem)); }
				// Adds <theItem> after the last item in
				// the list. The location of the iterator
				// is unchanged.

    // = LOOKING

    T&			first()
				{ return first_(); }
				// If the list is not empty, a reference
				// is returned to the item on the head of
				// the list. If the list is empty, an
				// exception is raised.

    T const&		first() const
				{ return first_(); }
				// If the list is not empty, a reference is
				// returned to the item on the head of the
				// list. If the list is empty, an exception
				// is raised.

    T&			last()
				{ return last_(); }
				// If the list is not empty, a reference is
				// returned to the last item in the list. If
				// the list is empty, an exception is raised.

    T const&		last() const
				{ return last_(); }
				// If the list is not empty, a reference is
				// returned to the last item in the list. If
				// the list is empty, an exception is raised.

    T*			pFirst()
				{ return list_.isEmpty() ? 0 : &first_(); }
				// If the list is not empty, a pointer
				// is returned to the item on the head of
				// the list. If the list is empty, a null
				// pointer is returned.

    T const*		pFirst() const
				{ return list_.isEmpty() ? 0 : &first_(); }
				// If the list is not empty, a pointer
				// is returned to the item on the head of
				// the list. If the list is empty, a null
				// pointer is returned.

    T*			pLast()
				{ return list_.isEmpty() ? 0 : &last_(); }
				// If the list is not empty, a pointer is
				// returned to the last item in the list. If
				// the list is empty, a null pointer is
				// returned.

    T const*		pLast() const
				{ return list_.isEmpty() ? 0 : &last_(); }
				// If the list is not empty, a pointer is
				// returned to the last item in the list. If
				// the list is empty, a null pointer is
				// returned.

    // = REMOVING

    void		removeAll()
				{ iter_->resetStart(); list_.removeAll(); }
				// Kills all buckets holding items in the
				// list. The iterator is reset to being off
				// the start of the list.

    void		remove();
				// If the iterator is located over a valid
				// item, the bucket which is holding the item
				// is killed. The bucket is only unlinked
				// from the list and destroyed when the
				// iterator is moved. If the iterator is
				// not over a valid link within the list,
				// an exception is raised.

    void		removeFirst(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeFirst(theProtection); }
				// If the list is not empty, the first
				// item in the list is killed. If the
				// internal iterator is not located on
				// the first item, it will be removed
				// immediately. If the list were empty,
				// an exception is raised unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    void		removeLast(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeLast(theProtection); }
				// If the list is not empty, the last
				// item in the list is killed. If the
				// internal iterator is not located on
				// the first item, it will be removed
				// immediately. If the list were empty,
				// an exception is raised unless
				// <theProtection> is <OTCLIB_UNSAFE>.

  public:

                        operator OTC_DequeHI<T>() const
                                { return list_; }
                                // Special to use of <OTC_MListIterator>
                                // and <OTC_MListModifier>.

                        operator OTC_DequeHI<T>()
                                { return list_; }
                                // Special to use of <OTC_MListIterator>
                                // and <OTC_MListModifier>.

  private:

    T&			item_() const;
				// If the iterator is located over a valid
				// item, a reference to the item is
				// returned. If there is not a valid item
				// under the iterator, an exception is
				// raised.

    T&			first_() const;
				// If the list is not empty, a reference
				// is returned to the item on the head of
				// the list. If the list is empty, an
				// exception is raised.

    T&			last_() const;
				// If the list is not empty, a reference
				// is returned to the last item in
				// the list. If the list is empty, an
				// exception is raised.

    OTC_Link*		link_(T const& theItem);
				// Creates an instance of <OTC_Bucket>
				// to hold <theItem>.

    OTC_DequeI		list_;
				// The actual items.

    OTC_LinkIterator*	iter_;
				// Internal iterator over items in the list.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/ilist.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ILIST_HH */
