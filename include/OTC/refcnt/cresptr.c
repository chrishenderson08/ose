#ifndef OTC_REFCNT_CRESPTR_C
#define OTC_REFCNT_CRESPTR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/cresptr.c
// 
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CResPtr<T>::OTC_CResPtr()
 : object_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CResPtr<T>::OTC_CResPtr(T const* theItem)
 : object_(0)
{
  setItem(theItem);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CResPtr<T>::OTC_CResPtr(OTC_CResPtr<T> const& theRP)
 : object_(0)
{
  setItem(theRP);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CResPtr<T>::~OTC_CResPtr()
{
  if (object_ != 0)
    object_->unReference();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CResPtr<T>::setItem(OTC_CResPtr<T> const& theRP)
{
  setItem(theRP.object_);
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CResPtr<T>::setItem(T const* theItem)
{
  if (object_ == theItem)
    return;

  if (theItem != 0)
    theItem->reference();
  if (object_ != 0)
    object_->unReference();
  object_ = (T*)theItem;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CResPtr<T>& OTC_CResPtr<T>::operator=(OTC_CResPtr<T> const& theRP)
{
  setItem(theRP);
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CResPtr<T>& OTC_CResPtr<T>::operator=(T const* theItem)
{
  setItem(theItem);
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
u_int OTC_CResPtr<T>::numRefs() const
{
  if (object_ == 0)
    return 0;

  else
    return object_->numRefs();
}

/* ------------------------------------------------------------------------- */
template<class T>
T* OTC_CResPtr<T>::itemNoNull_() const
{
  OTCLIB_ENSURE_FN((object_ != 0),
   "OTC_CResPtr::itemNoNull_()",
   "no item held");

  return object_;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_RESPTR_C */
