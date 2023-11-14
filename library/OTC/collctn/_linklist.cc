/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     collctn/_linklist.cc
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

#include <OTC/collctn/linklist.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class TST_Link : public OTC_Link
{
  public:

			TST_Link(u_int theValue)
			  : value_(theValue) {}

			~TST_Link();

    u_int		value() const
				{ return value_; }

  private:

    u_int		value_;
};

TST_Link::~TST_Link()
{
  // Nothing to do.
}

// = TEST 2
//     An empty list.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_LinkList list;

  tracer() << "list.first() = " << list.first() << endl;
  tracer() << "list.last() = " << list.last() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.first() = " << list.first() << endl;
  tracer() << "list.last() = " << list.last() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;
}

// = TEST 3
//     Iterator over an empty list.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_LinkList list;
  OTC_LinkIterator iter(list.items());

  tracer() << "iter.resetStart()" << endl;
  iter.resetStart();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter = list.items()" << endl;
  iter = list.items();
  tracer() << "iter.resetEnd()" << endl;
  iter.resetEnd();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.resetStart()" << endl;
  iter.resetStart();

  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.resetEnd()" << endl;
  iter.resetEnd();

  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;
}

// = TEST 4
//     List with two items.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list.addFirst(link1)" << endl;
  list.addFirst(link1);
  tracer() << "list.addFirst(link2)" << endl;
  list.addLast(link2);

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.first() = " << list.first() << endl;
  tracer() << "list.last() = " << list.last() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;
}

// = TEST 5
//     Iterator on list with two items.

void test5()
{
  OTC_Tracer tracer("void test4()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list.addFirst(link1)" << endl;
  list.addFirst(link1);
  tracer() << "list.addFirst(link2)" << endl;
  list.addLast(link2);

  OTC_LinkIterator iter(list.items());
  tracer() << "iter.resetStart()" << endl;
  iter.resetStart();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter = list.items()" << endl;
  iter = list.items();
  tracer() << "iter.resetEnd()" << endl;
  iter.resetEnd();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.first() = " << list.first() << endl;
  tracer() << "list.last() = " << list.last() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;
}

// = TEST 6
//     Valid iterator when removeAll() called.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list.addFirst(link1)" << endl;
  list.addFirst(link1);
  tracer() << "list.addFirst(link2)" << endl;
  list.addLast(link2);

  OTC_LinkIterator iter(list.items());
  iter.resetStart();
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "list.removeAll()" << endl;
  list.removeAll();

  tracer() << "list.first() = " << list.first() << endl;
  tracer() << "list.last() = " << list.last() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;
}

// = TEST 7
//     Kill items explicitly.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list.addFirst(link1)" << endl;
  list.addFirst(link1);
  tracer() << "list.addFirst(link2)" << endl;
  list.addLast(link2);

  OTC_LinkIterator iter(list.items());
  iter.resetStart();
  iter.next();

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "iter.link()->kill()" << endl;
  iter.link()->kill();

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  tracer() << "iter.next()" << endl;
  iter.next();
  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;
}

// = TEST 8
//     Add items in middle of list.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list.addFirst(link1)" << endl;
  list.addFirst(link1);
  tracer() << "list.addFirst(link2)" << endl;
  list.addLast(link2);

  OTC_LinkIterator iter(list.items());
  iter.resetStart();
  iter.next();

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link->isDead() = " << iter.link()->isDead() << endl;
  tracer() << "iter.link->value() = ";
  tracer() << ((TST_Link*)iter.link())->value() << endl;

  TST_Link* link3 = new TST_Link(3);

  tracer() << "iter.link()->addAfter(link3)" << endl;
  iter.link()->addAfter(link3);

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  iter.resetStart();
  iter.next();
  tracer() << "list.items() =" << flush;
  while (iter.isLink())
  {
    tracer() << " " << ((TST_Link*)iter.link())->value() << flush;
    iter.next();
  }
  tracer() << endl;
}

// = TEST 9
//     Insertion of links already in a list.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_LinkList list1;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list1.addFirst(link1)" << endl;
  list1.addFirst(link1);
  tracer() << "list1.addFirst(link2)" << endl;
  list1.addLast(link2);

  tracer() << "list1.first()->value() = ";
  tracer() << ((TST_Link*)list1.first())->value() << endl;
  tracer() << "list1.last()->value() = ";
  tracer() << ((TST_Link*)list1.last())->value() << endl;
  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  OTC_LinkList list2;

  tracer() << "list2.addLast(link1)" << endl;
  list2.addLast(link1);

  tracer() << "list1.first()->value() = ";
  tracer() << ((TST_Link*)list1.first())->value() << endl;
  tracer() << "list1.last()->value() = ";
  tracer() << ((TST_Link*)list1.last())->value() << endl;
  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list2.first()->value() = ";
  tracer() << ((TST_Link*)list2.first())->value() << endl;
  tracer() << "list2.last()->value() = ";
  tracer() << ((TST_Link*)list2.last())->value() << endl;
  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;

  tracer() << "list2.addFirst(link2)" << endl;
  list2.addFirst(link2);

  tracer() << "list1.first() = " << list1.first() << endl;
  tracer() << "list1.last() = " << list1.last() << endl;
  tracer() << "list1.population() = " << list1.population() << endl;
  tracer() << "list1.isEmpty() = " << list1.isEmpty() << endl;

  tracer() << "list2.first()->value() = ";
  tracer() << ((TST_Link*)list2.first())->value() << endl;
  tracer() << "list2.last()->value() = ";
  tracer() << ((TST_Link*)list2.last())->value() << endl;
  tracer() << "list2.population() = " << list2.population() << endl;
  tracer() << "list2.isEmpty() = " << list2.isEmpty() << endl;
}

// = TEST 10
//     Iterator with lifetime longer than list.

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_LinkList* list = new OTC_LinkList;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list->addFirst(link1)" << endl;
  list->addFirst(link1);
  tracer() << "list->addFirst(link2)" << endl;
  list->addLast(link2);

  OTC_LinkIterator iter(list->items());
  iter.resetStart();

  tracer() << "delete list" << endl;
  delete list;

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.next()" << endl;
  iter.next();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;

  tracer() << "iter.prev()" << endl;
  iter.prev();

  tracer() << "iter.isLink() = " << iter.isLink() << endl;
  tracer() << "iter.isStart() = " << iter.isStart() << endl;
  tracer() << "iter.isEnd() = " << iter.isEnd() << endl;
  tracer() << "iter.link() = " << iter.link() << endl;
}

// = TEST 11
//     Skipping of dead links or anchor points by iterator.

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);

  tracer() << "list.addFirst(link1)" << endl;
  list.addFirst(link1);
  tracer() << "list.addFirst(link2)" << endl;
  list.addLast(link2);

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "iter1 = list.items()" << endl;
  OTC_LinkIterator iter1(list.items());
  tracer() << "iter1.resetStart()" << endl;
  iter1.resetStart();
  tracer() << "iter1.next()" << endl;
  iter1.next();

  tracer() << "iter1.isLink() = " << iter1.isLink() << endl;
  tracer() << "iter1.isStart() = " << iter1.isStart() << endl;
  tracer() << "iter1.isEnd() = " << iter1.isEnd() << endl;
  tracer() << "iter1.link->isDead() = " << iter1.link()->isDead() << endl;
  tracer() << "iter1.link->value() = ";
  tracer() << ((TST_Link*)iter1.link())->value() << endl;

  tracer() << "iter1.link()->kill()" << endl;
  iter1.link()->kill();

  tracer() << "list.first()->value() = ";
  tracer() << ((TST_Link*)list.first())->value() << endl;
  tracer() << "list.last()->value() = ";
  tracer() << ((TST_Link*)list.last())->value() << endl;
  tracer() << "list.population() = " << list.population() << endl;
  tracer() << "list.isEmpty() = " << list.isEmpty() << endl;

  tracer() << "iter1 = iter2" << endl;
  OTC_LinkIterator iter2(iter1);
  iter2.resetStart();
  iter2.next();
  tracer() << "list.items() =" << flush;
  while (iter2.isLink())
  {
    tracer() << " " << ((TST_Link*)iter2.link())->value() << flush;
    iter2.next();
  }
  tracer() << endl;
  iter2.prev();
  tracer() << "list.items() =" << flush;
  while (iter2.isLink())
  {
    tracer() << " " << ((TST_Link*)iter2.link())->value() << flush;
    iter2.prev();
  }
  tracer() << endl;
}

// = TEST 12
//     Iterate over restricted range of links.

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_LinkList list;

  TST_Link* link1 = new TST_Link(1);
  TST_Link* link2 = new TST_Link(2);
  TST_Link* link3 = new TST_Link(3);
  TST_Link* link4 = new TST_Link(4);
  TST_Link* link5 = new TST_Link(5);

  OTC_Anchor* anchor1 = new OTC_Anchor;
  OTC_Anchor* anchor2 = new OTC_Anchor;

  list.addLast(link1);
  list.addLast(link2);
  list.addLast(link3);
  list.addLast(link4);
  list.addLast(link5);

  link1->addAfter(anchor1);
  link5->addBefore(anchor2);

  OTC_LinkIterator iter1(anchor1,anchor2);

  iter1.resetStart();
  iter1.next();
  tracer() << "iter1.items() =" << flush;
  while (iter1.isLink())
  {
    tracer() << " " << ((TST_Link*)iter1.link())->value() << flush;
    iter1.next();
  }
  tracer() << endl;

  OTC_LinkIterator iter2(list.items());
  iter2.resetStart();
  iter2.next();
  tracer() << "iter2.items() =" << flush;
  while (iter2.isLink())
  {
    tracer() << " " << ((TST_Link*)iter2.link())->value() << flush;
    iter2.next();
  }
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
