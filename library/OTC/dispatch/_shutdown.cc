/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_shutdown.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/evagent.hh>
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

class Agent1 : public OTC_EVAgent
{
  public:

			~Agent1();

			Agent1();

    void		handle(OTC_Event* theEven);
};

Agent1::~Agent1()
{
  OTCEV_Shutdown::unsubscribe(id());
}

Agent1::Agent1()
{
  OTCEV_Shutdown::subscribe(id());
}

void Agent1::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent1::handle()");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    OTCEV_Shutdown* theShutdownEvent;
    theShutdownEvent = (OTCEV_Shutdown*)theEvent;

    if (theShutdownEvent->category() == OTCLIB_SHUTDOWN_PENDING)
    {
      OTCEV_Shutdown::suspend();

      OTCEV_Timeout::start(id(),10000);
    }
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Shutdown::resume();
  }

  theEvent->destroy();
}

class Agent2 : public OTC_EVAgent
{
  public:

			~Agent2();

			Agent2();

    void		handle(OTC_Event* theEven);
};

Agent2::~Agent2()
{
  OTCEV_Shutdown::unsubscribe(id());
}

Agent2::Agent2()
{
  OTCEV_Shutdown::subscribe(id());

  OTCEV_Shutdown::suspend();
}

void Agent2::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent2::handle()");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    OTCEV_Shutdown* theShutdownEvent;
    theShutdownEvent = (OTCEV_Shutdown*)theEvent;

    if (theShutdownEvent->category() == OTCLIB_SHUTDOWN_PENDING)
    {
      OTCEV_Timeout::start(id(),10000);
    }
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Shutdown::resume();
  }

  theEvent->destroy();
}

class Agent3 : public OTC_EVAgent
{
  public:

			~Agent3();

			Agent3();

    void		handle(OTC_Event* theEven);
};

Agent3::~Agent3()
{
  OTCEV_Shutdown::unsubscribe(id());
}

Agent3::Agent3()
{
  OTCEV_Shutdown::subscribe(id());
}

void Agent3::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent3::handle()");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Shutdown::typeId())
  {
    OTCEV_Shutdown* theShutdownEvent;
    theShutdownEvent = (OTCEV_Shutdown*)theEvent;

    if (theShutdownEvent->category() == OTCLIB_SHUTDOWN_PENDING)
    {
      OTCEV_Timeout::start(id(),200,0);

      OTCEV_Shutdown::suspend();
    }
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout* theTimeoutEvent;
    theTimeoutEvent = (OTCEV_Timeout*)theEvent;

    if (theTimeoutEvent->data() == 0)
    {
      OTCEV_Timeout::start(id(),200,1);

      OTCEV_Shutdown::resume();
    }
    else if (theTimeoutEvent->data() == 1)
    {
      OTCEV_Timeout::start(id(),200,2);

      OTCEV_Shutdown::suspend();
    }
    else
    {
      OTCEV_Shutdown::resume();
    }
  }

  theEvent->destroy();
}

void test1()
{
  OTC_Tracer tracer("test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();
}

void test2()
{
  OTC_Tracer tracer("test2()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent1 agent;

  OTC_Dispatcher::run();
}

void test3()
{
  OTC_Tracer tracer("test3()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent2 agent;

  OTC_Dispatcher::run();
}

void test4()
{
  OTC_Tracer tracer("test4()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent3 agent;

  OTC_Dispatcher::run();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4
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
