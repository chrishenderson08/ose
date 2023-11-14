/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_bndqueue.cc
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

#include <OTC/collctn/bndqueue.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test of empty queue.

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_BoundedQueue<int> queue1(0);
  tracer() << "queue1.isEmpty() = " << queue1.isEmpty() << endl;
  tracer() << "queue1.isFull() = " << queue1.isFull() << endl;

  OTC_BoundedQueue<int> queue2(1);
  tracer() << "queue2.isEmpty() = " << queue2.isEmpty() << endl;
  tracer() << "queue2.isFull() = " << queue2.isFull() << endl;

  OTC_BoundedQueue<int> queue3(2);
  tracer() << "queue3.isEmpty() = " << queue3.isEmpty() << endl;
  tracer() << "queue3.isFull() = " << queue3.isFull() << endl;
}

// = TEST 3
//     Test of filling queue and then emptying it.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_BoundedQueue<int> queue1(10);
  int i=0;
  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    if (queue1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  tracer() << endl;
  tracer() << "queue1.remove() =" << flush;
  while (!queue1.isEmpty())
  {
    int head = queue1.head();
    i = queue1.remove();
    if (head != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (queue1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 4
//     Test of cycling over length of buffer.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_BoundedQueue<int> queue1(5);
  int i=0;
  tracer() << "dump =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " -/" << i << flush;
    queue1.add(i);
    if (queue1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  while (i != 20)
  {
    int head = queue1.remove();
    tracer() << " " << head << "/" << i << flush;
    if (queue1.isFull())
      tracer() << "XXX - this should never happen" << endl;
    queue1.add(i);
    i++;
  }
  while (!queue1.isEmpty())
  {
    int head = queue1.remove();
    tracer() << " " << head << "/-" << flush;
    if (queue1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 5
//     Test of count() and clear().

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_BoundedQueue<int> queue1(10);

  tracer() << "queue1.capacity() = " << queue1.capacity() << endl;

  int i=0;
  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    i++;
  }
  tracer() << endl;

  tracer() << "queue1.count() = " << queue1.count() << endl;

  tracer() << "queue1.clear()" << endl;
  queue1.clear();

  tracer() << "queue1.count() = " << queue1.count() << endl;
}

// = TEST 6
//     Test of removing set number of items from queue.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_BoundedQueue<int> queue1(10);

  tracer() << "queue1.capacity() = " << queue1.capacity() << endl;

  int i=0;
  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    i++;
  }
  tracer() << endl;

  tracer() << "queue1.count() = " << queue1.count() << endl;

  tracer() << "queue1.discard(4)" << endl;
  queue1.discard(4);

  tracer() << "queue1.count() = " << queue1.count() << endl;

  tracer() << "queue1.remove() = " << queue1.remove() << endl;
  tracer() << "queue1.remove() = " << queue1.remove() << endl;

  tracer() << "queue1.discard(4)" << endl;
  queue1.discard(4);

  tracer() << "queue1.count() = " << queue1.count() << endl;
}

// = TEST 7
//     Test of array on stack.

void test7()
{
  OTC_Tracer tracer("void test7()");

  int array[10];
  OTC_BoundedQueue<int> queue1(10,array);
  int i=0;
  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    i++;
  }
  tracer() << endl;

  tracer() << "queue1.remove() =" << flush;
  while (!queue1.isEmpty())
  {
    i = queue1.remove();
    tracer() << " " << i << flush;
  }
  tracer() << endl;
}

// = TEST 8
//     Test of peek().

void test8()
{
  OTC_Tracer tracer("void test8()");

  int array[10];
  OTC_BoundedQueue<int> queue1(10,array);
  u_int i=0;

  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    i++;
  }
  tracer() << endl;

  tracer() << "queue1.remove() = ";
  tracer() << " " << queue1.remove();
  tracer() << " " << queue1.remove();
  tracer() << " " << queue1.remove();
  tracer() << " " << queue1.remove();
  tracer() << " " << queue1.remove();
  tracer() << endl;

  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    i++;
  }
  tracer() << endl;

  for (i=0; i<queue1.capacity(); i++)
    tracer() << "queue1.peek(" << i << ") = " << queue1.peek(i) << endl;
}

// = TEST 9
//     Test of copy constructor.

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_BoundedQueue<int> queue1(10);
  int i=0;
  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    if (queue1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  tracer() << endl;

  OTC_BoundedQueue<int> queue2 = queue1;

  tracer() << "queue1.remove() =" << flush;
  while (!queue1.isEmpty())
  {
    int head = queue1.head();
    i = queue1.remove();
    if (head != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (queue1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;

  tracer() << "queue2.remove() =" << flush;
  while (!queue2.isEmpty())
  {
    int head = queue2.head();
    i = queue2.remove();
    if (head != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (queue2.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;
}

// = TEST 10
//     Test of assignment operator.

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_BoundedQueue<int> queue1(10);
  int i=0;
  tracer() << "queue1.add() =" << flush;
  while (!queue1.isFull())
  {
    tracer() << " " << i << flush;
    queue1.add(i);
    if (queue1.isEmpty())
      tracer() << "XXX - this should never happen" << endl;
    i++;
  }
  tracer() << endl;

  OTC_BoundedQueue<int> queue2(10);
  queue2 = queue1;

  tracer() << "queue1.remove() =" << flush;
  while (!queue1.isEmpty())
  {
    int head = queue1.head();
    i = queue1.remove();
    if (head != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (queue1.isFull())
      tracer() << "XXX - this should never happen" << endl;
  }
  tracer() << endl;

  tracer() << "queue2.remove() =" << flush;
  while (!queue2.isEmpty())
  {
    int head = queue2.head();
    i = queue2.remove();
    if (head != i)
      tracer() << "XXX - this should never happen" << endl;
    tracer() << " " << i << flush;
    if (queue2.isFull())
      tracer() << "XXX - this should never happen" << endl;
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
  test10
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
