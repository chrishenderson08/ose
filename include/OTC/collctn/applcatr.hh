#ifndef OTC_COLLCTN_APPLCATR_HH
#define OTC_COLLCTN_APPLCATR_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/applcatr.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/OTC.h>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/applcatr.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_Applicator
    // = TITLE
    //     Base class for applicators.
    //
    // = DESCRIPTION
    //     The <OTC_Applicator> is a base class for applicator classes.
    //     It provides default implementations for <start()> and <finish()>.
    //     This is done here so as to avoid the need for derived template
    //     applicators to have out of line code. If the derived template
    //     class had to have out of line template code, it would result
    //     in longer compilation times on some compilers. Note that this
    //     class does not have a virtual destructor. Thus, you should not
    //     hold and delete an applicator through a pointer to this class.
{
  public:

    // = STARTING/FINISHING

    virtual void        start();
                                // Called by a collection before the
                                // <action()> function is applied to any
                                // items in the collection. The default
				// action is to do nothing. This function
                                // should be defined in a derived class
				// if something else is required to be
				// done.

    virtual void        finish();
				// Called after the <action()> function has
				// been applied to all items in a collection,
				// or if the return value of the <action()>
				// function requested that traversal of items
				// in the collection be stopped. The default
				// action is to do nothing. This function
				// should be defined in a derived class if
				// something else is required to be done.

  protected:

			OTC_Applicator();

  private:

			OTC_Applicator(OTC_Applicator const&);
				// Do not define an implementation for this.

    OTC_Applicator&	operator=(OTC_Applicator const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_APPLCATR_HH */
