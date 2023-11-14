/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/_ensure.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/tracer.hh>
#include <OTC/debug/excption.hh>
#include <OTC/debug/precfail.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 1
//     Test failure of precondition.

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTCLIB_ENSURE((1 == 0),"1 is not equal to 0");

  tracer() << "XXX - Shouldn't reach here" << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTCLIB_ENSURE_W((1 == 0),"1 is not equal to 0");
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTCLIB_ENSURE_FN((1 == 0),"test3()","1 is not equal to 0");

  tracer() << "XXX - Shouldn't reach here" << endl;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTCLIB_ENSURE_FN_W((1 == 0),"test4()","1 is not equal to 0");
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  try
  {
    tracer() << "try" << endl;
    OTCLIB_ENSURE((1 == 0),"1 is not equal to 0");
  }
  catch (OTCERR_PreconditionFailure& theException2)
  {
    tracer() << "catch OTCERR_PreconditionFailure" << endl;
    theException2.display(tracer());
    tracer() << flush;
  }
  catch (OTC_Exception& theException1)
  {
    tracer() << "catch OTC_Exception" << endl;
    theException1.display(tracer());
    tracer() << flush;
  }
}

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
