#ifndef TSTITEM3_HH
#define TSTITEM3_HH

#include <OTC/debug/tracer.hh>

class TST_Item1
{
  public:

                        TST_Item1();

                        ~TST_Item1();

    int                 value() const
                                { return value_; }

  private:

                        TST_Item1(TST_Item1 const& theItem);

    TST_Item1&          operator=(TST_Item1 const& theItem);

    int                 value_;

    static int          count_;
};

inline TST_Item1::TST_Item1()
  : value_(count_++)
{
  OTC_Tracer tracer("TST_Item1::TST_Item1()");
  tracer() << "this->value() = " << value() << endl;
}

inline TST_Item1::~TST_Item1()
{
  OTC_Tracer tracer("TST_Item1::~TST_Item1()");
  tracer() << "this->value() = " << value() << endl;
}

#endif /* TSTITEM3_HH */
