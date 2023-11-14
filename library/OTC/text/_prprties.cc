/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_prprties.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1998-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/prprties.hh>

#include <OTC/text/slogger.hh>
#include <OTC/debug/tracer.hh>

#include <OTC/stdlib/iostream.h>
#include <OTC/stdlib/strstream.h>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

char* text =
 "one : value0 \t \t\n"
 "*.two : value1\n"
 "one.two : value2\n"
 "one.two.three : value3\n"
 "one.*.two.three : value4\n"
 "one.two.*.three : value5\n"
 "one.two.three.four : value5\\\nvalue6";

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Properties theDatabase;
  theDatabase.load(cin);

  tracer() << endl;
  tracer() << endl;
  tracer() << endl;

  theDatabase.save(tracer());

  tracer() << endl;
  tracer() << endl;
  tracer() << endl;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_SLogger theCapture("@TEST");

  istrstream ins(text,strlen(text));

  OTC_Properties theDatabase;
  theDatabase.load(ins,"@TEST");

  OTC_String theLine;

  while (cin.good())
  {
    theLine.readLine(cin);
    if (!cin.fail())
    {
      OTC_String theValue;

      theValue = theDatabase.lookup(theLine);

      if (theValue.isUndefined())
	tracer() << "<undefined>" << endl;
      else
	tracer() << "\"" << theValue << "\"" << endl;
    }
  }

  tracer() << endl;
  tracer() << endl;
  tracer() << endl;

  tracer() << theCapture.messages() << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_SLogger theCapture("@TEST");

  OTC_Properties theDatabase;
  theDatabase.merge("prefix",cin,"@TEST");

  tracer() << endl;
  tracer() << endl;
  tracer() << endl;

  tracer() << theDatabase.encode() << flush;

  tracer() << endl;
  tracer() << endl;
  tracer() << endl;

  tracer() << theCapture.messages() << endl;
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
