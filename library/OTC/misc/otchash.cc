/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/otchash.cc
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
#pragma implementation "OTC/misc/hash.hh"
#endif
#endif

#include <OTC/misc/hash.hh>

extern "C"
{
#include "../hash/fnv.h"
};

#if 0
/* ------------------------------------------------------------------------- */
int OTC_Hash::hash(char const* s)
{
  if (s == 0 || *s == '\0')
    return 0;

  // Commonly used ELF hash algorithm.

  unsigned int hash = 0;
  unsigned int x    = 0;

  // for (unsigned int i = 0; i < len; i++)
  while (*s != '\0')
  {
     // hash = (hash << 4) + s[i];
     hash = (hash << 4) + *s++;
     if ((x = hash & 0xF0000000L) != 0)
     {
        hash ^= (x >> 24);
        hash &= ~x;
     }
  }

  return int(hash & 0x7FFFFFFF);
}

/* ------------------------------------------------------------------------- */
int OTC_Hash::hash(char const* s, size_t len)
{
  if (s == 0 || len == 0)
    return 0;

  // Commonly used ELF hash algorithm.

  unsigned int hash = 0;
  unsigned int x    = 0;

  for (unsigned int i = 0; i < len; i++)
  {
     hash = (hash << 4) + s[i];
     if ((x = hash & 0xF0000000L) != 0)
     {
        hash ^= (x >> 24);
        hash &= ~x;
     }
  }

  return int(hash & 0x7FFFFFFF);
}
#endif

/* ------------------------------------------------------------------------- */
int OTC_Hash::hash(char const* s)
{
  if (s == 0 || *s == '\0')
    return 0;

  Fnv32_t hash_val;
  
  hash_val = fnv_32_str((char*)s,FNV1_32_INIT);

  return int(hash_val);
}

/* ------------------------------------------------------------------------- */
int OTC_Hash::hash(char const* s, size_t len)
{
  if (s == 0 || len == 0)
    return 0;

  Fnv32_t hash_val;
  
  hash_val = fnv_32_buf((void*)s,len,FNV1_32_INIT);

  return int(hash_val);
}

/* ------------------------------------------------------------------------- */
