#ifndef OTC_COLLCTN_IVCURSOR_C
#define OTC_COLLCTN_IVCURSOR_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/ivcursor.c
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
OTC_IdxVecCursor<T>::~OTC_IdxVecCursor()
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
OTC_IdxVecCursor<T>::OTC_IdxVecCursor(
 T* theVector,
 int theLower,
 u_int theSize,
 OTC_Count* theCount,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : vector_(theVector),
    lower_(theLower),
    start_(0),
    length_(theSize),
    refcnt_(theCount),
    direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ENSURE_FN((theVector != 0 || theSize == 0),
   "OTC_IdxVecCursor<T>::OTC_IdxVecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid vector");
  OTCLIB_ENSURE_FN((theCount != 0 || theProtection == false),
   "OTC_IdxVecCursor<T>::OTC_IdxVecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid reference count object");

  if (protection_ == OTCLIB_SAFE)
    refcnt_->inc();

  reset();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IdxVecCursor<T>::OTC_IdxVecCursor(
 T* theVector,
 int theLower,
 u_int theStart,
 u_int theLength,
 OTC_Count* theCount,
 OTC_Direction theDirection,
 OTC_Protection theProtection
)
  : vector_(theVector),
    lower_(theLower),
    start_(theStart),
    length_(theLength),
    refcnt_(theCount),
    direction_(theDirection),
    protection_(theProtection)
{
  OTCLIB_ENSURE_FN((theVector != 0 || theLength == 0),
   "OTC_IdxVecCursor<T>::OTC_IdxVecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid vector");
  OTCLIB_ENSURE_FN((theCount != 0 || theProtection == false),
   "OTC_IdxVecCursor<T>::OTC_IdxVecCursor(T*, u_int, OTC_Count, OTC_Direction)",
   "invalid reference count object");

  if (protection_ == OTCLIB_SAFE)
    refcnt_->inc();

  reset();
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_IdxVecCursor<T>::OTC_IdxVecCursor(OTC_IdxVecCursor<T> const& theIter)
  : vector_(theIter.vector_),
    lower_(theIter.lower_),
    start_(theIter.start_),
    length_(theIter.length_),
    refcnt_(theIter.refcnt_),
    direction_(theIter.direction_),
    offset_(theIter.offset_),
    position_(theIter.position_)
{
  if (protection_ != OTCLIB_SAFE)
    refcnt_->inc();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IdxVecCursor<T>::reset()
{
  if (direction_ == OTCLIB_FORWARD)
    offset_ = start_;
  else
    offset_ = start_ + length_ - 1;
}

/* ------------------------------------------------------------------------- */
template<class T>
int& OTC_IdxVecCursor<T>::key()
{
  OTCLIB_ENSURE_FN((isValid() != false),
   "OTC_IdxVecCursor<T>::key()",
   "no key/item under cursor");

  position_ = lower_ + offset_;

  return position_;
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_IdxVecCursor<T>::item()
{
  OTCLIB_ENSURE_FN((isValid() != false),
   "OTC_IdxVecCursor<T>::item()",
   "no key/item under cursor");

  return vector_[offset_];
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_IdxVecCursor<T>::next()
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
bool OTC_IdxVecCursor<T>::isValid() const
{
  return (offset_ >= (int)start_) && (offset_ < int(start_ + length_));
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PairCursor<int,T>* OTC_IdxVecCursor<T>::clone()
{
  OTC_IdxVecCursor<T>* theIter;
  theIter = new OTC_IdxVecCursor<T>(*this);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_IVCURSOR_C */
