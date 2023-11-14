#ifndef OTC_COLLCTN_TSET_HH
#define OTC_COLLCTN_TSET_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/tset.hh
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
#include <OTC/collctn/replcmnt.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/avltree.hh>
#include <OTC/collctn/avllnode.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/tset.hh"
#endif
#pragma interface "OTC/collctn/tset.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_TSet
    // = TITLE
    //	   Class implementing a set of objects.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   <OTC_TSet> implements a collection for maintaining a set of objects
    //	   and for determining membership of that set. Once an item is placed
    //	   into the set, it cannot be retrieved. If this behaviour is
    //	   required, one of the map classes should be used.
    //	   
    //     Commensurate with the properties of a set, no duplicate items are
    //     allowed. In addition, It must also be possible to compare the
    //     keys. The method of comparing keys can be determined by providing
    //     a template override version of <OTC_RankActions> for the type of
    //     the key.
    //     
    // = NOTES
    //	   When a pointer type is used as the item and the hash value
    //	   generated from it is based, not on the pointer, but on some part
    //	   of the object being pointed at, it is important that the pointer
    //	   be of type pointer to const object. You should define the set as
    //	   <OTC_TSet\<EX_Foo const*\>> and not just <OTC_TSet\<EX_Foo*\>>. If
    //	   this is not done, it would be possible for a user to modify parts
    //	   of the item from which the hash value is generated. If the hash
    //	   value for an object changes, the object will no longer be
    //	   accessible.
    //	   
    //	   The <OTC_Bucket> class is used internally to hold the items. Thus,
    //	   the <OTC_BaseActions> class may be used to provide actions to be
    //	   performed when items are inserted or removed from the set.
    //	   
    //	   Since an attempt to remove an item from the set which is not
    //	   there, will generate an exception, it is important to first check
    //	   that an item is actually in the set, by calling the <contains()>
    //	   function.
    //	   
    // = SEE ALSO
    //	   <OTC_Iterator>, <OTC_Bucket>, <OTC_RankActions>
{
  public:

    virtual		~OTC_TSet();

    // = INITIALISATION

			OTC_TSet(int (*theRankFn)(T const&,T const&)=0);
				// Creates an empty set. <theRankFn> is an
				// optional comparison function to be used in
				// placed of <OTC_RankActions>.

			OTC_TSet(OTC_TSet<T> const& theSet);
				// Creates a copy of <theSet>.

                        OTC_TSet(OTC_TSet<T> const& theSet, OTC_ShallowCopy);
                                // Creates a set which is a shallow copy
                                // of <theSet>. That is, both reference
                                // the same data with changes in one being
                                // seen in the other.

    // = INSERTION

    OTC_TSet<T>&	operator=(OTC_TSet<T> const& theSet);
				// Replaces the contents of this set with
				// the items contained in <theSet>.
				// Returns a reference to this set.

    void		add(
			 T const& theItem,
			 OTC_Replacement theFlag=OTCLIB_ADDONLY
			);
				// If <theItem> is not in this set already,
				// it will be added to this set. If <theItem>
				// is in the set, an exception is raised if
				// <theFlag> is <OTCLIB_ADDONLY>, but if
				// <theFlag> is <OTCLIB_REPLACE>, <theItem>
				// will replace the item already in the set.

    void		add(
    			 OTC_Iterator<T> theItems,
			 OTC_Replacement theFlag=OTCLIB_ADDONLY
			);
				// Adds items accessable from the current
				// point of <theItems> to its end, into
				// this set. The original iterator will be
				// left at its end point. If an item is
				// already in the set, an exception is raised
				// if <theFlag> is <OTCLIB_ADDONLY>, but if
				// <theFlag> is <OTCLIB_REPLACE>, that item
				// will replace the item already in the set.

    // = REMOVAL

    void		removeAll();
				// Removes all items in this set.

    void		remove(
			 T const& theItem,
			 OTC_Protection theFlag=OTCLIB_SAFE
			);
				// If <theItem> is in this set, it will be
				// removed and deleted. If <theItem> is not
				// in this set, an exception will be raised,
				// unless <Flag> is set to <OTCLIB_UNSAFE>.

    // = SEARCH

    bool		contains(T const& theItem) const;
				// Returns <true> if <theItem> is contained
				// in this set.

    // = RETRIEVAL

    T const&            item(T const& theItem) const;
                                // If an item corresponding to <theItem>
                                // exists in this map, a reference to it will
                                // be returned. If an item corresponding to
                                // <theItem> does not exist, an exception
                                // will be raised. Provided for the case
                                // where an item is provided which is
                                // similar enough to match one in the set
                                // but is not exactly the same in other
                                // respects and where the one in the set
                                // is really the one which is required.

    T const*            pItem(T const& theItem) const;
                                // If an item corresponding to <theItem>
                                // exists in this map, a pointer to it will
                                // be returned. If an item corresponding to
                                // <theItem> does not exist, a null pointer
                                // will be returned. Provided for the case
                                // where an item is provided which is
                                // similar enough to match one in the set
                                // but is not exactly the same in other
                                // respects and where the one in the set
                                // is really the one which is required.

    T const&		pickItem() const;
				// Returns an item from the collection. If
				// the set is empty, an exception is raised.
				// The function is not guaranteed to
				// return a different item on each call.

    // = QUERY

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of items in this set.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> if the collection is empty.

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
    //     direction of traversal of the iterator. Traversal in the direction
    //     of first to last item is indicated by a value of <OTCLIB_FORWARD>.
    //     Traversal in the reverse direction is indicated by a value of
    //     <OTCLIB_BACKWARD>. The default value is <OTCLIB_FORWARD>. In the
    //     <OTC_TSet> class, traversal in the forward direction will result
    //     in the first item being that for which the item is least ranked.
    //     Moving the iterator will result in successively higher ranked
    //     items.

    OTC_Iterator<T>	items(
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over the set.

    OTC_Iterator<T>	items(
			 T const& theItem,
			 int (*theRankFn)(T const&,T const&),
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Returns an iterator over items in the set
				// which rank equal to <theItem> according to
				// <theRankFn>. Note that <theRankFn> must
				// define a partial ordering over the total
				// ordering of the rank function used by the
				// set when inserting items. That is, the
				// items which match using <theRankFn> must
				// all be adjacent to each other in the set.
				// <theItem> will always be passed as the
				// first argument to <theRankFn>.

  protected:

    // = RANKING

    virtual int		rank(T const& item1, T const& item2) const;
				// Ranks two items.

  private:

    int                 searchIndex_(
                         OTC_AVLLinkNode* theRoot,
                         T const& theItem,
                         OTC_AVLLinkNode*& theNode
                        ) const;
				// Searches for the node in the tree with
				// in the tree with <theRoot> corresponding
				// to <theItem>. <theNode> will be set the
				// node and zero returned if the key existed.
				// If no node corresponding to <theItem>
				// exists, a non zero value will be returned
				// and <theNode> will be set to the node
				// either side of where <theItem> would have
				// been. If the value returned if less than
				// zero, <theNode> will be the node following
				// where <theItem> should have been,
				// otherwise, it will be the node which
				// preceeded where <theItem> should have
				// been. The return value will be non zero
				// and <theNode> also zero if the index is
				// empty.

    OTC_AVLTree*	index_;
                                // Tree for index.

    OTC_LinkList*       list_;
                                // List of items.

    int			(*rankFn_)(T const&,T const&);
				// Comparison function to be used in
				// place of <OTC_RankActions>.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/tset.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TSET_HH */
