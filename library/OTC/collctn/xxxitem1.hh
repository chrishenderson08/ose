#ifndef TSTITEM1_HH
#define TSTITEM1_HH

#include <OTC/OTC.h>

class TST_Item1
{
  public:

			TST_Item1(u_int theValue)
			  : value_(theValue) {}

			TST_Item1(TST_Item1 const& theItem)
			  : value_(theItem.value_) {}

    TST_Item1&		operator=(TST_Item1 const& theItem)
				{ value_ = theItem.value_; return *this; }

    int			value() const
				{ return value_; }

    void		invert()
				{ value_ = -value_; }

  private:

    int			value_;
};

#endif /* TSTITEM1_HH */
