/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_range.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/range.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Iterator<int> iter;

  tracer() << "range1 = ()" << endl;
  OTC_Range range1;
  tracer() << "range1.isNullRange() = " << range1.isNullRange() << endl;
  tracer() << "range1 =" << flush;
  iter = range1.indices();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  tracer() << "range2 = (5)" << endl;
  OTC_Range range2(5);
  tracer() << "range2.isNullRange() = " << range2.isNullRange() << endl;
  tracer() << "range2 =" << flush;
  iter = range2.indices();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  tracer() << "range3 = (-5,10)" << endl;
  OTC_Range range3(-5,10);
  tracer() << "range3.isNullRange() = " << range3.isNullRange() << endl;
  tracer() << "range3 =" << flush;
  iter = range3.indices();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  tracer() << "range3(backward) =" << flush;
  iter = range3.indices(OTCLIB_BACKWARD);
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  tracer() << "range1.isSubRange(range1) = ";
  tracer() << range1.isSubRange(range1) << endl;
  tracer() << "range1.isSubRange(range2) = ";
  tracer() << range1.isSubRange(range2) << endl;
  tracer() << "range2.isSubRange(range1) = ";
  tracer() << range2.isSubRange(range1) << endl;
  tracer() << "range3.isSubRange(range2) = ";
  tracer() << range3.isSubRange(range2) << endl;
  tracer() << "range2.isSubRange(range3) = ";
  tracer() << range2.isSubRange(range3) << endl;
  tracer() << "range3.isSubRange(range3) = ";
  tracer() << range3.isSubRange(range3) << endl;
}

// = TEST 1
//     All tests.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
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
