/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_pattern.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/pattern.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

ostream& operator<<(ostream& o, OTC_Range const& r)
{
  o << "(" << r.lower() << "," << r.length() << ")";
  return o;
}

// = TEST 2
//     Test match anything.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Pattern thePattern(OTCLIB_REGEX_EXTENDED,0,".*");

  tracer() << "isValid = " << thePattern.isValid() << endl;
  tracer() << "error = " << thePattern.error() << endl;

  u_int i;

  tracer() << "match(\"\") = " << thePattern.match("") << endl;
  tracer() << "error = " << thePattern.error() << endl;
  for (i=0; i<=thePattern.count(); i++)
    tracer() << "range(" << i << ") = " << thePattern.range(i) << endl;

  tracer() << "match(\"XXX\") = " << thePattern.match("XXX") << endl;
  tracer() << "error = " << thePattern.error() << endl;
  for (i=0; i<=thePattern.count(); i++)
    tracer() << "range(" << i << ") = " << thePattern.range(i) << endl;

  tracer() << "match(\"X\\0X\",3) = " << thePattern.match("X\0X",3) << endl;
  tracer() << "error = " << thePattern.error() << endl;
  for (i=0; i<=thePattern.count(); i++)
    tracer() << "range(" << i << ") = " << thePattern.range(i) << endl;
}

// = TEST 3
//     Test match sub patterns.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Pattern thePattern(OTCLIB_REGEX_EXTENDED,0,"^((X)((X)(X)))$");

  u_int i;

  tracer() << "isValid = " << thePattern.isValid() << endl;
  tracer() << "error = " << thePattern.error() << endl;

  tracer() << "match(\"\") = " << thePattern.match("") << endl;
  tracer() << "error = " << thePattern.error() << endl;
  for (i=0; i<=thePattern.count(); i++)
  {
    tracer() << "start(" << i << ") = " << thePattern.start(i) << endl;
    tracer() << "range(" << i << ") = " << thePattern.range(i) << endl;
  }

  tracer() << "match(\"XXX\") = " << thePattern.match("XXX") << endl;
  tracer() << "error = " << thePattern.error() << endl;
  for (i=0; i<=thePattern.count(); i++)
  {
    tracer() << "start(" << i << ") = " << thePattern.start(i) << endl;
    tracer() << "range(" << i << ") = " << thePattern.range(i) << endl;
  }

  tracer() << "match(\"XXXX\") = " << thePattern.match("XXXX") << endl;
  tracer() << "error = " << thePattern.error() << endl;
  for (i=0; i<=thePattern.count(); i++)
  {
    tracer() << "start(" << i << ") = " << thePattern.start(i) << endl;
    tracer() << "range(" << i << ") = " << thePattern.range(i) << endl;
  }
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
