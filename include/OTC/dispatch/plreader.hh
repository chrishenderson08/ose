#ifndef OTC_DISPATCH_PLREADER_HH
#define OTC_DISPATCH_PLREADER_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/plreader.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/types/duration.hh>
#include <OTC/types/time.hh>
#include <OTC/text/domnode.hh>
#include <OTC/text/symbol.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/collctn/list.hh>
#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/tmap.hh>

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma interface "OTC/dispatch/plreader.hh"
#endif
#endif

/* ------------------------------------------------------------------------- */

class OTC_ROPayload;

class OSE_EXPORT OTC_PLReader
    // = TITLE
    //     Helper class to read values out of a service payload object.
    //
    // = CLASS TYPE
    //     Concrete
    //
    // = DESCRIPTION
    //     The <OTC_PLReader> class is used for reading values out of a
    //     service payload object, ie., instance of <OTC_ROPayload> or
    //     a derived class. The class can handle various type conversions
    //     and also flag when a conversion isn't valid. An error will also
    //     be flagged if the payload node reference is invalid.
    //
    // = EXAMPLE
    //     You shouldn't create an instance of this class directly, but
    //     instead rely on the comma operator present on <OTC_ROPayload>.
    //     The right hand argument to the comma operator should be a
    //     boolean value which has been initialised to true. If after
    //     your conversion the boolen is false then an error occurred.
    //
    // = BEGIN<CODE>
    //       OTC_SVPayload thePayload;
    //       thePayload["count"] <<= 100;
    //       thePayload["average"] <<= 49.9;
    //
    //       long theCount = 0;
    //       double theAverage = 0;
    //
    //       bool theStatus = true;
    //
    //       (thePayload["count"],theStatus) >>= theCount;
    //       (thePayload["average"],theStatus) >>= theAverage;
    //
    //       if (theStatus != false)
    //          ... all okay
    // = END<CODE>
    //
    //     You can define your own operator to allow you to read more
    //     complicated types out of a payload.
    //
    // = BEGIN<CODE>
    //       void operator>>=(OTC_PLReader theReader, List& theList)
    //       {
    //         if (theReader.errorOccurred())
    //           return;
    //
    //         OTC_ROPayload theObject;
    //         theObject = theReader.node();
    //
    //         if (theObject.nodeType() != "array")
    //         {
    //           theReader.flagError();
    //           return;
    //         }
    //
    //         theList.removeAll();
    //
    //         OTC_ROPayload tmpObject;
    //         OTC_String theString;
    //         bool theStatus = true;
    //
    //         tmpObject = theObject.firstChild();
    //         while (tmpObject.isValid())
    //         {
    //           (tmpObject,theStatus) >>= theString;
    //           if (theStatus == false)
    //           {
    //             theReader.flagError();
    //             return;
    //           }
    //
    //           theList.addLast(theString);
    //           tmpObject = tmpObject.nextSibling();
    //         }
    //       }
    // = END<CODE>
    //
    //     Which would be used as:
    //
    // = BEGIN<CODE>
    //       OTC_SVPayload thePayload;
    //       thePayload[0] <<= "one";
    //       thePayload[1] <<= "two";
    //       thePayload[2] <<= "three";
    //
    //       List theList;
    //
    //       bool theStatus = true;
    //
    //       (thePayload,theStatus) >>= theList;
    //
    //       if (theStatus != false)
    //          ... all okay
    // = END<CODE>
    //
    // = SEE ALSO
    //     <OTC_ROPayload>, <OTC_RWPayload>, <OTC_SVPayload>
{
  public:

    			~OTC_PLReader() {}

    // = INITIALISATION
    //     Note that rely on default copy constructor and assignment
    //     operator.

    			OTC_PLReader(
			 OTC_ROPayload const& theNode,
			 bool& theStatus
			)
			  : node_(theNode), status_(theStatus) {}
				// Initialiser reader on <theNode> where
				// <theStatus> is reference to boolean
				// in which errors should be flagged.

    // = STATUS

    bool		status() const
      				{ return status_; }
    				// Returns the status flag.

    bool&		status()
      				{ return status_; }
    				// Returns the status flag. In this case
				// an actual reference to status flag is
				// returned.

    bool		errorOccurred() const
      				{ return status_ == false; }
				// Returns <true> if an error has been
				// flagged.

    void		flagError()
      				{ status_ = false; }
    				// Flags that an error has occurred.

    // = NODE

    OTC_ROPayload const&	node() const
      				{ return node_; }
				// Returns a reference to the payload node
				// from which values are being read.

    // = INPUT
    //     The following will all flag an error if the node being read from
    //     isn't a value. In the case of integer and floating point values,
    //     if the number can't be read into a variable type because it
    //     is too large an error will be flagged.

    void		operator>>=(OTC_String& theString);
    				// Reads value into <theString> regardless
				// of the type.

    void		operator>>=(OTC_Symbol& theSymbol);
    				// Reads value into <theSymbol> regardless
				// of the type.

    void		operator>>=(bool& theValue);
    				// Reads boolen value into <theValue>.
				// Standard conversions from integer and
				// floating point types will be done.

    void		operator>>=(long& theValue);
				// Reads long value into <theValue>. Standard
				// conversions from other integer and
				// floating point types and the boolean type
				// will be done.

    void		operator>>=(char& theValue)
      				{
				  long tmpValue;
				  (*this) >>= tmpValue;
				  theValue = char(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads char value into <theValue>. Standard
				// conversions from other integer and
				// floating point types and the boolean type
				// will be done.

    void		operator>>=(short& theValue)
      				{
				  long tmpValue;
				  (*this) >>= tmpValue;
				  theValue = short(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads short value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

    void		operator>>=(int& theValue)
      				{
				  long tmpValue;
				  (*this) >>= tmpValue;
				  theValue = int(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads int value into <theValue>. Standard
				// conversions from other integer and
				// floating point types and the boolean type
				// will be done.

    void		operator>>=(u_long& theValue);
				// Reads unsigned long value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

    void		operator>>=(u_char& theValue)
      				{
				  u_long tmpValue;
				  (*this) >>= tmpValue;
				  theValue = u_char(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads unsigned char value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

    void		operator>>=(u_short& theValue)
      				{
				  u_long tmpValue;
				  (*this) >>= tmpValue;
				  theValue = u_short(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads unsigned short value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

    void		operator>>=(u_int& theValue)
      				{
				  u_long tmpValue;
				  (*this) >>= tmpValue;
				  theValue = u_int(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads unsigned int value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

#if SIZEOF_LONG_LONG != 0
//#if defined(HAVE_STRTOLL)

    void		operator>>=(long long& theValue);
				// Reads long value into <theValue>. Standard
				// conversions from other integer and
				// floating point types and the boolean type
				// will be done.

    void		operator>>=(unsigned long long& theValue);
				// Reads long value into <theValue>. Standard
				// conversions from other integer and
				// floating point types and the boolean type
				// will be done.

//#endif
#endif

    void		operator>>=(double& theValue);
				// Reads double value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

#if SIZEOF_LONG_DOUBLE != 0
#if SIZEOF_LONG_DOUBLE != SIZEOF_DOUBLE
//#if defined(HAVE_STRTOLD)

    void		operator>>=(long double& theValue);
				// Reads double value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

//#endif
#endif
#endif

    void		operator>>=(float& theValue)
      				{
				  double tmpValue;
				  (*this) >>= tmpValue;
				  theValue = float(tmpValue);
				  if (theValue != tmpValue)
				    flagError();
				}
				// Reads float value into <theValue>.
				// Standard conversions from other integer
				// and floating point types and the boolean
				// type will be done.

    void		operator>>=(OTC_Date& theDate);
				// Reads value with type "date" into
				// <theValue>.

    void		operator>>=(OTC_Time& theTime);
				// Reads value with type "dateTime" into
				// <theValue>. Note that time zone
				// information or fractions of sections are
				// not handled.

    void		operator>>=(OTC_Duration& theDuration);
				// Reads value with type "duration" into
				// <theValue>.

    void		operator>>=(OTC_ROPayload& theNode);
				// Makes <theNode> an alias to the data to
				// be read.

  private:

    OTC_ROPayload const&	node_;
    				// The node from which values are being
				// read.

    bool&			status_;
    				// The status flag indicating if errors
				// have occurred. If <true> all is still
				// okay.
};

/* ------------------------------------------------------------------------- */

#endif /* OTC_DISPATCH_PLREADER_HH */
