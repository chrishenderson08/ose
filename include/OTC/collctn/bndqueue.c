#ifndef OTC_COLLCTN_BNDQUEUE_C
#define OTC_COLLCTN_BNDQUEUE_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/bndqueue.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992-1993 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedQueue<T>::~OTC_BoundedQueue()
{
  if (items_ && destroy_)
  {
    delete [] items_;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedQueue<T>::OTC_BoundedQueue(u_int theCapacity, T* theMem)
  : capacity_(theCapacity), count_(0), get_(0), put_(0),
    items_(theMem), destroy_(false)
{
  if (theMem == 0 && capacity_ > 0)
  {
    items_ = new T[capacity_];
    OTCLIB_ASSERT_M(items_ != 0);

    destroy_ = true;
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedQueue<T>::OTC_BoundedQueue(OTC_BoundedQueue<T> const& theQueue)
  : capacity_(theQueue.capacity_), count_(0),
    get_(0), put_(0), items_(0), destroy_(true)
{
  items_ = new T[capacity_];
  OTCLIB_ASSERT_M(items_ != 0);

  for (u_int i=0; i<theQueue.count(); i++)
    add(theQueue.peek(i));
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_BoundedQueue<T>& OTC_BoundedQueue<T>::operator=(
 OTC_BoundedQueue<T> const& theQueue
)
{
  if (&theQueue == this)
    return *this;

  if (destroy_)
    delete [] items_;

  capacity_ = theQueue.capacity_;
  destroy_ = true;
  count_ = 0;
  get_ = 0;
  put_ = 0;

  items_ = new T[capacity_];
  OTCLIB_ASSERT_M(items_ != 0);

  for (u_int i=0; i<theQueue.count(); i++)
    add(theQueue.peek(i));

  return *this;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_BoundedQueue<T>::add(T const& theItem)
{
  OTCLIB_ENSURE_FN((!isFull()),
   "OTC_BoundedQueue<T>::add(T const&)",
   "queue full");

  if (put_ >= capacity_)
    put_ = 0;
  items_[put_++] = theItem;
  if (put_ >= capacity_)
    put_ = 0;
  count_++;
}

/* ------------------------------------------------------------------------- */
template<class T>
T OTC_BoundedQueue<T>::remove()
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_BoundedQueue<T>::remove()",
   "queue empty");

  count_--;
  u_int theIndex = get_;
  get_++;
  if (get_ >= capacity_)
    get_ = 0;
  return items_[theIndex];
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_BoundedQueue<T>::discard(u_int theCount)
{
  OTCLIB_ENSURE_FN((theCount <= count_),
   "OTC_BoundedQueue<T>::discard()",
   "insufficient items");

  if (capacity_ != 0)
    get_ = (get_ + theCount) % capacity_;
  count_ -= theCount;
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_BoundedQueue<T>::head_() const
{
  OTCLIB_ENSURE_FN((!isEmpty()),
   "OTC_BoundedQueue<T>::head_()",
   "queue empty");

  return items_[get_];
}

/* ------------------------------------------------------------------------- */
template<class T>
T& OTC_BoundedQueue<T>::peek_(u_int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex < count_),
   "OTC_BoundedQueue<T>::peek_()",
   "index out of range");

  u_int anIndex = get_+theIndex;
  if (anIndex >= capacity_)
    anIndex -= capacity_;

  return items_[anIndex];
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_BNDQUEUE_C */
