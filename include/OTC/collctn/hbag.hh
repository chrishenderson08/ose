#ifndef OTC_COLLCTN_HBAG_HH
#define OTC_COLLCTN_HBAG_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/hbag.hh
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

#include <OTC/collctn/iterator.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/hashtble.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/hbag.hh"
#endif
#pragma interface "OTC/collctn/hbag.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_HBag
    // = TITLE
    //	   Class implementing a bag of objects.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class implements a collection for maintaining a set of
    //	   objects and for determining membership of that set. Commensurate
    //	   with the properties of a bag, duplicate items are allowed.
    //	   
    //	   Once an item is placed into the bag, it cannot be retrieved from
    //	   the set. If this behaviour is required, one of the map classes
    //	   should be used.
    //	   
    //	   The type of the item placed into the bag, must provide a way of
    //	   generating a hash value for the item. This can be defined
    //	   by providing a template override version of <OTC_HashActions>.
    //	   A way of comparing two items needs also to be defined. This
    //	   can be defined by providing a template override version of
    //	   the <OTC_RankActions> class.
    //	   
    // = NOTES
    //	   When a pointer type is used as the item, and the hash value
    //	   generated from it is based, not on the pointer, but on some part of
    //	   the object being pointed at, it is important that the pointer be
    //	   of type pointer to const object. You should define the bag as
    //	   <OTC_HBag\<EX_Foo const*\>> and not just <OTC_HBag\<EX_Foo*\>>. If
    //	   this is not done, it would be possible for a user to modify parts
    //	   of the item from which the hash value is generated. If the hash
    //	   value for an object changes, it will no longer be accessible.
    //	   
    //	   The <OTC_Bucket> class is used internally to hold the items. Thus
    //	   the <OTC_BaseActions> class may be used to provide actions to be
    //	   performed when items are inserted or removed from the bag.
    //	   
    //	   Since an attempt to remove an item from the bag which is not there,
    //	   will generate an exception, it is important to check first that an
    //	   item is actually in the bag, by calling the <contains()> function.
    //	   
    // = SEE ALSO
    //	   <OTC_Iterator>, <OTC_Bucket>, <OTC_BaseActions>,
    //     <OTC_HashActions>, <OTC_RankActions>
{
  public:

    virtual		~OTC_HBag();

    // = INITIALISATION

			OTC_HBag(int (*theRankFn)(T const&,T const&)=0);
				// Creates an empty bag. <theRankFn> is an
				// optional comparison function to use in
				// place of <OTC_RankActions>.

			OTC_HBag(OTC_HBag<T> const& theBag);
				// Creates a copy of <theBag>.

                        OTC_HBag(OTC_HBag<T> const& theBag, OTC_ShallowCopy);
                                // Creates a bag which is a shallow copy
                                // of <theBag>. That is, both reference
                                // the same data with changes in one being
                                // seen in the other.

    // = INSERTION

    OTC_HBag<T>&	operator=(OTC_HBag<T> const& theBag);
				// Replaces the contents of this bag with
				// the items contained in <theBag>.
				// Returns a reference to this bag.

    void		add(T const& theItem);
				// Adds <theItem> to the bag.

    void		add(OTC_Iterator<T> theItems);
				// Adds items accessable from the current
				// point of <theItems> to its end, into
				// this bag. The original iterator will be
				// left at its end point.

    // = REMOVAL

    void		removeAll();
				// Removes all items in this bag.

    void		removeAll(T const& theItem);
				// Removes all occurrences of <theItem> in
				// the bag. If <theItem> doesn't exist,
				// nothing is done.

    void		remove(
			 T const& theItem,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// If <theItem> is in this bag, the first
				// occurrence of it found will be removed and
				// deleted. If <theItem> is not in this bag,
				// an exception will be raised, unless
				// <theProtection> is set to <OTCLIB_UNSAFE>.

    // = SEARCH

    bool		contains(T const& theItem) const;
				// Returns <true> if <theItem> is contained
				// in this bag.

    u_int		frequency(T const& theItem) const;
				// Returns the number of times that
				// <theItem> occurs in the bag.

    // = RETRIEVAL

    T const&		pickItem() const;
				// Returns an item from the collection. If
				// the bag is empty, an exception is raised.
				// The function is not guaranteed to return
				// a different item on each call.

    // = QUERY

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of items in this bag.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> is the collection
				// is empty.

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
    //     indicated by a value of <OTCLIB_BACKWARD>. The default value
    //     is <OTCLIB_FORWARD>. In the <OTC_HBag> class, traversal in
    //     the forward direction will result in the first item being
    //     accessible being the oldest item in the set. Moving the
    //     iterator will result in successively newer items in the set
    //     being accessible.
    //
    //     When iterating over items, in the order in which they were
    //     inserted, be very careful about inserting new items into the set.
    //     This is because any new items inserted, will also be seen by the
    //     iterator. If you were inserting a new item for every item seen,
    //     you would end up with an infinite loop.

    OTC_Iterator<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over the bag.

  protected:

    // = RANKING

    virtual int		rank(T const& item1, T const& item2) const;
				// Rank two items.

  private:

    OTC_HashTable*      index_;
                                // Hash table for index.

    OTC_LinkList*       list_;
                                // List of items.

    int			(*rankFn_)(T const&,T const&);
				// Comparison function to use in place
				// of <OTC_RankActions>.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/hbag.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HBAG_HH */
