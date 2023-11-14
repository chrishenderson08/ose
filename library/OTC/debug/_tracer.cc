/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/_tracer.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#define OTCLIB_TRACE 1

#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void dummy()
{
  OTCLIB_MARKBLOCK(1,"void dummy()");
}

void function()
{
  OTCLIB_MARKBLOCK(1,"void function()");
  OTCLIB_TRACER(0) << "Level 0" << endl;
  OTCLIB_TRACER(1) << "Level 1" << endl;
  OTCLIB_TRACER(2) << "Level 2" << endl;
  OTCLIB_TRACER(3) << "Level 3" << endl;
  OTCLIB_TRACER(4) << "Level 4" << endl;
  OTCLIB_TRACER(5) << "Level 5" << endl;
}

// = TEST 1
//     Test tracer at various levels explicitly.

void test1()
{
  OTC_Tracer::setLevel(1);
  OTCLIB_MARKBLOCK(1,"void test1()");

  for (int i=0; i<=5; i++)
  {
    OTC_Tracer::setLevel(i);
    function();
  }
  OTC_Tracer::setLevel(1);
}

// = TEST 2
//     Test of standalone trace.

void test2()
{
  OTCLIB_MARKBLOCK(1,"void test2()");

  OTCLIB_TRACER(0) << "Level 0" << endl;
  OTCLIB_TRACER(1) << "Level 1" << endl;
  OTCLIB_TRACER(2) << "Level 2" << endl;
  OTCLIB_TRACER(3) << "Level 3" << endl;
  OTCLIB_TRACER(4) << "Level 4" << endl;
}

// = TEST 3
//     Test without use of OTCLIB_MARKBLOCK.

void test3()
{
  OTCLIB_TRACER(4) << "Level 4" << endl;
  OTCLIB_TRACER(3) << "Level 3" << endl;
  OTCLIB_TRACER(2) << "Level 2" << endl;
  OTCLIB_TRACER(1) << "Level 1" << endl;
  OTCLIB_TRACER(0) << "Level 0" << endl;
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3
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
