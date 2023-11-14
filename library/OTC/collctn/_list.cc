/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_list.cc
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

#include "xxxitem1.hh"

#include <OTC/collctn/list.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

extern long tst_random();
extern void tst_srandom(long, long);

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Empty list.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_List<TST_Item1> list;

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  OTC_Iterator<TST_Item1> iter(list.items());
  tracer() << "iter.isValid() = " << iter.isValid() << endl;
}

// = TEST 3
//     Test of addition, complete removal and looking at ends of list.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_List<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);

  tracer() << "list.addFirst(item1)" << endl;
  list.addFirst(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.addFirst(item1)" << endl;
  list.addFirst(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.first().value() = ";
  tracer() << list.first().value() << endl;
  tracer() << "list.last().value() = ";
  tracer() << list.last().value() << endl;

  OTC_Iterator<TST_Item1> iter(list.items());
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item().value() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_List<TST_Item1> list1;

  TST_Item1 item1(1);
  TST_Item1 item2(2);

  tracer() << "list1.addFirst(item1)" << endl;
  list1.addFirst(item1);
  tracer() << "list1.addLast(item2)" << endl;
  list1.addLast(item2);

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  OTC_Iterator<TST_Item1> iter1(list1.items());
  tracer() << "list1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list1 = list1" << endl;
  list1 = list1;

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2(list1)" << endl;
  OTC_List<TST_Item1> list2(list1);

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  OTC_Iterator<TST_Item1> iter2(list2.items());
  tracer() << "list2.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "list1.removeAll()" << endl;
  list1.removeAll();

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list2.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "list1 = list2" << endl;
  list1 = list2;

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list2.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;
}

// = TEST 5
//     Test of single removals.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_List<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.first().value() = ";
  tracer() << list.first().value() << endl;
  tracer() << "list.last().value() = ";
  tracer() << list.last().value() << endl;

  tracer() << "list.removeFirst()" << endl;
  list.removeFirst();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.first().value() = ";
  tracer() << list.first().value() << endl;
  tracer() << "list.last().value() = ";
  tracer() << list.last().value() << endl;

  tracer() << "list.removeLast()" << endl;
  list.removeLast();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.first().value() = ";
  tracer() << list.first().value() << endl;
  tracer() << "list.last().value() = ";
  tracer() << list.last().value() << endl;
}

// = TEST 6
//     Test of looking at items by index.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_List<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.item() =" << flush;
  for (u_int i=0; i<list.population(); i++)
    tracer() << " " << list.item(i).value() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of removing items by index.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_List<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeItem()" << endl;
  while (!list.isEmpty())
    list.removeItem(list.population()-1);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;
}

// = TEST 8
//     Test of removing range of items by index.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_List<TST_Item1> list;

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

  OTC_Range range(1,2);
  tracer() << "list.removeRange(1,2)" << endl;
  list.removeRange(range);

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.items() =" << flush;
  OTC_Iterator<TST_Item1> iter(list.items());
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item().value() << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of iterators over a range of items in a list.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_List<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);
  TST_Item1 item4(4);
  TST_Item1 item5(5);

  list.addLast(item1);
  list.addLast(item2);
  list.addLast(item3);
  list.addLast(item4);
  list.addLast(item5);

  OTC_Iterator<TST_Item1> iter1 = 0;

  OTC_Range range1(0,list.population());
  iter1 = list.items(range1,OTCLIB_FORWARD);
  tracer() << "iter1 = list.items(0,list.population(),OTCLIB_FORWARD) =";
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  iter1 = list.items(range1,OTCLIB_BACKWARD);
  tracer() << "iter1 = list.items(0,list.population(),OTCLIB_BACKWARD) =";
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  OTC_Range range2(1,list.population()-2);
  iter1 = list.items(range2,OTCLIB_FORWARD);
  tracer() << "iter1 = list.items(1,list.population()-2,OTCLIB_FORWARD) =";
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  iter1 = list.items(range2,OTCLIB_BACKWARD);
  tracer() << "iter1 = list.items(1,list.population()-2,OTCLIB_BACKWARD) =";
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  OTC_Iterator<TST_Item1> iter2 = 0;

  iter2 = list.items(range1,OTCLIB_FORWARD);
  tracer() << "iter2 = list.items(0,list.population(),OTCLIB_FORWARD) =";
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;
}

// = TEST 10
//     Test of replacement routines.

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_List<int> list;
  int i;
  for (i=0; i<10; i++)
    list.addLast(i);

  OTC_Iterator<int> iter;

  iter = list.items();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << iter.item() << " ";
  tracer() << endl;

  int j;
  for (i=0,j=9; i<10; i++,j--)
    list.replaceItem(i,j);

  iter = list.items();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << iter.item() << " ";
  tracer() << endl;

  list.replaceFirst(j);
  list.replaceLast(i);

  iter = list.items();
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << iter.item() << " ";
  tracer() << endl;
}

// = TEST 11
//     Test of addition and removal of data by index using random data set.
//     This primarily tests whether or not balancing of the AVL tree is
//     working correctly or not.

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_List<int> list;
  int i;
  for (i=0; i<100; i++)
    list.addLast(i);

  tst_srandom(1,0);
  i = 0;
  while (i < 20000)
  {
    int v;
    v = int(tst_random() % list.population());
    list.removeItem(v);
    list.addBeforeItem(v,v);
    i++;
    if (i % 1000 == 0)
    {
      tracer() << i << " --- ";
      OTC_Iterator<int> iter(list.items());
      for (iter.reset(); iter.isValid(); iter.next())
        tracer() << iter.item() << " ";
      tracer() << endl;
    }
  }
}

// = TEST 12
//     Test of adding iterator.

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_Deque<int> items;

  tst_srandom(3,0);
  for (int i=0; i<10; i++)
  {
    int value = int(tst_random() % 50);
    items.addLast(value);
  }

  tracer() << "items.population() = " << items.population() << endl;
  tracer() << "items.isEmpty() = " << items.isEmpty() << endl;

  OTC_Iterator<int> iter = 0;

  iter = items.items();
  tracer() << "items.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  OTC_List<int> list;

  list.addLast(items.items(OTCLIB_BACKWARD));

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  iter = list.items();
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  list.addFirst(items.items(OTCLIB_BACKWARD));

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  iter = list.items();
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  list.addBeforeItem(items.items(OTCLIB_FORWARD),items.population());

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  iter = list.items();
  tracer() << "list.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
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
  test9();
  test10();
  test11();
  test12();
}

/* -------------------------------------------------------------------------- */

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
  test8,
  test9,
  test10,
  test12
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
