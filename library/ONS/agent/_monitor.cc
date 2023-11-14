/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/_monitor.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/monitor.hh>

#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/svbroker.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class Monitor1 : public ONS_Monitor
{
  public:

			Monitor1();

    void		createServiceBroker();

    void		destroyServiceBroker();

    void		joinServiceGroup();

    void		leaveServiceGroup();

    void		serviceNameCallback_2c(
			 OTC_ServiceBinding* theBinding,
			 OTC_ServiceStatus theStatus
			) const;

    void		serviceNameCallback_2(
			 OTC_ServiceBinding* theBinding,
			 OTC_ServiceStatus theStatus
			);

    void		serviceNameCallback_3c(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus
			) const;

    void		serviceNameCallback_3(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus
			);

    void		serviceGroupCallback_2c(
			 OTC_ServiceBinding* theBinding,
			 OTC_ServiceStatus theStatus
			) const;

    void		serviceGroupCallback_2(
			 OTC_ServiceBinding* theBinding,
			 OTC_ServiceStatus theStatus
			);

    void		serviceGroupCallback_3c(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus
			) const;

    void		serviceGroupCallback_3(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus
			);

    void		serviceAddressCallback_3c(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus
			) const;

    void		serviceAddressCallback_3(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus
			);

    void		serviceAddressCallback_4c(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus,
			 OTC_String const& theSubscription
			) const;

    void		serviceAddressCallback_4(
			 OTC_ServiceBinding* theBinding,
			 OTC_String const& theGroup,
			 OTC_ServiceStatus theStatus,
			 OTC_String const& theSubscription
			);

    void		serviceAddressCallback_5c(
			 OTC_ServiceBinding* theBinding,
			 char const* theGroup,
			 OTC_ServiceStatus theStatus,
			 char const* theSubscription
			) const;

    void		serviceAddressCallback_5(
			 OTC_ServiceBinding* theBinding,
			 char const* theGroup,
			 OTC_ServiceStatus theStatus,
			 char const* theSubscription
			);

  private:

    OTC_SVBroker*	svbroker_;
};

Monitor1::Monitor1()
  : svbroker_(0)
{
  OTC_Tracer tracer("Monitor1::Monitor1()");

  subscribeServiceName(&Monitor1::serviceNameCallback_2c,"SERVICE-NAME");
  subscribeServiceName(&Monitor1::serviceNameCallback_2,"SERVICE-NAME");
  subscribeServiceName(&Monitor1::serviceNameCallback_3c,"SERVICE-NAME");
  subscribeServiceName(&Monitor1::serviceNameCallback_3,"SERVICE-NAME");

  subscribeServiceGroup(&Monitor1::serviceGroupCallback_2c,"SERVICE-GROUP");
  subscribeServiceGroup(&Monitor1::serviceGroupCallback_2,"SERVICE-GROUP");
  subscribeServiceGroup(&Monitor1::serviceGroupCallback_3c,"SERVICE-GROUP");
  subscribeServiceGroup(&Monitor1::serviceGroupCallback_3,"SERVICE-GROUP");

  subscribeStartup(&Monitor1::createServiceBroker);
}

void Monitor1::createServiceBroker()
{
  OTC_Tracer tracer("Monitor1::createServiceBroker()");

  if (svbroker_ == 0)
  {
    svbroker_ = new OTC_SVBroker(this,"SERVICE-NAME");

    subscribeServiceAddress(&Monitor1::serviceAddressCallback_3c,
     svbroker_->serviceBinding()->serviceAddress());
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_3,
     svbroker_->serviceBinding()->serviceAddress());
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_4c,
     svbroker_->serviceBinding()->serviceAddress());
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_4,
     svbroker_->serviceBinding()->serviceAddress());

    subscribeServiceAddress(&Monitor1::serviceAddressCallback_3c,
     svbroker_->serviceBinding()->serviceAddress(),"SUBSCRIPTION");
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_3,
     svbroker_->serviceBinding()->serviceAddress(),"SUBSCRIPTION");
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_4c,
     svbroker_->serviceBinding()->serviceAddress(),"SUBSCRIPTION");
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_4,
     svbroker_->serviceBinding()->serviceAddress(),"SUBSCRIPTION");
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_5c,
     svbroker_->serviceBinding()->serviceAddress(),"SUBSCRIPTION");
    subscribeServiceAddress(&Monitor1::serviceAddressCallback_5,
     svbroker_->serviceBinding()->serviceAddress(),"SUBSCRIPTION");

    startTimer(&Monitor1::joinServiceGroup,2.0);
    startTimer(&Monitor1::leaveServiceGroup,3.0);

    startTimer(&Monitor1::destroyServiceBroker,5.0);
  }
}

void Monitor1::destroyServiceBroker()
{
  OTC_Tracer tracer("Monitor1::destroyServiceBroker()");

  if (svbroker_ != 0)
  {
    OTC_String tmpServiceAddress;
    tmpServiceAddress = svbroker_->serviceBinding()->serviceAddress();

    delete svbroker_;
    svbroker_ = 0;

    unsubscribeServiceAddress(tmpServiceAddress);
    unsubscribeServiceAddress(tmpServiceAddress,"SUBSCRIPTION");

    startTimer(&Monitor1::createServiceBroker,5.0);
  }
}

void Monitor1::joinServiceGroup()
{
  OTC_Tracer tracer("Monitor1::joinServiceGroup()");

  if (svbroker_ != 0)
    svbroker_->joinGroup("SERVICE-GROUP");
}

void Monitor1::leaveServiceGroup()
{
  OTC_Tracer tracer("Monitor1::joinServiceGroup()");

  if (svbroker_ != 0)
    svbroker_->leaveGroup("SERVICE-GROUP");
}

void Monitor1::serviceNameCallback_2c(
 OTC_ServiceBinding* theBinding,
 OTC_ServiceStatus theStatus
) const
{
  OTC_Tracer tracer("Monitor1::serviceNameCallback_2c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceNameCallback_2(
 OTC_ServiceBinding* theBinding,
 OTC_ServiceStatus theStatus
)
{
  OTC_Tracer tracer("Monitor1::serviceNameCallback_2()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceNameCallback_3c(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus
) const
{
  OTC_Tracer tracer("Monitor1::serviceNameCallback_3c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceNameCallback_3(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus
)
{
  OTC_Tracer tracer("Monitor1::serviceNameCallback_3()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceGroupCallback_2c(
 OTC_ServiceBinding* theBinding,
 OTC_ServiceStatus theStatus
) const
{
  OTC_Tracer tracer("Monitor1::serviceGroupCallback_2c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceGroupCallback_2(
 OTC_ServiceBinding* theBinding,
 OTC_ServiceStatus theStatus
)
{
  OTC_Tracer tracer("Monitor1::serviceGroupCallback_2()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceGroupCallback_3c(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus
) const
{
  OTC_Tracer tracer("Monitor1::serviceGroupCallback_3c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceGroupCallback_3(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus
)
{
  OTC_Tracer tracer("Monitor1::serviceGroupCallback_3()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceAddressCallback_3c(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus
) const
{
  OTC_Tracer tracer("Monitor1::serviceAddressCallback_3c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceAddressCallback_3(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus
)
{
  OTC_Tracer tracer("Monitor1::serviceAddressCallback_3()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
}

void Monitor1::serviceAddressCallback_4c(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus,
 OTC_String const& theSubscription
) const
{
  OTC_Tracer tracer("Monitor1::serviceAddressCallback_4c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
  tracer() << "subscription = " << theSubscription << endl;
}

void Monitor1::serviceAddressCallback_4(
 OTC_ServiceBinding* theBinding,
 OTC_String const& theGroup,
 OTC_ServiceStatus theStatus,
 OTC_String const& theSubscription
)
{
  OTC_Tracer tracer("Monitor1::serviceAddressCallback_4()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
  tracer() << "subscription = " << theSubscription << endl;
}

void Monitor1::serviceAddressCallback_5c(
 OTC_ServiceBinding* theBinding,
 char const* theGroup,
 OTC_ServiceStatus theStatus,
 char const* theSubscription
) const
{
  OTC_Tracer tracer("Monitor1::serviceAddressCallback_5c()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
  tracer() << "subscription = " << theSubscription << endl;
}

void Monitor1::serviceAddressCallback_5(
 OTC_ServiceBinding* theBinding,
 char const* theGroup,
 OTC_ServiceStatus theStatus,
 char const* theSubscription
)
{
  OTC_Tracer tracer("Monitor1::serviceAddressCallback_5()");

  tracer() << "serviceName = " << theBinding->serviceName() << endl;
  tracer() << "agentIdentity = " << theBinding->agentIdentity() << endl;
  tracer() << "processIdentity = " << theBinding->processIdentity() << endl;
  tracer() << "serviceGroup = " << theGroup << endl;
  tracer() << "serviceStatus = " << theStatus << endl;
  tracer() << "subscription = " << theSubscription << endl;
}

void test1()
{
  OTC_Tracer tracer("test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Monitor1 monitor1;
  monitor1.scheduleStartup();

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
