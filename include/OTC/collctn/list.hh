#ifndef OTC_COLLCTN_LIST_HH
#define OTC_COLLCTN_LIST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/list.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/modifier.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/range.hh>
#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/listi.hh>
#include <OTC/collctn/shlwcopy.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/list.hh"
#endif
#pragma interface "OTC/collctn/list.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_List
    // = TITLE
    //	   A collection class based on a list.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   Class implementing a list of objects. Objects can be accessed at
    //	   the ends of the list, or by their location in the list. Location
    //	   indexes start at <0>.
    //
    // = NOTES
    //	   Indexes are not an inherent part of the list structure, but are
    //	   implemented by a height balanced AVL tree. This means that the
    //	   space consumed by the list is twice that of a list which doesn't
    //	   allow access by indexes. Thus, if access is not required by index
    //	   it is suggested that <OTC_Deque> be used instead.
    //	   
    //	   The <OTC_Bucket> class is used internally to hold items in
    //	   the list. Thus the <OTC_BaseActions> class may be used to provide
    //	   actions to be performed, when items are inserted or removed from
    //	   the list.
    //
    // = SEE ALSO
    //	   <OTC_Iterator>, <OTC_Modifier>, <OTC_Bucket>, <OTC_BaseActions>
{
  public:

			~OTC_List() {}

    // = INITIALISATION

			OTC_List() {}
				// Creates an empty list.

			OTC_List(OTC_List<T> const& theList);
				// Creates a copy of <theList>.

			OTC_List(OTC_List<T>& theList, OTC_ShallowCopy);
                                // Creates a list which is a shallow copy
                                // of <theList>. That is, both reference
                                // the same data with changes in one being
                                // seen in the other.

    // = ASSIGNMENT

    OTC_List<T>&	operator=(OTC_List<T> const& theList);
				// Replaces the contents of this list with
				// items contained in <theList>.

    // = QUERY

    u_int		population() const
				{ return list_.population(); }
				// Returns the number of items in the
				// list.

    bool		isEmpty() const
				{ return list_.isEmpty(); }
				// Returns <true> if the the list is empty.

    // = ADDITION

    void		addFirst(T const& theItem)
				{ list_.addFirst(link_(theItem)); }
				// Inserts <theItem> at the head of the list.

    void		addFirst(OTC_Iterator<T> theItems);
				// Adds items accessable from the current
				// point of <theItems> to its end, to the
				// head of this list. The order of items
				// accessed using the iterator is preserved
				// in the list when they are added. The
				// original iterator will be left at its end
				// point.

    void		addLast(T const& theItem)
				{ list_.addLast(link_(theItem)); }
				// Appends <theItem> to the tail of the list.

    void		addLast(OTC_Iterator<T> theItems);
				// Adds items accessable from the current
				// point of <theItems> to its end, to the
				// tail of this list. The original iterator
				// will be left at its end point.

    void		addBeforeItem(T const& theItem, u_int theIndex)
				{
				  list_.addBeforeItem(link_(theItem),
				   theIndex);
				}
				// Adds <theItem> into the list before
				// the item at location <theIndex>.
				// If <theIndex> is equal to the population
				// of the list, then <theItem> is added
				// at the end of the list. If <theIndex>
				// is greater than the population of the
				// list, an exception is raised.

    void		addBeforeItem(
    			 OTC_Iterator<T> theItems,
			 u_int theIndex
			);
				// Adds items accessable from the current
				// point of <theItems> to its end, into
				// the list before the item at location
				// <theIndex>. The order of items
				// accessed using the iterator is preserved
				// in the list when they are added. The
				// original iterator will be left at its end
				// point. If <theIndex> is equal to the
				// population of the list, then the items are
				// added at the end of the list. If
				// <theIndex> is greater than the population
				// of the list, an exception is raised.

    // = REPLACEMENT

    void		replaceFirst(T const& theItem)
				{ replaceItem_(theItem,0); }
				// Replaces the first item in the list
				// with <theItem>. Raises an exception
				// if the list is empty.

    void		replaceLast(T const& theItem)
				{ replaceItem_(theItem,list_.population()-1); }
				// Replaces the last item in the list
				// with <theItem>. Raises an exception
				// if the list is empty.

    void		replaceItem(T const& theItem, u_int theIndex)
				{ replaceItem_(theItem,theIndex); }
				// Replaces the item in the list at
				// <theIndex> position with <theItem>. Raises
				// an exception if <theIndex> doesn't
				// describe a valid position in the list.

    // = ACCESS

    T&			first()
				{
				  return ((OTC_Bucket<T>*)
				   list_.first())->item();
				}
				// Returns a reference to the first item in
				// the list. If the list is empty, an
				// exception is raised.

    T const&		first() const
				{
				  return ((OTC_Bucket<T>*)
				   list_.first())->item();
				}
				// Returns a reference to the first item in
				// the list. If the list is empty, an
				// exception is raised.

    T&			last()
				{
				  return ((OTC_Bucket<T>*)
				   list_.last())->item();
				}
				// Returns a reference to the last item in
				// the list. If the list is empty, an
				// exception is raised.

    T const&		last() const
				{
				  return ((OTC_Bucket<T>*)
				   list_.last())->item();
				}
				// Returns a reference to the last item in
				// the list. If the list is empty, an
				// exception is raised.

    T&			item(u_int theIndex)
				{
				  return ((OTC_Bucket<T>*)
				   list_.item(theIndex))->item();
				}
				// Returns a reference to the item in the
				// list at the location <theIndex>. If the
				// list is empty, or the index is outside the
				// bounds of the list, an exception is
				// raised.

    T const&		item(u_int theIndex) const
				{
				  return ((OTC_Bucket<T>*)
				   list_.item(theIndex))->item();
				}
				// Returns a reference to the item in the
				// list at the location <theIndex>. If the
				// list is empty, or the index is outside the
				// bounds of the list, an exception is
				// raised.

    T*			pFirst()
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.first())->item();
				}
				// Returns a pointer to the first item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T const*		pFirst() const
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.first())->item();
				}
				// Returns a pointer to the first item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T*			pLast()
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.last())->item();
				}
				// Returns a pointer to the last item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T const*		pLast() const
				{
				  return list_.isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_.last())->item();
				}
				// Returns a pointer to the last item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T*			pItem(u_int theIndex)
				{
				  return theIndex >= list_.population()
				   ? 0 : &((OTC_Bucket<T>*)list_.item(
				   theIndex))->item();
				}
				// Returns a pointer to the item in the
				// list at the location <theIndex>. If the
				// list is empty, or the index is outside the
				// bounds of the list, a null pointer is
				// returned.

    T const*		pItem(u_int theIndex) const
				{
				  return theIndex >= list_.population()
				   ? 0 : &((OTC_Bucket<T>*)list_.item(
				   theIndex))->item();
				}
				// Returns a reference to the item in the
				// list at the location <theIndex>. If the
				// list is empty, or the index is outside the
				// bounds of the list, a null pointer is
				// returned.

    // = REMOVAL

    void		removeAll()
				{ list_.removeAll(); }
				// Removes all items from the list.

    void		removeFirst(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeItem(0,theProtection); }
				// Removes the first item in the list. If
				// the list is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeLast(OTC_Protection theProtection=OTCLIB_SAFE)
				{
				  list_.removeItem(list_.population()-1,
				   theProtection);
				}
				// Removes the last item in the list. If
				// the list is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeItem(
			 u_int theIndex,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{ list_.removeItem(theIndex,theProtection); }
				// Removes the item at location <theIndex>.
				// Raises an exception if there is no item at
				// location <theIndex>, unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    void		removeRange(
			 u_int theStart,
			 u_int theLength,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  list_.removeRange(theStart,theLength,
				   theProtection);
				}
				// Removes <theLength> items starting at
				// location <theStart>. Raises an exception
                                // if the range specified is invalid, unless
                                // <theProtection> is <OTCLIB_UNSAFE>.

    void		removeRange(
			 OTC_Range theRange,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  list_.removeRange(theRange.lower(),
				   theRange.length(),theProtection);
				}
				// Removes <theLength> items starting at
				// location <theStart>. Raises an exception
                                // if the range specified is invalid, unless
                                // <theProtection> is <OTCLIB_UNSAFE>.

    // = ITERATION
    //	   By default, iterators will perform reference counts on the
    //	   buckets in the collection as the iterator moves over the items.
    //	   Performing the reference counts ensures that the iterator
    //	   is not corrupted by additions or removals to the collection.
    //     If an unsafe iterator is required, for example, within the bounds
    //     of a read operation against data protected by a reader/writer
    //     mutex, a second argument can be passed to the following functions.
    //     The value of this argument is either <OTCLIB_SAFE> or
    //     <OTCLIB_UNSAFE>. To get an unsafe iterator, the <OTCLIB_UNSAFE>
    //     argument should be used.
    //     
    //     The second to last argument to the following functions indicates
    //     the direction of traversal of the iterator. Traversal in the
    //     direction of first to last item is indicated by a value of
    //     <OTCLIB_FORWARD>. Traversal in the reverse direction is indicated
    //     by a value of <OTCLIB_BACKWARD>. The default value is
    //     <OTCLIB_FORWARD>.

    OTC_Iterator<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{ return items_(theDirection,theProtection); }
				// Returns an iterator over the list.

    OTC_Modifier<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{ return items_(theDirection,theProtection); }
				// Returns an iterator over the list.

    OTC_Iterator<T>	items(
			 OTC_Range const& theRange,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return items_(theRange.lower(),
				   theRange.length(),theDirection,
				   theProtection);
				}
				// Returns an iterator over a range of
				// items in the list.

    OTC_Modifier<T>	items(
			 OTC_Range const& theRange,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  return items_(theRange.lower(),
				   theRange.length(),theDirection,
				   theProtection);
				}
				// Returns an iterator over a range of
				// items in the list.

    OTC_Iterator<T>	items(
			 u_int theStart,
			 u_int theLength,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return items_(theStart,theLength,
				   theDirection,theProtection);
				}
				// Returns an iterator over a range of
				// items in the list.

    OTC_Modifier<T>	items(
			 u_int theStart,
			 u_int theLength,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  return items_(theStart,theLength,
				   theDirection,theProtection);
				}
				// Returns an iterator over a range of
				// items in the list.

  private:

    OTC_Cursor<T>*	items_(
			 OTC_Direction theDirection,
			 OTC_Protection theProtection
			) const;
				// Returns a cursor over the list.

    OTC_Cursor<T>*	items_(
			 int theStart,
			 u_int theLength,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns a cursor over a portion of
				// the list.

    OTC_Link*		link_(T const& theItem);
				// Creates an instance of <OTC_Bucket> to
				// hold <theItem>.

    void		replaceItem_(T const& theItem, int theIndex);
				// Replaces the item in the list at
				// <theIndex> position with <theItem>. Raises
				// an exception if <theIndex> doesn't
				// describe a valid position in the list.

    OTC_ListI		list_;
				// The actual list.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/list.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LIST_HH */
