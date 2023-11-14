/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_ilist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "xxxitem1.hh"

#include <OTC/collctn/ilist.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Empty list.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_IList<TST_Item1> list;

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.next()" << endl;
  list.next();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.resetLast()" << endl;
  list.resetLast();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.prev()" << endl;
  list.prev();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;
}

// = TEST 3
//     Items added at ends using addFirst() and addLast().

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_IList<TST_Item1> list;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  TST_Item1 item1(1);
  TST_Item1 item2(2);

  tracer() << "list.addFirst(item1)" << endl;
  list.addFirst(item1);

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.first().value() = ";
  tracer() << list.first().value() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.last().value() = ";
  tracer() << list.last().value() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.next()" << endl;
  list.next();

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.next()" << endl;
  list.next();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.resetLast()" << endl;
  list.resetLast();

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.prev()" << endl;
  list.prev();

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;
}

// = TEST 4
//     Items added using addBefore() and addAfter() while at start or end.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_IList<TST_Item1> list;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();
  tracer() << "list.prev()" << endl;
  list.prev();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  TST_Item1 item1(1);

  tracer() << "list.addBefore(item1)" << endl;
  list.addBefore(item1);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();
  tracer() << "list.prev()" << endl;
  list.prev();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  TST_Item1 item2(2);

  tracer() << "list.addAfter(item2)" << endl;
  list.addAfter(item2);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.resetLast()" << endl;
  list.resetLast();
  tracer() << "list.next()" << endl;
  list.next();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  TST_Item1 item3(3);

  tracer() << "list.addBefore(item3)" << endl;
  list.addBefore(item3);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.resetLast()" << endl;
  list.resetLast();
  tracer() << "list.next()" << endl;
  list.next();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  TST_Item1 item4(4);

  tracer() << "list.addAfter(item4)" << endl;
  list.addAfter(item4);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;
}

// = TEST 5
//     Items added using addBefore() and addAfter() while on item.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_IList<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);
  TST_Item1 item4(4);

  tracer() << "list.addFirst(item1)" << endl;
  list.addFirst(item1);
  tracer() << "list.addLast(item4)" << endl;
  list.addLast(item4);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();
  tracer() << "list.addAfter(item2)" << endl;
  list.addAfter(item2);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.resetLast()" << endl;
  list.resetLast();
  tracer() << "list.addBefore(item3)" << endl;
  list.addBefore(item3);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;
}

// = TEST 6
//     Explicit killing of items.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_IList<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);
  TST_Item1 item4(4);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);
  tracer() << "list.addLast(item4)" << endl;
  list.addLast(item4);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.remove()" << endl;
  list.remove();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.remove()" << endl;
  list.remove();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.remove()" << endl;
  list.remove();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;

  tracer() << "list.resetFirst()" << endl;
  list.resetFirst();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "list.item().value() = ";
  tracer() << list.item().value() << endl;

  tracer() << "list.remove()" << endl;
  list.remove();

  tracer() << "list.isStart() = " << list.isStart() << endl;
  tracer() << "list.isValid() = " << list.isValid() << endl;
  tracer() << "list.isEnd() = " << list.isEnd() << endl;

  tracer() << "items =" << flush;
  list.resetFirst();
  while (list.isValid())
  {
    tracer() << " " << list.item().value() << flush;
    list.next();
  }
  tracer() << endl;
}

// = TEST 7
//     Test of copy constructor.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_IList<int> list1;

  for (int i=0; i<10; i++)
    list1.addLast(i);

  OTC_IList<int> list2 = list1;

  tracer() << "list1.items() =" << flush;
  for (list1.resetFirst(); list1.isValid(); list1.next())
    tracer() << " " << list1.item() << flush;
  tracer() << endl;

  tracer() << "list2.items() =" << flush;
  for (list2.resetFirst(); list2.isValid(); list2.next())
    tracer() << " " << list2.item() << flush;
  tracer() << endl;
}

// = TEST 8
//     Test of assignment operator.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_IList<int> list1;

  for (int i=0; i<10; i++)
    list1.addLast(i);

  OTC_IList<int> list2;
  list2 = list1;

  tracer() << "list1.items() =" << flush;
  for (list1.resetFirst(); list1.isValid(); list1.next())
    tracer() << " " << list1.item() << flush;
  tracer() << endl;

  tracer() << "list2.items() =" << flush;
  for (list2.resetFirst(); list2.isValid(); list2.next())
    tracer() << " " << list2.item() << flush;
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
  test6();
  test7();
  test8();
}

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
