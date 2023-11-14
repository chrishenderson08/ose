#ifndef OTC_COLLCTN_HASHTBLE_HH
#define OTC_COLLCTN_HASHTBLE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/hashtble.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1996-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/refcnt/resource.hh>
#include <OTC/collctn/hashntry.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/hashtble.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_HashTable : public OTC_Resource
    // = TITLE
    //     Wrapper around data needed for a hash table.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_HashTable> class is a wrapper around the data needed for
    //     a hash table. The class also tries to encapsulate the bulk
    //     of the functionality associated with a hash table. It isn't
    //     possible for all the functionality of the hash table to be
    //     emboddied in this class as it doesn't have any knowledge of
    //     what items are being held, it only encapsulating the indexing
    //     aspect of a hash table.
    //
    // = SEE ALSO
    //     <OTC_HashEntry>
{
  public:

			~OTC_HashTable();

    // = INITIALISATION

			OTC_HashTable(
			 u_int theStartSize=16,
			 u_int theFloorSize=256,
			 u_int theLowThreshold=25,
			 u_int theHighThreshold=75
			);
				// Initialises the hash table. The initial
				// size of the hash table is determined by
				// <theStartSize>. The floor size of the hash
				// table, ie., that size which once exceeded
				// the size of the hash table will not be
				// reduced below, is determined by
				// <theFloorSize>. The low threshold of the
				// hash table, that is the number of items in
				// the hash table, expressed as a percentage,
				// which the population of the hash table
				// must drop below before the size of the
				// hash table is reduced is determined by
				// <theLowThreshold>. The high threshold of
				// the hash table, that is the number of
				// items in the hash table, expressed as a
				// percentage, which the population of the
				// hash table must rise above before the size
				// of the hash table is increased is
				// determined by <theHighThreshold>. Note
				// that <theStartSize> may be increased
				// internally in size due to the manner in
				// which the hash table is implemented. Note
				// also, that if setting <theStartSize> to
				// ensure some initial capacity in order to
				// avoid the hash table needing to grow in
				// size while being populated, you will need
				// to consider that the table will be forced
				// to grow when <theHighThreshold> is reached.
				// Thus, <theStartSize> will actually need
				// to be set to be higher than the capacity
				// you want to ensure is available. Note that
				// <theLowThreshold> must be less than <50>
				// and <theHighThreshold> must greater than
				// <50>.

    // = QUERY

    u_int		startSize() const
				{ return startSize_; }
				// The initial size of the table. This is the
				// adjusted start size and not necessarily
				// the same as what was supplied to the
				// constructor.

    u_int		floorSize() const
				{ return floorSize_; }
				// The size of the hash table, which once
				// exceeded the size will not be reduced
				// below.

    u_int		lowThreshold() const
				{ return lowThreshold_; }
				// Returns the numbers of items, expressed as
				// a precentage of hash table size which if
				// less than will result in the hash table
				// size being reduced.

    u_int		highThreshold() const
				{ return highThreshold_; }
				// Returns the numbers of items, expressed as
				// a percentage of hash table size which if
				// greater than will result in the hash table
				// size being increased.

    u_int		lowWaterMark() const
				{ return lowWaterMark_; }
				// Returns the number of items which if less
				// than will result in the hash table size
				// being reduced.

    u_int		highWaterMark() const
				{ return highWaterMark_; }
				// Returns the number of items which if
				// greater than will result in the hash table
				// size being increased.

    u_int		tableSize() const
				{ return tableSize_; }
				// Returns the current size of the hash
				// table.

    u_int		population() const
				{ return population_; }
				// Returns the number of entries in the
				// hash table which are occupied.

    bool		isEmpty() const
				{ return population_ == 0; }
				// Returns <true> if the hash table contains
				// no entries which are occupied.

    // = SEARCH

    OTC_HashEntry*	tableEntry(u_int theIndex) const;
				// Returns the hash entry at position
				// <theIndex>.

    OTC_HashEntry*	searchHash(
			 int theHashValue,
			 u_int& theIteration,
			 u_int& theStartIndex,
			 u_int& theEntryIndex
			);
				// Returns the next entry which holds or
				// is eligible to hold an item with
				// <theHashValue>. The variable referenced by
				// <theIteration> should be set to <0> the
				// first time this function is called for a
				// specific search. <theIteration> will be
				// incremented by this function for each time
				// it is called. <theStartIndex> will be
				// ignored for <theIteration> being <0>, but
				// when <theIteration> is non zero,
				// <theStartIndex> will be used as the place
				// to start the search, <theStartIndex>
				// having being filled out by the previous
				// call to this function. <theEntryIndex>
				// will be set to position of the entry which
				// is returned.

    // = MODIFIERS

    void		add(
			 u_int theIndex,
			 void* theItem,
			 int theHashValue
			);
				// Adds <theItem> at position <theIndex> in
				// the hash table and marks the entry as
				// occupied. <theHashValue> is cached as the
				// hash value so as to allow resizing of the
				// hash table without having to recalculate
				// hash values. The population is incremented
				// by one. If position <theIndex> was already
				// occupied, an exception is raised.

    void		replace(
			 u_int theIndex,
			 void* theItem,
			 int theHashValue
			);
				// Replaces the item at position <theIndex>
				// in the hash table with <theItem>. If
				// position <theIndex> wasn't occupied with
				// an item with equivalent hash value, an
				// exception is raised.

    void		remove(u_int theIndex);
				// Marks as deleted, the entry at position
				// <theIndex>. Raises an exception if the
				// position was not occupied.

    void		remove(void* theItem, int theHashValue);
				// Marks as deleted the first entry found
				// containing <theItem>. <theHashValue>
				// should be the hash value for <theItem> and
				// is required to locate the appropriate
				// entry. Raises an exception if the entry
				// couldn't be found.

    void		removeAll();
				// Marks as empty, all entries in the hash
				// table.

  private:

			OTC_HashTable(OTC_HashTable const&);
				// Do not define an implementation for this.

    OTC_HashTable&	operator=(OTC_HashTable const&);
				// Do not define an implementation for this.

    u_int		mask_(int theHashValue) const
				{ return theHashValue & (tableSize_ - 1); }
				// Converts <theHashValue> into an index
				// into the hash table.

    u_int		start_(int theHashValue) const
				{
				  /* Thomas Wang's 32 bit Mix Function. */
				  theHashValue += ~(theHashValue << 15);
				  theHashValue ^=  (theHashValue >> 10);
				  theHashValue +=  (theHashValue << 3);
				  theHashValue ^=  (theHashValue >> 6);
				  theHashValue += ~(theHashValue << 11);
				  theHashValue ^=  (theHashValue >> 16);
				  return mask_(theHashValue);
				}
				// Special version of <mask_()> for finding
				// starting position in hash vector to look
				// for item. This is needed to mix up hash
				// to avoid clumping when poor hash values
				// provided.

    void		grow_();
				// Doubles the size of the hash table,
				// copying the old contents to the new.

    void		shrink_();
				// Halves the size of the hash table, copying
				// the old contents to the new. An exception
				// will be raised if the new capacity is
				// insufficient for hold the number of items
				// being referenced by the hash table.

    void		rebuild_(u_int theIndex);
    				// Shifts around items in the hashtable
				// if possible following the deletion of
				// a single entry at <theIndex>.

    void		rebuild_(
			 OTC_HashEntry* theHashTable,
			 u_int theTableSize
			);
				// Rebuilds the hash table by copying
				// the entries in <theHashTable> into
				// the current table. <theTableSize>
				// is the size of <theHashTable>.

    u_int		waterMark_(u_int theThreshold)
				{ return (theThreshold<<tablePower_)/100; }
				// Calculates the water mark. <theThreshold>
				// a percentage in the range <0-100>.

    u_int		startSize_;
				// The initial size of the hash table.

    u_int		startPower_;
				// The initial table size is equivalent to 2
				// to the power of <startPower_>.

    u_int		floorSize_;
				// Once the hash table size has grown
				// beyond this value, it will not be
				// reduced to below this value.

    u_int		lowThreshold_;
				// Numbers of items, expressed as a
				// precentage of hash table size which if
				// less than will result in the hash table
				// size being reduced.

    u_int		highThreshold_;
				// Numbers of items, expressed as a
				// percentage of hash table size which if
				// greater than will result in the hash table
				// size being increased.

    u_int		lowWaterMark_;
				// Number of items which if less than will
				// result in the hash table size being
				// reduced.

    u_int		highWaterMark_;
				// Number of items which if greater than
				// will result in the hash table size being
				// increased.

    u_int		tableSize_;
				// The size of the hash table. This is
				// always a power of two.

    u_int		tablePower_;
				// The table size is equivalent to 2 to
				// the power of <tablePower_>. Track this
				// to avoid multiplications and divisions,
				// use bit shifting instead.

    OTC_HashEntry*	hashTable_;
				// The hash table.

    u_int		population_;
				// The number of items referenced by the
				// hash table.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HASHTBLE_HH */
