/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/_ptr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/ptr.hh>
#include <OTC/debug/tracer.hh>

#include "tstitem1.hh"

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Ptr<TST_Item1> count;
  tracer() << "count.isUndefined() = " << count.isUndefined() << endl;
  tracer() << "*count = 42" << endl;
  *count = 42;
  tracer() << "count.isUndefined() = " << count.isUndefined() << endl;
  tracer() << "*count == " << *count << endl;
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
