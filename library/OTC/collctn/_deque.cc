/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_deque.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include "xxxitem1.hh"
#include "xxxitem2.hh"

#include <OTC/collctn/deque.hh>
#include <OTC/refcnt/resource.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

extern long tst_random();
extern void tst_srandom(long, long);

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
//     Empty deque.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Deque<TST_Item1> deque;

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.removeAll()" << endl;
  deque.removeAll();

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  OTC_Iterator<TST_Item1> iter(deque.items());
  tracer() << "iter.isValid() = " << iter.isValid() << endl;
}

// = TEST 3
//     Test of addition, complete removal and looking at ends of deque.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Deque<TST_Item1> deque;

  TST_Item1 item1(1);
  TST_Item1 item2(2);

  tracer() << "deque.addFirst(item1)" << endl;
  deque.addFirst(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.removeAll()" << endl;
  deque.removeAll();

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.addFirst(item1)" << endl;
  deque.addFirst(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.first().value() = ";
  tracer() << deque.first().value() << endl;
  tracer() << "deque.last().value() = ";
  tracer() << deque.last().value() << endl;

  OTC_Iterator<TST_Item1> iter(deque.items());
  tracer() << "deque.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item().value() << flush;
  tracer() << endl;
}

// = TEST 4
//     Test of copy constructor and assignment operator.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Deque<TST_Item1> deque1;

  TST_Item1 item1(1);
  TST_Item1 item2(2);

  tracer() << "deque1.addFirst(item1)" << endl;
  deque1.addFirst(item1);
  tracer() << "deque1.addLast(item2)" << endl;
  deque1.addLast(item2);

  tracer() << "deque1.population() = " << deque1.population() << endl;
  tracer() << "deque1.isEmpty() = " << deque1.isEmpty() << endl;

  OTC_Iterator<TST_Item1> iter1(deque1.items());
  tracer() << "deque1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "deque1 = deque1" << endl;
  deque1 = deque1;

  tracer() << "deque1.population() = " << deque1.population() << endl;
  tracer() << "deque1.isEmpty() = " << deque1.isEmpty() << endl;

  tracer() << "deque1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "deque2(deque1)" << endl;
  OTC_Deque<TST_Item1> deque2(deque1);

  tracer() << "deque1.population() = " << deque1.population() << endl;
  tracer() << "deque1.isEmpty() = " << deque1.isEmpty() << endl;

  tracer() << "deque1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "deque2.population() = " << deque2.population() << endl;
  tracer() << "deque2.isEmpty() = " << deque2.isEmpty() << endl;

  OTC_Iterator<TST_Item1> iter2(deque2.items());
  tracer() << "deque2.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "deque1.removeAll()" << endl;
  deque1.removeAll();

  tracer() << "deque1.population() = " << deque1.population() << endl;
  tracer() << "deque1.isEmpty() = " << deque1.isEmpty() << endl;

  tracer() << "deque1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "deque2.population() = " << deque2.population() << endl;
  tracer() << "deque2.isEmpty() = " << deque2.isEmpty() << endl;

  tracer() << "deque2.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "deque1 = deque2" << endl;
  deque1 = deque2;

  tracer() << "deque1.population() = " << deque1.population() << endl;
  tracer() << "deque1.isEmpty() = " << deque1.isEmpty() << endl;

  tracer() << "deque1.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "deque2.population() = " << deque2.population() << endl;
  tracer() << "deque2.isEmpty() = " << deque2.isEmpty() << endl;

  tracer() << "deque2.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;
}

// = TEST 5
//     Test of single removals.

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Deque<TST_Item1> deque;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.first().value() = ";
  tracer() << deque.first().value() << endl;
  tracer() << "deque.last().value() = ";
  tracer() << deque.last().value() << endl;

  tracer() << "deque.removeFirst()" << endl;
  deque.removeFirst();

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.first().value() = ";
  tracer() << deque.first().value() << endl;
  tracer() << "deque.last().value() = ";
  tracer() << deque.last().value() << endl;

  tracer() << "deque.removeLast()" << endl;
  deque.removeLast();

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  tracer() << "deque.first().value() = ";
  tracer() << deque.first().value() << endl;
  tracer() << "deque.last().value() = ";
  tracer() << deque.last().value() << endl;
}

// = TEST 6
//     Test of iterator traversal, backward as well as forward.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Deque<TST_Item1> deque;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  OTC_Iterator<TST_Item1> iter1(deque.items(OTCLIB_FORWARD));
  tracer() << "deque.items(OTCLIB_FORWARD) =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  OTC_Iterator<TST_Item1> iter2(deque.items(OTCLIB_BACKWARD));
  tracer() << "deque.items(OTCLIB_BACKWARD) =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;

  tracer() << "deque.items(OTCLIB_FORWARD) =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;

  tracer() << "deque.items(OTCLIB_BACKWARD) =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of modification of items using iterator.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Deque<TST_Item1> deque;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  OTC_Modifier<TST_Item1> iter1(deque.items());
  tracer() << "deque.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;
  tracer() << "items.invert()" << endl;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    iter1.item().invert();
  tracer() << "deque.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;
}

// = TEST 8
//     Test of cloning iterators.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Deque<TST_Item1> deque;

  TST_Item1 item1(1);
  TST_Item1 item2(2);
  TST_Item1 item3(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  OTC_Iterator<TST_Item1> iter1(deque.items());
  OTC_Iterator<TST_Item1> iter2(iter1.clone());
  tracer() << "deque.items() =" << flush;
  for (iter1.reset(); iter1.isValid(); iter1.next())
    tracer() << " " << iter1.item().value() << flush;
  tracer() << endl;
  tracer() << "deque.items() =" << flush;
  for (iter2.reset(); iter2.isValid(); iter2.next())
    tracer() << " " << iter2.item().value() << flush;
  tracer() << endl;
}

// = TEST 9
//     Test lifetimes of objects when all are removed together.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Deque<TST_Item2*> deque;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  tracer() << "deque.removeAll()" << endl;
  deque.removeAll();

  TST_Item2* item4 = new TST_Item2(4);
  TST_Item2* item5 = new TST_Item2(5);
  TST_Item2* item6 = new TST_Item2(6);

  tracer() << "deque.addLast(item4)" << endl;
  deque.addLast(item4);
  tracer() << "deque.addLast(item5)" << endl;
  deque.addLast(item5);
  tracer() << "deque.addLast(item6)" << endl;
  deque.addLast(item6);
}

// = TEST 10
//     Test lifetimes of objects in multiple deque's.

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_Deque<TST_Item2*> deque1;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "deque1.addLast(item1)" << endl;
  deque1.addLast(item1);
  tracer() << "deque1.addLast(item2)" << endl;
  deque1.addLast(item2);
  tracer() << "deque1.addLast(item3)" << endl;
  deque1.addLast(item3);

  OTC_Deque<TST_Item2*> deque2;

  tracer() << "deque2.addLast(item2)" << endl;
  deque2.addLast(item2);

  tracer() << "deque1.removeAll()" << endl;
  deque1.removeAll();

  tracer() << "deque2.removeAll()" << endl;
  deque2.removeAll();
}

// = TEST 11
//     Test lifetimes of objects when individual objects removed.

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_Deque<TST_Item2*> deque;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  tracer() << "deque.removeFirst()" << endl;
  deque.removeFirst();
  tracer() << "deque.removeFirst()" << endl;
  deque.removeFirst();
  tracer() << "deque.removeFirst()" << endl;
  deque.removeFirst();
}

// = TEST 12
//     Test lifetime of object when indivdual object is referenced by
//     iterator when removed.

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_Deque<TST_Item2*> deque;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "deque.addLast(item1)" << endl;
  deque.addLast(item1);
  tracer() << "deque.addLast(item2)" << endl;
  deque.addLast(item2);
  tracer() << "deque.addLast(item3)" << endl;
  deque.addLast(item3);

  OTC_Iterator<TST_Item2*> iter(deque.items());
  iter.reset();
  while (iter.isValid())
  {
    tracer() << "deque.removeFirst()" << endl;
    deque.removeFirst();
    tracer() << "iter.item()->value() = " << iter.item()->value() << endl;
    tracer() << "iter.next()" << endl;
    iter.next();
  }
}

// = TEST 13
//     Test lifetime of objects when deque is referenced by iterator
//     and whole deque deleted but objects not explicitly removed.

void test13()
{
  OTC_Tracer tracer("void test13()");

  OTC_Deque<TST_Item2*>* deque = new OTC_Deque<TST_Item2*>;

  TST_Item2* item1 = new TST_Item2(1);
  TST_Item2* item2 = new TST_Item2(2);
  TST_Item2* item3 = new TST_Item2(3);

  tracer() << "deque->addLast(item1)" << endl;
  deque->addLast(item1);
  tracer() << "deque->addLast(item2)" << endl;
  deque->addLast(item2);
  tracer() << "deque->addLast(item3)" << endl;
  deque->addLast(item3);

  OTC_Iterator<TST_Item2*> iter = 0;
  tracer() << "iter = deque->items()" << endl;
  iter = deque->items();

  tracer() << "delete deque" << endl;
  delete deque;

  tracer() << "iter.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item()->value() << flush;
  tracer() << endl;

  tracer() << "iter = 0" << endl;
  iter = 0;

  tracer() << "iter.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item()->value() << flush;
  tracer() << endl;
}

// = TEST 14
//     Test of adding iterator.

void test14()
{
  OTC_Tracer tracer("void test14()");

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

  OTC_Deque<int> deque;

  deque.addLast(items.items(OTCLIB_BACKWARD));

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  iter = deque.items();
  tracer() << "deque.items() =" << flush;
  for (iter.reset(); iter.isValid(); iter.next())
    tracer() << " " << iter.item() << flush;
  tracer() << endl;

  deque.addFirst(items.items(OTCLIB_FORWARD));

  tracer() << "deque.population() = " << deque.population() << endl;
  tracer() << "deque.isEmpty() = " << deque.isEmpty() << endl;

  iter = deque.items();
  tracer() << "deque.items() =" << flush;
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
  test13();
  test14();
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
  test13,
  test14
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
