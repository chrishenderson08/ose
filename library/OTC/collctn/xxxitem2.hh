#ifndef TSTITEM2_HH
#define TSTITEM2_HH

#include <OTC/refcnt/resource.hh>
#include <OTC/debug/tracer.hh>

class TST_Item2 : public OTC_Resource
{
  public:

			TST_Item2(u_int theValue)
			  : value_(theValue) {}

			~TST_Item2();

    int			value() const
				{ return value_; }

  private:

			TST_Item2(TST_Item2 const& theItem);

    TST_Item2&		operator=(TST_Item2 const& theItem);

    int			value_;
};

inline TST_Item2::~TST_Item2()
{
  OTC_Tracer tracer("TST_Item2::~TST_Item2()");
  tracer() << "this->value() = " << value() << endl;
}

#endif /* TSTITEM2_HH */
