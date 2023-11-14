/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     system/_timeclck.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#define OTCLIB_TIMING 1

#include <OTC/system/timeclck.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void func1()
{
  OTC_Tracer tracer("void func1()");

  OTCLIB_STOPWATCH(TIMER1);

  int i = 0;

  for (int j=1; j<5; j++)
  {
    OTCLIB_TIMEBLOCK(TIMER1);

    for (i=1; i!=10000000; i++) ;
  }
}

void func2()
{
  OTC_Tracer tracer("void func2()");

  OTCLIB_STOPWATCH(TIMER2);

  int i = 0;

  for (int j=1; j<5; j++)
  {
    OTCLIB_TIMEBLOCK(TIMER2);

    for (i=1; i!=20000000; i++) ;
  }
}

void func3()
{
  OTC_Tracer tracer("void func3()");

  OTCLIB_STOPWATCH(TIMER3);

  int i = 0;

  for (int j=1; j<5; j++)
  {
    OTCLIB_TIMEBLOCK(TIMER3);

    for (i=1; i!=30000000; i++) ;
  }
}

void func4()
{
  OTC_Tracer tracer("void func4()");

  OTCLIB_STOPWATCH(TIMER4);

  int i = 0;

  for (int j=1; j<5; j++)
  {
    OTCLIB_TIMEBLOCK(TIMER4);

    for (i=1; i!=40000000; i++) ;
  }
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  func1();

  OTCLIB_STOPWATCH_REPORT(tracer());
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  for (int i=1; i<5; i++)
  {
    func1();
    func2();
    func3();
    func4();
  }

  OTCLIB_STOPWATCH_REPORT(tracer());
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
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
