#ifndef OTC_DISPATCH_FNAGENT_HH
#define OTC_DISPATCH_FNAGENT_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/fnagent.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/evagent.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/fnagent.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_FNAgent : public OTC_EVAgent
    // = TITLE
    //     Agent which forwards events onto a standard function.
    //     
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_FNAgent> class is used where it is required for
    //     event callbacks to be forwarded to a standard function rather
    //     than to a virtual function of a class. For simple applications,
    //     this can avoid the need to write a new class in order to be
    //     able to receive events.
    //
    // = SEE ALSO
    //     <OTC_EVAgent>, <OTC_Event>
{
  public:

			~OTC_FNAgent();

    // = INITIALISATION

			OTC_FNAgent(void (*theCallback)(OTC_Event*,int));
				// Events will be forwarded to <theCallback>.

  protected:

    // = EVENT CALLBACK

    void		handle(OTC_Event* theEvent);
				// Forwards <theEvent> to the supplied
				// callback function.

  private:

			OTC_FNAgent(OTC_FNAgent const&);
				// Do not define an implementation for this.

    OTC_FNAgent&	operator=(OTC_FNAgent const&);
				// Do not define an implementation for this.

    void		(*callback_)(OTC_Event*,int);
				// Callback function.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_FNAGENT_HH */
