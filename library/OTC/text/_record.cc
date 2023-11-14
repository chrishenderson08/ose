/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     text/_record.cc
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

#include <OTC/text/record.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test AWK style splitting with explicit character.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Record record(".one..two.three.",'.');

  for (u_int i=0; i<=record.numFields(); i++)
  {
    tracer() << "record.field(" << i << ") = [";
    tracer() << record.field(i) << "]" << endl;
  }

  tracer() << "record.fields() =" << flush;
  OTC_Iterator<OTC_String> iter = 0;
  iter = record.fields();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " [" << iter.item() << "]" << flush;
  tracer() << endl;
}

// = TEST 3
//     Test AWK style splitting with whitespace as the field separater.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Record record(" one\t\ttwo three\t");

  for (u_int i=0; i<=record.numFields(); i++)
  {
    tracer() << "record.field(" << i << ") = [";
    tracer() << record.field(i) << "]" << endl;
  }

  tracer() << "record.fields() =" << flush;
  OTC_Iterator<OTC_String> iter = 0;
  iter = record.fields();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " [" << iter.item() << "]" << flush;
  tracer() << endl;
}

// = TEST 4
//     Test strtok style splitting using whitespace characters.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Record record(" one\t two three\t"," \t\n\r\f");

  for (u_int i=0; i<=record.numFields(); i++)
  {
    tracer() << "record.field(" << i << ") = [";
    tracer() << record.field(i) << "]" << endl;
  }

  tracer() << "record.fields() =" << flush;
  OTC_Iterator<OTC_String> iter = 0;
  iter = record.fields();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " [" << iter.item() << "]" << flush;
  tracer() << endl;
}

// = TEST 5
//     Test strtok style splitting using explicit character.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Record record(".one..two.three.",".");

  for (u_int i=0; i<=record.numFields(); i++)
  {
    tracer() << "record.field(" << i << ") = [";
    tracer() << record.field(i) << "]" << endl;
  }

  tracer() << "record.fields() =" << flush;
  OTC_Iterator<OTC_String> iter = 0;
  iter = record.fields();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " [" << iter.item() << "]" << flush;
  tracer() << endl;
}

// = TEST 1
//     All tests.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
  test5();
}

/* ------------------------------------------------------------------------- */

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
