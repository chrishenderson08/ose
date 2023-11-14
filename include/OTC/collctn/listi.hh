#ifndef OTC_COLLCTN_LISTI_HH
#define OTC_COLLCTN_LISTI_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/listi.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/range.hh>
#include <OTC/collctn/prtction.hh>
#include <OTC/collctn/linklist.hh>
#include <OTC/collctn/avltree.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/listi.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_ListI
    // = TITLE
    //	   Core for implementing a list.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //     The <OTC_ListI> class extends the functionality of <OTC_TIndexI>
    //     with extra functions required for implementing a list which has
    //     its items indexable by their position. Factoring these details
    //     into this class means that they do not have to be part of the
    //     template interface which would be wrapped around this class. This
    //     class still provides access to the underlying list and also to the
    //     links in the list so that iteration mechanisms can be provided.
    //     You should however, always use the member functions of this class
    //     when wishing to manipulate the list. In other words you should not
    //     add or remove links directly to or from the list as that will
    //     invalidate the cached population maintained by this class.
{
  public:

			~OTC_ListI();

    // = INITIALISATION

			OTC_ListI();
				// Creates an empty list.

			OTC_ListI(OTC_ListI const& theList);
				// Creates a list which is an alias for
				// <theList>. That is, both lists reference
				// the same data.

    // = QUERY

    OTC_AVLTree*        tree() const
                                { return index_; }
                                // Returns the index for the keys.

    OTC_LinkList*       list() const
                                { return list_; }
                                // Returns the list of objects.

    u_int		population() const
				{ return index_->population(); }
				// Returns the number of links in the
				// list.

    bool		isEmpty() const
				{ return index_->population() == 0; }
				// Returns <true> if the the list is empty.

    // = REMOVAL

    void                removeAll();
                                // Removes all links from the list.

    void		removeFirst(OTC_Protection theProtection=OTCLIB_SAFE)
				{ removeItem(0,theProtection); }
				// Removes the first link in the list. If
				// the list is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeLast(OTC_Protection theProtection=OTCLIB_SAFE)
				{
				  removeItem(index_->population()-1,
				   theProtection);
				}
				// Removes the last link in the list. If
				// the list is empty, an exception is
				// raised, unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeItem(
			 int theIndex,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Removes the link at location <theIndex>.
				// Raises an exception if there is no link at
				// location <theIndex>, unless <theProtection>
				// is <OTCLIB_UNSAFE>.

    void		removeRange(
			 int theStart,
			 u_int theLength,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Removes <theLength> links starting at
				// location <theStart>. Raises an exception
				// if the range specified is invalid, unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    void		removeRange(
			 OTC_Range theRange,
			 OTC_Protection theProtection=OTCLIB_SAFE
			)
				{
				  removeRange(theRange.lower(),
				   theRange.length(),theProtection);
				}
				// Removes <theLength> links starting at
				// location <theStart>. Raises an exception
				// if the range specified is invalid, unless
				// <theProtection> is <OTCLIB_UNSAFE>.

    // = RETRIEVAL

    OTC_Link*		first() const;
				// Returns the first link in the list.
				// Raises an exception if the list is empty.

    OTC_Link*		last() const;
				// Returns the last link in the list. Raises
				// an exception if the list is empty.

    OTC_Link*		item(int theIndex) const;
				// Returns the link an location <theIndex>.
				// Raises an exception if <theIndex> is
				// invalid.

    // = ADDITION

    void		addFirst(OTC_Link* theLink)
				{ addBeforeItem(theLink,0); }
				// Adds <theLink> at the start of the
				// the list.

    void		addLast(OTC_Link* theLink)
				{
				  addBeforeItem(theLink,
				   index_->population());
				}
				// Adds <theLink> at the end of the
				// the list.

    void		addBeforeItem(OTC_Link* theLink, int theIndex);
				// Adds <theLink> in the list at a location
				// <theIndex>. If <theIndex> is equivalent
				// to the population of the list, <theItem>
				// is appended to the list.

    // = REPLACEMENT

    void		replaceFirst(OTC_Link* theLink)
				{ replaceItem(theLink,0); }
				// Replaces the first link in the list
				// with <theLink>. Raises an exception
				// if the list is empty.

    void		replaceLast(OTC_Link* theLink)
				{ replaceItem(theLink,index_->population()-1); }
				// Replaces the last link in the list
				// with <theLink>. Raises an exception
				// if the list is empty.

    void		replaceItem(OTC_Link* theLink, int theIndex);
				// Replaces the link in the list at
				// position <theIndex> with <theLink>.
				// Raises an exception if <theIndex>
				// doesn't describe a valid position
				// in the list.

  private:

    OTC_ListI&		operator=(OTC_ListI const&);
				// Do not define an implementation for this.

    OTC_AVLTree*        index_;
                                // Positional index.

    OTC_LinkList*       list_;
                                // List of items.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LISTI_HH */
