#ifndef OTC_COLLCTN_MLSTITER_HH
#define OTC_COLLCTN_MLSTITER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/mlstiter.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_COLLCTN_MLIST_HH
#include <OTC/collctn/mlist.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/mlstiter.hh"
#pragma interface "OTC/collctn/mlstiter.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_MListIterator
    // = TITLE
    //	   Iterator over basic lists. No modification allowed.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     This class is an iterator which will work on <OTC_MList>,
    //     <OTC_IList> or <OTC_Deque>. Modification of items through the
    //     iterator is not permitted. If modification of items held by the
    //     list, through the iterator is required, the <OTC_MListModifier>
    //     class should be used.
    //     
    //	   This iterator can be moved both forward and backwards. The
    //	   life of the iterator, cannot extend beyond the life of the
    //	   list being iterated over.
    //	   
    // = SEE ALSO
    //	   <OTC_MListModifier>, <OTC_Deque>, <OTC_IList>, <OTC_MList>
{

  protected:

    T&			item_() const
				{
				  OTCLIB_ENSURE_FN((iter_.isLink()),
				   "OTC_MListIterator<T>::item()",
				   "iterator not on link");
				  return ((OTC_Bucket<T>*)
				   iter_.link())->item();
				}
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no item under the
				// iterator.

    OTC_Link*           link_(T const& theItem)
				{
				  OTC_Bucket<T>* theBucket;
				  theBucket = new OTC_Bucket<T>(theItem);
				  OTCLIB_ASSERT_M(theBucket != 0);
				  return theBucket;
				}
				// Creates an instance of <OTC_Bucket>
				// to hold <theItem>.

    void		remove_()
				{ list_.remove(iter_.link()); }
				// If the iterator is located on a valid item
				// in the list, the item is removed from the
				// list.

    void		addBefore_(T const& theItem)
				{
				  if (iter_.isStart())
				    list_.addFirst(link_(theItem));
				  else if (iter_.isEnd())
				    list_.addLast(link_(theItem));
				  else
				  {
				    list_.addBeforeLink(
				     link_(theItem),iter_.link());
				  }
				}
				// Adds <theItem> in the list before the
				// item where the iterator is currently
				// located. If the iterator is at the
				// start of the list, the item will be
				// added as the first item in the list.

    void		addAfter_(T const& theItem)
				{
				  if (iter_.isStart())
				    list_.addFirst(link_(theItem));
				  else if (iter_.isEnd())
				    list_.addLast(link_(theItem));
				  else
				  {
				    list_.addAfterLink(
				     link_(theItem),iter_.link());
				  }
				}
				// Adds <theItem> in the list after the
				// item where the iterator is currently
				// located. If the iterator is at the
				// end of the list, the item will be
				// added as the last item in the list.

  public:

    // = INITIALISATION

			OTC_MListIterator(OTC_MListIterator<T> const& theIter)
			  : list_(((OTC_MListIterator<T>&)theIter).list_),
			    iter_(theIter.iter_) {}
				// Creates an iterator which is initially
				// located at the same position as <theIter>.
				// The iterator thought will operator
				// independently of <theIter>.

			OTC_MListIterator(
			 OTC_DequeHI<T> const& theHandle,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
			  : list_(*(OTC_DequeI*)&theHandle.list_),
			    iter_(theHandle.list_.list()->items(theProtection))
				{ iter_.resetFirst(); }
				// Initialises the iterator. The iterator
				// will be located over the first item in
				// the list referenced by <theHandle>, or if
				// the list is empty at the start of the
				// list. The argument <theProtection>
				// determines if reference counting will
				// actually be performed on links in the
				// list. The default of <OTCLIB_SAFE> will
				// result in reference counting being
				// performed. A value of <OTCLIB_UNSAFE> will
				// turn off reference counting. In this later
				// case, the list should not be modified
				// while the iterator exists as the state of
				// the iterator will be corrupted.

    // = MOVEMENT

    void		prev()
				{ iter_.prev(); }
				// Moves the iterator to the previous item.

    void		next()
				{ iter_.next(); }
				// Moves the iterator to the next item.

    void		resetFirst()
				{ iter_.resetFirst(); }
				// Resets the iterator to the first item,
				// or if the list is empty, at the start
				// of the list.

    void		resetLast()
				{ iter_.resetLast(); }
				// Resets the iterator to the last item,
				// or if the list is empty, at the end
				// of the list.

    // = RETRIEVAL

    T const&		item() const
				{ return item_(); }
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no item under the
				// iterator.

    bool		isValid() const
				{ return iter_.isLink(); }
				// Returns <true> while there is a valid item
				// under the current location of the
				// iterator.

  private:

    OTC_MListIterator<T>&	operator=(OTC_MListIterator<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.

    OTC_DequeI&		list_;
				// Reference to the list being iterated
				// over.

    OTC_LinkIterator	iter_;
				// The real iterator.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_MLSTITER_HH */
