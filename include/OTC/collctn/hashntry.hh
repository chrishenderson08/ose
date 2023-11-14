#ifndef OTC_COLLCTN_HASHNTRY_HH
#define OTC_COLLCTN_HASHNTRY_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/hashntry.hh
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

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/hashntry.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

enum OTC_HashEntryState
{
  OTCLIB_ENTRY_EMPTY,
  OTCLIB_ENTRY_OCCUPIED,
  OTCLIB_ENTRY_DELETED
};

class OSE_EXPORT OTC_HashEntry
    // = TITLE
    //     Entry in a hash table.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_HashEntry> class is used as an entry in a hash array.
    //     The entry holds a pointer to the item, held as a <void*>,
    //     and a cached hash value for the item. The entry also tracks
    //     whether a valid item is actually being held, and if not
    //     whether that is because the entry has never been used, or
    //     because it was being used and the item was deleted from
    //     the hash array. At no time does the entry take responsibility
    //     for deleting the item.
    //
    // = SEE ALSO
    //     <OTC_HashTable>
{
  public:

    // = DESTRUCTION

			~OTC_HashEntry();
				// Does not delete the item if the entry
				// was occupied.

    // = INITIALISATION

			OTC_HashEntry();
				// The entry is initialised to be empty.

    // = QUERY

    bool		isEmpty() const
				{ return state_ == OTCLIB_ENTRY_EMPTY; }
				// Returns <true> if the entry is empty.

    bool		isOccupied() const
				{ return state_ == OTCLIB_ENTRY_OCCUPIED; }
				// Returns <true> if the entry is occupied.

    bool		isDeleted() const
				{ return state_ == OTCLIB_ENTRY_DELETED; }
				// Returns <true> if entry was occupied but
				// that the item in the entry has been
				// deleted.

    int			hashValue() const
				{ return hashValue_; }
				// Returns the cached hash value for the
				// item.

    void*		item() const
				{ return item_; }
				// Returns the pointer to the value.

    // = UPDATE

    void		markEmpty()
				{
				  state_ = OTCLIB_ENTRY_EMPTY;
				  item_ = 0;
				}
				// Mark the entry as being empty.

    void		markOccupied(void* theItem, int theHashValue)
				{
				  state_ = OTCLIB_ENTRY_OCCUPIED;
				  item_ = theItem;
				  hashValue_ = theHashValue;
				}
				// Marks this entry as being occupied
				// and holding <theItem>. <theHashValue>
				// will be cached as the hash value for
				// the item.

    void		markDeleted()
				{
				  state_ = OTCLIB_ENTRY_DELETED;
				  item_ = 0;
				}
				// Mark the item in the entry as being
				// deleted. Does not actually delete
				// the item.

  private:

			OTC_HashEntry(OTC_HashEntry const&);
				// Do not provide an implementation for this.

    OTC_HashEntry&	operator=(OTC_HashEntry const&);
				// Do not provide an implementation for this.

    OTC_HashEntryState	state_;
				// State of entry.

    void*		item_;
				// The pointer to the item.

    int			hashValue_;
				// The cached hash value of the item.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_HASHNTRY_HH */
