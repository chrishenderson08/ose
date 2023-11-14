/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/_resptr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/refcnt/resptr.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/OTC.h>

#include <stdlib.h>

#include "xxxitem2.hh"

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test of pointers referencing nothing.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_ResPtr<TST_Item1> ptr1;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;

  tracer() << "ptr1 = 0" << endl;
  ptr1 = 0;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;

  tracer() << "ptr1 = ptr1" << endl;
  ptr1 = ptr1;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;

  OTC_ResPtr<TST_Item1> ptr2;

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "ptr2.item() = " << ptr2.item() << endl;
  tracer() << "ptr2 = " << (TST_Item1*)ptr2 << endl;

  tracer() << "ptr1 = ptr2" << endl;
  ptr1 = ptr2;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "ptr2.item() = " << ptr2.item() << endl;
  tracer() << "ptr2 = " << (TST_Item1*)ptr2 << endl;
}

// = TEST 3
//     Test of single pointer and item.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_ResPtr<TST_Item1> ptr1;

  tracer() << "ptr1 = new TST_Item1(1)" << endl;
  ptr1 = new TST_Item1(1);

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  tracer() << "ptr1 = 0" << endl;
  ptr1 = 0;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;
}

// = TEST 4
//     Test of single pointer and two items.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_ResPtr<TST_Item1> ptr1;

  tracer() << "ptr1 = new TST_Item1(1)" << endl;
  ptr1 = new TST_Item1(1);

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  tracer() << "ptr1 = new TST_Item1(2)" << endl;
  ptr1 = new TST_Item1(2);

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;
}

// = TEST 5
//     Test of two pointers and single item.

void test5()
{
  OTC_Tracer tracer("void test5()");

  tracer() << "ptr1 = new TST_Item1(1)" << endl;
  OTC_ResPtr<TST_Item1> ptr1 = new TST_Item1(1);

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  tracer() << "ptr1 = ptr1" << endl;
  ptr1 = ptr1;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  tracer() << "ptr2 = ptr1" << endl;
  OTC_ResPtr<TST_Item1> ptr2 = ptr1;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "!ptr2 = " << !ptr2 << endl;
  tracer() << "ptr2.item()->value() = " << ptr2.item()->value() << endl;
  tracer() << "ptr2->value() = " << ptr2->value() << endl;
  tracer() << "(*ptr2).value() = " << (*ptr2).value() << endl;

  tracer() << "ptr1 = 0" << endl;
  ptr1 = 0;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "!ptr2 = " << !ptr2 << endl;
  tracer() << "ptr2.item()->value() = " << ptr2.item()->value() << endl;
  tracer() << "ptr2->value() = " << ptr2->value() << endl;
  tracer() << "(*ptr2).value() = " << (*ptr2).value() << endl;

  tracer() << "ptr2 = ptr1" << endl;
  ptr2 = ptr1;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "ptr1.item() = " << ptr1.item() << endl;
  tracer() << "ptr1 = " << (TST_Item1*)ptr1 << endl;

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "ptr2.item() = " << ptr2.item() << endl;
  tracer() << "ptr2 = " << (TST_Item1*)ptr2 << endl;
}

// = TEST 6
//     Test of two pointers and two items.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_ResPtr<TST_Item1> ptr1;

  tracer() << "ptr1 = new TST_Item1(1)" << endl;
  ptr1 = new TST_Item1(1);

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  OTC_ResPtr<TST_Item1> ptr2;

  tracer() << "ptr2 = new TST_Item1(2)" << endl;
  ptr2 = new TST_Item1(2);

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "!ptr2 = " << !ptr2 << endl;
  tracer() << "ptr2.item()->value() = " << ptr2.item()->value() << endl;
  tracer() << "ptr2->value() = " << ptr2->value() << endl;
  tracer() << "(*ptr2).value() = " << (*ptr2).value() << endl;

  tracer() << "ptr1 = ptr2" << endl;
  ptr1 = ptr2;

  tracer() << "ptr1.numRefs() = " << ptr1.numRefs() << endl;
  tracer() << "!ptr1 = " << !ptr1 << endl;
  tracer() << "ptr1.item()->value() = " << ptr1.item()->value() << endl;
  tracer() << "ptr1->value() = " << ptr1->value() << endl;
  tracer() << "(*ptr1).value() = " << (*ptr1).value() << endl;

  tracer() << "ptr2.numRefs() = " << ptr2.numRefs() << endl;
  tracer() << "!ptr2 = " << !ptr2 << endl;
  tracer() << "ptr2.item()->value() = " << ptr2.item()->value() << endl;
  tracer() << "ptr2->value() = " << ptr2->value() << endl;
  tracer() << "(*ptr2).value() = " << (*ptr2).value() << endl;
}

// = TEST 7
//     Test of implicit conversions for 'if' etc.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_ResPtr<TST_Item1> ptr1;

  tracer() << "ptr1 = new TST_Item1(1)" << endl;
  ptr1 = new TST_Item1(1);

//  if (ptr1)
//    tracer() << "(ptr)" << endl;

  if (ptr1 == 0)
    tracer() << "(ptr == 0)" << endl;

  if (ptr1 != 0)
    tracer() << "(ptr != 0)" << endl;
}

// = TEST 1
//     All tests. Excluding those which generate exceptions.

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
  test3();
  test4();
  test5();
  test6();
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
  test7
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
