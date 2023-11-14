/*
// ============================================================================
//
// = LIBRARY
//     ONS
//
// = FILENAME
//     agent/_callback.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/cbobject.hh>
#include <ONS/agent/callback.hh>
#include <OTC/types/time.hh>
#include <OTC/text/string.hh>
#include <OTC/text/symbol.hh>

#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class Object : public ONS_CBObject
{
  public:

			~Object();

			Object(int i);

    void		output_0a_c() const;

    void		output_0a();

    void		output_0b_c() const;

    void		output_0b();

    void		output_1_c(int) const;

    void		output_1(int);

    void		output_2_c(int, char const*) const;

    void		output_2(int, char const*);

    void		output_3_c(int, char const*,
    			 OTC_String const&) const;

    void		output_3(int, char const*,
    			 OTC_String const&);

    void		output_4_c(int, char const*,
			 OTC_String const&, bool) const;

    void		output_4(int, char const*,
			 OTC_String const&, bool);

    void		output_5_c(int, char const*,
			 OTC_String const&, bool, OTC_String) const;

    void		output_5(int, char const*,
			 OTC_String const&, bool, OTC_String);

    void		output_6_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&) const;

    void		output_6(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&);

    void		output_7_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&) const;

    void		output_7(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&);

    void		output_8_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&) const;

    void		output_8(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&);

    void		output_9_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&, OTC_Symbol const&) const;

    void		output_9(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&, OTC_Symbol const&);

  private:

    int			i_;
};

Object::~Object()
{
  OTC_Tracer tracer("~Object()");

  tracer() << "i = " << i_ << endl;
}

Object::Object(int i)
  : i_(i)
{
  OTC_Tracer tracer("Object()");

  tracer() << "i = " << i_ << endl;
}

void Object::output_0a_c() const
{
  OTC_Tracer tracer("output_0a_c()");

  tracer() << "i = " << i_ << endl;
}

void Object::output_0a()
{
  OTC_Tracer tracer("output_0a()");

  tracer() << "i = " << i_ << endl;
}

void Object::output_0b()
{
  OTC_Tracer tracer("output_0b()");

  tracer() << "i = " << i_ << endl;
}

void Object::output_0b_c() const
{
  OTC_Tracer tracer("output_0b_c()");

  tracer() << "i = " << i_ << endl;
}

void Object::output_1_c(int theArg1) const
{
  OTC_Tracer tracer("output_1_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
}

void Object::output_1(int theArg1)
{
  OTC_Tracer tracer("output_1()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
}

void Object::output_2_c(int theArg1, char const* theArg2) const
{
  OTC_Tracer tracer("output_2_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
}

void Object::output_2(int theArg1, char const* theArg2)
{
  OTC_Tracer tracer("output_2()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
}

void Object::output_3_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3) const
{
  OTC_Tracer tracer("output_3_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
}

void Object::output_3(int theArg1, char const* theArg2,
 OTC_String const& theArg3)
{
  OTC_Tracer tracer("output_3()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
}

void Object::output_4_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4) const
{
  OTC_Tracer tracer("output_4_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
}

void Object::output_4(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4)
{
  OTC_Tracer tracer("output_4()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
}

void Object::output_5_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5) const
{
  OTC_Tracer tracer("output_5_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
}

void Object::output_5(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5)
{
  OTC_Tracer tracer("output_5()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
}

void Object::output_6_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6) const
{
  OTC_Tracer tracer("output_6_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
}

void Object::output_6(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6)
{
  OTC_Tracer tracer("output_6()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
}

void Object::output_7_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6, OTC_Time const& theArg7) const
{
  OTC_Tracer tracer("output_7_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
  tracer() << "arg-7 = " << theArg7 << endl;
}

void Object::output_7(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6, OTC_Time const& theArg7)
{
  OTC_Tracer tracer("output_7()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
  tracer() << "arg-7 = " << theArg7 << endl;
}

void Object::output_8_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6, OTC_Time const& theArg7,
 OTC_Date const& theArg8) const
{
  OTC_Tracer tracer("output_8_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
  tracer() << "arg-7 = " << theArg7 << endl;
  tracer() << "arg-8 = " << theArg8 << endl;
}

void Object::output_8(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6, OTC_Time const& theArg7,
 OTC_Date const& theArg8)
{
  OTC_Tracer tracer("output_8()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
  tracer() << "arg-7 = " << theArg7 << endl;
  tracer() << "arg-8 = " << theArg8 << endl;
}

void Object::output_9_c(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6, OTC_Time const& theArg7,
 OTC_Date const& theArg8, OTC_Symbol const& theArg9) const
{
  OTC_Tracer tracer("output_9_c()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
  tracer() << "arg-7 = " << theArg7 << endl;
  tracer() << "arg-8 = " << theArg8 << endl;
  tracer() << "arg-9 = " << theArg9 << endl;
}

void Object::output_9(int theArg1, char const* theArg2,
 OTC_String const& theArg3, bool theArg4, OTC_String theArg5,
 short const& theArg6, OTC_Time const& theArg7,
 OTC_Date const& theArg8, OTC_Symbol const& theArg9)
{
  OTC_Tracer tracer("output_9()");

  tracer() << "i = " << i_ << endl;

  tracer() << "arg-1 = " << theArg1 << endl;
  tracer() << "arg-2 = " << theArg2 << endl;
  tracer() << "arg-3 = " << theArg3 << endl;
  tracer() << "arg-4 = " << theArg4 << endl;
  tracer() << "arg-5 = " << theArg5 << endl;
  tracer() << "arg-6 = " << theArg6 << endl;
  tracer() << "arg-7 = " << theArg7 << endl;
  tracer() << "arg-8 = " << theArg8 << endl;
  tracer() << "arg-9 = " << theArg9 << endl;
}

void test2()
{
  OTC_Tracer tracer("test2()");

  ONS_Callback0 SIGNAL;

  tracer() << "create" << endl;

  Object* object1 = new Object(1);
  Object* object2 = new Object(2);

  tracer() << endl;
  tracer() << "connect" << endl;

  SIGNAL.connect(object1,&Object::output_0a);
  SIGNAL.connect(object1,&Object::output_0a);
  SIGNAL.connect(object1,&Object::output_0b);

  tracer() << endl;
  tracer() << "signal" << endl;

  SIGNAL();

  tracer() << endl;
  tracer() << "disconnect" << endl;

  SIGNAL.disconnect();

  tracer() << endl;
  tracer() << "connect" << endl;

  SIGNAL.connect(object1,&Object::output_0a);
  SIGNAL.connect(object1,&Object::output_0b);
  SIGNAL.connect(object1,&Object::output_0b);

  SIGNAL.connect(object2,&Object::output_0a_c);
  SIGNAL.connect(object2,&Object::output_0a);
  SIGNAL.connect(object2,&Object::output_0b);

  tracer() << endl;
  tracer() << "signal" << endl;

  SIGNAL.execute();

  tracer() << endl;
  tracer() << "delete" << endl;

  delete object2;

  tracer() << endl;
  tracer() << "signal" << endl;

  SIGNAL();

  tracer() << endl;
  tracer() << "delete" << endl;

  delete object1;
}

void test3()
{
  OTC_Tracer tracer("test3()");

  ONS_Callback1<int> SIGNAL1;
  ONS_Callback2<int,char const*> SIGNAL2;
  ONS_Callback3<int,char const*,OTC_String const&> SIGNAL3;
  ONS_Callback4<int,char const*,OTC_String const&,bool> SIGNAL4;
  ONS_Callback5<int,char const*,OTC_String const&,bool,OTC_String> SIGNAL5;
  ONS_Callback6<int,char const*,OTC_String const&,bool,OTC_String,
   short const&> SIGNAL6;
  ONS_Callback7<int,char const*,OTC_String const&,bool,OTC_String,
   short const&,OTC_Time const&> SIGNAL7;
  ONS_Callback8<int,char const*,OTC_String const&,bool,OTC_String,
   short const&,OTC_Time const&,OTC_Date const&> SIGNAL8;
  ONS_Callback9<int,char const*,OTC_String const&,bool,OTC_String,
   short const&,OTC_Time const&,OTC_Date const&,OTC_Symbol const&> SIGNAL9;

  Object* object1 = new Object(1);

  SIGNAL1.connect(object1,&Object::output_1_c);
  SIGNAL1.connect(object1,&Object::output_1);

  SIGNAL2.connect(object1,&Object::output_2_c);
  SIGNAL2.connect(object1,&Object::output_2);

  SIGNAL3.connect(object1,&Object::output_3_c);
  SIGNAL3.connect(object1,&Object::output_3);

  SIGNAL4.connect(object1,&Object::output_4_c);
  SIGNAL4.connect(object1,&Object::output_4);

  SIGNAL5.connect(object1,&Object::output_5_c);
  SIGNAL5.connect(object1,&Object::output_5);

  SIGNAL6.connect(object1,&Object::output_6_c);
  SIGNAL6.connect(object1,&Object::output_6);

  SIGNAL7.connect(object1,&Object::output_7_c);
  SIGNAL7.connect(object1,&Object::output_7);

  SIGNAL8.connect(object1,&Object::output_8_c);
  SIGNAL8.connect(object1,&Object::output_8);

  SIGNAL9.connect(object1,&Object::output_9_c);
  SIGNAL9.connect(object1,&Object::output_9);

  OTC_Date theDate(7,7,7);
  OTC_Time theTime(theDate,8,8,8);
  OTC_Symbol theSymbol("nine");

  SIGNAL1(1);
  SIGNAL2(1,"2");
  SIGNAL3(1,"2","3");
  SIGNAL4(1,"2","3",true);
  SIGNAL5(1,"2","3",true,"5");
  SIGNAL6(1,"2","3",true,"5",6);
  SIGNAL7(1,"2","3",true,"5",6,theTime);
  SIGNAL8(1,"2","3",true,"5",6,theTime,theDate);
  SIGNAL9(1,"2","3",true,"5",6,theTime,theDate,theSymbol);

  SIGNAL1.execute(1);
  SIGNAL2.execute(1,"2");
  SIGNAL3.execute(1,"2","3");
  SIGNAL4.execute(1,"2","3",true);
  SIGNAL5.execute(1,"2","3",true,"5");
  SIGNAL6.execute(1,"2","3",true,"5",6);
  SIGNAL7.execute(1,"2","3",true,"5",6,theTime);
  SIGNAL8.execute(1,"2","3",true,"5",6,theTime,theDate);
  SIGNAL9.execute(1,"2","3",true,"5",6,theTime,theDate,theSymbol);

  delete object1;
}

void test1()
{
  OTC_Tracer tracer("test1()");

  test2();
  test3();
}

/* ------------------------------------------------------------------------- */

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
