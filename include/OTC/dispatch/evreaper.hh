#ifndef OTC_DISPATCH_EVREAPER_HH
#define OTC_DISPATCH_EVREAPER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/mcreaper.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/event.hh>

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_EventReaper
    // = TITLE
    //     Destroys event.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The class <OTC_EventReaper> assists in ensuring that an event is
    //     destroyed when the stack is unwound as a result of an exception.
    //     This is achieved by using an instance of this class as a handle to
    //     the event. Once the event is grabbed using an instance of
    //     this class, the section of code in which an exception could be
    //     raised is executed. If an exception does occur then the instance
    //     of this class will be destroyed and the event also destroyed. If no
    //     exception occurs a call can be made to release the event, when the
    //     destructor is finally called the event would not be destroyed.
    //     The class can also be used to ensure an event is destroyed when
    //     it is necessary to exit from middle of function. This avoids
    //     having to remember to destroy it.
    //     
    // = EXAMPLE
    //
    // = BEGIN<CODE>
    //     void Class::handle(OTC_Event* theEvent)
    //     {
    //       if (theEvent == 0)
    //         return;
    //
    //       OTC_EventReaper xxxEvent;
    //       xxxEvent.grab(theEvent);
    //
    //       if (theEvent->type() == A::typeId())
    //       {
    //         // ...
    //         return;
    //       }
    //       else if (theEvent->type() == B::typeId())
    //       {
    //         // ...
    //       }
    //     }
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_Event>
{
  public:

    // = INITIALISATION

			OTC_EventReaper()
			 : event_(0) {}
				// Initialises the handle to <0>.

    // = DESTRUCTION

			~OTC_EventReaper()
				{ if (event_) event_->destroy(); }
				// If the handle references any event, ie.,
				// it is not <0>, the event will be destroyed.

    // = GRAB/RELEASE

    void		grab(OTC_Event* theEvent)
				{ event_ = theEvent; }
				// Sets the handle to <theEvent>. If the
				// handle was set to point at another event,
				// it will now point to the new event and the
				// first event will not be changed.

    void		release()
				{ event_ = 0; }
                                // Sets the handle to <0>. This is equivalent
                                // to calling <grab(0)>.

  private:

			OTC_EventReaper(OTC_EventReaper const&);
				// Do not define an implementation for this.

    OTC_EventReaper&	operator=(OTC_EventReaper const&);
				// Do not define an implementation for this.

    OTC_Event*		event_;
				// Pointer to the event.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_EVREAPER_HH */
