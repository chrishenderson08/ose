#ifndef OTC_COLLCTN_BITSET_HH
#define OTC_COLLCTN_BITSET_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/bitset.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 OTC LIMITED
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/range.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/baseactn.hh>
#include <OTC/collctn/shlwcopy.hh>

#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/bitset.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_BitSet
    // = TITLE
    //     A fixed size bit set.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_BitSet> class is a fixed size bit set. What the range
    //     of values the set can hold however, can be defined at the time of
    //     construction.
    //
    // = SEE ALSO
    //     <OTC_BitSet>
{
  private:

    static u_int	bytes(u_int theLength)
				{ return (theLength + 8) >> 3; }
				// Returns number of bytes necessary to
				// hold a bit set containing <theLength>
				// bits.

    u_int		offset(int theBit) const
				{ return theBit - range_.lower(); }
				// Translates the bit address so that the
				// smallest bit address is at <0>.

    u_int		byte(int theBit) const
				{ return u_int(offset(theBit) >> 3); }
				// Returns the index of the byte holding the
				// bit for <theBit>.

    u_char		mask(int theBit) const
				{ return u_char(1 << (offset(theBit) & 0x07)); }
				// Returns a bit mask locating the bit within
				// its byte, for <theBit>.

  public:

			~OTC_BitSet();

    int                 rank(OTC_BitSet const& theSet) const;
                                // Ranks this set and <theSet>.

    int                 hash() const;
                                // Returns a hash value for this set.

    // = INITIALISATION

    			OTC_BitSet(u_int theLength);
				// Creates a bit set which can hold values
				// in the range <0> to <theLength-1>.

    			OTC_BitSet(int theLower, u_int theLength);
				// Creates a bit set which can hold values
				// in the range <theLower> to
				// <theLower+theLength-1>.

    			OTC_BitSet(OTC_Range const& theRange);
				// Creates a bit set which can hold values
				// in the range given by <theRange>.

			OTC_BitSet(OTC_BitSet const& theSet);
				// Creates a copy of <theSet>. If the size
				// of the bit set is large, this could be
				// an expensive operation.

			OTC_BitSet(OTC_BitSet const& theSet, OTC_ShallowCopy);
				// Creates a shallow copy of <theSet>. That
				// is, this set will reference the same
				// underlying data as <theSet>. Changes
				// made in one will be seen in the other.
				// This is provided as an optimisation for
				// cases where a bit set is being returned
				// as an object from a function, which for
				// large bit sets could be expensive.

    OTC_BitSet&		operator=(OTC_BitSet const& theSet);
				// Replaces this bit set with the contents
				// of <theSet>. This may result in a change
				// in the size of the bit set.

    // = QUERY

    u_int		count() const;
				// Returns the number of bits set.

    OTC_Range const&	range() const
				{ return range_; }
				// Returns range of values that can be
				// held in the bit set.

    int			test(int theBit) const
				{
				  OTCLIB_ENSURE_FN((range_.contains(theBit)),
				   "OTC_BitSet::test(int)","bit not in set");
				  return set_[byte(theBit)] & mask(theBit);
				}
				// Returns a non zero value if <theBit> is set.

    int			operator[](int theBit) const
				{ return test(theBit); }
				// Returns a non zero value if <theBit> is set.

    bool         operator==(OTC_BitSet const& theSet) const;
				// Returns <true> if this bit set is
				// equivalent to <theSet>. Can be used on bit
				// sets of different sizes. Equivalence is
				// where all the same bits are set.

    bool         operator!=(OTC_BitSet const& theSet) const
				{ return !operator==(theSet); }
				// Returns <true> if this bit set is not
				// equivalent to <theSet>. Can be used on
				// bit sets of different sizes. Equivalence
				// is where all the same bits are set.

    // = MANIPULATORS

    void		reset()
				{ memset(set_,0,size_); }
				// Resets all bits to <0>.

    void		reset(int theBit)
				{
				  OTCLIB_ENSURE_FN((range_.contains(theBit)),
				   "OTC_BitSet::reset(int)","bit not in set");
				  set_[byte(theBit)] &= u_char(~mask(theBit));
				}
				// Resets <theBit> to <0>.

    void		reset(OTC_Range const& theRange);
				// Resets bits in <theRange> to <0>.
				// If the range is not a subset of the
				// range of the bit set, an exception
				// is raised.

    void		reset(int theLower, u_int theLength)
				{ reset(OTC_Range(theLower,theLength)); }
				// Resets bits in the range starting at
				// <theLower> with <theLength> to <0>.
				// If the range is not a subset of the
				// range of the bit set, an exception
				// is raised.

    void		set()
				{ memset(set_,~0,size_); }
				// Sets all bits to <1>.

    void		set(int theBit)
				{
				  OTCLIB_ENSURE_FN((range_.contains(theBit)),
				   "OTC_BitSet::set(int)","bit not in set");
				  set_[byte(theBit)] |= mask(theBit);
				}
				// Sets <theBit> to <1>.

    void		set(OTC_Range const& theRange);
				// Sets bits in <theRange> to <1>.
				// If the range is not a subset of the
				// range of the bit set, an exception
				// is raised.

    void		set(int theLower, u_int theLength)
				{ set(OTC_Range(theLower,theLength)); }
				// Sets bits in the range starting at
				// <theLower> with <theLength> to <1>.
				// If the range is not a subset of the
				// range of the bit set, an exception
				// is raised.

    void		complement();
				// Complements, or inverts all bits.

    void		complement(int theBit)
				{
				  OTCLIB_ENSURE_FN((range_.contains(theBit)),
				   "OTC_BitSet::complement(int)",
				   "bit not in set");
				  set_[byte(theBit)] ^= mask(theBit);
				}
				// Complements, or inverts <theBit>.

    void		complement(OTC_Range const& theRange);
				// Complements or inverst all bits in
				// <theRange>. If the range is not a subset
				// of the range of the bit set, an exception
				// is raised.

    void		complement(int theLower, u_int theLength)
				{ complement(OTC_Range(theLower,theLength)); }
				// Complements or inverts all bits in
				// the range starting at <theLower> with
				// <theLength>. If the range is not a subset
				// of the range of the bit set, an exception
				// is raised.

    // = OPERATORS

    OTC_BitSet		operator~() const;
                                // Returns a set which is the complement of
                                // this set, ie. with all bits inverted.

    OTC_BitSet		operator&(OTC_BitSet const& theSet);
                                // Returns the result of performing an
                                // intersection between <theSet> and this
                                // set. If the sets didn't cover the same
				// range, the result will be made large
				// enough to cover both.

    OTC_BitSet&		operator&=(OTC_BitSet const& theSet);
                                // Performs an intersection of <theSet> and
                                // this set with the result being left in
				// this set. If the sets didn't cover the
				// same range, this set will be made large
				// enough to cover both.

    OTC_BitSet		operator|(OTC_BitSet const& theSet);
                                // Returns the result of performing a union
				// of <theSet> and this set. If the sets
				// didn't cover the same range, the result
				// will be large enough to cover both.

    OTC_BitSet&		operator|=(OTC_BitSet const& theSet);
                                // Performs a union of <theSet> and this set
                                // with the result being left in this set.
				// If the sets didn't cover the same range,
				// this set will be made large enough to
				// cover both.

    OTC_BitSet		operator^(OTC_BitSet const& theSet);
                                // Returns the result of performing an
                                // exclusive or of <theSet> and this set.
				// If the sets didn't cover the same range,
				// the result will be large enough to cover
				// both.

    OTC_BitSet&		operator^=(OTC_BitSet const& theSet);
                                // Performs an exclusive or between <theSet>
                                // and this set with the result being left in
				// this set. If the sets didn't cover the
				// same range, this set will be made large
				// enough to cover both.

  private:

    static u_char       globCounts_[256];
                                // Look up table for quickly determining
                                // the number of bits set.

    OTC_Range		range_;
				// The range of integers that the bit set
				// can hold.

    int			size_;
				// The number of bytes used to hold the
				// bit set.

    u_char*		set_;
				// The actual memory holding the bit set.
};

OSE_TEMPLATE_SPECIALISATION
class OTC_RankActions<OTC_BitSet>
{
  public:
    static int          rank(OTC_BitSet const& s1, OTC_BitSet const& s2)
                                { return s1.rank(s2); }
};

OSE_TEMPLATE_SPECIALISATION
class OTC_HashActions<OTC_BitSet>
{
  public:
    static int          hash(OTC_BitSet const& s1)
                                { return s1.hash(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BITSET_HH */
