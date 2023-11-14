/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_svbroker.cc
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
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/dispatch/shutdown.hh>
#include <OTC/dispatch/action.hh>
#include <OTC/dispatch/request.hh>
#include <OTC/dispatch/report.hh>
#include <OTC/dispatch/sbscrptn.hh>
#include <OTC/dispatch/pbntfctn.hh>
#include <OTC/dispatch/fnagent.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/strstream.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif
#include <stdlib.h>
#include <signal.h>

/* ------------------------------------------------------------------------- */

class Agent0 : public OTC_EVAgent
{
  public:

			~Agent0();

			Agent0();

    void		handle(OTC_Event* theEvent);

    OTC_ServiceBinding const*	serviceBinding() const
      				{ return broker_->serviceBinding(); }

  private:

    OTC_SVBroker*	broker_;
};

Agent0* agent;

Agent0::~Agent0()
{
  delete broker_;
}

Agent0::Agent0()
{
  OTC_Tracer tracer("Agent0::Agent0()");

  broker_ = new OTC_SVBroker(this,"NAME");

  tracer() << "agent-identity=";
  tracer() << serviceBinding()->agentIdentity() << endl;
  tracer() << "process-identity=";
  tracer() << serviceBinding()->processIdentity() << endl;

  OTC_SVPayload theContent;
  theContent <<= "CONTENT";

  broker_->sendRequest(broker_->serviceBinding(),
   "METHOD-NAME",theContent);

  broker_->subscribeReports("NAME","*");
  broker_->subscribeReports("NAME","SUBJECT");

  broker_->joinGroup("GROUP1");
  broker_->joinGroup("GROUP2");

  OTCEV_Action::schedule(id(),OTCLIB_IDLE_JOB);
}

void Agent0::handle(OTC_Event* theEvent)
{
  OTC_Tracer tracer("Agent0::handle()");

  if (theEvent == 0)
    return;

  tracer() << *theEvent << endl;

  if (theEvent->type() == OTCEV_Action::typeId())
  {
    OTC_SVPayload theContent;
    theContent <<= "CONTENT";

    broker_->publishReport("SUBJECT",theContent);

    broker_->leaveGroup("GROUP1");

    broker_->publishReport("DELETE",theContent);
  }
  else if (theEvent->type() == OTCEV_Request::typeId())
  {
    OTCEV_Request* theRequest;
    theRequest = (OTCEV_Request*)theEvent;

    OTC_SVPayload theContent;
    theContent <<= "CONTENT-IN-RETURN";

    theRequest->sendResponse(theContent);
  }
  else if (theEvent->type() == OTCEV_Subscription::typeId())
  {
    OTCEV_Subscription* theSubscription;
    theSubscription = (OTCEV_Subscription*)theEvent;

    if (theSubscription->status() == OTCLIB_SUBSCRIPTION_REQUESTED)
    {
      OTC_SVPayload theContent;
      theContent <<= theSubscription->subject();

      broker_->sendReport(theSubscription->subscriber(),
       "VERIFY-SUBSCRIPTION",theContent);
    }
  }
  else if (theEvent->type() == OTCEV_Report::typeId())
  {
    OTCEV_Report* theReport;
    theReport = (OTCEV_Report*)theEvent;

    if (theReport->subjectName() == "DELETE")
    {
      delete agent;
      agent = 0;
    }
  }

  theEvent->destroy();
}

void event_callback(OTC_Event* theEvent, int)
{
  OTC_Tracer tracer("event_callback()");

  if (theEvent == 0)
    return;

  cout << *theEvent << endl;

  theEvent->destroy();
}

void test1()
{
  OTC_Tracer tracer("test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  agent = new Agent0;

  OTC_FNAgent theCallback(event_callback);

  OTC_SVRegistry* theRegistry;
  theRegistry = OTC_SVBroker::registry();

  theRegistry->subscribeRegistry(theCallback.id());

  theRegistry->subscribeServiceAddress(theCallback.id(),
   agent->serviceBinding()->serviceAddress());

  theRegistry->subscribeServiceName(theCallback.id(),"NAME");

  theRegistry->subscribeServiceGroup(theCallback.id(),"GROUP1");
  theRegistry->subscribeServiceGroup(theCallback.id(),"GROUP2");

  OTC_Dispatcher::run();

  if (agent != 0)
    delete agent;
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
