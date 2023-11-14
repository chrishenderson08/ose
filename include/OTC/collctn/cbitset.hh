#ifndef OTC_COLLCTN_CBITSET_HH
#define OTC_COLLCTN_CBITSET_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/cbitset.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1994 OTC LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/hashactn.hh>
#include <OTC/collctn/rankactn.hh>
#include <OTC/collctn/range.hh>

#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/collctn/cbitset.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OSE_EXPORT OTC_CBitSet
    // = TITLE
    //     A bit set of size 256.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     <OTC_CBitSet> is a bit set for the range of values <0> to <255>.
    //     The class is designed for tracking character classes in parsing
    //     type applications.
{
  private:

    u_char		byte(u_char theBit) const
				{ return u_char(theBit >> 3); }
				// Returns the index of the byte holding the
				// bit for <theBit>.

    u_char		mask(u_char theBit) const
				{ return u_char(1 << (theBit & 0x07)); }
				// Returns the mask for <theBit> within its
				// byte.

  public:

    int			rank(OTC_CBitSet const& theSet) const
				{ return memcmp(set_,theSet.set_,32); }
				// Ranks this set and <theSet>.

    int			hash() const
				{ return OTC_Hash::hash((char const*)set_,32); }
				// Returns a hash value for this set.

    // = INITIALISATION

    			OTC_CBitSet()
				{ memset(set_,0,32); }
				// Creates a bit set with all bits set to <0>.

			OTC_CBitSet(OTC_CBitSet const& theSet)
				{ memcpy(set_,theSet.set_,32); }
				// Creates a bit set which is a copy of
				// <theSet>.

			OTC_CBitSet(char const* theString);
				// Creates a bit set which has bits set
				// corresponding to the characters listed
				// in <theString>. <theString> should be
				// null terminated. The null terminator
				// does not result in bit <0> being set.

			OTC_CBitSet(char const* theString, u_int theNum);
				// Creates a bit set which has bits set
				// corresponding to first <theNum> characters
				// listed in <theString>.

    OTC_CBitSet&	operator=(OTC_CBitSet const& theSet)
				{
				  memcpy(set_,theSet.set_,32);
				  return *this;
				}
				// Replaces this set with <theSet>.

    // = QUERY

    u_int		count() const;
				// Returns the number of bits set.

    OTC_Range		range() const
				{ return OTC_Range(0,256); }
				// Returns range of bit set, ie., starts
				// at <0> and has length <256>.

    int			test(char theBit) const
				{ return set_[byte(theBit)] & mask(theBit); }
				// Returns a non zero value if <theBit> is set.

    int			operator[](char theBit) const
				{ return set_[byte(theBit)] & mask(theBit); }
				// Returns a non zero value if <theBit> is set.

    bool		operator==(OTC_CBitSet const& theSet) const
				{ return memcmp(set_,theSet.set_,32) == 0; }
				// Returns <true> if this bit set is
				// equivalent to <theSet>.

    bool		operator!=(OTC_CBitSet const& theSet) const
				{ return memcmp(set_,theSet.set_,32) != 0; }
				// Returns <true> if this bit set is not
				// equivalent to <theSet>.

    // = MANIPULATORS

    void		reset()
				{ memset(set_,0,32); }
				// Resets all bits to <0>.

    void		reset(u_char theBit)
				{ set_[byte(theBit)] &= u_char(~mask(theBit)); }
				// Resets <theBit> to <0>.

    void		reset(OTC_Range const& theRange);
				// Resets bits to <0> in <theRange>. Raises
				// an exception if <theRange> isn't within
				// the range on this set.

    void		reset(u_char theStart, u_char theNum)
				{ reset(OTC_Range(theStart,theNum)); }
				// Resets <theNum> bits starting at <theStart>
				// to <0>.

    void		set()
				{ memset(set_,~0,32); }
				// Sets all bits to <1>.

    void		set(u_char theBit)
				{ set_[byte(theBit)] |= mask(theBit); }
				// Sets <theBit> to <1>.

    void		set(OTC_Range const& theRange);
				// Sets bits to <1> in <theRange>. Raises
				// an exception if <theRange> isn't within
				// the range on this set.

    void		set(u_char theStart, u_char theNum)
				{ set(OTC_Range(theStart,theNum)); }
				// Sets <theNum> bits starting at <theStart>
				// to <1>.

    void		complement();
				// Complements, or inverts all bits.

    void		complement(u_char theBit)
				{ set_[byte(theBit)] ^= mask(theBit); }
				// Complements, or inverts <theBit>.

    void		complement(OTC_Range const& theRange);
				// Complements bits in <theRange>. Raises
				// an exception if <theRange> isn't within
				// the range on this set.

    void		complement(u_char theStart, u_char theNum)
				{ complement(OTC_Range(theStart,theNum)); }
				// Complements <theNum> bits starting at
				// <theStart>.

    // = OPERATORS

    OTC_CBitSet		operator~() const;
				// Returns a set which is the complement of
				// this set, ie. with all bits inverted.

    OTC_CBitSet		operator&(OTC_CBitSet const& theSet);
				// Returns the result of performing an
				// intersection between <theSet> and this
				// set.

    OTC_CBitSet&	operator&=(OTC_CBitSet const& theSet);
				// Performs an intersection of <theSet> and
				// this set with the result being left in
				// this set.

    OTC_CBitSet		operator|(OTC_CBitSet const& theSet);
				// Returns the result of performing a union
				// of <theSet> and this set.

    OTC_CBitSet&	operator|=(OTC_CBitSet const& theSet);
				// Performs a union of <theSet> and this set
				// with the result being left in this set.

    OTC_CBitSet		operator^(OTC_CBitSet const& theSet);
				// Returns the result of performing an
				// exclusive or of <theSet> and this set.

    OTC_CBitSet&	operator^=(OTC_CBitSet const& theSet);
				// Performs an exclusive or between <theSet>
				// and this set with the result being left in
				// this set.

  private:

    static u_char	globCounts_[256];
				// Look up table for quickly determining
				// the number of bits set.

    u_char		set_[32];
				// The actual data, distributed across <32>
				// unsigned chars.
};

OSE_TEMPLATE_SPECIALISATION
class OTC_RankActions<OTC_CBitSet>
{
  public:
    static int          rank(OTC_CBitSet const& s1, OTC_CBitSet const& s2)
				{ return s1.rank(s2); }
};

OSE_TEMPLATE_SPECIALISATION
class OTC_HashActions<OTC_CBitSet>
{
  public:
    static int          hash(OTC_CBitSet const& s1)
				{ return s1.hash(); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_CBITSET_HH */
