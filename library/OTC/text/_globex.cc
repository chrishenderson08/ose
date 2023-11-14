/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_globex.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1996-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/globex.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test exact match.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Globex expr("one");
  tracer() << "expr.pattern() = " << expr.pattern() << endl;
  tracer() << "expr.isValid() = " << expr.isValid() << endl;

  tracer() << "expr.match(\"\") = " << expr.match("") << endl;
  tracer() << "expr.match(\"one\") = " << expr.match("one") << endl;
  tracer() << "expr.match(\"two\") = " << expr.match("two") << endl;
}

// = TEST 3
//     Test globbing.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Globex expr("*.cc");
  tracer() << "expr.pattern() = " << expr.pattern() << endl;
  tracer() << "expr.isValid() = " << expr.isValid() << endl;

  tracer() << "expr.match(\"\") = " << expr.match("") << endl;
  tracer() << "expr.match(\".ccc\") = " << expr.match(".ccc") << endl;
  tracer() << "expr.match(\".cc\") = " << expr.match(".cc") << endl;
  tracer() << "expr.match(\"one.cc\") = " << expr.match("one.cc") << endl;
  tracer() << "expr.match(\"two.cc\") = " << expr.match("two.cc") << endl;
}

// = TEST 1
//     All tests.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
}

/* ------------------------------------------------------------------------- */

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
