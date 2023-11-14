/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_priqueue.cc
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

#include <OTC/collctn/priqueue.hh>
#include <OTC/collctn/dscqueue.hh>
#include <OTC/collctn/ascqueue.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

extern long tst_random();
extern void tst_srandom(long, long);

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test properties of an empty queue.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_PriorityQueue<int> queue;

  tracer() << "queue.isEmpty() = " << queue.isEmpty() << endl;
}

// = TEST 3
//      Test addition and removal of items from ascending queue.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_PriorityQueue<int> queue(OTCLIB_ASCENDING);

  tst_srandom(1,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    queue.add(value);
  }
  tracer() << "queue items =" << flush;
  while (!queue.isEmpty())
  {
    int head = queue.head();
    int remove = queue.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;
}

// = TEST 4
//      Test addition and removal of items from descending queue.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_PriorityQueue<int> queue(OTCLIB_DESCENDING);

  tst_srandom(1,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    queue.add(value);
  }
  tracer() << "queue items =" << flush;
  while (!queue.isEmpty())
  {
    int head = queue.head();
    int remove = queue.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;
}

// = TEST 5
//     Test of count() and clear().

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_PriorityQueue<int> queue;

  tracer() << "queue.count() = " << queue.count() << endl;

  for (int i=0; i<10; i++)
    queue.add(i);

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.clear()" << endl;
  queue.clear();

  tracer() << "queue.count() = " << queue.count() << endl;
}

// = TEST 6
//     Test of removal of set number of items.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_PriorityQueue<int> queue;

  tracer() << "queue.count() = " << queue.count() << endl;

  for (int i=0; i<10; i++)
    queue.add(i);

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.discard(4)" << endl;
  queue.discard(4);

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.remove() = " << queue.remove() << endl;
  tracer() << "queue.remove() = " << queue.remove() << endl;

  tracer() << "queue.count() = " << queue.count() << endl;

  tracer() << "queue.discard(4)" << endl;
  queue.discard(4);

  tracer() << "queue.count() = " << queue.count() << endl;
}

// = TEST 7
//     Test supplying of rank function.

int test7rank(int const& item1, int const& item2)
{
  if (item1 < item2)
    return 1;
  else if (item1 > item2)
    return -1;
  else
    return 0;
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_PriorityQueue<int> queue(test7rank,OTCLIB_ASCENDING);

  tst_srandom(1,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    queue.add(value);
  }
  tracer() << "queue items =" << flush;
  while (!queue.isEmpty())
  {
    int head = queue.head();
    int remove = queue.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;
}

// = TEST 8
//     Test of copy constructor.

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_PriorityQueue<int> queue1;

  tst_srandom(1,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    queue1.add(value);
  }

  OTC_PriorityQueue<int> queue2 = queue1;

  tracer() << "queue1 items =" << flush;
  while (!queue1.isEmpty())
  {
    int head = queue1.head();
    int remove = queue1.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;

  tracer() << "queue2 items =" << flush;
  while (!queue2.isEmpty())
  {
    int head = queue2.head();
    int remove = queue2.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;
}

// = TEST 9
//     Test of assignment for derived priority queues.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_AscendingQueue<int> queue1;
  OTC_DescendingQueue<int> queue3;

  tst_srandom(1,0);
  for (int i=0; i<500; i++)
  {
    int value = int(tst_random() % 50);
    queue1.add(value);
    queue3.add(value);
  }

  OTC_AscendingQueue<int> queue2;
  queue2 = queue1;

  OTC_DescendingQueue<int> queue4;
  queue4 = queue3;

  tracer() << "queue1 items =" << flush;
  while (!queue1.isEmpty())
  {
    int head = queue1.head();
    int remove = queue1.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;

  tracer() << "queue2 items =" << flush;
  while (!queue2.isEmpty())
  {
    int head = queue2.head();
    int remove = queue2.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;

  tracer() << "queue3 items =" << flush;
  while (!queue3.isEmpty())
  {
    int head = queue3.head();
    int remove = queue3.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
  }
  tracer() << endl;

  tracer() << "queue4 items =" << flush;
  while (!queue4.isEmpty())
  {
    int head = queue4.head();
    int remove = queue4.remove();
    if (head != remove)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << remove << flush;
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
  test8,
  test9
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
