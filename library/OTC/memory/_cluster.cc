/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/_cluster.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1999-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/cluster.hh>
#include <OTC/memory/mcobject.hh>

#include <OTC/debug/tracer.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class ONE : public OTC_MCObject
{
  public:

			ONE(int i);

  protected:

			~ONE();

  private:

    int			_i;
};

ONE::ONE(int i)
  : _i(i)
{
  OTC_Tracer tracer("ONE()");
  tracer() << "i = " << _i << endl;
}

ONE::~ONE()
{
  OTC_Tracer tracer("~ONE()");
  tracer() << "i = " << _i << endl;
}

class TWO
{
  public:

			TWO();

			~TWO();
};

TWO::TWO()
{
  OTC_Tracer tracer("TWO()");

  OTC_Cluster cluster;
  ONE* obj1;

  obj1 = new (cluster) ONE(1);
}

TWO::~TWO()
{
  OTC_Tracer tracer("~TWO()");
}

class THREE : public TWO, public OTC_MCObject
{
  public:

			THREE();

			~THREE();
};

THREE::THREE()
{
  OTC_Tracer tracer("THREE()");
}

THREE::~THREE()
{
  OTC_Tracer tracer("~THREE()");
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Cluster cluster;

  THREE* obj1 = new (cluster) THREE;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Cluster cluster;

  THREE* obj1 = new (cluster) THREE;
  THREE* obj2 = new (*OTC_Cluster::of(obj1)) THREE;

  char* str1 = OTC_Cluster::of(obj2)->allocate(256);
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  THREE obj1;

  tracer() << (size_t)OTC_Cluster::of(&obj1) << endl;
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3
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
