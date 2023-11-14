#ifndef OTC_COLLCTN_STRGACTN_HH
#define OTC_COLLCTN_STRGACTN_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/strgactn.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OTC_COLLCTN_HASHACTN_HH
#include <OTC/collctn/hashactn.hh>
#endif
#ifndef OTC_COLLCTN_RANKACTN_HH
#include <OTC/collctn/rankactn.hh>
#endif
#ifndef OTC_COLLCTN_COPYACTN_HH
#include <OTC/collctn/copyactn.hh>
#endif

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<char*>
{
  public:
    static int          hash(char* const& s)
				{ return OTC_Hash::hash(s); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_HashActions<const char*>
{
  public:
    static int          hash(const char* const& s)
				{ return OTC_Hash::hash(s); }
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<char*>
{
  public:
    static int          rank(char* const& s1, char* const& s2)
				{ return OTC_Rank::rank(s1,s2); }
};

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_RankActions<const char*>
{
  public:
    static int          rank(const char* const& s1, const char* const& s2)
				{ return OTC_Rank::rank(s1,s2); }
};

/* ------------------------------------------------------------------------- */

OSE_TEMPLATE_SPECIALISATION
class OSE_EXPORT OTC_CopyActions<char>
{
  public:
    static void		copy(char* s1, char const* s2, size_t l)
				{ OTC_Copy::copy(s1,s2,l); }
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_STRGACTN_HH */
