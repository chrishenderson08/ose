/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/otcrank.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/misc/rank.hh"
#endif
#endif

#include <OTC/misc/rank.hh>

#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

#if defined(HAVE_WCHAR_T)
#include <wchar.h>
#include <wctype.h>
#endif

/* ------------------------------------------------------------------------- */
static char OTCLIB_CASEMAP[] =
{
  '\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
  '\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
  '\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
  '\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
  '\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
  '\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
  '\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
  '\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
  '\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
  '\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
  '\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
  '\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
  '\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
  '\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
  '\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
  '\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
  '\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
  '\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
  '\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
  '\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
  '\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
  '\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
  '\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
  '\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
  '\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
  '\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
  '\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
  '\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
  '\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377'
};

/* ------------------------------------------------------------------------- */
int OTC_Rank::rank(
 char const* theString1,
 size_t theLength1,
 char const* theString2,
 size_t theLength2,
 OTC_SRankType theType
)
{
  // If both are zero length, they are equal.

  if (!theLength1 && !theLength2)
    return 0;

  // Work out which is shorter.

  size_t theLength = theLength1;
  if (theLength2 < theLength)
    theLength = theLength2;

  // If one is zero length, it is the least ranked.

  if (!theLength)
    return (theLength2 > 0) ? -1 : 1;

#if !defined(HAVE_STRCOLL)
  if (theType == OTCLIB_LOCALEORDER)
    theType = OTCLIB_ASCIIORDER;
#endif

  if (theType == OTCLIB_ASCIIORDER)
  {
    int theRank = memcmp(theString1,theString2,theLength);

    if (theRank != 0)
      return theRank;
  }
  else if (theType == OTCLIB_ASCIIORDER_IGNORECASE)
  {
    while (theLength-- != 0)
    {
      if (OTCLIB_CASEMAP[size_t(*theString1++)] !=
       OTCLIB_CASEMAP[size_t(*theString2++)])
      {
	return (OTCLIB_CASEMAP[size_t(*--theString1)] -
	 OTCLIB_CASEMAP[size_t(*--theString2)]);
      }
    }
  }
#if defined(HAVE_STRCOLL)
  else
  {
     return strcoll(theString1,theString2);
  }
#endif

  return (theLength1 == theLength2) ? 0 : ((theLength1 < theLength2) ? -1 : 1);
}

/* ------------------------------------------------------------------------- */
int OTC_Rank::rank(char const* s1, char const* s2, OTC_SRankType theType)
{
#if !defined(HAVE_STRCOLL)
  if (theType == OTCLIB_LOCALEORDER)
    theType = OTCLIB_ASCIIORDER;
#endif

  if (theType == OTCLIB_ASCIIORDER)
  {
    // Do it quickly.

    if (s1 == s2)
      return 0;

    else if (s1 == 0)
      return -1;

    else if (s2 == 0)
      return 1;

    else
    {
      int res = strcmp(s1,s2);
      if (res < 0)
	return -1;

      else if (res > 0)
	return 1;

      return 0;
    }
  }

  size_t l1 = 0;
  size_t l2 = 0;

  if (s1 != 0)
    l1 = strlen(s1);

  if (s2 != 0)
    l2 = strlen(s2);

  return OTC_Rank::rank(s1,l1,s2,l2,theType);
}

/* ------------------------------------------------------------------------- */
#if defined(HAVE_WCHAR_T)
int OTC_Rank::rank(
 wchar_t const* theString1,
 size_t theLength1,
 wchar_t const* theString2,
 size_t theLength2,
 OTC_SRankType theType
)
{
  // If both are zero length, they are equal.

  if (!theLength1 && !theLength2)
    return 0;

  // Work out which is shorter.

  size_t theLength = theLength1;
  if (theLength2 < theLength)
    theLength = theLength2;

  // If one is zero length, it is the least ranked.

  if (!theLength)
    return (theLength2 > 0) ? -1 : 1;

  if (theType == OTCLIB_ASCIIORDER)
  {
    int theRank = memcmp((char const*)theString1,
     (char const*)theString2,theLength*sizeof(wchar_t));

    if (theRank != 0)
      return theRank;
  }
  else if (theType == OTCLIB_ASCIIORDER_IGNORECASE)
  {
    while (theLength-- != 0)
    {
      wchar_t c1 = *theString1;
      wchar_t c2 = *theString2;

      if (iswupper(c1))
	c1 = towlower(c1);

      if (iswupper(c2))
	c2 = towlower(c2);

      if (c1 != c2)
	return c1 - c2;

      theString1++;
      theString2++;
    }
  }
  else
  {
     return wcscoll(theString1,theString2);
  }

  return (theLength1 == theLength2) ? 0 : ((theLength1 < theLength2) ? -1 : 1);
}
#endif

/* ------------------------------------------------------------------------- */
