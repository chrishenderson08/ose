#ifndef OTC_COLLCTN_PRWORKER_HH
#define OTC_COLLCTN_PRWORKER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/prworker.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/progress.hh>
#include <OTC/collctn/applcatr.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/prworker.hh"
#pragma interface "OTC/collctn/prworker.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class OTC_PairWorker : public OTC_Applicator
    // = TITLE
    //     Abstract interface for objects which act on key/item pairs.
    //     
    // = CLASS TYPE
    //     Abstract
    //
    // = DESCRIPTION
    //     The <OTC_PairWorker> class provides an alternative to using the
    //     <OTC_PairModifier> class to act on key/item pairs in a collection.
    //     Support for the <OTC_PairWorker> class can also be provided on
    //     collection type objects which do not support an iterator.
    //     
    //     To use this concept, a derived version of this class must be
    //     created. The derived class must redefine the <action()> function
    //     to perform whatever is to be done for each key/item pair in
    //     the collection. An instance of the derived class is then applied
    //     to the collection. The collection will do the work of calling the
    //     <action()> function on each key/item pair in the collection. If
    //     necessary, the <action()> function, through is return value, can
    //     prematurely stop or restart the process.
    //
    //     The <start()> and <finish()> functions are inherited from the
    //     <OTC_Applicator> base class. You can override these methods
    //     to perform special actions at the start and finish of having
    //     the derived class applied to the items in a collection
    //
    //     Through the <OTC_PairWorker> class, it is possible to modify the
    //     item of the key/item pair to which the <action()> function is
    //     being applied. The key of the key/item pair may not be modified.
    //     
    // = NOTES
    //     The destructor of this class is <[NOT]> defined as virtual. The
    //     destructor is not defined as virtual so that a separate template
    //     expansion is not required for this class. The implication of the
    //     destructor not being virtual is that it is not possible to delete
    //     an instance of a derived class, which has been created on the free
    //     store, using a pointer to the base class. It is expected that
    //     instances of derived classes will always being created on the
    //     stack and thus this restriction should not affect the use of the
    //     class.
    //
    // = SEE ALSO
    //     <OTC_PairVisitor>, <OTC_Applicator>
{
  public:

			OTC_PairWorker() {}

    // = ACTION

    virtual OTC_Progress	action(T1 const& theKey, T2& theItem) = 0;
                                // Called for each key/item pair in the
                                // collection. This function must be
                                // redefined in a derived class to perform
                                // whatever action is desired on <theKey> and
                                // <theItem>. The function must return either
                                // <OTCLIB_RESTART>, <OTCLIB_CONTINUE> or
                                // <OTCLIB_FINISH>. The value
                                // <OTCLIB_RESTART> should be returned if it
                                // is desired to start back at the beginning
                                // and go through each key/item pair again.
                                // The value <OTCLIB_FINISH> should be
                                // returned if it is desired to prematurely
                                // stop going through the key/item pairs. The
                                // value <OTCLIB_CONTINUE> should be returned
                                // if it is desired to move to the next
                                // key/item pair.

  private:

			OTC_PairWorker(OTC_PairWorker<T1,T2> const&)
				{ terminate(); }
				// Do not define an implementation for this.

    OTC_PairWorker<T1,T2>&	operator=(OTC_PairWorker<T1,T2> const&)
				{ terminate(); return *this; }
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_PRWORKER_HH */
