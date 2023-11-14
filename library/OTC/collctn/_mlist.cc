/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_mlist.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "xxxitem1.hh"
#include "xxxitem2.hh"

#include <OTC/collctn/mlist.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OTC_BaseActions<TST_Item2*>
{
  public:

    static TST_Item2* const&	add(TST_Item2* const& theItem)
				{ theItem->reference(); return theItem; }

    static void		remove(TST_Item2*& theItem)
				{ theItem->unReference(); }
};

// = TEST 2
//     Empty list.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_MList<TST_Item1> list;
  OTC_MList<TST_Item1> const& listC = list;

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  OTC_MListIterator<TST_Item1> iter(listC);
  tracer() << "iter.isValid() = " << iter.isValid() << endl;
}

// = TEST 3
//     Test of addition, complete removal and looking at ends of list.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_MList<TST_Item1> list;

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

  OTC_MListIterator<TST_Item1> iter(list);
  tracer() << "list.items() =" << flush;
  for (iter.resetFirst(); iter.isValid(); iter.next())
    tracer() << " " << iter.item().value() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_MList<TST_Item1> list1;

  TST_Item1 item1(1);
  TST_Item1 item2(2);

  tracer() << "list1.addFirst(item1)" << endl;
  list1.addFirst(item1);
  tracer() << "list1.addLast(item2)" << endl;
  list1.addLast(item2);

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  OTC_MListIterator<TST_Item1> iter1(list1);
  tracer() << "list1.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list1 = list1" << endl;
  list1 = list1;

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2(list1)" << endl;
  OTC_MList<TST_Item1> list2(list1);

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  OTC_MListIterator<TST_Item1> iter2(list2);
  tracer() << "list2.items() =" << flush;
  for (iter2.resetFirst(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "list1.removeAll()" << endl;
  list1.removeAll();

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list2.items() =" << flush;
  for (iter2.resetFirst(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "list1 = list2" << endl;
  list1 = list2;

  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list1.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list2.items() =" << flush;
  for (iter2.resetFirst(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;
}

// = TEST 5
//     Test of single removals.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_MList<TST_Item1> list;

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
//     Test of iterator traversal, backward as well as forward.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_MList<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  OTC_MListIterator<TST_Item1> iter1(list);
  tracer() << "list.items(OTCLIB_FORWARD) =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "list.items(OTCLIB_BACKWARD) =" << flush;
  for (iter1.resetLast(); iter1.isValid(); iter1.prev())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of modification of items using iterator.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_MList<TST_Item1> list;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  OTC_MListModifier<TST_Item1> iter1(list);
  tracer() << "list.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;
  tracer() << "items.invert()" << endl;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    iter1.item().invert();
  tracer() << "list.items() =" << flush;
  for (iter1.resetFirst(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;
}

// = TEST 8
//     Test lifetimes of objects when all are removed together.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_MList<TST_Item2*> list;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  TST_Item2* item4 = new TST_Item2(4);
  TST_Item2* item5 = new TST_Item2(5);
  TST_Item2* item6 = new TST_Item2(6);

  tracer() << "list.addLast(item4)" << endl;
  list.addLast(item4);
  tracer() << "list.addLast(item5)" << endl;
  list.addLast(item5);
  tracer() << "list.addLast(item6)" << endl;
  list.addLast(item6);

  tracer() << "list.dtor()" << endl;
}

// = TEST 9
//     Test lifetimes of objects in multiple list's.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_MList<TST_Item2*> list1;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "list1.addLast(item1)" << endl;
  list1.addLast(item1);
  tracer() << "list1.addLast(item2)" << endl;
  list1.addLast(item2);
  tracer() << "list1.addLast(item3)" << endl;
  list1.addLast(item3);

  OTC_MList<TST_Item2*> list2;

  tracer() << "list2.addLast(item2)" << endl;
  list2.addLast(item2);

  tracer() << "list1.removeAll()" << endl;
  list1.removeAll();

  tracer() << "list2.removeAll()" << endl;
  list2.removeAll();
}

// = TEST 10
//     Test lifetimes of objects when individual objects removed.

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_MList<TST_Item2*> list;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  tracer() << "list.removeFirst()" << endl;
  list.removeFirst();
  tracer() << "list.removeFirst()" << endl;
  list.removeFirst();
  tracer() << "list.removeFirst()" << endl;
  list.removeFirst();
}

// = TEST 11
//     Test lifetime of object when indivdual object is referenced by
//     iterator when removed.

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_MList<TST_Item2*> list;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  OTC_MListIterator<TST_Item2*> iter(list);
  iter.resetFirst();
  while (iter.isValid())
  {
    tracer() << "list.removeFirst()" << endl;
    list.removeFirst();
    tracer() << "iter.item()->value() = " << iter.item()->value() << endl;
    tracer() << "iter.next()" << endl;
    iter.next();
  }
}

// = TEST 12
//     Test remove all of items via modifier.

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_MList<TST_Item2*> list;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "list.addLast(item1)" << endl;
  list.addLast(item1);
  tracer() << "list.addLast(item2)" << endl;
  list.addLast(item2);
  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);

  tracer() << "list.population() = " << list.population() << endl;

  OTC_MListModifier<TST_Item2*> iter(list);
  iter.resetFirst();
  while (iter.isValid())
  {
    tracer() << "iter.remove()" << endl;
    iter.remove();
    tracer() << "iter.item()->value() = " << iter.item()->value() << endl;
    tracer() << "list.population() = " << list.population() << endl;
    tracer() << "iter.next()" << endl;
    iter.next();
  }
}

// = TEST 13
//     Test addition of items via modifier.

void test13()
{
  OTC_Tracer tracer("void test13()");

  OTC_MList<TST_Item2*> list;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);
  TST_Item2* item4 = new TST_Item2(4);
  TST_Item2* item5 = new TST_Item2(5);
  TST_Item2* item6 = new TST_Item2(6);

  tracer() << "list.addLast(item3)" << endl;
  list.addLast(item3);
  tracer() << "list.addLast(item4)" << endl;
  list.addLast(item4);

  tracer() << "list.population() = " << list.population() << endl;

  tracer() << "iter(list)" << endl;
  OTC_MListModifier<TST_Item2*> iter(list);

  tracer() << "iter.resetFirst()" << endl;
  iter.resetFirst();
  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.addAfter(item2)" << endl;
  iter.addAfter(item2);
  tracer() << "iter.addBefore(item1)" << endl;
  iter.addBefore(item1);

  tracer() << "list.population() = " << list.population() << endl;

  tracer() << "iter.resetLast()" << endl;
  iter.resetLast();
  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.addBefore(item5)" << endl;
  iter.addBefore(item5);
  tracer() << "iter.addAfter(item6)" << endl;
  iter.addAfter(item6);

  tracer() << "list.population() = " << list.population() << endl;

  tracer() << "list.items() =" << flush;
  for (iter.resetFirst(); iter.isValid(); iter.next())
    tracer() << " " << iter.item()->value() << flush;
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
  test13();
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
  test11,
  test12,
  test13
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
