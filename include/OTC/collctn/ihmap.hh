#ifndef OTC_COLLCTN_IHMAP_HH
#define OTC_COLLCTN_IHMAP_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ihmap.hh
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
#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/visitor.hh>
#include <OTC/collctn/worker.hh>
#include <OTC/collctn/prvisitr.hh>
#include <OTC/collctn/prworker.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/hashtble.hh>
#include <OTC/collctn/shlwcopy.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/ihmap.hh"
#endif
#pragma interface "OTC/collctn/ihmap.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_IHMap
    // = TITLE
    //	   Class implementing a one way map between two objects.
    //	   
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   This class implements a one way mapping between two objects. The
    //	   first of these objects, the key, must be unique and must be able
    //	   to have a hash value generated for it using the <OTC_HashActions>
    //	   class. It must also be possible to compare the keys. The method
    //	   of comparing keys can be determined by providing a template
    //	   override version of <OTC_RankActions> for the type of the key.
    //	   
    // = NOTES
    //	   When a pointer type is used as a key and the hash value generated
    //	   from it, is based not on the pointer, but on some part of the
    //	   object being pointed at, then it is important that the pointer be
    //	   of type pointer to const object. In other words you should define
    //	   the map as <OTC_IHMap\<EX_Foo const*,EX_Bar*\>> and not just
    //	   <OTC_IHMap\<EX_Foo*,EX_Bar*\>>. If this is not done, it would be
    //	   possible for a user to modify parts of the key from which the hash
    //	   value is generated. If the hash value of an object changes, it
    //	   will no longer be accessible.
    //	   
    //	   The <OTC_PairBucket> class is used internally to hold both key and
    //	   item. Thus the <OTC_BaseActions> class may be used to provide
    //	   actions to be performed, when items are inserted or removed from
    //	   the map.
    //
    //	   Since an attempt to access or remove an item from the map, which
    //	   isn't there, will raise an exception, it is important to first
    //	   check that an item is actually in the map by calling the
    //	   <contains()> function.
    //	   
    // = SEE ALSO
    //	   <OTC_HashActions>, <OTC_PairBucket>, <OTC_BaseActions>,
    //	   <OTC_RankActions>
{
  public:

    virtual 		~OTC_IHMap();

    // = INITIALISATION

			OTC_IHMap(int (*theRankFn)(T1 const&,T1 const&)=0);
				// Creates an empty map. <theRankFn> is an
				// optional comparison function to be used in
				// place of <OTC_RankActions> for the key.

			OTC_IHMap(OTC_IHMap<T1,T2> const& theMap);
				// Creates a copy of <theMap>.

                        OTC_IHMap(
                         OTC_IHMap<T1,T2> const& theMap,
                         OTC_ShallowCopy
                        );
                                // Creates a map which is a shallow copy
                                // of <theMap>. That is, both reference
                                // the same data with changes in one being
                                // seen in the other.

    // = DESTRUCTION

    void		removeAll();
				// Removes all key/item pairs in this map.

    // = INSERTION

    OTC_IHMap<T1,T2>&	operator=(OTC_IHMap<T1,T2> const& theMap);
				// Replaces the contents of this map with
				// the contents of <theMap>.

    void		add(T1 const& theKey, T2 const& theItem);
				// If an item corresponding to <theKey> does
				// not exist in this map, then <theItem> will
				// be entered into this map. If an item
				// corresponding to <theKey> already exists,
				// an exception will be raised.

    // = RETRIEVAL/SEARCH

    void		remove(
			 T1 const& theKey,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// If an item corresponding to <theKey>
				// exists in this map, it will be removed
				// and deleted. If an item corresponding to
				// <theKey> does not exist, an exception
				// will be raised unless <theProtection>
				// is <OTCLIB_UNSAFE>.

    bool		contains(T1 const& theKey) const;
				// Returns <true> if this map contains a item
				// corresponding to <theKey>.

    T2&			item(T1 const& theKey)
				{ return item_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a reference to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, an exception will
				// be raised.

    T2 const&		item(T1 const& theKey) const
				{ return item_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a reference to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, an exception will
				// be raised.

    T2*			pItem(T1 const& theKey)
				{ return pItem_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, a null pointer
				// is returned.

    T2 const*		pItem(T1 const& theKey) const
				{ return pItem_(theKey); }
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to it will
				// be returned. If an item corresponding to
				// <theKey> does not exist, a null pointer
				// is returned.

    T1 const&		pickKey() const;
				// Returns a key from the collection. If
				// the map is empty, an exception is raised.
				// The function is not guaranteed to
				// return a different key on each call.

    T2 const&		pickItem() const;
				// Returns an item from the collection. If
				// the map is empty, an exception is raised.
				// The function is not guaranteed to
				// return a different item on each call.

    // = QUERY

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of key/item mappings
				// contained in this map.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> if the collection is empty.

    // = ITERATION

    T1 const&		key() const
				{ return key_(); }
				// If the iterator is located over a valid
				// key/item pair, a reference to the key is
				// returned. If there is not a valid key/item
				// pair under the iterator, an exception is
				// raised.

    T2&			item()
				{ return item_(); }
				// If the iterator is located over a valid
				// key/item pair, a reference to the item is
				// returned. If there is not a valid key/item
				// pair under the iterator, an exception is
				// raised.

    T2 const&		item() const
				{ return item_(); }
				// If the iterator is located over a valid
				// key/item pair, a reference to the item is
				// returned. If there is not a valid key/item
				// pair under the iterator, an exception is
				// raised.

    bool		isValid() const
				{
				  return iter_->isLink() &&
				   !iter_->link()->isDead();
				}
				// Returns <true> if the iterator is located
				// over a live key/item pair.

    bool		isStart() const
				{ return iter_->isStart(); }
				// Returns <true> if the iterator is located
				// off the start of the list of key/item
				// pairs.

    bool		isEnd() const
				{ return iter_->isEnd(); }
				// Returns <true> if the iterator is located
				// off the end of the list of key/item pairs.

    void		next() const
				{ iter_->next(); }
				// Moves the iterator onto the next key/item
				// pair.

    void		prev() const
				{ iter_->prev(); }
				// Moves the iterator onto the previous
				// key/item pair.

    void		resetFirst() const
				{ iter_->resetFirst(); }
				// Moves the iterator to the first key/item
				// pair. If the map is empty, the iterator
				// will be placed off the conceptual end of
				// the list.

    void		resetLast() const
				{ iter_->resetLast(); }
				// Moves the iterator to the last key/item
				// pair. If the map is empty, the iterator
				// will be placed off the conceptual start of
				// the list.

    void		remove();
				// Removes the key/item pair currently
				// located under the iterator. If there
				// is no valid key/item pair currently
				// under the iterator, an exception is
				// raised.

    // = APPLICATORS

    void		applyKeys(
			 OTC_Visitor<T1>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Applies <theApplicator> to each of the
				// keys in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

    void		applyItems(
			 OTC_Visitor<T2>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Applies <theApplicator> to each of the
				// items in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

    void		applyItems(
			 OTC_Worker<T2>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Applies <theApplicator> to each of the
				// items in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

    void		applyPairs(
			 OTC_PairVisitor<T1,T2>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Applies <theApplicator> to each of the
				// key/item pairs in the collection. The
				// direction being determined by
				// <theDirection>. Valid values are
				// <OTCLIB_FORWARD> and <OTCLIB_BACKWARD>.

    void		applyPairs(
			 OTC_PairWorker<T1,T2>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Applies <theApplicator> to each of the
				// key/item pairs in the collection. The
				// direction being determined by
				// <theDirection>. Valid values are
				// <OTCLIB_FORWARD> and <OTCLIB_BACKWARD>.

  protected:

    // = RANKING

    virtual int		rank(T1 const& key1, T1 const& key2) const;
				// Ranks two keys.

  private:

    T1&                 key_() const;
				// If the iterator is located over a valid
				// key/item pair, a reference to the key is
				// returned. If there is not a valid key/item
				// pair under the iterator, an exception is
				// raised.

    T2&                 item_() const;
				// If the iterator is located over a valid
				// key/item pair, a reference to the item is
				// returned. If there is not a valid key/item
				// pair under the iterator, an exception is
				// raised.

    T2&			item_(T1 const& theKey) const;
				// If an item corresponding to <theKey>
				// exists in this map, a reference to
				// it will be returned. If an item
				// corresponding to <theKey> does not exist,
				// an exception will be raised.

    T2*			pItem_(T1 const& theKey) const;
				// If an item corresponding to <theKey>
				// exists in this map, a pointer to
				// it will be returned. If an item
				// corresponding to <theKey> does not exist,
				// a null pointer will be returned.

    OTC_LinkList*	list_;
				// List of pairs.

    OTC_HashTable*	index_;
				// Key index table.

    OTC_LinkIterator*	iter_;
				// Internal iterator over key/item pairs
				// in the map.

    int			(*rankFn_)(T1 const&,T1 const&);
				// Comparison function to be used in
				// place of <OTC_RankActions> for key.
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/ihmap.c>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_IHMAP_HH */
