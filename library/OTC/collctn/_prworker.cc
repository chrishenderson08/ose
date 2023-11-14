/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_prworker.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/debug/tracer.hh>
#include <OTC/collctn/hmap.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

template<class T1, class T2>
class EXWK_Square : public OTC_PairWorker<T1,T2>
{
  public:

			EXWK_Square()
                                {
#if defined(__SUNPRO_CC)
                                  bool theDummy;
                                  theDummy = false;
                                  if (theDummy != false)
                                  {
                                    EXWK_Square<T1,T2>* theObject1;
                                    T1* theObject2;
                                    T2* theObject3;
                                    theObject1->start();
                                    theObject1->finish();
                                    theObject1->action(*theObject2,*theObject3);
                                  }
#endif
                                }

    void		start();

    void		finish();

    OTC_Progress	action(T1 const& theKey, T2& theItem);
};

template<class T1, class T2>
void EXWK_Square<T1,T2>::start()
{
  // Nothing to do.
}

template<class T1, class T2>
void EXWK_Square<T1,T2>::finish()
{
  // Nothing to do.
}

template<class T1, class T2>
OTC_Progress EXWK_Square<T1,T2>::action(T1 const& theKey, T2& theItem)
{
  theItem = theKey * theKey;
  return OTCLIB_CONTINUE;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_HMap<int,int> map;
  for (int i=0; i<100; i++)
    map.add(i,i);

  EXWK_Square<int,int> square;
  map.pairs().apply(square);

  OTC_PairIterator<int,int> pairs = 0;
  pairs = map.pairs();
  for (pairs.reset(); pairs.isValid(); pairs.next())
    cout << "(" << pairs.key() << "," << pairs.item() << ") ";
  cout << endl;
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  test2();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
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
