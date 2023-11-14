/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_worker.cc
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
#include <OTC/collctn/deque.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

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

    void		start();

    void		finish();

    OTC_Progress	action(T& theItem);
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

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Deque<int> list;
  for (int i=0; i<100; i++)
    list.addLast(i);

  EXWK_Square<int> square;
  list.items().apply(square);

  OTC_Iterator<int> items = 0;
  items = list.items();
  for (items.reset(); items.isValid(); items.next())
    cout << items.item() << " ";
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
