/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/_resource.cc
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

#include <OTC/refcnt/resource.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

#include "xxxitem2.hh"

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test of stack object with no references.

void test2()
{
  OTC_Tracer tracer("void test2()");

  TST_Item1 item(1);
  tracer() << "item.numRefs() = " << item.numRefs() << endl;
}

// = TEST 3
//     Test of heap object with no references.

void test3()
{
  OTC_Tracer tracer("void test3()");

  TST_Item1* item = new TST_Item1(1);
  tracer() << "item->numRefs() = " << item->numRefs() << endl;
  delete item;
}

// = TEST 4
//     Test of reference/dereference.

void test4()
{
  OTC_Tracer tracer("void test4()");

  TST_Item1* item = new TST_Item1(1);

  tracer() << "item->numRefs() = " << item->numRefs() << endl;

  tracer() << "item->reference()" << endl;
  item->reference();

  tracer() << "item->numRefs() = " << item->numRefs() << endl;

  tracer() << "item->reference()" << endl;
  item->reference();

  tracer() << "item->numRefs() = " << item->numRefs() << endl;

  tracer() << "item->unReference()" << endl;
  item->unReference();

  tracer() << "item->numRefs() = " << item->numRefs() << endl;

  tracer() << "item->unReference()" << endl;
  item->unReference();
}

// = TEST 5
//     Test of explicit deletion of referenced object.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  TST_Item1* item = new TST_Item1(1);

  tracer() << "item->numRefs() = " << item->numRefs() << endl;

  tracer() << "item->reference()" << endl;
  item->reference();

  tracer() << "item->numRefs() = " << item->numRefs() << endl;

  tracer() << "delete item" << endl;
  delete item;

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 6
//     Test of reference of stack object.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  TST_Item1 item(1);

  tracer() << "item.reference()" << endl;
  item.reference();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 7
//     Test of unReference of stack object.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  TST_Item1 item(1);

  tracer() << "item.unReference()" << endl;
  item.unReference();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 8
//     Test of unReference of non referenced object.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Logger::notify(OTCLIB_LOG_INFO,"This test is meant to fail");

  TST_Item1* item = new TST_Item1(1);

  tracer() << "item->unReference()" << endl;
  item->unReference();

  tracer() << "XXX - shouldn't reach here" << endl;
}

// = TEST 1
//     All tests. Excluding those that generate exceptions.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
}

/* ------------------------------------------------------------------------- */

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5,
  test6,
  test7,
  test8
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
