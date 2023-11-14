#ifndef OTC_COLLCTN_LINKLIST_HH
#define OTC_COLLCTN_LINKLIST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/linklist.hh
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

#include <OTC/collctn/linkiter.hh>
#include <OTC/collctn/link.hh>
#include <OTC/collctn/anchor.hh>
#include <OTC/refcnt/resource.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/linklist.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_LinkList : public OTC_Resource
    // = TITLE
    //     A doubly linked list.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_LinkList> provides the anchor points on which to hang a list
    //     of objects derived from the <OTC_Link> class. The class gives easy
    //     access to the start and end of the list, and the ability to insert
    //     objects at the start and end.
    //     
    //     If it is necessary to insert objects into the middle of the list,
    //     you will need to use the iterator to get to a point, adjacent
    //     to where the insertion is required, and use either of the
    //     <addBefore()> and <addAfter()> operations provided by the
    //     <OTC_Link> object.
    //
    // = NOTE
    //     Any instances of objects derived from <OTC_Link> placed into
    //     the list, must be created on the free store. If objects created
    //     on the stack, at global scope, or as members of another object are
    //     added to the list, the result is undefined. In this situation,
    //     the destructor of the object added to the list will ultimately
    //     have its destructor invoked twice.
{
  public:

			OTC_LinkList();

    // = DESTRUCTION

			~OTC_LinkList();
				// Invokes <removeAll()> to kill all the
				// links in the list.

    // = QUERY

    u_int		population() const;
				// Returns the number of live links in the
				// list. This is calculated by iterating over
				// the contents of the list.

    bool		isEmpty() const;
				// Returns <true> if the list is empty.

    // = ITERATION

    OTC_LinkIterator	items(OTC_Protection theProtection=OTCLIB_SAFE) const
				{
				  return OTC_LinkIterator(
				   (OTC_Linkable*)start_,
				   (OTC_Linkable*)end_,
				   theProtection
				  );
				}
				// Returns an iterator over items in the
				// list. <theProtection>
                                // determines whether reference counting is
                                // performed on the links in the list.
                                // Reference counting ensures the integrity of
                                // the iterator when removals and additions
                                // are perfomed. The values for
                                // <theProtection> are <OTCLIB_SAFE> and
                                // <OTCLIB_UNSAFE>. The default argument is
                                // <OTCLIB_UNSAFE>.

    // = ACCESS

    OTC_Link*		first() const;
				// Returns the link at the head of the list,
				// or <0> if the list is empty.

    OTC_Link*		last() const;
				// Returns the last link in the list, or
				// <0> if the list is empty.

    // = INSERTION

    void		addFirst(OTC_Link* theLink)
				{ start_->addAfter(theLink); }
				// Add <theLink> at the start of the list.

    void		addLast(OTC_Link* theLink)
				{ end_->addBefore(theLink); }
				// Add <theLink> at the end of the list.

    // = REMOVAL

    void		removeAll();
				// Kills all links in the list.

    void		removeFirst();
				// Kills first link in the list. Raises an
				// exception if the list is empty.

    void		removeLast();
				// Kills last link in the list. Raises an
				// exception if the list is empty.

  private:

			OTC_LinkList(OTC_LinkList const&);
				// Do not define an implementation for this.

    OTC_LinkList&	operator=(OTC_LinkList const&);
				// Do not define an implementation for this.

    OTC_Anchor*		start_;
				// Anchor point for the start of the list.

    OTC_Anchor*		end_;
				// Anchor point for the end of the list.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_LINKLIST_HH */
