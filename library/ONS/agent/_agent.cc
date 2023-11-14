/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/_agent.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/agent.hh>

#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/dispatch.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

#include <stdlib.h>
#include <signal.h>
#include <time.h>

/* ------------------------------------------------------------------------- */

class Agent1 : public ONS_Agent
{
  public:

			~Agent1();

			Agent1();

    void		startupCallback_1c() const;

    void		startupCallback_1();

    void		shutdownCallback_1c(
			 OTC_ShutdownNotification theCategory
			) const;

    void		shutdownCallback_1(
			 OTC_ShutdownNotification theCategory
			);

    void		shutdownCallback_2c(
			 OTC_ShutdownNotification theCategory,
			 char const* theDescription
			) const;

    void		shutdownCallback_2(
			 OTC_ShutdownNotification theCategory,
			 char const* theDescription
			);

    void		shutdownCallback_3c(
			 OTC_ShutdownNotification theCategory,
			 OTC_String const& theDescription
			) const;

    void		shutdownCallback_3(
			 OTC_ShutdownNotification theCategory,
			 OTC_String const& theDescription
			);

    void		signalCallback_1c() const;

    void		signalCallback_1();

    void		signalCallback_2c(int theSignal) const;

    void		signalCallback_2(int theSignal);

    void		alarmCallback_1c() const;

    void		alarmCallback_1();

    void		alarmCallback_2c(char const* theAlarm) const;

    void		alarmCallback_2(char const* theAlarm);

    void		alarmCallback_3c(OTC_String const& theAlarm) const;

    void		alarmCallback_3(OTC_String const& theAlarm);

    void		timerCallback_1c() const;

    void		timerCallback_1();

    void		timerCallback_2c(char const* theTimer) const;

    void		timerCallback_2(char const* theTimer);

    void		timerCallback_3c(OTC_String const& theTimer) const;

    void		timerCallback_3(OTC_String const& theTimer);

    void		actionCallback_1c() const;

    void		actionCallback_1();

    void		actionCallback_2c(char const* theAction) const;

    void		actionCallback_2(char const* theAction);

    void		actionCallback_3c(OTC_String const& theAction) const;

    void		actionCallback_3(OTC_String const& theAction);

    void		cronCallback_1c() const;

    void		cronCallback_1();

    void		cronCallback_2c(char const* theAction) const;

    void		cronCallback_2(char const* theAction);

    void		cronCallback_3c(OTC_String const& theAction) const;

    void		cronCallback_3(OTC_String const& theAction);

  private:

#if 0
    ONS_Callback0	startupCallback_cb1;

    ONS_Callback1<OTC_ShutdownNotification>	shutdownCallback_cb1;

    ONS_Callback2<OTC_ShutdownNotification,
     char const*>	shutdownCallback_cb2;

    ONS_Callback2<OTC_ShutdownNotification,
     OTC_String const&>	shutdownCallback_cb3;

    ONS_Callback0	signalCallback_cb1;

    ONS_Callback1<int>	signalCallback_cb2;

    ONS_Callback0	alarmCallback_cb1;

    ONS_Callback1<char const*>	alarmCallback_cb2;

    ONS_Callback1<OTC_String const&>	alarmCallback_cb3;

    ONS_Callback0	timerCallback_cb1;

    ONS_Callback1<char const*>	timerCallback_cb2;

    ONS_Callback1<OTC_String const&>	timerCallback_cb3;

    ONS_Callback0	actionCallback_cb1;

    ONS_Callback1<char const*>	actionCallback_cb2;

    ONS_Callback1<OTC_String const&>	actionCallback_cb3;

    ONS_Callback0	cronCallback_cb1;

    ONS_Callback1<char const*>	cronCallback_cb2;

    ONS_Callback1<OTC_String const&>	cronCallback_cb3;
#endif
};

Agent1::~Agent1()
{
  OTC_Tracer tracer("Agent1::~Agent1()");
}

Agent1::Agent1()
{
  OTC_Tracer tracer("Agent1::Agent1()");

  // Need to do this for each callback prototype to
  // make sure template member functions are correct.
  // Only the last of each will actually be called.

  subscribeStartup(&Agent1::startupCallback_1c);
  subscribeStartup(&Agent1::startupCallback_1);

#if 0
  subscribeStartup(&Agent1::startupCallback_cb1);

  startupCallback_cb1.connect(this,&Agent1::startupCallback_1c,"A");
  startupCallback_cb1.connect(this,&Agent1::startupCallback_1,"B");
  startupCallback_cb1.connect(this,&Agent1::startupCallback_1c,"A");

  startupCallback_cb1.disconnect("B");
#endif

  subscribeShutdown(&Agent1::shutdownCallback_1c);
  subscribeShutdown(&Agent1::shutdownCallback_1);
  subscribeShutdown(&Agent1::shutdownCallback_2c);
  subscribeShutdown(&Agent1::shutdownCallback_2);
  subscribeShutdown(&Agent1::shutdownCallback_3c);
  subscribeShutdown(&Agent1::shutdownCallback_3);

#if 0
  subscribeShutdown(&Agent1::shutdownCallback_cb1);
  subscribeShutdown(&Agent1::shutdownCallback_cb2);
  subscribeShutdown(&Agent1::shutdownCallback_cb3);

  shutdownCallback_cb1.connect(this,&Agent1::shutdownCallback_1c,"A");
  shutdownCallback_cb1.connect(this,&Agent1::shutdownCallback_1,"B");
  shutdownCallback_cb2.connect(this,&Agent1::shutdownCallback_2c,"A");
  shutdownCallback_cb2.connect(this,&Agent1::shutdownCallback_2,"B");
  shutdownCallback_cb3.connect(this,&Agent1::shutdownCallback_3c,"A");
  shutdownCallback_cb3.connect(this,&Agent1::shutdownCallback_3,"B");

  shutdownCallback_cb3.disconnect("B");
#endif

  subscribeSignal(&Agent1::signalCallback_1c,SIGHUP);
  subscribeSignal(&Agent1::signalCallback_1,SIGHUP);
  subscribeSignal(&Agent1::signalCallback_2c,SIGHUP);
  subscribeSignal(&Agent1::signalCallback_2,SIGHUP);

#if 0
  subscribeSignal(&Agent1::signalCallback_cb1,SIGHUP);
  subscribeSignal(&Agent1::signalCallback_cb2,SIGHUP);

  signalCallback_cb1.connect(this,&Agent1::signalCallback_1c,"A");
  signalCallback_cb1.connect(this,&Agent1::signalCallback_1,"B");
  signalCallback_cb2.connect(this,&Agent1::signalCallback_2c,"A");
  signalCallback_cb2.connect(this,&Agent1::signalCallback_2,"B");

  signalCallback_cb2.disconnect("B");
#endif

  setAlarm(&Agent1::alarmCallback_1c,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_1,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_3c,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_3,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_2c,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_2,time(0)+5,"ALARM");

#if 0
  setAlarm(&Agent1::alarmCallback_cb1,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_cb3,time(0)+5,"ALARM");
  setAlarm(&Agent1::alarmCallback_cb2,time(0)+5,"ALARM");

  alarmCallback_cb1.connect(this,&Agent1::alarmCallback_1c,"A");
  alarmCallback_cb1.connect(this,&Agent1::alarmCallback_1,"B");
  alarmCallback_cb3.connect(this,&Agent1::alarmCallback_3c,"A");
  alarmCallback_cb3.connect(this,&Agent1::alarmCallback_3,"B");
  alarmCallback_cb2.connect(this,&Agent1::alarmCallback_2c,"A");
  alarmCallback_cb2.connect(this,&Agent1::alarmCallback_2,"B");

  alarmCallback_cb2.disconnect("B");
#endif

  startTimer(&Agent1::timerCallback_1c,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_1,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_3c,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_3,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_2c,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_2,5.0,"TIMEOUT");

#if 0
  startTimer(&Agent1::timerCallback_cb1,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_cb3,5.0,"TIMEOUT");
  startTimer(&Agent1::timerCallback_cb2,5.0,"TIMEOUT");

  timerCallback_cb1.connect(this,&Agent1::timerCallback_1c,"A");
  timerCallback_cb1.connect(this,&Agent1::timerCallback_1,"B");
  timerCallback_cb3.connect(this,&Agent1::timerCallback_3c,"A");
  timerCallback_cb3.connect(this,&Agent1::timerCallback_3,"B");
  timerCallback_cb2.connect(this,&Agent1::timerCallback_2c,"A");
  timerCallback_cb2.connect(this,&Agent1::timerCallback_2,"B");

  timerCallback_cb2.disconnect("B");
#endif

  scheduleAction(&Agent1::actionCallback_1c,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_1,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_3c,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_3,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_2c,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_2,OTCLIB_IDLE_JOB,"ACTION");

#if 0
  scheduleAction(&Agent1::actionCallback_cb1,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_cb3,OTCLIB_IDLE_JOB,"ACTION");
  scheduleAction(&Agent1::actionCallback_cb2,OTCLIB_IDLE_JOB,"ACTION");

  actionCallback_cb1.connect(this,&Agent1::actionCallback_1c,"A");
  actionCallback_cb1.connect(this,&Agent1::actionCallback_1,"B");
  actionCallback_cb3.connect(this,&Agent1::actionCallback_3c,"A");
  actionCallback_cb3.connect(this,&Agent1::actionCallback_3,"B");
  actionCallback_cb2.connect(this,&Agent1::actionCallback_2c,"A");
  actionCallback_cb2.connect(this,&Agent1::actionCallback_2,"B");

  actionCallback_cb2.disconnect("B");
#endif

  scheduleAction(&Agent1::cronCallback_1c,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_1,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_3c,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_3,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_2c,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_2,"* * * * *","CRONJOB");

#if 0
  scheduleAction(&Agent1::cronCallback_cb1,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_cb3,"* * * * *","CRONJOB");
  scheduleAction(&Agent1::cronCallback_cb2,"* * * * *","CRONJOB");

  cronCallback_cb1.connect(this,&Agent1::cronCallback_1c,"A");
  cronCallback_cb1.connect(this,&Agent1::cronCallback_1,"B");
  cronCallback_cb3.connect(this,&Agent1::cronCallback_3c,"A");
  cronCallback_cb3.connect(this,&Agent1::cronCallback_3,"B");
  cronCallback_cb2.connect(this,&Agent1::cronCallback_2c,"A");
  cronCallback_cb2.connect(this,&Agent1::cronCallback_2,"B");

  cronCallback_cb2.disconnect("B");
#endif
}

void Agent1::startupCallback_1c() const
{
  OTC_Tracer tracer("Agent1::startupCallback_1c()");
}

void Agent1::startupCallback_1()
{
  OTC_Tracer tracer("Agent1::startupCallback_1()");
}

void Agent1::shutdownCallback_1c(OTC_ShutdownNotification theCategory) const
{
  OTC_Tracer tracer("Agent1::shutdownCallback_1c()");

  tracer() << "category = " << theCategory << endl;
}

void Agent1::shutdownCallback_1(OTC_ShutdownNotification theCategory)
{
  OTC_Tracer tracer("Agent1::shutdownCallback_1()");

  tracer() << "category = " << theCategory << endl;
}

void Agent1::shutdownCallback_2c(
 OTC_ShutdownNotification theCategory,
 char const* theDescription
) const
{
  OTC_Tracer tracer("Agent1::shutdownCallback_2c()");

  tracer() << "category = " << theCategory << endl;
  tracer() << "description = " << theDescription << endl;
}

void Agent1::shutdownCallback_2(
 OTC_ShutdownNotification theCategory,
 char const* theDescription
)
{
  OTC_Tracer tracer("Agent1::shutdownCallback_2()");

  tracer() << "category = " << theCategory << endl;
  tracer() << "description = " << theDescription << endl;
}

void Agent1::shutdownCallback_3c(
 OTC_ShutdownNotification theCategory,
 OTC_String const& theDescription
) const
{
  OTC_Tracer tracer("Agent1::shutdownCallback_3c()");

  tracer() << "category = " << theCategory << endl;
  tracer() << "description = " << theDescription << endl;
}

void Agent1::shutdownCallback_3(
 OTC_ShutdownNotification theCategory,
 OTC_String const& theDescription
)
{
  OTC_Tracer tracer("Agent1::shutdownCallback_3()");

  tracer() << "category = " << theCategory << endl;
  tracer() << "description = " << theDescription << endl;
}

void Agent1::signalCallback_1c() const
{
  OTC_Tracer tracer("Agent1::signalCallback_1c()");
}

void Agent1::signalCallback_1()
{
  OTC_Tracer tracer("Agent1::signalCallback_1()");
}

void Agent1::signalCallback_2c(int theSignal) const
{
  OTC_Tracer tracer("Agent1::signalCallback_2c()");

  tracer() << "signal = " << theSignal << endl;
}

void Agent1::signalCallback_2(int theSignal)
{
  OTC_Tracer tracer("Agent1::signalCallback_2()");

  tracer() << "signal = " << theSignal << endl;
}

void Agent1::alarmCallback_1c() const
{
  OTC_Tracer tracer("Agent1::alarmCallback_1c()");
}

void Agent1::alarmCallback_1()
{
  OTC_Tracer tracer("Agent1::alarmCallback_1()");

  setAlarm(&Agent1::alarmCallback_1,time(0)+5);
}

void Agent1::alarmCallback_2c(char const* theAlarm) const
{
  OTC_Tracer tracer("Agent1::alarmCallback_2c()");

  tracer() << "alarm = " << theAlarm << endl;
}

void Agent1::alarmCallback_2(char const* theAlarm)
{
  OTC_Tracer tracer("Agent1::alarmCallback_2()");

  tracer() << "alarm = " << theAlarm << endl;

  setAlarm(&Agent1::alarmCallback_2,time(0)+5);
}

void Agent1::alarmCallback_3c(OTC_String const& theAlarm) const
{
  OTC_Tracer tracer("Agent1::alarmCallback_3c()");

  tracer() << "alarm = " << theAlarm << endl;
}

void Agent1::alarmCallback_3(OTC_String const& theAlarm)
{
  OTC_Tracer tracer("Agent1::alarmCallback_3()");

  tracer() << "alarm = " << theAlarm << endl;

  setAlarm(&Agent1::alarmCallback_2,time(0)+5);
}

void Agent1::timerCallback_1c() const
{
  OTC_Tracer tracer("Agent1::timerCallback_1c()");
}

void Agent1::timerCallback_1()
{
  OTC_Tracer tracer("Agent1::timerCallback_1()");

  startTimer(&Agent1::timerCallback_1,5.0);
}

void Agent1::timerCallback_2c(char const* theTimer) const
{
  OTC_Tracer tracer("Agent1::timerCallback_2c()");

  tracer() << "timer = " << theTimer << endl;
}

void Agent1::timerCallback_2(char const* theTimer)
{
  OTC_Tracer tracer("Agent1::timerCallback_2()");

  tracer() << "timer = " << theTimer << endl;

  startTimer(&Agent1::timerCallback_2,5.0);
}

void Agent1::timerCallback_3c(OTC_String const& theTimer) const
{
  OTC_Tracer tracer("Agent1::timerCallback_3c()");

  tracer() << "timer = " << theTimer << endl;
}

void Agent1::timerCallback_3(OTC_String const& theTimer)
{
  OTC_Tracer tracer("Agent1::timerCallback_3()");

  tracer() << "timer = " << theTimer << endl;

  startTimer(&Agent1::timerCallback_2,5.0);
}

void Agent1::actionCallback_1c() const
{
  OTC_Tracer tracer("Agent1::actionCallback_1c()");
}

void Agent1::actionCallback_1()
{
  OTC_Tracer tracer("Agent1::actionCallback_1()");
}

void Agent1::actionCallback_2c(char const* theAction) const
{
  OTC_Tracer tracer("Agent1::actionCallback_2c()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::actionCallback_2(char const* theAction)
{
  OTC_Tracer tracer("Agent1::actionCallback_2()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::actionCallback_3c(OTC_String const& theAction) const
{
  OTC_Tracer tracer("Agent1::actionCallback_3c()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::actionCallback_3(OTC_String const& theAction)
{
  OTC_Tracer tracer("Agent1::actionCallback_3()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::cronCallback_1c() const
{
  OTC_Tracer tracer("Agent1::cronCallback_1c()");
}

void Agent1::cronCallback_1()
{
  OTC_Tracer tracer("Agent1::cronCallback_1()");
}

void Agent1::cronCallback_2c(char const* theAction) const
{
  OTC_Tracer tracer("Agent1::cronCallback_2c()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::cronCallback_2(char const* theAction)
{
  OTC_Tracer tracer("Agent1::cronCallback_2()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::cronCallback_3c(OTC_String const& theAction) const
{
  OTC_Tracer tracer("Agent1::cronCallback_3c()");

  tracer() << "action = " << theAction << endl;
}

void Agent1::cronCallback_3(OTC_String const& theAction)
{
  OTC_Tracer tracer("Agent1::cronCallback_3()");

  tracer() << "action = " << theAction << endl;
}

void test1()
{
  OTC_Tracer tracer("test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Agent1 agent1;
  agent1.scheduleStartup();

  OTC_Dispatcher::run();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1
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
