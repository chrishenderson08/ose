#ifndef OTC_REFCNT_CCTRVPTR_C
#define OTC_REFCNT_CCTRVPTR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/cctrvptr.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1994-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrVecPtr<T>::OTC_CCtrVecPtr()
 : array_(0), count_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrVecPtr<T>::OTC_CCtrVecPtr(T const* theItem)
 : array_(0), count_(0)
{
  setItem(theItem);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrVecPtr<T>::OTC_CCtrVecPtr(OTC_CCtrVecPtr<T> const& theCtrPtr)
 : array_(0), count_(0)
{
  setItem(theCtrPtr);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrVecPtr<T>::~OTC_CCtrVecPtr()
{
  if (count_ != 0)
  {
    count_->dec();
    if (count_->count() == 0)
      cleanup();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CCtrVecPtr<T>::setItem(OTC_CCtrVecPtr<T> const& theCtrPtr)
{
  if (theCtrPtr.array_ == array_)
    return;

  else
  {
    if (count_ != 0)
    {
      count_->dec();
      if (count_->count() == 0)
        cleanup();
    }
    array_ = theCtrPtr.array_;
    count_ = 0;
    if (array_ != 0)
    {
      count_ = theCtrPtr.count_;
      count_->inc();
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CCtrVecPtr<T>::setItem(T const* theItem)
{
  if (array_ == theItem)
    return;

  else
  {
    if (count_ != 0)
    {
      count_->dec();
      if (count_->count() == 0)
	cleanup();
    }
    array_ = (T*)theItem;
    count_ = 0;
    if (array_ != 0)
    {
      count_ = new OTC_Count;
      OTCLIB_ASSERT_M(count_ != 0);

      count_->inc();
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrVecPtr<T>& OTC_CCtrVecPtr<T>::operator=(
  OTC_CCtrVecPtr<T> const& theCtrPtr
)
{
  setItem(theCtrPtr);
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrVecPtr<T>& OTC_CCtrVecPtr<T>::operator=(T const* theItem)
{
  setItem(theItem);
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
u_int OTC_CCtrVecPtr<T>::numRefs() const
{
  if (array_ == 0)
    return 0;

  else
    return count_->count();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CCtrVecPtr<T>::cleanup()
{
  if (count_ != 0)
    delete count_;
  count_ = 0; 
  if (array_ != 0)
    delete [] array_;
  array_ = 0;
}

/* ------------------------------------------------------------------------- */
template<class T>
T* OTC_CCtrVecPtr<T>::itemNoNull_() const
{
  OTCLIB_ENSURE_FN((array_ != 0),
   "OTC_CCtrVecPtr::itemNoNull_()",
   "no item held");

  return array_;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_CCTRVPTR_C */
