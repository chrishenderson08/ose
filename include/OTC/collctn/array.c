#ifndef OTC_COLLCTN_ARRAY_C
#define OTC_COLLCTN_ARRAY_C
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     collctn/array.c
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1991-1993 OTC LIMITED
//     Copyright 1995-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/collctn/vcursor.hh>
#include <OTC/collctn/ivcursor.hh>

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::~OTC_Array()
{
  if (refcnt_->count() == 0)
  {
    delete [] vector_;
    delete refcnt_;
  }
  else
  {
    refcnt_->dec();
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(u_int theLength)
  : range_(0,theLength)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(T const* theVector, u_int theLength)
  : range_(0,theLength)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  OTC_CopyActions<T>::copy(vector_,theVector,range_.length());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(int theStart, u_int theLength)
  : range_(theStart,theLength)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(int theStart, u_int theLength, T const& theValue)
  : range_(theStart,theLength)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  for (u_int i=0; i<range_.length(); i++)
    vector_[i] = theValue;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(OTC_Range const& theRange)
  : range_(theRange)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(OTC_Range const& theRange, T const& theValue)
  : range_(theRange)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  for (u_int i=0; i<range_.length(); i++)
    vector_[i] = theValue;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(OTC_Array<T> const& theVector)
  : range_(theVector.range_)
{
  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  OTC_CopyActions<T>::copy(vector_,theVector.vector_,range_.length());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(
 OTC_Array<T> const& theVector,
 u_int theLength
)
  : range_(0,theLength)
{
  OTCLIB_ENSURE_FN((theLength <= theVector.size()),
   "OTC_Array<T>::OTC_Array(OTC_Array<T> const&, u_int)",
   "invalid length");

  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  OTC_CopyActions<T>::copy(vector_,theVector.vector_,range_.length());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(
 OTC_Array<T> const& theVector,
 int theStart,
 u_int theLength
)
  : range_(0,theLength)
{
  OTC_Range theRange(theStart,theLength);

  OTCLIB_ENSURE_FN((theRange.isSubRange(theVector.range())),
   "OTC_Array<T>::OTC_Array(OTC_Array<T> const&, int, u_int)",
   "invalid range");

  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  T const* tmpVector;
  tmpVector = theVector.vector_+(theStart-theVector.range_.lower());

  OTC_CopyActions<T>::copy(vector_,tmpVector,range_.length());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(
 OTC_Array<T> const& theVector,
 OTC_Range const& theRange
)
  : range_(0,theRange.length())
{
  OTCLIB_ENSURE_FN((theRange.isSubRange(theVector.range())),
   "OTC_Array<T>::OTC_Array(OTC_Array<T> const&, OTC_Range const&)",
   "invalid range");

  vector_ = new T[range_.length()];
  OTCLIB_ASSERT_M(vector_ != 0);

  refcnt_ = new OTC_Count;
  OTCLIB_ASSERT_M(refcnt_ != 0);

  T const* tmpVector;
  tmpVector = theVector.vector_+(theRange.lower()-theVector.range_.lower());

  OTC_CopyActions<T>::copy(vector_,tmpVector,range_.length());
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Array<T>::OTC_Array(OTC_Array<T>& theVector, OTC_ShallowCopy)
  : refcnt_(theVector.refcnt_),
    range_(theVector.range_),
    vector_(theVector.vector_)
{
  refcnt_->inc();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::assign(OTC_Array<T> const& theVector)
{
  if (&theVector != this)
  {
    if (refcnt_->count() == 0)
    {
      delete [] vector_;
    }
    else
    {
      refcnt_->dec();

      refcnt_ = new OTC_Count;
      OTCLIB_ASSERT_M(refcnt_ != 0);
    }

    range_ = theVector.range_;

    vector_ = new T[range_.length()];
    OTCLIB_ASSERT_M(vector_ != 0);

    OTC_CopyActions<T>::copy(vector_,theVector.vector_,range_.length());
  }
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::replaceRange(OTC_Range const& theRange, T const& theValue)
{
  OTCLIB_ENSURE_FN((theRange.isSubRange(range_)),
   "OTC_Array<T>::replaceRange(OTC_Range const&, T const&)",
   "invalid range");

  for (int i=theRange.lower(); i<theRange.upper(); i++)
    vector_[i-range_.lower()] = theValue;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_Cursor<T>* OTC_Array<T>::items_(
 int theStart,
 u_int theLength,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  if (theLength == 0)
    return 0;

  OTCLIB_ENSURE_FN((theStart >= range_.lower()),
   "OTC_Array<T>::items_(int, u_int, OTC_Direction, OTC_Protection)",
   "invalid range");
  OTCLIB_ENSURE_FN((int(theStart+theLength) <= range_.upper()),
   "OTC_Array<T>::items_(int, u_int, OTC_Direction, OTC_Protection)",
   "invalid range");

  OTC_Cursor<T>* theIter;
  theIter = new OTC_VecCursor<T>((T*)vector_,theStart-range_.lower(),
   theLength,refcnt_,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
OTC_PairCursor<int,T>* OTC_Array<T>::pairs_(
 int theStart,
 u_int theLength,
 OTC_Direction theDirection,
 OTC_Protection theProtection
) const
{
  if (theLength == 0)
    return 0;

  OTCLIB_ENSURE_FN((theStart >= range_.lower()),
   "OTC_Array<T>::pairs_(int, u_int, OTC_Direction, OTC_Protection)",
   "invalid range");
  OTCLIB_ENSURE_FN((int(theStart+theLength) <= range_.upper()),
   "OTC_Array<T>::pairs_(int, u_int, OTC_Direction, OTC_Protection)",
   "invalid range");

  OTC_PairCursor<int,T>* theIter;
  theIter = new OTC_IdxVecCursor<T>((T*)vector_,range_.lower(),
   theStart-range_.lower(),theLength,refcnt_,theDirection,theProtection);
  OTCLIB_ASSERT_M(theIter != 0);
  return theIter;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::apply(
 OTC_Worker<T>& theApplicator,
 OTC_Direction theDirection
)
{
  theApplicator.start();

  OTC_Progress theProgress;

  if (theDirection == OTCLIB_FORWARD)
  {
    int i = range_.lower();

    while (i < range_.upper())
    {
      theProgress = theApplicator.action(vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.lower();
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i++;
      }
      else
	break;
    }
  }
  else
  {
    int i = range_.upper() - 1;

    while (i >= range_.lower())
    {
      theProgress = theApplicator.action(vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.upper() - 1;
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i--;
      }
      else
	break;
    }
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::apply(
 OTC_Visitor<T>& theApplicator,
 OTC_Direction theDirection
)
{
  theApplicator.start();

  OTC_Progress theProgress;

  if (theDirection == OTCLIB_FORWARD)
  {
    int i = range_.lower();

    while (i < range_.upper())
    {
      theProgress = theApplicator.action(vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.lower();
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i++;
      }
      else
	break;
    }
  }
  else
  {
    int i = range_.upper() - 1;

    while (i >= range_.lower())
    {
      theProgress = theApplicator.action(vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.upper() - 1;
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i--;
      }
      else
	break;
    }
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::apply(
 OTC_PairWorker<int,T>& theApplicator,
 OTC_Direction theDirection
)
{
  theApplicator.start();

  OTC_Progress theProgress;

  if (theDirection == OTCLIB_FORWARD)
  {
    int i = range_.lower();

    while (i < range_.upper())
    {
      theProgress = theApplicator.action(i,vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.lower();
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i++;
      }
      else
	break;
    }
  }
  else
  {
    int i = range_.upper() - 1;

    while (i >= range_.lower())
    {
      theProgress = theApplicator.action(i,vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.upper() - 1;
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i--;
      }
      else
	break;
    }
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::apply(
 OTC_PairVisitor<int,T>& theApplicator,
 OTC_Direction theDirection
)
{
  theApplicator.start();

  OTC_Progress theProgress;

  if (theDirection == OTCLIB_FORWARD)
  {
    int i = range_.lower();

    while (i < range_.upper())
    {
      theProgress = theApplicator.action(i,vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.lower();
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i++;
      }
      else
	break;
    }
  }
  else
  {
    int i = range_.upper() - 1;

    while (i >= range_.lower())
    {
      theProgress = theApplicator.action(i,vector_[i-range_.lower()]);
      if (theProgress == OTCLIB_RESTART)
      {
	i = range_.upper() - 1;
      }
      else if (theProgress == OTCLIB_CONTINUE)
      {
	i--;
      }
      else
	break;
    }
  }

  theApplicator.finish();
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::resize(OTC_Range const& theRange)
{
  if (theRange == range_)
    return;

  T* tmpVector;
  tmpVector = new T[theRange.length()];
  OTCLIB_ASSERT_M(tmpVector != 0);

  if (theRange.length() <= range_.length())
    OTC_CopyActions<T>::copy(tmpVector,vector_,theRange.length());
  else
    OTC_CopyActions<T>::copy(tmpVector,vector_,range_.length());

  if (refcnt_->count() == 0)
  {
    delete [] vector_;
  }
  else
  {
    refcnt_->dec();

    refcnt_ = new OTC_Count;
    OTCLIB_ASSERT_M(refcnt_ != 0);
  }

  vector_ = tmpVector;
  range_ = theRange;
}

/* ------------------------------------------------------------------------- */
template<class T>
void OTC_Array<T>::resize(OTC_Range const& theRange, T const& theValue)
{
  if (theRange == range_)
    return;

  T* tmpVector;
  tmpVector = new T[theRange.length()];
  OTCLIB_ASSERT_M(tmpVector != 0);

  if (theRange.length() <= range_.length())
    OTC_CopyActions<T>::copy(tmpVector,vector_,theRange.length());
  else
    OTC_CopyActions<T>::copy(tmpVector,vector_,range_.length());

  int i;

  for (i=theRange.lower(); i<range_.lower(); i++)
    tmpVector[i] = theValue;
  for (i=range_.upper(); i<theRange.upper(); i++)
    tmpVector[i] = theValue;

  if (refcnt_->count() == 0)
  {
    delete [] vector_;
  }
  else
  {
    refcnt_->dec();

    refcnt_ = new OTC_Count;
    OTCLIB_ASSERT_M(refcnt_ != 0);
  }

  vector_ = tmpVector;
  range_ = theRange;
}

/* ------------------------------------------------------------------------- */

#endif /* OTC_COLLCTN_ARRAY_C */
