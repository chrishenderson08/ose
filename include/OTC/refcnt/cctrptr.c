#ifndef OTC_REFCNT_CCTRPTR_C
#define OTC_REFCNT_CCTRPTR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     refcnt/cctrptr.c
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
OTC_CCtrPtr<T>::OTC_CCtrPtr()
 : object_(0), count_(0)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrPtr<T>::OTC_CCtrPtr(T const* theItem)
 : object_(0), count_(0)
{
  setItem(theItem);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrPtr<T>::OTC_CCtrPtr(OTC_CCtrPtr<T> const& theCtrPtr)
 : object_(0), count_(0)
{
  setItem(theCtrPtr);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrPtr<T>::~OTC_CCtrPtr()
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
void OTC_CCtrPtr<T>::setItem(OTC_CCtrPtr<T> const& theCtrPtr)
{
  if (theCtrPtr.object_ == object_)
    return;

  else
  {
    if (count_ != 0)
    {
      count_->dec();
      if (count_->count() == 0)
        cleanup();
    }
    object_ = theCtrPtr.object_;
    count_ = 0;
    if (object_ != 0)
    {
      count_ = theCtrPtr.count_;
      count_->inc();
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CCtrPtr<T>::setItem(T const* theItem)
{
  if (object_ == theItem)
    return;

  else
  {
    if (count_ != 0)
    {
      count_->dec();
      if (count_->count() == 0)
	cleanup();
    }
    object_ = (T*)theItem;
    count_ = 0;
    if (object_ != 0)
    {
      count_ = new OTC_Count;
      OTCLIB_ASSERT_M(count_ != 0);
      count_->inc();
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrPtr<T>& OTC_CCtrPtr<T>::operator=(OTC_CCtrPtr<T> const& theCtrPtr)
{
  setItem(theCtrPtr);
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_CCtrPtr<T>& OTC_CCtrPtr<T>::operator=(T const* theItem)
{
  setItem(theItem);
  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
u_int OTC_CCtrPtr<T>::numRefs() const
{
  if (object_ == 0)
    return 0;

  else
    return count_->count();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_CCtrPtr<T>::cleanup()
{
  if (count_ != 0)
  {
    delete count_;
  }
  count_ = 0; 
  if (object_ != 0)
  {
    delete object_;
  }
  object_ = 0;
}

/* ------------------------------------------------------------------------- */
template<class T>
T* OTC_CCtrPtr<T>::itemNoNull_() const
{
  OTCLIB_ENSURE_FN((object_ != 0),
   "OTC_CCtrPtr::itemNoNull_()",
   "no item held");

  return object_;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_REFCNT_CCTRPTR_C */
