/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_dispatch.cc
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
#include <OTC/dispatch/timeout.hh>
#include <OTC/dispatch/alarm.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/ioevent.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/evhandle.hh>
#include <OTC/dispatch/action.hh>

#include <OTC/system/program.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

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

class TestJob : public OTC_Job
{
  public:

			TestJob(int theOptions)
			  : type_(theOptions)  {}

    void		execute();


  private:

    int			type_;
};

void TestJob::execute()
{
  OTC_Tracer tracer("TestJob::execute()");

  tracer() << "type = " << type_ << endl;
}

class Agent1 : public OTC_EVAgent
{
  public:

			Agent1()
			  : count_(0)
				{}

  protected:

    void		handle(OTC_Event* theEvent);

  private:

    int			count_;
};

void Agent1::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent1::handle()");
  tracer() << *theEvent << endl;

  static int count = 0;

  if (theEvent->type() == OTCEV_Alarm::typeId())
  {
    tracer() << count << endl;
    count = 0;
    OTCEV_Alarm::set(id(),time(0)+20);
    OTCEV_Timeout::start(id(),1000);
    OTCEV_Timeout::start(id(),2000);
  }
  else if (theEvent->type() == OTCEV_Signal::typeId())
  {
    OTCEV_Signal* theSignal = (OTCEV_Signal*)theEvent;

    static int theCount = 0;

    if (theSignal->signal() == SIGINT)
    {
      theCount++;
      if (theCount == 3)
        exit(1);
    }

    TestJob* testJob;
    testJob = new TestJob(OTCLIB_IDLE_JOB);
    OTC_Dispatcher::schedule(testJob,OTCLIB_IDLE_JOB);
    testJob = new TestJob(OTCLIB_STANDARD_JOB);
    OTC_Dispatcher::schedule(testJob,OTCLIB_STANDARD_JOB);
    testJob = new TestJob(OTCLIB_PRIORITY_JOB);
    OTC_Dispatcher::schedule(testJob,OTCLIB_PRIORITY_JOB);

    OTCEV_Action::schedule(id());
    OTCEV_Action::schedule(id());
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction = (OTCEV_Action*)theEvent;
    OTCEV_Action::cancel(theAction->action()+1);
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    count++;
  }
  else if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    OTCEV_IOEvent* tmpEvent;
    tmpEvent = (OTCEV_IOEvent*)theEvent;

    if (tmpEvent->events() & OTCLIB_POLLIN)
    {
      char buf[1];
      read(0,buf,1);
      tracer() << "char = " << buf[0] << endl;
    }
  }

  theEvent->destroy();
}

class Agent2 : public OTC_EVAgent
{
  public:

			Agent2()
			  : count_(0)
				{}

  protected:

    void		handle(OTC_Event* theEvent);

  private:

    int			count_;
};

void Agent2::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent2::handle()");
  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Signal::typeId())
  {
    OTCEV_Signal* theSignal = (OTCEV_Signal*)theEvent;

    static int theCount = 0;

    if (theSignal->signal() == SIGINT)
    {
      theCount++;
      if (theCount == 3)
        exit(1);
    }

    TestJob* testJob;
    testJob = new TestJob(OTCLIB_IDLE_JOB);
    OTC_Dispatcher::schedule(testJob,OTCLIB_IDLE_JOB);
    testJob = new TestJob(OTCLIB_STANDARD_JOB);
    OTC_Dispatcher::schedule(testJob,OTCLIB_STANDARD_JOB);
    testJob = new TestJob(OTCLIB_PRIORITY_JOB);
    OTC_Dispatcher::schedule(testJob,OTCLIB_PRIORITY_JOB);

    OTCEV_Action::schedule(id());
    OTCEV_Action::schedule(id());
  }
  else if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTCEV_Action* theAction = (OTCEV_Action*)theEvent;
    OTCEV_Action::cancel(theAction->action()+1);
  }
  else if (theEvent->type() == OTCEV_Timeout::typeId())
  {
    OTCEV_Timeout::start(id(),1);
  }
  else if (theEvent->type() == OTCEV_IOEvent::typeId())
  {
    char buf[1];
    read(0,buf,1);
    tracer() << "char = " << buf[0] << endl;
  }

  theEvent->destroy();
}

class Agent3 : public OTC_EVAgent
{
  public:

			Agent3()
			  : handler_(this,1,4,2)
				{ }

			~Agent3()
				{ }

  protected:

    void		execute(OTC_Event* theEvent);

    void		handle(OTC_Event* theEvent);

  private:

    OTC_EVHandler	handler_;
};

void Agent3::execute(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent3::execute()");
  tracer() << *theEvent << endl;

  handler_.process(theEvent);
}

void Agent3::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent3::handle()");
  tracer() << *theEvent << endl;

  theEvent->destroy();
}

class Condition1 : public OTC_Condition
{
  public:

			Condition1(int theCount)
			  : count_(theCount), current_(0) {}

    void		evaluate();

  private:

    int			count_;

    int			current_;
};

void Condition1::evaluate()
{
  if (++current_ >= count_)
    set();
}

void test1()
{
  OTC_Tracer tracer("test1()");

  OTC_JobQueue* queue = new OTC_JobQueue;
  OTCLIB_ASSERT_M(queue != 0);

  OTC_Dispatcher::initialise(queue);

  Agent1 agent;

#if defined(OSE_SYS_UNIX)
  OTCEV_IOEvent::subscribe(agent.id(),0,OTCLIB_POLLIN);
#endif

  OTCEV_Signal::capacity(4);

#ifdef SIGUSR1
  OTCEV_Signal::subscribe(agent.id(),SIGUSR1);
  OTCEV_Signal::subscribe(-1,SIGUSR1);
#endif
#ifdef SIGUSR2
  OTCEV_Signal::subscribe(agent.id(),SIGUSR2);
  OTCEV_Signal::subscribe(-1,SIGUSR2);
#endif
  OTCEV_Signal::subscribe(agent.id(),SIGINT);

  long theTime = time(0);
  OTCEV_Alarm::set(agent.id(),theTime+2);
  OTCEV_Alarm::set(agent.id(),theTime+3);
  OTCEV_Alarm::set(agent.id(),theTime+3);
  OTCEV_Alarm::set(agent.id(),theTime+2);

  TestJob* testJob = new TestJob(OTCLIB_IDLE_JOB);
  OTC_Dispatcher::schedule(testJob);

  OTC_Dispatcher::run();
}

void test2()
{
  OTC_Tracer tracer("test2()");

  OTC_JobQueue* queue = new OTC_JobQueue;
  OTCLIB_ASSERT_M(queue != 0);

  OTC_Dispatcher::initialise(queue);

  Agent2 agent;

  OTCEV_IOEvent::subscribe(agent.id(),0,OTCLIB_POLLIN);

  OTCEV_Signal::capacity(4);

#ifdef SIGUSR1
  OTCEV_Signal::subscribe(agent.id(),SIGUSR1);
  OTCEV_Signal::subscribe(-1,SIGUSR1);
#endif
#ifdef SIGUSR2
  OTCEV_Signal::subscribe(agent.id(),SIGUSR2);
  OTCEV_Signal::subscribe(-1,SIGUSR2);
#endif
  OTCEV_Signal::subscribe(agent.id(),SIGINT);

  OTCEV_Timeout::start(agent.id(),1);
  OTCEV_Timeout::start(agent.id(),1);
  OTCEV_Timeout::start(agent.id(),1);
  OTCEV_Timeout::start(agent.id(),1);
  OTCEV_Timeout::start(agent.id(),1);

  TestJob* testJob = new TestJob(OTCLIB_IDLE_JOB);
  OTC_Dispatcher::schedule(testJob);

  OTC_Dispatcher::run();
}

void test3()
{
  OTC_Tracer tracer("test3()");

  Agent3 agent;

  OTC_Event* theEvent;
  for (int i=0; i<20; i++)
  {
    theEvent = new OTCEV_Action(OTCLIB_ACTION_JOB,i);
    theEvent->deliver(agent.id());
  }

  tracer() << "shutdown" << endl;

  OTCEV_Shutdown::notifyImmediately(OTCLIB_SHUTDOWN_ARRIVED);

  tracer() << "finished" << endl;
}

void test4()
{
  OTC_Tracer tracer("test4()");

  OTC_JobQueue* queue = new OTC_JobQueue;
  OTCLIB_ASSERT_M(queue != 0);

  OTC_Dispatcher::initialise(queue);

  Agent1 agent;

#if defined(OSE_SYS_UNIX)
  OTCEV_IOEvent::subscribe(agent.id(),0,OTCLIB_POLLIN);
#endif

  OTCEV_Signal::capacity(4);

#ifdef SIGUSR1
  OTCEV_Signal::subscribe(agent.id(),SIGUSR1);
  OTCEV_Signal::subscribe(-1,SIGUSR1);
#endif
#ifdef SIGUSR2
  OTCEV_Signal::subscribe(agent.id(),SIGUSR2);
  OTCEV_Signal::subscribe(-1,SIGUSR2);
#endif
  OTCEV_Signal::subscribe(agent.id(),SIGINT);

  long theTime = time(0);
  OTCEV_Alarm::set(agent.id(),theTime+2);
  OTCEV_Alarm::set(agent.id(),theTime+3);
  OTCEV_Alarm::set(agent.id(),theTime+3);
  OTCEV_Alarm::set(agent.id(),theTime+2);

  TestJob* testJob = new TestJob(OTCLIB_IDLE_JOB);
  OTC_Dispatcher::schedule(testJob);

  Condition1 theCondition(100);

  int theResult;

  theResult = OTC_Dispatcher::run(&theCondition);

  tracer() << "result = " << theResult << endl;
}

void test5()
{
  OTC_Tracer tracer("test5()");

  OTC_JobQueue* queue = new OTC_JobQueue;
  OTCLIB_ASSERT_M(queue != 0);

  OTC_Dispatcher::initialise(queue);

  Agent1 agent;

#if defined(OSE_SYS_UNIX)
  OTCEV_IOEvent::subscribe(agent.id(),0,OTCLIB_POLLIN);
#endif

  OTCEV_Signal::capacity(4);

#ifdef SIGUSR1
  OTCEV_Signal::subscribe(agent.id(),SIGUSR1);
  OTCEV_Signal::subscribe(-1,SIGUSR1);
#endif
#ifdef SIGUSR2
  OTCEV_Signal::subscribe(agent.id(),SIGUSR2);
  OTCEV_Signal::subscribe(-1,SIGUSR2);
#endif
  OTCEV_Signal::subscribe(agent.id(),SIGINT);

  int theId;

  theId = OTCEV_Action::schedule(agent.id(),"* * * * *");

  tracer() << theId << " - " << "* * * * *" << endl;

  int theResult;

  theResult = OTC_Dispatcher::run();

  tracer() << "result = " << theResult << endl;
}

void test6()
{
  OTC_Tracer tracer("test6()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::task()->start();

  OTC_Dispatcher::task()->wait();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5,
  test6
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
