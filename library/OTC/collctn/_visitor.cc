/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/_visitor.cc
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

    void		start();

    void		finish();

    OTC_Progress	action(T const& theItem);

    T			sum() const
				{ return sum_; }

			operator T() const
				{ return sum_; }

  private:

    T			sum_;
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

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Deque<int> list;
  for (int i=0; i<100; i++)
    list.addLast(i);

  EXVS_Sum<int> sum;
  list.items().apply(sum);

  tracer() << "sum = " << sum << endl;

  OTC_Iterator<int> iter = 0;
  iter = list.items();
  iter.apply(sum);

  tracer() << "sum = " << sum << endl;
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
