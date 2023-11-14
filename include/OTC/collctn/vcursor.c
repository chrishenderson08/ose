#ifndef OTC_COLLCTN_VCURSOR_C
#define OTC_COLLCTN_VCURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/vcursor.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_VecCursor<T>::~OTC_VecCursor()
{
  if (protection_ == OTCLIB_SAFE)
  {
    if (refcnt_->count() == 0)
    {
      if (vector_ != 0)
	delete [] vector_;
      delete refcnt_;
    }
    else
    {
      refcnt_->dec();
    }
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_VecCursor<T>::OTC_VecCursor(
 T* theVector,
 u_int theSize,
 OTC_Count* theCount,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : vector_(theVector),
    start_(0),
    length_(theSize),
    refcnt_(theCount),
    direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ENSURE_FN((theVector != 0 || theSize == 0),
   "OTC_VecCursor<T>::OTC_VecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid vector");
  OTCLIB_ENSURE_FN((theCount != 0 || theProtection == false),
   "OTC_VecCursor<T>::OTC_VecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid reference count object");

  if (protection_ == OTCLIB_SAFE)
    refcnt_->inc();

  reset();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_VecCursor<T>::OTC_VecCursor(
 T* theVector,
 u_int theStart,
 u_int theLength,
 OTC_Count* theCount,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : vector_(theVector),
    start_(theStart),
    length_(theLength),
    refcnt_(theCount),
    direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ENSURE_FN((theVector != 0 || theLength == 0),
   "OTC_VecCursor<T>::OTC_VecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid vector");
  OTCLIB_ENSURE_FN((theCount != 0 || theProtection == false),
   "OTC_VecCursor<T>::OTC_VecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid reference count object");

  if (protection_ == OTCLIB_SAFE)
    refcnt_->inc();

  reset();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_VecCursor<T>::OTC_VecCursor(OTC_VecCursor<T> const& theIter)
  : vector_(theIter.vector_),
    start_(theIter.start_),
    length_(theIter.length_),
    refcnt_(theIter.refcnt_),
    direction_(theIter.direction_),
    offset_(theIter.offset_)
{
  if (protection_ != OTCLIB_SAFE)
    refcnt_->inc();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_VecCursor<T>::reset()
{
  if (direction_ == OTCLIB_FORWARD)
    offset_ = start_;
  else
    offset_ = start_ + length_ - 1;
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_VecCursor<T>::item()
{
  OTCLIB_ENSURE_FN((isValid() != false),
   "OTC_VecCursor<T>::item()",
   "no item under cursor");

  return vector_[offset_];
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_VecCursor<T>::next()
{
  if ((offset_ >= (int)start_) && (offset_ < int(start_ + length_)))
  {
    if (direction_ == OTCLIB_FORWARD)
      offset_++;
    else
      offset_--;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
bool OTC_VecCursor<T>::isValid() const
{
  return (offset_ >= (int)start_) && (offset_ < int(start_ + length_));
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>* OTC_VecCursor<T>::clone()
{
  OTC_VecCursor<T>* theIter;
  theIter = new OTC_VecCursor<T>(*this);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_VCURSOR_C */
