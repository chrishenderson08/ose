#ifndef OTC_COLLCTN_DEQUEHI_HH
#define OTC_COLLCTN_DEQUEHI_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/dequehi.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/dequei.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/dequehi.hh"
#pragma interface "OTC/collctn/dequehi.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_DequeI;

template<class T> class OTC_Deque;
template<class T> class OTC_IList;
template<class T> class OTC_MList;
template<class T> class OTC_MListIterator;
template<class T> class OTC_MListModifier;

template<class T>
class OTC_DequeHI
    // = TITLE
    //     Handle to allow separate iterators on deque based collections.
    //
    // = DESCRIPTION
    //     The <OTC_DequeHI> class allows for separate iterators over
    //     collections making use of <OTC_DequeI> in their implementation.
    //     All members are private and access only granted using friends.
    //     The class is quite specific to implementation of this library.
{
  public:

			OTC_DequeHI(OTC_DequeHI<T> const& theHandle)
			  : list_(theHandle.list_) {}

			~OTC_DequeHI() {}
  private:

    friend class        OTC_Deque<T>;
    friend class        OTC_IList<T>;
    friend class        OTC_MList<T>;
    friend class        OTC_MListIterator<T>;
    friend class        OTC_MListModifier<T>;

                        OTC_DequeHI(OTC_DequeI& theList)
                          : list_(theList) {}

                        OTC_DequeHI(OTC_DequeI const& theList)
                          : list_(theList) {}

    OTC_DequeI const&	list_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_DEQUEHI_HH */
