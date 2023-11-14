#ifndef OTC_MEMORY_TSTITEM1_HH
#define OTC_MEMORY_TSTITEM1_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     memory/tstitem1.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

class TST_Item1
{
  public:

			TST_Item1() : value_(0) {}

			operator int() const
				{ return value_; }

    TST_Item1&		operator=(int theValue)
				{ value_ = theValue; return *this; }

  private:

    int			value_;
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_MEMORY_TSTITEM1_HH */
