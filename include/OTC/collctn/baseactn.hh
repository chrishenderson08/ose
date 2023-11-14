#ifndef OTC_COLLCTN_BASEACTN_HH
#define OTC_COLLCTN_BASEACTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/baseactn.hh
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

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/baseactn.hh"
#pragma interface "OTC/collctn/baseactn.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T>
class OTC_BaseActions
    // = TITLE
    //     Default actions for an item contained in a bucket.
    //     
    // = CLASS TYPE
    //     Static
    //
    // = DESCRIPTION
    //     This class encapsulates the default actions which are executed
    //     when an item is placed into a bucket and when a bucket is deleted.
    //
    //     These may be overridden for a particular type, to produce type
    //     specific behaviour. For example, if <T> is a pointer to a class
    //     derived from <OTC_Resource> the actions may increment and
    //     decrement the reference count.
    //     
    // = EXAMPLE
    //     Example of a class specific version, where <EX_Foo> is derived
    //     from <OTC_Resource>.
    //
    // = BEGIN<CODE>
    //     class OTC_BaseActions<EX_Foo*>
    //     {
    //       public:
    //         EX_Foo* add(EX_Foo* theItem)
    //           {
    //             OTCLIB_ENSURE_FN((theItem != 0),
    //              "OTC_BaseActions<EX_Foo*>::add()",
    //              "invalid pointer");
    //             theItem->reference();
    //             return theItem;
    //           }
    //         void remove(EX_Foo* theItem)
    //           { theItem->unReference(); }
    //     };
    // = END<CODE>
    //
    //     If the type is a pointer, you may simplify the arguments and
    //     return type to that shown above. If <T> is a class object, the
    //     prototype of your <add()> and <remove()> functions must match
    //     exactly, those shown in the class definition below.
    //
    //     If you need to prevent a null pointer being placed into a
    //     collection, you should include a condition check in the <add()>
    //     function.
    //     
    // = SEE ALSO
    //     <OTC_Bucket>
{
  public:

    // = ACTIONS

    static T const&	add(T const& theItem)
				{ return theItem; }
				// Executed when an item is placed into a
				// bucket.

    static void		remove(T&)
				{}
				// Executed when a bucket is deleted.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BASEACTN_HH */
