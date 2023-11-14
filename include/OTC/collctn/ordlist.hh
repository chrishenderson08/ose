#ifndef OTC_COLLCTN_ORDLIST_HH
#define OTC_COLLCTN_ORDLIST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ordlist.hh
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
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/range.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/avllnode.hh>
#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/shlwcopy.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/ordlist.hh"
#endif
#pragma interface "OTC/collctn/ordlist.hh"
#endif
#endif

#ifdef index
#undef index
#endif

/* ------------------------------------------------------------------------- */

enum OTC_Occurrence
{
  OTCLIB_FIRST = -1,
  OTCLIB_ANY = 0,
  OTCLIB_LAST = 1
};

enum OTC_Placement
{
  OTCLIB_LT,
  OTCLIB_LE,
  OTCLIB_EQ,
  OTCLIB_GE,
  OTCLIB_GT
};

template<class T>
class OTC_OrderedList
    // = TITLE
    //	   A collection class based on an ordered list.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   <OTC_OrderedList> implements a list of objects, where the order of
    //	   objects is always maintained. This is achieved by ranking objects,
    //	   so that objects of least rank are located at the head of the list.
    //	   The ordering can be dictated by the user by defining an explicit
    //	   version of <OTC_RankActions>, for the type that the list is
    //	   parameterised over.
    //	   
    // = NOTES
    //	   The <OTC_Bucket> class is used internally to hold items in
    //	   the list. Thus, the <OTC_BaseActions> class may be used to provide
    //	   actions to be performed, when items are inserted or removed from
    //	   the list.
    //
    //	   Duplicates are allowed in the list. However users should be aware,
    //	   that the order in which similarly ranked items are ordered is
    //	   undefined. When adding in a duplicate, it may be added before, or
    //	   after the existing item. In addition, when removing an item using
    //	   an instance of an item, if there is a similarly ranked item found
    //	   before the actual one being used as the lookup item, it will be
    //	   removed. If more deterministic control is required, the
    //	   <OTC_AVLTree> class may be used to implement a new list.
    //	   
    //	   When parameterised over a pointer type, where the rank is based on
    //	   a part of the object and not the value of the pointer, it is
    //	   important that the list be parameterised over a pointer to <const>
    //	   object. If this is not done, it would be possible for someone
    //	   to change the object while it is in the list, thus corrupting the
    //	   ordering within the list.
    //	   
    // = SEE ALSO
    //	   <OTC_Iterator>, <OTC_Bucket>, <OTC_BaseActions>,
    //     <OTC_RankActions>, <OTC_AVLTree>
{
  public:

    virtual		~OTC_OrderedList();

    // = INITIALISATION

			OTC_OrderedList(int (*theRankFn)(T const&,T const&)=0);
				// Creates an empty list. <theRankFn> is
				// a rank function to be used instead of
				// <OTC_RankActions>.

			OTC_OrderedList(OTC_OrderedList<T> const& theList);
				// Creates a copy of <theList>.

			OTC_OrderedList(
			 OTC_OrderedList<T> const& theList,
			 OTC_ShallowCopy
			);
				// Creates an alias for <theList>.

    // = DESTRUCTION

    void		removeAll();
				// Removes all items in the list.

    // = INSERTION

    OTC_OrderedList<T>& operator=(OTC_OrderedList<T> const& theList);
				// Replaces the contents of this list with
				// the items contained in <theList>.
				// Returns a reference to this list.

    u_int		add(T const& theItem);
				// Inserts <theItem> into the list. Note that
				// duplicates are allowed. If you do not want
				// duplicates, you should check by calling
				// the <contains()> function for an existing
				// item, before adding the item in. The index
				// where the item was added will be
				// returned, with <0> being the first
				// location. Note, that with subsequent
				// additions, this index may become invalid.

    void		add(OTC_Iterator<T> theItems);
				// Inserts items accessable from the current
				// point of <theItems> to its end, into
				// this list. The original iterator will be
				// left at its end point.

    // = QUERY

    int			index(
			 T const& theItem,
			 OTC_Occurrence theOccurrence=OTCLIB_ANY,
			 OTC_Placement thePlacement=OTCLIB_EQ
			) const;
				// Returns the index of <theItem> within the
				// list. If there are multiple occurrences of
				// <theItem>, the index of which item is
				// returned, is determined by <theOccurrence>,
				// which can be either <OTCLIB_ANY>,
				// <OTCLIB_FIRST> or <OTCLIB_LAST>, indicating
				// any item, the item with lowest index, or
				// the item with highest index respectively.
				// If <theItem> doesn't exist, then <-1> is
				// returned. <thePlacement> modifies what
				// is being searched for. If <thePlacement>
				// is <OTCLIB_EQ> the search will be as
				// described above. If <OTCLIB_LT>, the above
				// search will apply to the range of items
				// which are less than <theItem>. Ie.,
				// <OTCLIB_ANY> will result in any item which
				// is less than <theItem> being returned.
				// <OTCLIB_FIRST> will result in the first
				// item in the range of items less than
				// <theItem> and <OTCLIB_LAST> the last item
				// in the range of items less than <theItem>.
				// Other values for <thePlacement> are
				// <OTCLIB_GT>, <OTCLIB_LE> and <OTCLIB_GE>.
				// For these values the search is similarly
				// modified so as to apply to the items
				// greater than <theItem>, the items less
				// than and including <theItem>, and the
				// items greater than and including <theItem>.

    OTC_Range		range(T const& theItem) const;
				// Returns the range of indices occupied
				// by <theItem>.

    OTC_Range		range(
			 T const& theItem,
			 int (*theRankFn)(T const&, T const&)
			) const;
				// Returns the range of indices occupied by
				// items which rank equal to <theItem>
				// according to <theRankFn>. Note that
				// <theRankFn> must define a partial ordering
				// over the total ordering of the rank
				// function used by the list when inserting
				// items. That is, the items which match
				// using <theRankFn> must all be adjacent to
				// each other in the list. <theItem> will
				// always be passed as the first argument to
				// <theRankFn>.

    T const&		item(u_int theIndex) const;
				// Returns a reference to the item in the
				// list at the location <theIndex>. An
				// exception is raised if the index is
				// invalid.

    T const&		first() const;
				// Returns a reference to the first item in
				// the list. If the list is empty, an
				// exception is raised.

    T const&		last() const;
				// Returns a reference to the last item in
				// the list. If the list is empty, an
				// exception is raised.

    T const*		pItem(u_int theIndex) const
				{
				  return theIndex >= index_->population()
				   ? 0 : &item(theIndex);
				}
				// Returns a pointer to the item in the
				// list at the location <theIndex>. A null
				// pointer is returned if the list is empty
				// or the index is out of range.

    T const*		pFirst() const
				{
				  return index_->isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_->first())->item();
				}
				// Returns a reference to the first item in
				// the list. If the list is empty, a null
				// pointer is returned.

    T const*		pLast() const
				{
				  return index_->isEmpty() ? 0 :
				   &((OTC_Bucket<T>*)list_->last())->item();
				}
				// Returns a reference to the last item in
				// the list. If the list is empty, a null
				// pointer is returned.

    bool		contains(T const& theItem) const;
				// Returns <true> if <theItem> exists.

    u_int		frequency(T const& theItem) const;
				// Returns the number of times that <theItem>
				// appears in the list.

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of items in the list.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> if the collection is empty.

    // = REMOVAL

    void		removeAll(T const& theItem);
				// Removes all items equivalent to <theItem>.

    void		removeFirst(OTC_Protection theProtection=OTCLIB_SAFE);
				// Removes the first item in this list.
				// If the list is empty, an exception is
				// raised unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeLast(OTC_Protection theProtection=OTCLIB_SAFE);
				// Removes the last item in this list.
				// If the list is empty, an exception is
				// raised unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		remove(
			 T const& theItem,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Remove first equivalent item to <theItem>
				// found. Raises an exception if no
				// equivalent item is found unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    void		removeItem(
			 u_int theIndex,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Remove the item at the location
				// <theIndex>. Raises an exception if
				// <theIndex> is invalid unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    void		removeRange(
			 OTC_Range const& theRange,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  removeRange_(
				   theRange.lower(),
				   theRange.length(),
				   theProtection
				  );
				}
				// Removes items at locations <theStart>
				// through <theEnd>. If the list is
				// empty, unless the length of the range
				// is zero, or any of the items do not
				// exist, an exception is raised unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    void		removeRange(
			 u_int theStart,
			 u_int theLength,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  removeRange_(
				   theStart,
				   theLength,
				   theProtection
				  );
				}
				// Removes items in the range with length
				// <theLength> starting at index <theStart>.
				// If the list is empty, unless the length of
				// the range is zero, or any of the items do
				// not exist, an exception is raised unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    // = ITERATION
    //     By default, iterators will perform reference counts on the
    //     buckets in the collection as the iterator moves over the items.
    //     Performing the reference counts ensures that the iterator
    //     is not corrupted by additions or removals to the collection.
    //     If an unsafe iterator is required, for example, within the bounds
    //     of a read operation against data protected by a reader/writer
    //     mutex, a second argument can be passed to the following functions.
    //     The value of this argument is either <OTCLIB_SAFE> or
    //     <OTCLIB_UNSAFE>. To get an unsafe iterator, the <OTCLIB_UNSAFE>
    //     argument should be used.
    //     
    //     The first argument to the following functions indicates the
    //     direction of traversal of the iterator. Traversal in the
    //     direction of first to last item is indicated by a value of
    //     <OTCLIB_FORWARD>. Traversal in the reverse direction is
    //     indicated by a value of <OTCLIB_BACkWARD>. The default value
    //     is <OTCLIB_FORWARD>. In the <OTC_OrderedList> class, traversal
    //     in the forward direction will result in the first item being
    //     available being the least ranked item.

    OTC_Iterator<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over the list.

    OTC_Iterator<T>	items(
			 OTC_Range const& theRange,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return items_(
				   theRange.lower(),theRange.length(),
				   theDirection,theProtection
				  );
				}
				// Returns an iterator over a range of items
				// in the list.

    OTC_Iterator<T>	items(
			 u_int theStart,
			 u_int theLength,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const
				{
				  return items_(
				   theStart,theLength,
				   theDirection,theProtection
				  );
				}
				// Returns an iterator over a range of items
				// in the list.

    OTC_Iterator<T>     items(
                         T const& theItem,
                         int (*theRankFn)(T const&,T const&),
                         OTC_Direction theDirection=OTCLIB_FORWARD,
                         OTC_Protection theProtection=OTCLIB_SAFE
                        ) const;
                                // Returns an iterator over items in the list
                                // which rank equal to <theItem> according to
                                // <theRankFn>. Note that <theRankFn> must
                                // define a partial ordering over the total
                                // ordering of the rank function used by the
                                // list when inserting items. That is, the
                                // items which match using <theRankFn> must
                                // all be adjacent to each other in the list.
                                // <theItem> will always be passed as the
                                // first argument to <theRankFn>.

  protected:

    // = RANKING

    virtual int		rank(T const& item1, T const& item2) const;
				// Ranks two items.

  private:

    void		removeRange_(
			 int theStart,
			 u_int theLength,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Removes items in the range with length
				// <theLength>, starting at index <theStart>.
				// If the list is empty, unless the length
				// of the range is zero, or any of the items
				// do not exist, an exception is raised
				// unless <theProtection> is <OTCLIB_UNSAFE>.

    OTC_Cursor<T>*	items_(
			 int theStart,
			 u_int theLength,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over a range of items
				// in the list.

    OTC_AVLLinkNode*    range_(
                         OTC_AVLLinkNode* theRoot,
                         T const& theItem,
                         int (*theRankFn)(T const&, T const&),
                         OTC_AVLLinkNode*& theStartNode,
                         OTC_AVLLinkNode*& theEndNode
                        ) const;
                                // Searches for the node in the tree with
                                // <theRoot> corresponding to <theItem>
                                // according to <theRankFn>. If no such
                                // item is found, a null value is returned.
                                // If such an item is found, a pointer to
                                // that node is returned. Also, when the
                                // item is found, the range of nodes matching
                                // <theItem> is calculated, with the start
                                // and end of the range returned through
                                // <theStartNode> and <theEndNode>.

    OTC_AVLTree*	index_;
				// The tree used to build the search tree
				// on the list.

    OTC_LinkList*	list_;
				// The actual list of items.

    int			(*rankFn_)(T const&,T const&);
				// Comparison function to be used in
				// place of <OTC_RankActions>.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/ordlist.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ORDLIST_HH */
