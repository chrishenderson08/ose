/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_domstrng.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/domstrng.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

void test2()
{
  OTC_Tracer tracer("void test2()");

  char const* theArray = "text";

  OTC_DOMString theString(theArray);

  tracer() << "string=" << theString << endl;
  tracer() << "equal=" << (theString == "text") << endl;
  tracer() << "not-equal=" << (theString != "text") << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  char const* theArray = "text\000text";

  OTC_DOMString theString(theArray,6);

  tracer() << "string=" << theString << endl;
  tracer() << "equal=" << (theString == "text") << endl;
  tracer() << "not-equal=" << (theString != "text") << endl;
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
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
