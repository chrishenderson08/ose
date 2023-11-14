#ifndef VALIDATORSERVICE_HH
#define VALIDATORSERVICE_HH
/*
// ============================================================================
//
// = FILENAME
//     ValidatorService.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/service.hh>

#include <OTC/collctn/hmap.hh>
#include <OTC/collctn/deque.hh>

/* ------------------------------------------------------------------------- */

class TST_ValidatorService : public ONS_Service
    // = TITLE
    //     Implementation of a validator service.
    //
    // = DESCRIPTION
    //     The <TST_ValidatorService> class implements a service which
    //     implements the validation routines for the XML-RPC protocol. The
    //     same tests can also be used to test the NET-RPC protocol, but
    //     cannot strictly be used to test the SOAP protocol because the
    //     tests as originally written generated data that cannot be encoded
    //     using the SOAP protocol.
    //
    // = NOTES
    //     People using the XML-RPC protocol promote the idea of methods
    //     exported under a URL being separated into different namespaces if
    //     they correspond to different services, by prefixing a method name
    //     with a scope name, followed by a dot. In the case of the XML-RPC
    //     validator methods, they thus use names for the methods such as
    //     "validator1.nestedStructTest".
    //
    //     The better way of doing this would have been to use different
    //     URLs for the different services. The problem is that most XML-RPC
    //     server side implementations force you to use a single URL and
    //     can't necessarily easily handle there being multiple registered
    //     URLs.
    //
    //     So as not to continue this poor design decision, in this
    //     implementation, names of the test methods are not prefixed with
    //     the "validator1" scope name. Instead the test routines are within
    //     their own service and are thus will have their own URL. If you
    //     want to use this service with a third party XML-RPC validation
    //     client, you will need to change the exported method names to
    //     include the scope component.
    //
    //     Also note that the definition of tests themselves aren't very
    //     good as they don't properly enumerate what the types are in
    //     some cases. This is fine for a weakly typed language such as
    //     was used in the first implementations of the XML-RPC protocol,
    //     but doesn't help with a strongly typed language. Thus in some
    //     cases it is necessary to make the routines more generic than
    //     they should meaning they actually fulfill more than what the
    //     test called for.
{
  public:

    // = INITIALISATION
    //     Note that the session manager will not initially be visible, nor
    //     any exported methods registered. The constructor will however
    //     register the <serviceActivation()> method as a startup callback.
    //     The <serviceActivation()> method will take care of exporting any
    //     methods and making the service visible. The <scheduleStartup()>
    //     method should be called once the object has been fully constructed
    //     to trigger calling of the <serviceActivation()> method. Do not
    //     call <scheduleStartup()> from a derived class constructor, because
    //     in a threaded application this may result in events being received
    //     by the service prior to it being fully initialised.

    			TST_ValidatorService(
			 char const* theServiceName,
			 char const* theServiceAudience
			);
				// Initialises the new session manager with
				// <theServiceName> and <theServiceAudience>.

  protected:

    // = SERVICE ACTIVATION

    virtual void	serviceActivation();
				// Registered by the constructor as the
				// startup callback. The method will
				// subsequently be called as a result of the
				// <scheduleStartup()> method being called.
				// The <scheduleStartup()> method should
				// only be called outside of the scope of
				// the constructor and not within a derived
				// class constructor. The default version of
				// this method will export any service
				// methods and make the service visible.
				// A derived class may override this method
				// in which case it should call this classes
				// version of the method at some point, or
				// duplicate its functionality.

  protected:

    // = VALIDATOR TESTS

    int			arrayOfStructsTest(
			 OTC_Deque<OTC_HMap<OTC_String,int> > const& theArray
			) const;
				// This method takes a single parameter, an
				// array of structs, each of which contains
				// at least three elements named "moe",
				// "larry" and "curly", all integers. The
				// method adds all the struct elements named
				// "curly" and return the result.

    OTC_PairIterator<OTC_String,int>	countTheEntities(
    			 char const* theString
			) const;
				// This method takes a single parameter, a
				// string, that contains any number of
				// predefined entities, namely "left angle
				// brackets", "right angle brackets",
				// "ampersands", "apostrophes" and "quotes".
				// The method returns a struct that contains
				// five fields, all numbers with names
				// ctLeftAngleBrackets, ctRightAngleBrackets,
				// ctAmpersands, ctApostrophes, ctQuotes.
				// These will correspond to the count of the
				// corresponding entity.

    int			easyStructTest(
    			 OTC_HMap<OTC_String,int> const& theStruct
			) const;
				// This method takes a single parameter, a
				// struct, containing at least three elements
				// named "moe", "larry" and "curly", all
				// integers. The method adds the three
				// numbers and return the result.


    void		echoStructTest(OTC_ROPayload theArgument) const;
				// This method takes a single parameter, a
				// struct and returns it. Note that the test
				// definition says the argument is a struct
				// but not what the types of the values it
				// contains are. Thus simply echo back the
				// argument as received whatever the type
				// is. That is, even if it isn't a struct
				// it is returned as received. Also note that
				// this method sends an explicit reply by
				// calling the method <returnResponse()>.
				// This is why the return type is given as
				// <void> even though a value is actually
				// returned.

    void		manyTypesTest(
			 int theInteger,
			 bool theBoolean,
			 char const* theString,
			 double theFloat,
			 OTC_Time theDateTime,
			 ONS_Binary theBinary
			) const;
				// This method takes six parameters, and
				// returns an array containing all the
				// parameters. Note that this method sends
				// an explicit reply by calling the method
				// <returnResponse()>. This is why the return
				// type is given as <void> even though a value
				// is actually returned. This is done as it
				// is simpler to construct the result direct
				// into message payload object.

    OTC_String		moderateSizeArrayCheck(
			 OTC_Deque<OTC_String> const& theArray
			) const;
				// This method takes a single parameter,
				// which is an array. Each of the items is a
				// string. The method returns a new string
				// containing the concatenated text of the
				// first and last elements. The test says
				// there will be between 100 and 200 items
				// in the array, so no check made for single
				// element array or empty array.

    int			nestedStructTest(OTC_ROPayload theArgument) const;
				// This method takes a single parameter, a
				// struct, that models a daily calendar.
				// At the top level, there is one struct for
				// each year. Each year is broken down
				// into months, and months into days. Most of
				// the days are empty in the struct you
				// receive, but the entry for April 1, 2000
				// contains a least three elements named "moe",
				// "larry" and "curly", all integers. The
				// method adds up these three numbers and
				// returns the result.

    OTC_PairIterator<OTC_String,int>	simpleStructReturnTest(
			 int theValue
			) const;
				// This method takes one parameter, and
				// returns a struct containing three
				// elements, "times10", "times100" and
				// "times1000", the result of multiplying the
				// number by 10, 100 and 1000. Note that the
				// test definition doesn't say what the type
				// of the argument is, so assume it is an
				// integer.

  private:

  			TST_ValidatorService(TST_ValidatorService const&);
				// Do not define an implementation for this.

    TST_ValidatorService&	operator=(TST_ValidatorService const&);
				// Do not define an implementation for this.
};

/* ------------------------------------------------------------------------- */

#endif /* VALIDATORSERVICE_HH */
