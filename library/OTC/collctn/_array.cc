/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_array.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/array.hh>
#include <OTC/debug/logger.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

// = TEST 2
//     Test of fixed size array

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Array<int> vec(10);

  tracer() << "vec.size() = " << vec.size() << endl;

  int i;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    vec[i] = i*i;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;
}

// = TEST 3
//     Test copy constructor.

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Array<int> vec1(10);

  int i;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    vec1[i] = i*i;

  OTC_Array<int> vec2(vec1);

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;
}

// = TEST 4
//     Test copy constructor of range.

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Array<int> vec1(10);

  int i;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    vec1[i] = i*i;

  OTC_Range rng(1,8);
  OTC_Array<int> vec2(vec1,rng);

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;

  OTC_Array<int> vec3(vec1,rng.lower(),rng.length());

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  tracer() << "vec3 =" << flush;
  for (i=vec3.range().lower(); i<vec3.range().upper(); i++)
    tracer() << " " << vec3[i] << flush;
  tracer() << endl;
}

// = TEST 5
//     Test copying of array.

int test5vec1[] = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Array<int> vec2(test5vec1,10);

  tracer() << "vec1 =" << flush;
  int i;
  for (i=0; i<10; i++)
    tracer() << " " << test5vec1[i] << flush;
  tracer() << endl;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;
}

// = TEST 6
//     Test of resizing.

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Array<int> vec(10);

  tracer() << "vec.size() = " << vec.size() << endl;

  int i;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    vec[i] = i*i;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  tracer() << "vec.resize(5)" << endl;

  vec.resize(5);

  tracer() << "vec.size() = " << vec.size() << endl;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  tracer() << "vec.resize(10)" << endl;

  vec.resize(10);

  for (i=5; i<vec.range().upper(); i++)
    vec[i] = i*i;

  tracer() << "vec.size() = " << vec.size() << endl;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;
}

// = TEST 7
//     Test of lower bound different to 0.

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Array<int> vec1(int(-1),u_int(11));

  tracer() << "vec1.size() = " << vec1.size() << endl;
  tracer() << "vec1.range().lower() = " << vec1.range().lower() << endl;
  tracer() << "vec1.range().upper() = " << vec1.range().upper() << endl;

  int i;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    vec1[i] = i*i;

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  OTC_Array<int> vec2(OTC_Range(-1,11));

  tracer() << "vec2.size() = " << vec2.size() << endl;
  tracer() << "vec2.range().lower() = " << vec2.range().lower() << endl;
  tracer() << "vec2.range().upper() = " << vec2.range().upper() << endl;

  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    vec2[i] = i*i;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;
}

// = TEST 8
//     Test of initialisation while constructing.

void test8()
{
  OTC_Tracer tracer("void test8()");

  int i;

  i = 1;

  OTC_Array<int> vec1(int(-1),u_int(11),i);

  tracer() << "vec1.size() = " << vec1.size() << endl;
  tracer() << "vec1.range().lower() = " << vec1.range().lower() << endl;
  tracer() << "vec1.range().upper() = " << vec1.range().upper() << endl;

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  i = 2;

  OTC_Array<int> vec2(OTC_Range(-1,11),i);

  tracer() << "vec2.size() = " << vec2.size() << endl;
  tracer() << "vec2.range().lower() = " << vec2.range().lower() << endl;
  tracer() << "vec2.range().upper() = " << vec2.range().upper() << endl;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;
}

// = TEST 9
//     Test of replace.

void test9()
{
  OTC_Tracer tracer("void test9()");

  int i;

  i = 1;

  OTC_Array<int> vec(-1,u_int(11),i);

  tracer() << "vec.size() = " << vec.size() << endl;
  tracer() << "vec.range().lower() = " << vec.range().lower() << endl;
  tracer() << "vec.range().upper() = " << vec.range().upper() << endl;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  tracer() << "vec.replaceItem(2,0)" << endl;

  i = 2;

  vec.replaceItem(i,0);

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  tracer() << "vec.replaceRange(0,9,3)" << endl;

  i = 3;

  vec.replaceRange(0,9,i);

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;
}

// = TEST 10
//     Test of worker.

template<class T>
class EXWK_Square : public OTC_Worker<T>
{
  public:

                        EXWK_Square()
                                {
#if defined(__SUNPRO_CC)
                                  bool theDummy;
                                  theDummy = false;
                                  if (theDummy != false)
                                  {
                                    EXWK_Square<T>* theObject1;
                                    T* theObject2;
                                    theObject1->start();
                                    theObject1->finish();
                                    theObject1->action(*theObject2);
                                  }
#endif
                                }

    void                start();

    void                finish();

    OTC_Progress        action(T& theItem);
};

template<class T>
void EXWK_Square<T>::start()
{
  // Nothing to do.
}

template<class T>
void EXWK_Square<T>::finish()
{
  // Nothing to do.
}

template<class T>
OTC_Progress EXWK_Square<T>::action(T& theItem)
{
  theItem = theItem * theItem;
  return OTCLIB_CONTINUE;
}

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_Array<int> vec(-1,u_int(11));

  tracer() << "vec.size() = " << vec.size() << endl;

  int i;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    vec[i] = i;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  EXWK_Square<int> worker;

  tracer() << "vec.apply(worker)" << endl;

  vec.apply(worker);

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;
}

// = TEST 11
//     Test of visitor.

template<class T>
class EXVS_Sum : public OTC_Visitor<T>
{
  public:

                        EXVS_Sum()
			  : sum_(0)
				{
#if defined(__SUNPRO_CC)
                                  bool theDummy;
                                  theDummy = false;
                                  if (theDummy != false)
                                  {
                                    EXVS_Sum<T>* theObject1;
                                    T* theObject2;
                                    theObject1->start();
                                    theObject1->finish();
                                    theObject1->action(*theObject2);
                                  }
#endif
				}

    void                start();

    void                finish();

    OTC_Progress        action(T const& theItem);

    T                   sum() const
                                { return sum_; }

                        operator T() const
                                { return sum_; }

  private:

    T                   sum_;
};

template<class T>
void EXVS_Sum<T>::start()
{
  // Nothing to do.
}

template<class T>
void EXVS_Sum<T>::finish()
{
  // Nothing to do.
}

template<class T>
OTC_Progress EXVS_Sum<T>::action(T const& theItem)
{
  sum_ += theItem;
  return OTCLIB_CONTINUE;
}

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_Array<int> vec(-1,u_int(11));

  tracer() << "vec.size() = " << vec.size() << endl;

  int i;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    vec[i] = i;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  EXVS_Sum<int> visitor;

  tracer() << "vec.apply(visitor)" << endl;

  vec.apply(visitor);

  tracer() << "sum = " << visitor.sum() << endl;
}

// = TEST 12
//     Test of pair worker.

template<class T1, class T2>
class EXPWK_Square : public OTC_PairWorker<T1,T2>
{
  public:

                        EXPWK_Square()
                                {
#if defined(__SUNPRO_CC)
                                  bool theDummy;
                                  theDummy = false;
                                  if (theDummy != false)
                                  {
                                    EXPWK_Square<T1,T2>* theObject1;
                                    T1* theObject2;
                                    T2* theObject3;
                                    theObject1->start();
                                    theObject1->finish();
                                    theObject1->action(*theObject2,*theObject3);
                                  }
#endif
                                }

    void                start();

    void                finish();

    OTC_Progress        action(T1 const& theKey, T2& theItem);
};

template<class T1, class T2>
void EXPWK_Square<T1,T2>::start()
{
  // Nothing to do.
}

template<class T1, class T2>
void EXPWK_Square<T1,T2>::finish()
{
  // Nothing to do.
}

template<class T1, class T2>
OTC_Progress EXPWK_Square<T1,T2>::action(T1 const& theKey, T2& theItem)
{
  theItem = theKey * theKey;
  return OTCLIB_CONTINUE;
}

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_Array<int> vec(-1,u_int(11));

  tracer() << "vec.size() = " << vec.size() << endl;

  int i;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    vec[i] = i;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  EXPWK_Square<int,int> worker;

  tracer() << "vec.apply(worker)" << endl;

  vec.apply(worker);

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;
}

// = TEST 13
//     Test of pair visitor.

template<class T1, class T2>
class EXPVS_Sum : public OTC_PairVisitor<T1,T2>
{
  public:

                        EXPVS_Sum()
			  : sum_(0)
                                {
#if defined(__SUNPRO_CC)
                                  bool theDummy;
                                  theDummy = false;
                                  if (theDummy != false)
                                  {
                                    EXPVS_Sum<T1,T2>* theObject1;
                                    T1* theObject2;
                                    T2* theObject3;
                                    theObject1->start();
                                    theObject1->finish();
                                    theObject1->action(*theObject2,*theObject3);
                                  }
#endif
                                }

    void                start();

    void                finish();

    OTC_Progress        action(T1 const& theKey, T2 const& theItem);

    T2                  sum() const
                                { return sum_; }

                        operator T2() const
                                { return sum_; }

  private:

    T2                  sum_;
};

template<class T1, class T2>
void EXPVS_Sum<T1,T2>::start()
{
  // Nothing to do.
}

template<class T1, class T2>
void EXPVS_Sum<T1,T2>::finish()
{
  // Nothing to do.
}

template<class T1, class T2>
OTC_Progress EXPVS_Sum<T1,T2>::action(T1 const&, T2 const& theItem)
{
  sum_ += theItem;
  return OTCLIB_CONTINUE;
}

void test13()
{
  OTC_Tracer tracer("void test13()");

  OTC_Array<int> vec(-1,u_int(11));

  tracer() << "vec.size() = " << vec.size() << endl;

  int i;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    vec[i] = i;

  tracer() << "vec =" << flush;
  for (i=vec.range().lower(); i<vec.range().upper(); i++)
    tracer() << " " << vec[i] << flush;
  tracer() << endl;

  EXPVS_Sum<int,int> visitor;

  tracer() << "vec.apply(visitor)" << endl;

  vec.apply(visitor);

  tracer() << "sum = " << visitor.sum() << endl;
}

// = TEST 14
//     Test of shallow copy.

void test14()
{
  OTC_Tracer tracer("void test14()");

  OTC_Array<int> vec1(10);

  tracer() << "vec1.size() = " << vec1.size() << endl;

  int i;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    vec1[i] = i*i;

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  OTC_Array<int> vec2(vec1,OTCLIB_SHALLOWCOPY);

  tracer() << "vec1.buffer() == vec2.buffer() = ";
  tracer() << (vec1.buffer() == vec2.buffer()) << endl;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;

  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    vec1[i] = i+i;

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;

  tracer() << "vec2 =" << flush;
  for (i=vec2.range().lower(); i<vec2.range().upper(); i++)
    tracer() << " " << vec2[i] << flush;
  tracer() << endl;
}

// = TEST 15
//     Test of iterators.

void test15()
{
  OTC_Tracer tracer("void test15()");

  OTC_Array<int> vec1(10);

  tracer() << "vec1.size() = " << vec1.size() << endl;

  int i;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    vec1[i] = i*i;

  OTC_Iterator<int> items;

  items = vec1.items();
  tracer() << "items = " << flush;
  while (items.isValid())
  {
    tracer() << " " << items.item() << flush;
    items.next();
  }
  tracer() << endl;

  items = vec1.items(1,8);
  tracer() << "items(1,8) = " << flush;
  while (items.isValid())
  {
    tracer() << " " << items.item() << flush;
    items.next();
  }
  tracer() << endl;

  int v = 1;

  tracer() << "vec1.resize(0,1,v)" << endl;

  vec1.resize(0,1,v);

  items.reset();
  tracer() << "iter = " << flush;
  while (items.isValid())
  {
    tracer() << " " << items.item() << flush;
    items.next();
  }
  tracer() << endl;

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << vec1[i] << flush;
  tracer() << endl;
}

// = TEST 16
//     Test of iterators.

void test16()
{
  OTC_Tracer tracer("void test16()");

  OTC_Array<int> vec1(10);

  tracer() << "vec1.size() = " << vec1.size() << endl;

  int i;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    vec1[i] = i*i;

  OTC_PairIterator<int,int> pairs;

  pairs = vec1.pairs();
  tracer() << "pairs = " << flush;
  while (pairs.isValid())
  {
    tracer() << " " << pairs.key() << "/" << pairs.item() << flush;
    pairs.next();
  }
  tracer() << endl;

  pairs = vec1.pairs(1,8);
  tracer() << "pairs(1,8) = " << flush;
  while (pairs.isValid())
  {
    tracer() << " " << pairs.key() << "/" << pairs.item() << flush;
    pairs.next();
  }
  tracer() << endl;

  int v = 1;

  tracer() << "vec1.resize(0,1,v)" << endl;

  vec1.resize(0,1,v);

  pairs.reset();
  tracer() << "iter = " << flush;
  while (pairs.isValid())
  {
    tracer() << " " << pairs.key() << "/" << pairs.item() << flush;
    pairs.next();
  }
  tracer() << endl;

  tracer() << "vec1 =" << flush;
  for (i=vec1.range().lower(); i<vec1.range().upper(); i++)
    tracer() << " " << i << "/" << vec1[i] << flush;
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
  test15();
  test16();
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
  test9,
  test10,
  test11,
  test12,
  test13,
  test14,
  test15,
  test16
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
