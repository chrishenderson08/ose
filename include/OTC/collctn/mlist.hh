#ifndef OTC_COLLCTN_MLIST_HH
#define OTC_COLLCTN_MLIST_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/mlist.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/dequei.hh>
#include <OTC/collctn/shlwcopy.hh>
#include <OTC/collctn/bucket.hh>
#include <OTC/collctn/worker.hh>
#include <OTC/collctn/visitor.hh>
#include <OTC/collctn/dirction.hh>
#include <OTC/collctn/prtction.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/mlist.hh"
#endif
#pragma interface "OTC/collctn/mlist.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T> class OTC_MListIterator;

template<class T>
class OTC_MList
    // = TITLE
    //	   A list class which can be modified via an iterator.
    //
    // = CLASS TYPE
    //	   Concrete
    //
    // = DESCRIPTION
    //	   <OTC_MList> implements a list of objects where access to and
    //	   manipulation of items, is permitted on items occurring at the
    //	   ends of the list. Manipulation and removal of items may also
    //	   be achieved through a separate class specific iterator and
    //	   modifier.
    //	   
    // = NOTES
    //	   The <OTC_Bucket> class is used internally to hold items in the
    //	   list. Thus, the <OTC_BaseActions> class may be used to provide
    //	   actions to be performed, when items are inserted and removed from
    //	   the list.
    //
    // = SEE ALSO
    //	   <OTC_Bucket>, <OTC_BaseActions>, <OTC_MListIterator>,
    //	   <OTC_MListModifier>
{
    friend		class OTC_MListIterator<T>;

  public:

    // = INITIALISATION

			OTC_MList() {}
				// Creates an empty list.

			OTC_MList(OTC_MList<T> const& theList);
				// Creates a copy of <theList>.

			OTC_MList(OTC_MList<T>& theList, OTC_ShallowCopy)
			  : list_(theList.list_) {}
				// Creates an alias for <theList>.

    // = ASSIGNMENT

    OTC_MList<T>&	operator=(OTC_MList<T> const& theList);
				// Replaces the contents of this list with
				// the items contained in <theList>.

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

    void		addLast(T const& theItem)
				{ list_.addLast(link_(theItem)); }
				// Appends <theItem> to the tail of the list.

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

    // = REMOVAL

    void		removeAll()
				{ list_.removeAll(); }
				// Removes all items from the list.

    void		removeFirst(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeFirst(theProtection); }
				// Removes the first item in the list. If
				// the list is empty, an exception is
				// raised unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    void		removeLast(OTC_Protection theProtection=OTCLIB_SAFE)
				{ list_.removeLast(theProtection); }
				// Removes the first item in the list. If
				// the list is empty, an exception is
				// raised unless <theProtection> is
				// <OTCLIB_UNSAFE>.

    // = APPLICATORS

    void		apply(
			 OTC_Visitor<T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			) const;
				// Applies <theApplicator> to each of the
				// items in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

    void		apply(
			 OTC_Worker<T>& theApplicator,
			 OTC_Direction theDirection=OTCLIB_FORWARD,
			 OTC_Protection theProtection=OTCLIB_SAFE
			);
				// Applies <theApplicator> to each of the
				// items in the collection. The direction
				// being determined by <theDirection>. Valid
				// values are <OTCLIB_FORWARD> and
				// <OTCLIB_BACKWARD>.

  public:

			operator OTC_DequeHI<T>() const
				{ return list_; }
				// Special to use of <OTC_MListIterator>
				// and <OTC_MListModifier>.

			operator OTC_DequeHI<T>()
				{ return list_; }
				// Special to use of <OTC_MListIterator>
				// and <OTC_MListModifier>.

  private:

    OTC_Link*		link_(T const& theItem);
				// Creates an instance of <OTC_Bucket>
				// to hold <theItem>.

    OTC_DequeI		list_;
};

/* ------------------------------------------------------------------------- */

#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#include <OTC/collctn/mlist.c>
#endif

#ifndef OTC_COLLCTN_MLSTITER_HH
#include <OTC/collctn/mlstiter.hh>
#endif

#ifndef OTC_COLLCTN_MLSTMDFR_HH
#include <OTC/collctn/mlstmdfr.hh>
#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_MLIST_HH */
