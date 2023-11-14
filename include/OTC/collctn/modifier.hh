#ifndef OTC_COLLCTN_MODIFIER_HH
#define OTC_COLLCTN_MODIFIER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/modifier.hh
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

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/modifier.hh"
#pragma interface "OTC/collctn/modifier.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_Modifier : public OTC_Iterator<T>
    // = TITLE
    //     Smart pointer for iterators. Allows modification of items.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     This class exists so that the user of an iterator for a particular
    //     type of collection, does not have to worry about deleting the
    //     iterator when finished with it. This class will ensure
    //     that the iterator is destroyed when this class goes out of scope.
    //     In order for this to work correctly, an instance of this class
    //     should never be created using <new>.
    //
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void inc(OTC_List<int>& aList)
    //     {
    //       OTC_Modifier<int> anIter = 0;
    //       anIter = aList.items();
    //       for (anIter.reset(); anIter.isValid(); anIter.next())
    //         anIter.item()++;
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Iterator>
{
  public:

			~OTC_Modifier() {}

    // = INITIALISATION

			OTC_Modifier() {}
				// Creates a null iterator.

			OTC_Modifier(OTC_Cursor<T>* anIter)
			  : OTC_Iterator<T>(anIter) {}
				// Constructor used when an iterator
				// for a particular type of collection is
				// first created. If <anIter> is <0> a
				// null iterator is constructed.

			OTC_Modifier(OTC_Modifier<T> const& anIter)
			  : OTC_Iterator<T>(anIter) {}
				// Copy constructor for when an iterator is
				// used as return type.

    OTC_Modifier<T>&	operator=(OTC_Cursor<T>* anIter)
				{
				  OTC_Iterator<T>::operator=(anIter);
				  return *this;
				}
				// Sets this iterator to <anIter>.
				// If <anIter> is <0>, the iterator
				// is turned into a null iterator and
				// all associations with the previous
				// collection are lost.

    OTC_Modifier<T>&	operator=(OTC_Modifier<T> const& anIter)
				{
				  OTC_Iterator<T>::operator=(anIter);
				  return *this;
				}
				// Sets this iterator to <anIter>.

    // = RETRIEVAL

    T&			item() const
				{ return OTC_Iterator<T>::item_(); }
				// Returns the item under the current
				// location of the iterator. If no such item
				// exists, or this is a null iterator, an
				// exception is raised.

    // = CLONING

    OTC_Modifier<T>	clone() const
				{ return OTC_Iterator<T>::clone_(); }
				// Returns an iterator which is
                                // a clone of this iterator. The new
                                // iterator will initially be located
                                // over the same item as this iterator,
                                // however, subsequent operations will be
                                // independent of this iterator.

    // = APPLICATOR

    void                apply(OTC_Visitor<T>& theApplicator)
				{ apply_(theApplicator); }
                                // Applies <theApplicator> to each item
                                // accessible by this iterator.

    void                apply(OTC_Worker<T>& theApplicator)
				{ apply_(theApplicator); }
                                // Applies <theApplicator> to each item
                                // accessible by this iterator.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_MODIFIER_HH */
