/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/_warning.cc
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

#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 1
//     Test of message to logger at warning level. Visual test.

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTCLIB_WARNING("Error has occurred");
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
