/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     _stopwtch.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/system/stopwtch.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_StopWatch timer1("TIMER1");
  OTC_StopWatch timer2("TIMER2");
  OTC_StopWatch timer3;

  int i = 0;

  for (int j=1; j<5; j++)
  {
    OTC_StopWatch::report(tracer());

    tracer() << "timer1 = " << timer1.total() << endl;

    tracer() << "timer1.start()" << endl;

    timer1.start();

    tracer() << "loop" << endl;
    for (i=1; i!=10000000; i++) ;

    timer1.stop();

    tracer() << "timer1.stop()" << endl;

    OTC_StopWatch::report(tracer());

    tracer() << "timer1 = " << timer1.total() << endl;
  }

  tracer() << "timer1.reset()" << endl;
  tracer() << "timer1.start()" << endl;

  timer1.reset();
  timer1.start();

  tracer() << "loop" << endl;
  for (i=1; i!=10000000; i++) ;

  timer1.stop();

  tracer() << "timer1.stop()" << endl;

  OTC_StopWatch::report(tracer());

  tracer() << "timer1 = " << timer1.total() << endl;

  OTC_StopWatch::report(tracer());

  tracer() << "timer1.start()" << endl;

  timer1.start();

  tracer() << "loop" << endl;
  for (i=1; i!=10000000; i++) ;

  timer1.reset();

  tracer() << "timer1.reset()" << endl;

  OTC_StopWatch::report(tracer());

  tracer() << "timer1 = " << timer1.total() << endl;
}

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
