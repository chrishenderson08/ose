#ifndef OTC_COLLCTN_COPYACTN_C
#define OTC_COLLCTN_COPYACTN_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/copyactn.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1993 TELSTRA CORPORATION LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CopyActions<T>::copy(T* theDest, T const* theSrc, size_t theLength)
{
  if (theDest < theSrc)
  {
    while (theLength--)
      *theDest++ = *theSrc++;
  }
  else
  {
    theDest += theLength;
    theSrc += theLength;

    while (theLength--)
      *--theDest = *--theSrc;
  }
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_COPYACTN_C */
