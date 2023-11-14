/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/_pool.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/pool.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test1()
{
  OTC_Tracer tracer("void test1()");

  void* mem1;
  void* mem2;

  OTC_Pool pool1(10);
  mem1 = pool1.allocate();
  pool1.release(mem1);

  OTC_Pool pool2(10,30);
  mem1 = pool2.allocate();
  pool2.release(mem1);

  OTC_Pool pool3(10,0);
  mem1 = pool3.allocate();
  mem2 = pool3.allocate();
  pool3.release(mem1);
  pool3.release(mem2);

  OTC_Pool pool4(64);
  mem1 = pool4.allocate();
  mem2 = pool4.allocate();
  pool4.release(mem1);
  pool4.release(mem2);
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
