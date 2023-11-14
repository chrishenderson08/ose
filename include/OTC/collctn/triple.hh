#ifndef OTC_COLLCTN_TRIPLE_HH
#define OTC_COLLCTN_TRIPLE_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/triple.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/memory/mpobject.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#if !defined(OSE_DISABLE_TEMPLATE_INCLUSION)
#pragma implementation "OTC/collctn/triple.hh"
#pragma interface "OTC/collctn/triple.hh"
#endif
#endif
#endif

/* ------------------------------------------------------------------------- */

template<class T1, class T2, class T3>
class OTC_Triple : public OTC_MPObject
    // = TITLE
    //     Template class for encapsulating three values.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_Triple> class can be used to encapsulate three values.
    //     This facilitates returning of three values from a function.
    //     The class also allows the allocation of triples in the common
    //     memory pool.
    //
    // = SEE ALSO
    //     <OTC_Single>, <OTC_Pair>
{
  public:

    // = INITIALISATION

			OTC_Triple(
			 T1 const& theFirst,
			 T2 const& theSecond,
			 T3 const& theThird
			)
			  : first_(theFirst),
			    second_(theSecond),
			    third_(theThird)
				{}
				// Sets the value of this triple to
				// <theFirst>, <theSecond> and <theThird>.

			OTC_Triple(OTC_Triple<T1,T2,T3> const& theTriple)
			  : first_(theTriple.first_),
			    second_(theTriple.second_),
			    third_(theTriple.third_)
				{}
				// Sets the value of this triple to that
				// of <theTriple>.

    // = ASSIGNMENT

    OTC_Triple<T1,T2,T3>&	operator=(
				 OTC_Triple<T1,T2,T3> const& theTriple
				)
				{
				  first_ = theTriple.first_;
				  second_ = theTriple.second_;
				  third_ = theTriple.third_;
				  return *this;
				}
				// Sets the value of this triple to that
				// of <theTriple>.

    // = ACCESS

    T1 const&		first() const
				{ return first_; }
				// Returns the first value in this triple.

    T1&			first()
				{ return first_; }
				// Returns the first value in this triple.

    T2 const&		second() const
				{ return second_; }
				// Returns the second value in this triple.

    T2&			second()
				{ return second_; }
				// Returns the second value in this triple.

    T3 const&		third() const
				{ return third_; }
				// Returns the third value in this triple.

    T3&			third()
				{ return third_; }
				// Returns the third value in this triple.

  private:

    T1			first_;
				// The first value in the triple.

    T2			second_;
				// The second value in the triple.

    T3			third_;
				// The third value in the triple.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_TRIPLE_HH */
