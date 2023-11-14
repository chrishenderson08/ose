/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_action.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/jobqueue.hh>
#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/action.hh>

#include <OTC/system/program.hh>
#include <OTC/types/time.hh>
#include <OTC/text/string.hh>
#include <OTC/debug/logstrm.hh>
#include <OTC/debug/tracer.hh>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#if defined(OSE_SYS_WIN32)
#include <io.h>
#endif

/* ------------------------------------------------------------------------- */

class Agent1 : public OTC_EVAgent
{
  public:

			Agent1() {}

  protected:

    void		handle(OTC_Event* theEvent);
};

void Agent1::handle(OTC_Event* theEvent)
{

  if (theEvent->type() == OTCEV_Signal::typeId())
  {
    OTCEV_Signal* theSignal = (OTCEV_Signal*)theEvent;

    if (theSignal->signal() == SIGINT)
      OTC_Dispatcher::stop();
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theActionEvent;
    theActionEvent = (OTCEV_Action*)theEvent;

    cout << theActionEvent->action() << " - ";
    cout << OTC_Time::currentTime() << endl;

    if (theActionEvent->data() == 1)
    {
      static int count = 0;
      count++;

      if (count == 3)
      {
	OTCEV_Action::cancel(theActionEvent->action());

	cout << "CANCEL " << theActionEvent->action() << endl;
      }
    }

    if (theActionEvent->data() == 2)
    {
      static int count = 0;
      count++;

      if (count == 4)
      {
	OTCEV_Action::cancelAgent(id(),theActionEvent->data());

	cout << "CANCEL/AGENTDATA " << theActionEvent->action() << endl;
      }
    }

    if (theActionEvent->data() == 3)
    {
      static int count = 0;
      count++;

      if (count == 5)
      {
	OTCEV_Action::cancelAgent(id());

	cout << "CANCEL/AGENT " << theActionEvent->action() << endl;
      }
    }
  }

  theEvent->destroy();
}

void test1()
{
  OTC_Dispatcher::initialise();

  Agent1 agent;

  OTCEV_Signal::subscribe(agent.id(),SIGINT);

  int theId;

  theId = OTCEV_Action::schedule(agent.id(),"* * * * *",1);
  cout << theId << " - " << "* * * * *" << endl;

  theId = OTCEV_Action::schedule(agent.id(),"* * * * *",2);
  cout << theId << " - " << "* * * * *" << endl;

  theId = OTCEV_Action::schedule(agent.id(),"* * * * *",3);
  cout << theId << " - " << "* * * * *" << endl;

  cout << endl;

  OTC_Dispatcher::run();
}

void test2()
{
  OTC_Dispatcher::initialise();

  Agent1 agent;

  OTCEV_Signal::subscribe(agent.id(),SIGINT);

  OTC_String theInput;

  while (cin.good())
  {
    theInput.readLine(cin);
    if (!cin.fail())
    {
      int theId;

      theId = OTCEV_Action::schedule(agent.id(),theInput);
      if (theId == 0)
	cout << "FAIL - " << theInput << endl;
      else
	cout << theId << " - " << theInput << endl;
    }
  }

  cout << endl;

  OTC_Dispatcher::run();
}

char const* SPECIFICATIONS[] = {
 "* * * * *",
 "* * 1-31 * *",
 "* * 1 * *",
 "* * 2 * *",
 "* * 3 * *",
 "* * 4 * *",
 "* * 5 * *",
 "* * 6 * *",
 "* * 7 * *",
 "* * 1 * Sun",
 "* * * * Sun",
 "* * * * Mon",
 "* * * * Tue",
 "* * * * Wed",
 "* * * * Thu",
 "* * * * Fri",
 "* * * * Sat",
 "* * * * Mon-Fri",
 "* * * * Sat,Sun",
 0
};

void test3()
{
  OTC_Tracer tracer("test3()");

  time_t theTime = 0;

  for (u_int i=0; i<7; i++)
  {
    OTC_Time theDateTime(theTime);

    for (u_int j=0; SPECIFICATIONS[j] != 0; j++)
    {
      tracer() << theDateTime;
      tracer() << " (";
      tracer() << theDateTime.asString("%a");
      tracer() << ") - ";
      tracer() << SPECIFICATIONS[j];
      tracer() << " - ";
      tracer() << OTCEV_Action::matches(SPECIFICATIONS[j],theTime);
      tracer() << endl;
    }

    theTime += (60*60*24);
  }
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
