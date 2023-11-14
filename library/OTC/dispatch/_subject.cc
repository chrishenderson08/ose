/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_subject.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/subject.hh>
#include <OTC/dispatch/shutdown.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/system/program.hh>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>

/* ------------------------------------------------------------------------- */

class Agent1 : public OTC_EVAgent, public OTC_Subject
{
  public:

			~Agent1();

			Agent1(char const* theSubject);

    void		handle(OTC_Event* theEvent);
};

Agent1::~Agent1()
{
}

Agent1::Agent1(char const* theSubject)
{
  addObserver(id(),theSubject);

  OTCEV_Timeout::start(id(),2000);
}

void Agent1::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent1::handle()");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Action* theActionEvent;

    theActionEvent = new OTCEV_Action(OTCLIB_ACTION_JOB,0,0);
    notifyObservers(theActionEvent);

    theActionEvent = new OTCEV_Action(OTCLIB_ACTION_JOB,1,1);
    notifyObservers(theActionEvent,"ONE");

    theActionEvent = new OTCEV_Action(OTCLIB_ACTION_JOB,2,2);
    notifyObservers(theActionEvent,"TWO");

    OTCEV_Timeout::start(id(),2000);
  }

  theEvent->destroy();
}

void test1()
{
  OTC_Tracer tracer("test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent1 agent(0);

  OTC_Dispatcher::run();
}

void test2()
{
  OTC_Tracer tracer("test2()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent1 agent("ONE");

  OTC_Dispatcher::run();
}

void test3()
{
  OTC_Tracer tracer("test3()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent1 agent("*O*");

  OTC_Dispatcher::run();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
