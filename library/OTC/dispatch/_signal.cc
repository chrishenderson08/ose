/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/_signal.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/evagent.hh>
#include <OTC/dispatch/dispatch.hh>

#include <OTC/system/program.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

#include <stdlib.h>
#include <signal.h>
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

/* ------------------------------------------------------------------------- */

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

  theEvent->destroy();

  count_++;

  tracer() << "count = " << count_ << endl;

  if (count_ == 20)
  {
    tracer() << "stop dispatcher" << endl;

    OTC_Dispatcher::stop();
  }
}

int main(int argc, char* argv[])
{
  OTC_Program::initialise(argc,argv);

  OTC_Tracer tracer("main()");

  OTC_Dispatcher::initialise();

  Agent1 agent;

  OTCEV_Signal::capacity(4);

  OTCEV_Signal::subscribe(agent.id(),SIGINT);
#ifdef SIGUSR1
  OTCEV_Signal::subscribe(agent.id(),SIGUSR1);
#endif
#ifdef SIGUSR2
  OTCEV_Signal::subscribe(agent.id(),SIGUSR2);
#endif

  OTCEV_Signal::unsubscribeAgent(agent.id());

  OTCEV_Signal::subscribe(agent.id(),SIGINT);
#ifdef SIGUSR1
  OTCEV_Signal::subscribe(agent.id(),SIGUSR1);
#endif
#ifdef SIGUSR2
  OTCEV_Signal::subscribe(agent.id(),SIGUSR2);
#endif

  OTC_Dispatcher::run();

  return 0;
}

/* ------------------------------------------------------------------------- */
