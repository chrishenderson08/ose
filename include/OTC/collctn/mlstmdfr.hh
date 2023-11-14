#ifndef OTC_COLLCTN_MLSTMDFR_HH
#define OTC_COLLCTN_MLSTMDFR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/mlstmdfr.hh
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

#ifndef OTC_COLLCTN_MLSTITER_HH
#include <OTC/collctn/mlstiter.hh>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/mlstmdfr.hh"
#pragma interface "OTC/collctn/mlstmdfr.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_MListModifier : public OTC_MListIterator<T>
    // = TITLE
    //	   Inherits from <OTC_MListIterator> to allow modification of items.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     This class inherits from <OTC_MListIterator> to allow modification
    //     of items held by the list. Items in the list can also be removed
    //     using the iterator.
    //     
    //     This iterator can be moved both forward and backwards. The life of
    //     the iterator, cannot extend beyond the life of the list being
    //     iterated over.
    //     
    // = SEE ALSO
    //	   <OTC_MListIterator>, <OTC_MList>, <OTC_IList>, <OTC_Deque>
{
  public:

    // = INITIALISATION

			OTC_MListModifier(OTC_MListModifier<T> const& theIter)
			  : OTC_MListIterator<T>(theIter) {}
				// Creates an iterator which is inititally
				// located at the same position as <theIter>.
				// The iterator will operate independently of
				// <theIter>.

			OTC_MListModifier(
			 OTC_DequeHI<T> theHandle,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
			  : OTC_MListIterator<T>(theHandle,theProtection)
				{}
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
				// turn of reference counting. In this later
				// case, the list should not be modified
				// while the iterator exists as the state of
				// the iterator will be corrupted.

    // = RETRIEVAL

    T&			item() const
				{ return item_(); }
				// Returns the item under the current
				// location of the iterator. Raises an
				// exception if there is no item under the
				// iterator.

    // = REMOVAL

    void		remove()
				{ remove_(); }
				// If the iterator is located on a valid item
				// in the list, the item is removed from the
				// list.

    // = ADDITION

    void		addBefore(T const& theItem)
				{ addBefore_(theItem); }
				// Adds <theItem> in the list before the
				// item where the iterator is currently
				// located. If the iterator is at the
				// start of the list, the item will be
				// added as the first item in the list.

    void		addAfter(T const& theItem)
				{ addAfter_(theItem); }
				// Adds <theItem> in the list after the
				// item where the iterator is currently
				// located. If the iterator is at the
				// end of the list, the item will be
				// added as the last item in the list.

  private:

    OTC_MListModifier<T>&	operator=(OTC_MListModifier<T> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_MLSTMDFR_HH */
