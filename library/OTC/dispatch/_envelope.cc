/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     dispatch/_envelope.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1998-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/message.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/dispatch/receiver.hh>
#include <OTC/dispatch/envelope.hh>
#include <OTC/dispatch/shutdown.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>
#include <signal.h>

/* ------------------------------------------------------------------------- */

void callback1(OTC_Event* theEvent, int)
{
  OTC_Tracer tracer("void callback1(OTC_Event*,int)");

  tracer() << *theEvent << endl;

  theEvent->destroy();
}

void callback1a(OTC_Event* theEvent)
{
  callback1(theEvent,0);
}

void callback2(OTC_Event* theEvent, int)
{
  OTC_Tracer tracer("void callback2(OTC_Event*,int)");

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    theEnvelope->bounce("AGENT3");
  }

  theEvent->destroy();
}

void callback3(OTC_Event* theEvent, int)
{
  OTC_Tracer tracer("void callback3(OTC_Event*, int)");

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    theEnvelope->reply(theEnvelope->message()->clone());
    theEnvelope->forward("AGENT4");
  }

  theEvent->destroy();
}

void callback4(OTC_Event* theEvent, int)
{
  OTC_Tracer tracer("void callback4(OTC_Event*, int)");

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    static int count = 0;
    if (count++ < 4)
    {
      OTCEV_Envelope* theEnvelope;
      theEnvelope = (OTCEV_Envelope*)theEvent;

      theEnvelope->message()->clone()->queue("AGENT2","AGENT1");
    }
  }

  theEvent->destroy();
}

void callback5(OTC_Event* theEvent, int)
{
  OTC_Tracer tracer("void callback5(OTC_Event*,int)");

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Envelope::typeId())
  {
    OTCEV_Envelope* theEnvelope;
    theEnvelope = (OTCEV_Envelope*)theEvent;

    if (!theEnvelope->nextReceiver().isEmpty())
    {
      theEnvelope->bounce();

      return;
    }
  }

  theEvent->destroy();
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Dispatcher::initialise();

  OTC_FNAgent fnAgent1(callback1);
  OTC_FNAgent fnAgent2(callback2);
  OTC_FNAgent fnAgent3(callback3);
  OTC_FNAgent fnAgent4(callback4);

  OTC_Receiver inBox1(&fnAgent1,"AGENT1");
  OTC_Receiver inBox2(&fnAgent2,"AGENT2");
  OTC_Receiver inBox3(&fnAgent3,"AGENT3");
  OTC_Receiver inBox4(&fnAgent4,"AGENT4");

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message("CONTENT","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);

  theMessage->queue("AGENT2","AGENT1");

  // OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Dispatcher::initialise();

  OTC_FNAgent fnAgent1(callback1);
  OTC_FNAgent fnAgent2(callback1);
  OTC_FNAgent fnAgent3(callback5);

  OTC_Receiver inBox1(&fnAgent1,"AGENT1");
  OTC_Receiver inBox2(&fnAgent2,"AGENT2");
  OTC_Receiver inBox3(&fnAgent3,"AGENT3");

  OTCEV_Message* theMessage;
  theMessage = new OTCEV_Message("CONTENT","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);

  OTC_String theHeader;
  theHeader += "To: DUMMY1\n";
  theHeader += "To: AGENT1\n";
  theHeader += "To: AGENT2\n";
  theHeader += "To: DUMMY2\n";
  theHeader += "To: AGENT2\n";
  theHeader += "To: AGENT3!AGENT1\n";
  theHeader += "To: AGENT3!AGENT2\n";
  theHeader += "To: DUMMY3\n";
  theHeader += "To: AGENT3\n";

  theMessage->queue("*","AGENT0","SUBJECT",theHeader);

  // OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Dispatcher::initialise();

  OTC_FNAgent fnAgent1(callback1);

  OTC_Receiver inBox1(&fnAgent1);
  OTC_Receiver inBox2(&callback1a);

  OTCEV_Message* theMessage;

  theMessage = new OTCEV_Message("CONTENT1","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);
  theMessage->queue(inBox1.name(),"BOGUS");

  theMessage = new OTCEV_Message("CONTENT2","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);
  theMessage->queue(inBox2.name(),"BOGUS");

  // OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Dispatcher::initialise();

  OTC_FNAgent fnAgent1(callback1);

  OTC_Receiver inBox1(&fnAgent1,"AGENT1");

  OTCEV_Message* theMessage;

  theMessage = new OTCEV_Message("CONTENT-1","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);

  theMessage->queue("$RECEIVER!AGENT1","");

  theMessage = new OTCEV_Message("CONTENT-2","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);

  theMessage->queue("$LOCAL-RECEIVER!AGENT1","");

  theMessage = new OTCEV_Message("CONTENT-3","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);

  theMessage->queue("$REMOTE-RECEIVER!AGENT1","");

  // OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Dispatcher::initialise();

  OTC_FNAgent fnAgent1(callback1);

  char buf[64];
  sprintf(buf,"$AGENT!%d",fnAgent1.id());

  OTCEV_Message* theMessage;

  theMessage = new OTCEV_Message("CONTENT-1","CONTENT-TYPE","EVENT-TYPE");
  OTCLIB_ASSERT_M(theMessage != 0);

  theMessage->queue(buf,"");

  // OTCEV_Shutdown::monitor(SIGINT);

  OTC_Dispatcher::run();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5
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
