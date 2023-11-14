/*
// ============================================================================
//
// = LIBRARY
//     ONS
// 
// = FILENAME
//     agent/_service.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2003-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <ONS/agent/service.hh>

#include <OTC/dispatch/signal.hh>
#include <OTC/dispatch/dispatch.hh>
#include <OTC/dispatch/svbroker.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/logstrm.hh>

#include <stdlib.h>

/* ------------------------------------------------------------------------- */

class Service1 : public ONS_Service
{
  public:

			Service1(
			 char const* theServiceName,
			 char const* theServiceAudience
			);

    void		initiateTests();

    void		exportedMethod_vr_c(OTCEV_Request*) const;

    void		exportedMethod_vr(OTCEV_Request*);

    void		exportedMethod_v0_c() const;

    void		exportedMethod_v0();

    int			exportedMethod_o0_c() const;

    int			exportedMethod_o0();

    void		exportedMethod_v1_c(int) const;

    void		exportedMethod_v1(int);

    int			exportedMethod_o1_c(int) const;

    int			exportedMethod_o1(int);

    void		exportedMethod_v2_c(int, char const*) const;

    void		exportedMethod_v2(int, char const*);

    int			exportedMethod_o2_c(int, char const*) const;

    int			exportedMethod_o2(int, char const*);

    void		exportedMethod_v3_c(int, char const*,
			 OTC_String const&) const;

    void		exportedMethod_v3(int, char const*,
			 OTC_String const&);

    int			exportedMethod_o3_c(int, char const*,
			 OTC_String const&) const;

    int			exportedMethod_o3(int, char const*,
			 OTC_String const&);

    void		exportedMethod_v4_c(int, char const*,
			 OTC_String const&, bool) const;

    void		exportedMethod_v4(int, char const*,
			 OTC_String const&, bool);

    int			exportedMethod_o4_c(int, char const*,
			 OTC_String const&, bool) const;

    int			exportedMethod_o4(int, char const*,
			 OTC_String const&, bool);

    void		exportedMethod_v5_c(int, char const*,
			 OTC_String const&, bool, OTC_String) const;

    void		exportedMethod_v5(int, char const*,
			 OTC_String const&, bool, OTC_String);

    int			exportedMethod_o5_c(int, char const*,
			 OTC_String const&, bool, OTC_String) const;

    int			exportedMethod_o5(int, char const*,
			 OTC_String const&, bool, OTC_String);

    void		exportedMethod_v6_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&) const;

    void		exportedMethod_v6(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&);

    int			exportedMethod_o6_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&) const;

    int			exportedMethod_o6(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&);

    void		exportedMethod_v7_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&) const;

    void		exportedMethod_v7(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&);

    int			exportedMethod_o7_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&) const;

    int			exportedMethod_o7(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&);

    void		exportedMethod_v8_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&) const;

    void		exportedMethod_v8(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&);

    int			exportedMethod_o8_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&) const;

    int			exportedMethod_o8(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&);

    void		exportedMethod_v9_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&, OTC_Symbol const&) const;

    void		exportedMethod_v9(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&, OTC_Symbol const&);

    int			exportedMethod_o9_c(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&, OTC_Symbol const&) const;

    int			exportedMethod_o9(int, char const*,
			 OTC_String const&, bool, OTC_String,
			 short const&, OTC_Time const&,
			 OTC_Date const&, OTC_Symbol const&);

    void		handleResponse_r_c(OTCEV_Response*) const;

    void		handleResponse_r(OTCEV_Response*);

    void		handleResponse_v0_c() const;

    void		handleResponse_v0();

    void		handleResponse_v1_c(int) const;

    void		handleResponse_v1(int);

    void		handleResponse_v2_c(OTC_String const&, int) const;

    void		handleResponse_v2(OTC_String const&, int);

    void		handleFailure_f_c(OTCEV_Failure*) const;

    void		handleFailure_f(OTCEV_Failure*);

    void		handleFailure_v0_c() const;

    void		handleFailure_v0();

    void		handleFailure_v1_c(OTC_String const&) const;

    void		handleFailure_v1(OTC_String const&);

    void		handleFailure_v5_c(OTC_String const&, int,
			 OTC_String const&, OTC_String const&,
			 OTC_String const&) const;

    void		handleFailure_v5(OTC_String const&, int,
			 OTC_String const&, OTC_String const&,
			 OTC_String const&);

    void		delayedResponse_vr_c(OTCEV_Request*) const;

    void		delayedResponse_vr(OTCEV_Request*);

    void		abortResponse_v0_c() const;

    void		abortResponse_v0();

    void		suspendedResponse_v0_c();

    void		suspendedResponse_v0();

    void		suspendedResponse_o0_c();

    void		suspendedResponse_o0();

    void		suspendedResponse_v1_c();

    void		suspendedResponse_v1();

    void		suspendedResponse_o1_c();

    void		suspendedResponse_o1();

    void		resumedResponse_v0_c() const;

    void		resumedResponse_v0();

    int			resumedResponse_o0_c() const;

    int			resumedResponse_o0();

    void		resumedResponse_v1_c(OTC_String const&) const;

    void		resumedResponse_v1(OTC_String const&);

    int			resumedResponse_o1_c(OTC_String const&) const;

    int			resumedResponse_o1(OTC_String const&);

    void		cancelledResponse_v0(bool);

    void		cancelResponse_v0(OTC_String const&);

    void		cancelAllResponses_v0(OTC_String const&);

    void		unexpectedMessage(OTC_String const&,
			 OTC_String const&, OTC_String const&,
			 OTC_String const&, OTC_String const&, int);

    void		handleMessage_v3_c(OTC_String const&,
			 OTC_String const&, OTC_String const&) const;

    void		handleMessage_v3(OTC_String const&,
			 OTC_String const&, OTC_String const&);

    void		handleMessage_v4_c(OTC_String const&,
			 OTC_String const&, OTC_String const&, int) const;

    void		handleMessage_v4(OTC_String const&,
			 OTC_String const&, OTC_String const&, int);

    void		handleMessage_v6_c(OTC_String const&,
			 OTC_String const&, OTC_String const&,
			 OTC_String const&, OTC_String const&, int) const;

    void		handleMessage_v6(OTC_String const&,
			 OTC_String const&, OTC_String const&,
			 OTC_String const&, OTC_String const&, int);
};

Service1::Service1(
 char const* theServiceName,
 char const* theServiceAudience
)
  : ONS_Service(theServiceName,theServiceAudience)
{
  OTC_Tracer tracer("Service1::Service1()");

  joinGroup("PUBLIC");

  subscribeStartup(&Service1::initiateTests);
}

void Service1::initiateTests()
{
  OTC_Tracer tracer("Service1::initiateTests()");

  OTC_String theConversationId;

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_v5_c,theConversationId);
  cancelAllRequests(1,"CANCEL-ALL");
  cancelAllRequests(2,"CANCEL-ALL");

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_v5,theConversationId);
  cancelAllRequests(1,"CANCEL-ALL");
  cancelAllRequests(2,"CANCEL-ALL");

  sendRequest(serviceBinding(),"METHOD");
  sendRequest(serviceBinding(),"METHOD",1);
  sendRequest(serviceBinding(),"METHOD",1,2);
  sendRequest(serviceBinding(),"METHOD",1,2,3);
  sendRequest(serviceBinding(),"METHOD",1,2,3,4);
  sendRequest(serviceBinding(),"METHOD",1,2,3,4,5);
  sendRequest(serviceBinding(),"METHOD",1,2,3,4,5,6);
  sendRequest(serviceBinding(),"METHOD",1,2,3,4,5,6,7);
  sendRequest(serviceBinding(),"METHOD",1,2,3,4,5,6,7,8);
  sendRequest(serviceBinding(),"METHOD",1,2,3,4,5,6,7,8,9);

  OTC_SVPayload theSVPayload;
  theSVPayload = OTC_ROPayload::nullArray();

  OTC_ROPayload theROPayload = theSVPayload;
  OTC_RWPayload theRWPayload = theSVPayload;

  sendRequest(serviceBinding(),"ROPAYLOAD",theROPayload);
  sendRequest(serviceBinding(),"RWPAYLOAD",theRWPayload);
  sendRequest(serviceBinding(),"SVPAYLOAD",theSVPayload);

  exportMethod(&Service1::exportedMethod_vr_c,"exportedMethod_vr_c");
  exportMethod(&Service1::exportedMethod_vr,"exportedMethod_vr");

  sendRequest(serviceBinding(),"exportedMethod_vr_c");
  sendRequest(serviceBinding(),"exportedMethod_vr_c",1);
  sendRequest(serviceBinding(),"exportedMethod_vr_c",1,2);
  sendRequest(serviceBinding(),"exportedMethod_vr_c",1,2,3);

  sendRequest(serviceBinding(),"exportedMethod_vr");
  sendRequest(serviceBinding(),"exportedMethod_vr",1);
  sendRequest(serviceBinding(),"exportedMethod_vr",1,2);
  sendRequest(serviceBinding(),"exportedMethod_vr",1,2,3);

  exportMethod(&Service1::exportedMethod_v0_c,"exportedMethod_v0_c");
  exportMethod(&Service1::exportedMethod_v0,"exportedMethod_v0");
  exportMethod(&Service1::exportedMethod_o0_c,"exportedMethod_o0_c");
  exportMethod(&Service1::exportedMethod_o0,"exportedMethod_o0");

  sendRequest(serviceBinding(),"exportedMethod_v0_c");
  sendRequest(serviceBinding(),"exportedMethod_v0");
  sendRequest(serviceBinding(),"exportedMethod_o0_c");
  sendRequest(serviceBinding(),"exportedMethod_o0");

  exportMethod(&Service1::exportedMethod_v1_c,"exportedMethod_v1_c");
  exportMethod(&Service1::exportedMethod_v1,"exportedMethod_v1");
  exportMethod(&Service1::exportedMethod_o1_c,"exportedMethod_o1_c");
  exportMethod(&Service1::exportedMethod_o1,"exportedMethod_o1");

  sendRequest(serviceBinding(),"exportedMethod_v1_c",1);
  sendRequest(serviceBinding(),"exportedMethod_v1",1);
  sendRequest(serviceBinding(),"exportedMethod_o1_c",1);
  sendRequest(serviceBinding(),"exportedMethod_o1",1);

  exportMethod(&Service1::exportedMethod_v2_c,"exportedMethod_v2_c");
  exportMethod(&Service1::exportedMethod_v2,"exportedMethod_v2");
  exportMethod(&Service1::exportedMethod_o2_c,"exportedMethod_o2_c");
  exportMethod(&Service1::exportedMethod_o2,"exportedMethod_o2");

  sendRequest(serviceBinding(),"exportedMethod_v2_c",1,"2");
  sendRequest(serviceBinding(),"exportedMethod_v2",1,"2");
  sendRequest(serviceBinding(),"exportedMethod_o2_c",1,"2");
  sendRequest(serviceBinding(),"exportedMethod_o2",1,"2");

  exportMethod(&Service1::exportedMethod_v3_c,"exportedMethod_v3_c");
  exportMethod(&Service1::exportedMethod_v3,"exportedMethod_v3");
  exportMethod(&Service1::exportedMethod_o3_c,"exportedMethod_o3_c");
  exportMethod(&Service1::exportedMethod_o3,"exportedMethod_o3");

  sendRequest(serviceBinding(),"exportedMethod_v3_c",1,"2","3");
  sendRequest(serviceBinding(),"exportedMethod_v3",1,"2","3");
  sendRequest(serviceBinding(),"exportedMethod_o3_c",1,"2","3");
  sendRequest(serviceBinding(),"exportedMethod_o3",1,"2","3");

  exportMethod(&Service1::exportedMethod_v4_c,"exportedMethod_v4_c");
  exportMethod(&Service1::exportedMethod_v4,"exportedMethod_v4");
  exportMethod(&Service1::exportedMethod_o4_c,"exportedMethod_o4_c");
  exportMethod(&Service1::exportedMethod_o4,"exportedMethod_o4");

  sendRequest(serviceBinding(),"exportedMethod_v4_c",1,"2","3",true);
  sendRequest(serviceBinding(),"exportedMethod_v4",1,"2","3",true);
  sendRequest(serviceBinding(),"exportedMethod_o4_c",1,"2","3",true);
  sendRequest(serviceBinding(),"exportedMethod_o4",1,"2","3",true);

  exportMethod(&Service1::exportedMethod_v5_c,"exportedMethod_v5_c");
  exportMethod(&Service1::exportedMethod_v5,"exportedMethod_v5");
  exportMethod(&Service1::exportedMethod_o5_c,"exportedMethod_o5_c");
  exportMethod(&Service1::exportedMethod_o5,"exportedMethod_o5");

  sendRequest(serviceBinding(),"exportedMethod_v5_c",
   1,"2","3",true,"5");
  sendRequest(serviceBinding(),"exportedMethod_v5",
   1,"2","3",true,"5");
  sendRequest(serviceBinding(),"exportedMethod_o5_c",
   1,"2","3",true,"5");
  sendRequest(serviceBinding(),"exportedMethod_o5",
   1,"2","3",true,"5");

  exportMethod(&Service1::exportedMethod_v6_c,"exportedMethod_v6_c");
  exportMethod(&Service1::exportedMethod_v6,"exportedMethod_v6");
  exportMethod(&Service1::exportedMethod_o6_c,"exportedMethod_o6_c");
  exportMethod(&Service1::exportedMethod_o6,"exportedMethod_o6");

  sendRequest(serviceBinding(),"exportedMethod_v6_c",
   1,"2","3",true,"5",6);
  sendRequest(serviceBinding(),"exportedMethod_v6",
   1,"2","3",true,"5",6);
  sendRequest(serviceBinding(),"exportedMethod_o6_c",
   1,"2","3",true,"5",6);
  sendRequest(serviceBinding(),"exportedMethod_o6",
   1,"2","3",true,"5",6);

  exportMethod(&Service1::exportedMethod_v7_c,"exportedMethod_v7_c");
  exportMethod(&Service1::exportedMethod_v7,"exportedMethod_v7");
  exportMethod(&Service1::exportedMethod_o7_c,"exportedMethod_o7_c");
  exportMethod(&Service1::exportedMethod_o7,"exportedMethod_o7");

  OTC_Time theTime;

  sendRequest(serviceBinding(),"exportedMethod_v7_c",
   1,"2","3",true,"5",6,theTime);
  sendRequest(serviceBinding(),"exportedMethod_v7",
   1,"2","3",true,"5",6,theTime);
  sendRequest(serviceBinding(),"exportedMethod_o7_c",
   1,"2","3",true,"5",6,theTime);
  sendRequest(serviceBinding(),"exportedMethod_o7",
   1,"2","3",true,"5",6,theTime);

  exportMethod(&Service1::exportedMethod_v8_c,"exportedMethod_v8_c");
  exportMethod(&Service1::exportedMethod_v8,"exportedMethod_v8");
  exportMethod(&Service1::exportedMethod_o8_c,"exportedMethod_o8_c");
  exportMethod(&Service1::exportedMethod_o8,"exportedMethod_o8");

  OTC_Date theDate;

  sendRequest(serviceBinding(),"exportedMethod_v8_c",
   1,"2","3",true,"5",6,theTime,theDate);
  sendRequest(serviceBinding(),"exportedMethod_v8",
   1,"2","3",true,"5",6,theTime,theDate);
  sendRequest(serviceBinding(),"exportedMethod_o8_c",
   1,"2","3",true,"5",6,theTime,theDate);
  sendRequest(serviceBinding(),"exportedMethod_o8",
   1,"2","3",true,"5",6,theTime,theDate);

  exportMethod(&Service1::exportedMethod_v9_c,"exportedMethod_v9_c");
  exportMethod(&Service1::exportedMethod_v9,"exportedMethod_v9");
  exportMethod(&Service1::exportedMethod_o9_c,"exportedMethod_o9_c");
  exportMethod(&Service1::exportedMethod_o9,"exportedMethod_o9");

  sendRequest(serviceBinding(),"exportedMethod_v9_c",
   1,"2","3",true,"5",6,theTime,theDate,"9");
  sendRequest(serviceBinding(),"exportedMethod_v9",
   1,"2","3",true,"5",6,theTime,theDate,"9");
  sendRequest(serviceBinding(),"exportedMethod_o9_c",
   1,"2","3",true,"5",6,theTime,theDate,"9");
  sendRequest(serviceBinding(),"exportedMethod_o9",
   1,"2","3",true,"5",6,theTime,theDate,"9");

  sendRequest(serviceBinding(),"exportedMethod_v0_c",1);
  sendRequest(serviceBinding(),"exportedMethod_v1_c","one");

  theSVPayload = OTC_ROPayload::nullValue();

  sendRequest(serviceBinding(),"exportedMethod_v0_c",theSVPayload);

  theConversationId = sendRequest(
   serviceBinding(),"exportedMethod_vr_c",1,2,3);

  processResponse(&Service1::handleResponse_r_c,theConversationId);

  theConversationId = sendRequest(
   serviceBinding(),"exportedMethod_vr",1,2,3);

  processResponse(&Service1::handleResponse_r,theConversationId);

  theSVPayload.setName("invalid");

  theConversationId = sendRequest(
   serviceBinding(),"exportedMethod_vr_c",theSVPayload);

  processResponse(&Service1::handleResponse_r_c,theConversationId);
  processFailure(&Service1::handleFailure_f_c,theConversationId);

  theConversationId = sendRequest(
   serviceBinding(),"exportedMethod_vr",theSVPayload);

  processResponse(&Service1::handleResponse_r,theConversationId);
  processFailure(&Service1::handleFailure_f,theConversationId);

  exportMethod(&Service1::delayedResponse_vr_c,"delayedResponse_vr_c");
  exportMethod(&Service1::delayedResponse_vr,"delayedResponse_vr");

  theConversationId = sendRequest(serviceBinding(),"delayedResponse_vr_c");

  processResponse(&Service1::handleResponse_r_c,theConversationId);
  processFailure(&Service1::handleFailure_f_c,theConversationId,1.0);

  theConversationId = sendRequest(serviceBinding(),"delayedResponse_vr");

  processResponse(&Service1::handleResponse_r,theConversationId);
  processFailure(&Service1::handleFailure_f_c,theConversationId,1.0);

  OTC_ServiceBinding* tmpServiceBinding;

  tmpServiceBinding = serviceRegistry()->addServiceBinding(
   "FAKE-SERVICE","","FAKE-RECEIVER")->clone();

  theConversationId = sendRequest(tmpServiceBinding,"DUMMY");

  processResponse(&Service1::handleResponse_r_c,theConversationId);
  processFailure(&Service1::handleFailure_f_c,theConversationId);

  serviceRegistry()->removeServiceBinding(tmpServiceBinding->serviceAddress());

  theConversationId = sendRequest(tmpServiceBinding,"DUMMY");

  processResponse(&Service1::handleResponse_r,theConversationId);
  processFailure(&Service1::handleFailure_f,theConversationId);

  tmpServiceBinding->destroy();

  theConversationId = sendRequest(serviceBinding(),"exportedMethod_v0_c");

  processResponse(&Service1::handleResponse_v0_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"exportedMethod_v0");

  processResponse(&Service1::handleResponse_v0,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"exportedMethod_o0_c");

  processResponse(&Service1::handleResponse_v1_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"exportedMethod_o0");

  processResponse(&Service1::handleResponse_v1,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"exportedMethod_o0_c");

  processResponse(&Service1::handleResponse_v2_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"exportedMethod_o0");

  processResponse(&Service1::handleResponse_v2,theConversationId);

  exportMethod(&Service1::abortResponse_v0_c,"abortResponse_v0_c");
  exportMethod(&Service1::abortResponse_v0,"abortResponse_v0");

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_f_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_f,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_v0_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_v0,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_v1_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_v1,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_v5_c,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_v5,theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_v5_c,theConversationId);
  ignoreResponse(theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_v5,theConversationId);
  ignoreResponse(theConversationId);

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0_c");

  processFailure(&Service1::handleFailure_v5_c,theConversationId);
  cancelRequest(theConversationId,1,"CANCEL");
  cancelRequest(theConversationId,2,"CANCEL");

  theConversationId = sendRequest(serviceBinding(),"abortResponse_v0");

  processFailure(&Service1::handleFailure_v5,theConversationId);
  cancelRequest(theConversationId,1,"CANCEL");
  cancelRequest(theConversationId,2,"CANCEL");

  exportMethod(&Service1::suspendedResponse_v0_c,"suspendedResponse_v0_c");
  exportMethod(&Service1::suspendedResponse_v0,"suspendedResponse_v0");
  exportMethod(&Service1::suspendedResponse_o0_c,"suspendedResponse_o0_c");
  exportMethod(&Service1::suspendedResponse_o0,"suspendedResponse_o0");
  exportMethod(&Service1::suspendedResponse_v1_c,"suspendedResponse_v1_c");
  exportMethod(&Service1::suspendedResponse_v1,"suspendedResponse_v1");
  exportMethod(&Service1::suspendedResponse_o1_c,"suspendedResponse_o1_c");
  exportMethod(&Service1::suspendedResponse_o1,"suspendedResponse_o1");

  sendRequest(serviceBinding(),"suspendedResponse_v0_c");
  sendRequest(serviceBinding(),"suspendedResponse_v0");
  sendRequest(serviceBinding(),"suspendedResponse_o0_c");
  sendRequest(serviceBinding(),"suspendedResponse_o0");
  sendRequest(serviceBinding(),"suspendedResponse_v1_c");
  sendRequest(serviceBinding(),"suspendedResponse_v1");
  sendRequest(serviceBinding(),"suspendedResponse_o1_c");
  sendRequest(serviceBinding(),"suspendedResponse_o1");

  exportMethod(&Service1::cancelledResponse_v0,"cancelledResponse_v0");

  theConversationId = sendRequest(serviceBinding(),
   "cancelledResponse_v0",false);
  processFailure(&Service1::handleFailure_v5,theConversationId);

  theConversationId = sendRequest(serviceBinding(),
   "cancelledResponse_v0",true);
  processFailure(&Service1::handleFailure_v5,theConversationId);

  sendMessage(serviceBinding(),"MESSAGE-ID",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"MESSAGE-ID",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",2);
  sendMessage(serviceBinding(),"MESSAGE-ID",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",3);

  sendMessage(serviceBinding(),"MESSAGE-ID",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  OTC_String tmpSender;
  tmpSender = serviceBinding()->serviceAddress();

  processMessages(&Service1::handleMessage_v3_c,
   tmpSender,"handleMessage_v3_c");

  sendMessage(serviceBinding(),"handleMessage_v3_c",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"handleMessage_v3_c",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  processMessages(&Service1::handleMessage_v3,
   tmpSender,"handleMessage_v3");

  sendMessage(serviceBinding(),"handleMessage_v3",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"handleMessage_v3",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  processMessages(&Service1::handleMessage_v4_c,
   tmpSender,"handleMessage_v4_c");

  sendMessage(serviceBinding(),"handleMessage_v4_c",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"handleMessage_v4_c",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  processMessages(&Service1::handleMessage_v4,
   tmpSender,"handleMessage_v4");

  sendMessage(serviceBinding(),"handleMessage_v4",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"handleMessage_v4",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  processMessages(&Service1::handleMessage_v6_c,
   tmpSender,"handleMessage_v6_c");

  sendMessage(serviceBinding(),"handleMessage_v6_c",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"handleMessage_v6_c",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  processMessages(&Service1::handleMessage_v6,
   tmpSender,"handleMessage_v6");

  sendMessage(serviceBinding(),"handleMessage_v6",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);
  sendMessage(serviceBinding(),"handleMessage_v6",
   "DESCRIPTION","CONTENT","CONTENT-TYPE",1);

  // Reschedule the tests continually.

  startTimer(&Service1::initiateTests,5.0);
}

void Service1::exportedMethod_vr_c(OTCEV_Request* theRequest) const
{
  OTC_Tracer tracer("Service1::exportedMethod_vr_c()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "content = " << theRequest->content() << endl;

  if (theRequest->content().nodeType() != "array")
  {
    abortResponse(ONSLIB_SERVER_REQUEST_DECODING_ERROR,
     ONSLIB_SERVER_REQUEST_DECODING_ERROR_MESSAGE,"netsvc");
  }

  returnResponse(theRequest->content());
}

void Service1::exportedMethod_vr(OTCEV_Request* theRequest)
{
  OTC_Tracer tracer("Service1::exportedMethod_vr()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "content = " << theRequest->content() << endl;

  if (theRequest->content().nodeType() != "array")
  {
    abortResponse(ONSLIB_SERVER_REQUEST_DECODING_ERROR,
     ONSLIB_SERVER_REQUEST_DECODING_ERROR_MESSAGE,"netsvc");
  }

  returnResponse(theRequest->content());
}

void Service1::exportedMethod_v0_c() const
{
  OTC_Tracer tracer("Service1::exportedMethod_v0_c()");
}

void Service1::exportedMethod_v0()
{
  OTC_Tracer tracer("Service1::exportedMethod_v0()");
}

int Service1::exportedMethod_o0_c() const
{
  OTC_Tracer tracer("Service1::exportedMethod_o0_c()");

  return 0;
}

int Service1::exportedMethod_o0()
{
  OTC_Tracer tracer("Service1::exportedMethod_o0()");

  return 0;
}

void Service1::exportedMethod_v1_c(int a1) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v1_c()");

  tracer() << "a1 = " << a1 << endl;
}

void Service1::exportedMethod_v1(int a1)
{
  OTC_Tracer tracer("Service1::exportedMethod_v1()");

  tracer() << "a1 = " << a1 << endl;
}

int Service1::exportedMethod_o1_c(int a1) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o1_c()");

  tracer() << "a1 = " << a1 << endl;

  return 1;
}

int Service1::exportedMethod_o1(int a1)
{
  OTC_Tracer tracer("Service1::exportedMethod_o1()");

  tracer() << "a1 = " << a1 << endl;

  return 1;
}

void Service1::exportedMethod_v2_c(int a1, char const* a2) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v2_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
}

void Service1::exportedMethod_v2(int a1, char const* a2)
{
  OTC_Tracer tracer("Service1::exportedMethod_v2()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
}

int Service1::exportedMethod_o2_c(int a1, char const* a2) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o2_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;

  return 2;
}

int Service1::exportedMethod_o2(int a1, char const* a2)
{
  OTC_Tracer tracer("Service1::exportedMethod_o2()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;

  return 2;
}

void Service1::exportedMethod_v3_c(int a1, char const* a2,
 OTC_String const& a3) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v3_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
}

void Service1::exportedMethod_v3(int a1, char const* a2,
 OTC_String const& a3)
{
  OTC_Tracer tracer("Service1::exportedMethod_v3()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
}

int Service1::exportedMethod_o3_c(int a1, char const* a2,
 OTC_String const& a3) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o3_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;

  return 3;
}

int Service1::exportedMethod_o3(int a1, char const* a2,
 OTC_String const& a3)
{
  OTC_Tracer tracer("Service1::exportedMethod_o3()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;

  return 3;
}

void Service1::exportedMethod_v4_c(int a1, char const* a2,
 OTC_String const& a3, bool a4) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v4_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
}

void Service1::exportedMethod_v4(int a1, char const* a2,
 OTC_String const& a3, bool a4)
{
  OTC_Tracer tracer("Service1::exportedMethod_v4()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
}

int Service1::exportedMethod_o4_c(int a1, char const* a2,
 OTC_String const& a3, bool a4) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o4_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;

  return 4;
}

int Service1::exportedMethod_o4(int a1, char const* a2,
 OTC_String const& a3, bool a4)
{
  OTC_Tracer tracer("Service1::exportedMethod_o4()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;

  return 4;
}

void Service1::exportedMethod_v5_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v5_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
}

void Service1::exportedMethod_v5(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5)
{
  OTC_Tracer tracer("Service1::exportedMethod_v5()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
}

int Service1::exportedMethod_o5_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o5_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;

  return 5;
}

int Service1::exportedMethod_o5(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5)
{
  OTC_Tracer tracer("Service1::exportedMethod_o5()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;

  return 5;
}

void Service1::exportedMethod_v6_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v6_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
}

void Service1::exportedMethod_v6(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6)
{
  OTC_Tracer tracer("Service1::exportedMethod_v6()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
}

int Service1::exportedMethod_o6_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o6_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;

  return 6;
}

int Service1::exportedMethod_o6(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6)
{
  OTC_Tracer tracer("Service1::exportedMethod_o6()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;

  return 6;
}

void Service1::exportedMethod_v7_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v7_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
}

void Service1::exportedMethod_v7(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7)
{
  OTC_Tracer tracer("Service1::exportedMethod_v7()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
}

int Service1::exportedMethod_o7_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o7_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;

  return 7;
}

int Service1::exportedMethod_o7(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7)
{
  OTC_Tracer tracer("Service1::exportedMethod_o7()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;

  return 7;
}

void Service1::exportedMethod_v8_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v8_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;
}

void Service1::exportedMethod_v8(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8)
{
  OTC_Tracer tracer("Service1::exportedMethod_v8()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;
}

int Service1::exportedMethod_o8_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o8_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;

  return 8;
}

int Service1::exportedMethod_o8(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8)
{
  OTC_Tracer tracer("Service1::exportedMethod_o8()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;

  return 8;
}

void Service1::exportedMethod_v9_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8, OTC_Symbol const& a9) const
{
  OTC_Tracer tracer("Service1::exportedMethod_v9_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;
  tracer() << "a9 = " << a9 << endl;
}

void Service1::exportedMethod_v9(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8, OTC_Symbol const& a9)
{
  OTC_Tracer tracer("Service1::exportedMethod_v9()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;
  tracer() << "a9 = " << a9 << endl;
}

int Service1::exportedMethod_o9_c(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8, OTC_Symbol const& a9) const
{
  OTC_Tracer tracer("Service1::exportedMethod_o9_c()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;
  tracer() << "a9 = " << a9 << endl;

  return 9;
}

int Service1::exportedMethod_o9(int a1, char const* a2,
 OTC_String const& a3, bool a4, OTC_String a5, short const& a6,
 OTC_Time const& a7, OTC_Date const& a8, OTC_Symbol const& a9)
{
  OTC_Tracer tracer("Service1::exportedMethod_o9()");

  tracer() << "a1 = " << a1 << endl;
  tracer() << "a2 = " << a2 << endl;
  tracer() << "a3 = " << a3 << endl;
  tracer() << "a4 = " << a4 << endl;
  tracer() << "a5 = " << a5 << endl;
  tracer() << "a6 = " << a6 << endl;
  tracer() << "a7 = " << a7 << endl;
  tracer() << "a8 = " << a8 << endl;
  tracer() << "a9 = " << a9 << endl;

  return 9;
}

void Service1::handleResponse_r_c(OTCEV_Response* theResponse) const
{
  OTC_Tracer tracer("Service1::handleResponse_r_c()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "content = " << theResponse->content() << endl;
}

void Service1::handleResponse_r(OTCEV_Response* theResponse)
{
  OTC_Tracer tracer("Service1::handleResponse_r()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "content = " << theResponse->content() << endl;
}

void Service1::handleResponse_v0_c() const
{
  OTC_Tracer tracer("Service1::handleResponse_v0_c()");

  tracer() << "conversationId = " << conversationId() << endl;
}

void Service1::handleResponse_v0()
{
  OTC_Tracer tracer("Service1::handleResponse_v0()");

  tracer() << "conversationId = " << conversationId() << endl;
}

void Service1::handleResponse_v1_c(int theResult) const
{
  OTC_Tracer tracer("Service1::handleResponse_v1_c()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "result = " << theResult << endl;
}

void Service1::handleResponse_v1(int theResult)
{
  OTC_Tracer tracer("Service1::handleResponse_v1()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "result = " << theResult << endl;
}

void Service1::handleResponse_v2_c(
 OTC_String const& theConversationId,
 int theResult
) const
{
  OTC_Tracer tracer("Service1::handleResponse_v2_c()");

  tracer() << "conversationId = " << theConversationId << endl;
  tracer() << "result = " << theResult << endl;
}

void Service1::handleResponse_v2(
 OTC_String const& theConversationId,
 int theResult
)
{
  OTC_Tracer tracer("Service1::handleResponse_v2()");

  tracer() << "conversationId = " << theConversationId << endl;
  tracer() << "content = " << theResult << endl;
}

void Service1::handleFailure_f_c(OTCEV_Failure* theFailure) const
{
  OTC_Tracer tracer("Service1::handleFailure_f_c()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "error = " << theFailure->error() << endl;
  tracer() << "description = " << theFailure->description() << endl;
  tracer() << "origin = " << theFailure->origin() << endl;
  tracer() << "details = " << theFailure->details() << endl;
}

void Service1::handleFailure_f(OTCEV_Failure* theFailure)
{
  OTC_Tracer tracer("Service1::handleFailure_f()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "error = " << theFailure->error() << endl;
  tracer() << "description = " << theFailure->description() << endl;
  tracer() << "origin = " << theFailure->origin() << endl;
  tracer() << "details = " << theFailure->details() << endl;
}

void Service1::handleFailure_v0_c() const
{
  OTC_Tracer tracer("Service1::handleFailure_v0_c()");

  tracer() << "conversationId = " << conversationId() << endl;
}

void Service1::handleFailure_v0()
{
  OTC_Tracer tracer("Service1::handleFailure_v0()");

  tracer() << "conversationId = " << conversationId() << endl;
}

void Service1::handleFailure_v1_c(OTC_String const& theConversationId) const
{
  OTC_Tracer tracer("Service1::handleFailure_v1_c()");

  tracer() << "conversationId = " << theConversationId << endl;
}

void Service1::handleFailure_v1(OTC_String const& theConversationId)
{
  OTC_Tracer tracer("Service1::handleFailure_v1()");

  tracer() << "conversationId = " << theConversationId << endl;
}

void Service1::handleFailure_v5_c(
 OTC_String const& theConversationId,
 int theError,
 OTC_String const& theDescription,
 OTC_String const& theOrigin,
 OTC_String const& theDetails
) const
{
  OTC_Tracer tracer("Service1::handleFailure_v5_c()");

  tracer() << "conversationId = " << theConversationId << endl;
  tracer() << "error = " << theError << endl;
  tracer() << "description = " << theDescription << endl;
  tracer() << "origin = " << theOrigin << endl;
  tracer() << "details = " << theDetails << endl;
}

void Service1::handleFailure_v5(
 OTC_String const& theConversationId,
 int theError,
 OTC_String const& theDescription,
 OTC_String const& theOrigin,
 OTC_String const& theDetails
)
{
  OTC_Tracer tracer("Service1::handleFailure_v5()");

  tracer() << "conversationId = " << theConversationId << endl;
  tracer() << "error = " << theError << endl;
  tracer() << "description = " << theDescription << endl;
  tracer() << "origin = " << theOrigin << endl;
  tracer() << "details = " << theDetails << endl;
}

void Service1::delayedResponse_vr_c(OTCEV_Request* theRequest) const
{
  OTC_Tracer tracer("Service1::delayedResponse_vr_c()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "content = " << theRequest->content() << endl;

  delayResponse();
}

void Service1::delayedResponse_vr(OTCEV_Request* theRequest)
{
  OTC_Tracer tracer("Service1::delayedResponse_vr()");

  tracer() << "conversationId = " << conversationId() << endl;
  tracer() << "content = " << theRequest->content() << endl;

  delayResponse();
}

void Service1::abortResponse_v0_c() const
{
  OTC_Tracer tracer("Service1::abortResponse_v0_c()");

  tracer() << "conversationId = " << conversationId() << endl;

  abortResponse(1,"DESCRIPTION","ORIGIN","DETAILS");
}

void Service1::abortResponse_v0()
{
  OTC_Tracer tracer("Service1::abortResponse_v0()");

  tracer() << "conversationId = " << conversationId() << endl;

  abortResponse(1,"DESCRIPTION","ORIGIN","DETAILS");
}

void Service1::suspendedResponse_v0_c()
{
  OTC_Tracer tracer("Service1::suspendedResponse_v0_c()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_v0_c);
}

void Service1::suspendedResponse_v0()
{
  OTC_Tracer tracer("Service1::suspendedResponse_v0()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_v0);
}

void Service1::suspendedResponse_o0_c()
{
  OTC_Tracer tracer("Service1::suspendedResponse_o0_c()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_o0_c);
}

void Service1::suspendedResponse_o0()
{
  OTC_Tracer tracer("Service1::suspendedResponse_o0()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_o0);
}

void Service1::suspendedResponse_v1_c()
{
  OTC_Tracer tracer("Service1::suspendedResponse_v1_c()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_v1_c);
}

void Service1::suspendedResponse_v1()
{
  OTC_Tracer tracer("Service1::suspendedResponse_v1()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_v1);
}

void Service1::suspendedResponse_o1_c()
{
  OTC_Tracer tracer("Service1::suspendedResponse_o1_c()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_o1_c);
}

void Service1::suspendedResponse_o1()
{
  OTC_Tracer tracer("Service1::suspendedResponse_o1()");

  scheduleAction(&ONS_Service::resumeResponse,OTCLIB_IDLE_JOB,conversationId());
  suspendResponse(&Service1::resumedResponse_o1);
}

void Service1::resumedResponse_v0_c() const
{
  OTC_Tracer tracer("Service1::resumedResponse_v0_c()");
}

void Service1::resumedResponse_v0()
{
  OTC_Tracer tracer("Service1::resumedResponse_v0()");
}

int Service1::resumedResponse_o0_c() const
{
  OTC_Tracer tracer("Service1::resumedResponse_v0_c()");

  return 1;
}

int Service1::resumedResponse_o0()
{
  OTC_Tracer tracer("Service1::resumedResponse_v0()");

  return 1;
}

void Service1::resumedResponse_v1_c(OTC_String const& theConversationId) const
{
  OTC_Tracer tracer("Service1::resumedResponse_v1_c()");

  tracer() << "conversationId = " << theConversationId << endl;
}

void Service1::resumedResponse_v1(OTC_String const& theConversationId)
{
  OTC_Tracer tracer("Service1::resumedResponse_v1()");

  tracer() << "conversationId = " << theConversationId << endl;
}

int Service1::resumedResponse_o1_c(OTC_String const& theConversationId) const
{
  OTC_Tracer tracer("Service1::resumedResponse_o1_c()");

  tracer() << "conversationId = " << theConversationId << endl;

  return 1;
}

int Service1::resumedResponse_o1(OTC_String const& theConversationId)
{
  OTC_Tracer tracer("Service1::resumedResponse_o1()");

  tracer() << "conversationId = " << theConversationId << endl;

  return 1;
}

void Service1::cancelledResponse_v0(bool theFlag)
{
  OTC_Tracer tracer("Service1::cancelledResponse_v0()");

  tracer() << "flag = " << theFlag << endl;

  if (theFlag == false)
  {
    scheduleAction(&Service1::cancelResponse_v0,OTCLIB_IDLE_JOB,
     conversationId());
  }
  else
  {
    scheduleAction(&Service1::cancelAllResponses_v0,OTCLIB_IDLE_JOB,
     conversationId());
  }

  suspendResponse(&Service1::resumedResponse_v0);
}

void Service1::cancelResponse_v0(OTC_String const& theConversationId)
{
  OTC_Tracer tracer("Service1::cancelResponse_v0()");

  tracer() << "conversationId = " << theConversationId << endl;

  cancelResponse(theConversationId,1,"CANCEL-RESPONSE");
}

void Service1::cancelAllResponses_v0(OTC_String const& theConversationId)
{
  OTC_Tracer tracer("Service1::cancelAllResponses_v0()");

  tracer() << "conversationId = " << theConversationId << endl;

  cancelAllResponses(2,"CANCEL-ALL-RESPONSES");
}

void Service1::unexpectedMessage(
 OTC_String const& theSender,
 OTC_String const& theMessageId,
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType,
 int theSequenceNumber
)
{
  OTC_Tracer tracer("Service1::unexpectedMessage()");

  tracer() << "sender = " << theSender << endl;
  tracer() << "messageId = " << theMessageId << endl;
  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;
  tracer() << "sequenceNumber = " << theSequenceNumber << endl;
}

void Service1::handleMessage_v3_c(
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType
) const
{
  OTC_Tracer tracer("Service1::handleMessage_v3_c()");

  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;

  Service1* theSelf;
  theSelf = (Service1*)this;

  theSelf->discardMessages(currentMessage()->sender(),
   currentMessage()->messageId());
}

void Service1::handleMessage_v3(
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType
)
{
  OTC_Tracer tracer("Service1::handleMessage_v3()");

  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;

  discardMessages(currentMessage()->sender(),
   currentMessage()->messageId());
}

void Service1::handleMessage_v4_c(
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType,
 int theSequenceNumber
) const
{
  OTC_Tracer tracer("Service1::handleMessage_v4_c()");

  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;
  tracer() << "sequenceNumber = " << theSequenceNumber << endl;

  Service1* theSelf;
  theSelf = (Service1*)this;

  theSelf->discardMessages(currentMessage()->sender(),
   currentMessage()->messageId());
}


void Service1::handleMessage_v4(
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType,
 int theSequenceNumber
)
{
  OTC_Tracer tracer("Service1::handleMessage_v4()");

  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;
  tracer() << "sequenceNumber = " << theSequenceNumber << endl;

  discardMessages(currentMessage()->sender(),
   currentMessage()->messageId());
}

void Service1::handleMessage_v6_c(
 OTC_String const& theSender,
 OTC_String const& theMessageId,
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType,
 int theSequenceNumber
) const
{
  OTC_Tracer tracer("Service1::handleMessage_v6_c()");

  tracer() << "sender = " << theSender << endl;
  tracer() << "messageId = " << theMessageId << endl;
  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;
  tracer() << "sequenceNumber = " << theSequenceNumber << endl;

  Service1* theSelf;
  theSelf = (Service1*)this;

  theSelf->discardMessages(theSender,theMessageId);
}

void Service1::handleMessage_v6(
 OTC_String const& theSender,
 OTC_String const& theMessageId,
 OTC_String const& theDescription,
 OTC_String const& theContent,
 OTC_String const& theContentType,
 int theSequenceNumber
)
{
  OTC_Tracer tracer("Service1::handleMessage_v6()");

  tracer() << "sender = " << theSender << endl;
  tracer() << "messageId = " << theMessageId << endl;
  tracer() << "description = " << theDescription << endl;
  tracer() << "content = " << theContent << endl;
  tracer() << "contentType = " << theContentType << endl;
  tracer() << "sequenceNumber = " << theSequenceNumber << endl;

  discardMessages(theSender,theMessageId);
}

void test1()
{
  OTC_Tracer tracer("void test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Service1 service1("SERVICE","*");
  service1.scheduleStartup();

  OTC_Dispatcher::run();
}

class Service2 : public ONS_Service
{
  public:

			Service2(
			 char const* theServiceName,
			 char const* theServiceAudience
			);

    void		initiateTests();

    void		executeTests_1();

    void		executeTests_2();

    void		executeTests_3();

    void		executeTests_4();

    void		restartTests();

    void		handleReport_1c(char const*) const;

    void		handleReport_1(char const*);

    void		handleReport_2c(OTC_String const&) const;

    void		handleReport_2(OTC_String const&);

    void		handleReport_3c(int) const;

    void		handleReport_3(int);

    void		handleSubscription(
			 OTCEV_Subscription* theSubscription
			);

    void		handlePublisherNotification(
			 OTCEV_PublisherNotification* theNotification
			);

    void		unexpectedReport(
			 OTC_String const&,
			 OTC_String const&,
			 OTC_ROPayload const&
			);
};

Service2::Service2(
 char const* theServiceName,
 char const* theServiceAudience
)
  : ONS_Service(theServiceName,theServiceAudience)
{
  OTC_Tracer tracer("Service2::Service2()");

  joinGroup("PUBLIC");

  subscribeStartup(&Service2::initiateTests);
}

void Service2::initiateTests()
{
  OTC_Tracer tracer("Service2::initiateTests()");

  monitorReports(&Service2::handleReport_1,"SERVICE","expected.1");
  monitorReports(&Service2::handleReport_1c,"SERVICE","expected.1c");
  monitorReports(&Service2::handleReport_2,"SERVICE","expected.2");
  monitorReports(&Service2::handleReport_2c,"SERVICE","expected.2c");
  monitorReports(&Service2::handleReport_3,"SERVICE","expected.3");
  monitorReports(&Service2::handleReport_3c,"SERVICE","expected.3c");

  scheduleAction(&Service2::executeTests_1,OTCLIB_IDLE_JOB);
}

void Service2::executeTests_1()
{
  OTC_Tracer tracer("Service2::executeTests_1()");

  sendReport(serviceBinding(),"unexpected","SEND-REPORT");
  sendReport(serviceBinding(),"unexpected",1);
  sendReport(serviceBinding(),"unexpected",1.2);
  sendReport(serviceBinding(),"unexpected",true);

  publishReport("expected.1","PUBLISH-REPORT",0);
  publishReport("expected.1c","PUBLISH-REPORT",0);
  publishReport("expected.2","PUBLISH-REPORT",0);
  publishReport("expected.2c","PUBLISH-REPORT",0);
  publishReport("expected.3",1,0);
  publishReport("expected.3c",1,0);

  scheduleAction(&Service2::executeTests_2,OTCLIB_IDLE_JOB);
}

void Service2::executeTests_2()
{
  OTC_Tracer tracer("Service2::executeTests_2()");

  ignoreReports("SERVICE","expected.1");
  ignoreReports("SERVICE","expected.1c");
  ignoreReports("SERVICE","expected.2");
  ignoreReports("SERVICE","expected.2c");
  ignoreReports("SERVICE","expected.3");
  ignoreReports("SERVICE","expected.3c");

  publishReport("expected.1","PUBLISH-REPORT",0);
  publishReport("expected.1c","PUBLISH-REPORT",0);
  publishReport("expected.2","PUBLISH-REPORT",0);
  publishReport("expected.2c","PUBLISH-REPORT",0);
  publishReport("expected.3",1,0);
  publishReport("expected.3c",1,0);

  subscribeReports("SERVICE","dummy");

  scheduleAction(&Service2::executeTests_3,OTCLIB_IDLE_JOB);
}

void Service2::executeTests_3()
{
  OTC_Tracer tracer("Service2::executeTests_3()");

  acceptReports(&Service2::handleReport_1,"SERVICE","expected.1");
  acceptReports(&Service2::handleReport_1c,"SERVICE","expected.1c");
  acceptReports(&Service2::handleReport_2,"SERVICE","expected.2");
  acceptReports(&Service2::handleReport_2c,"SERVICE","expected.2c");
  acceptReports(&Service2::handleReport_3,"SERVICE","expected.3");
  acceptReports(&Service2::handleReport_3c,"SERVICE","expected.3c");

  sendReport(serviceBinding(),"expected.1","SEND-REPORT");
  sendReport(serviceBinding(),"expected.1c","SEND-REPORT");
  sendReport(serviceBinding(),"expected.2","SEND-REPORT");
  sendReport(serviceBinding(),"expected.2c","SEND-REPORT");
  sendReport(serviceBinding(),"expected.3",2);
  sendReport(serviceBinding(),"expected.3c",2);

  scheduleAction(&Service2::executeTests_4,OTCLIB_IDLE_JOB);
}

void Service2::executeTests_4()
{
  OTC_Tracer tracer("Service2::executeTests_4()");

  ignoreReports("SERVICE","expected.1");
  ignoreReports("SERVICE","expected.1c");
  ignoreReports("SERVICE","expected.2");
  ignoreReports("SERVICE","expected.2c");
  ignoreReports("SERVICE","expected.3");
  ignoreReports("SERVICE","expected.3c");

  sendReport(serviceBinding(),"expected.1","SEND-REPORT");
  sendReport(serviceBinding(),"expected.1c","SEND-REPORT");
  sendReport(serviceBinding(),"expected.2","SEND-REPORT");
  sendReport(serviceBinding(),"expected.2c","SEND-REPORT");
  sendReport(serviceBinding(),"expected.3",2);
  sendReport(serviceBinding(),"expected.3c",2);

  unsubscribeReports("SERVICE","dummy");

  scheduleAction(&Service2::restartTests,OTCLIB_IDLE_JOB);
}

void Service2::restartTests()
{
  OTC_Tracer tracer("Service2::restartTests()");

  // startTimer(&Service2::initiateTests,1.0);
  scheduleAction(&Service2::initiateTests,OTCLIB_IDLE_JOB);
}

void Service2::handleReport_1c(char const* theContent) const
{
  OTC_Tracer tracer("Service2::handleReport_1()");

  tracer() << "content = " << theContent << endl;
}

void Service2::handleReport_1(char const* theContent)
{
  OTC_Tracer tracer("Service2::handleReport_1c()");

  tracer() << "content = " << theContent << endl;
}

void Service2::handleReport_2c(OTC_String const& theContent) const
{
  OTC_Tracer tracer("Service2::handleReport_2()");

  tracer() << "content = " << theContent << endl;
}

void Service2::handleReport_2(OTC_String const& theContent)
{
  OTC_Tracer tracer("Service2::handleReport_2c()");

  tracer() << "content = " << theContent << endl;
}

void Service2::handleReport_3c(int theContent) const
{
  OTC_Tracer tracer("Service2::handleReport_3()");

  tracer() << "content = " << theContent << endl;
}

void Service2::handleReport_3(int theContent)
{
  OTC_Tracer tracer("Service2::handleReport_3c()");

  tracer() << "content = " << theContent << endl;
}

void Service2::handleSubscription(
 OTCEV_Subscription* theSubscription
)
{
  OTC_Tracer tracer("Service2::handleSubscription()");

  OTC_ServiceBinding* thePublisher;
  thePublisher = theSubscription->publisher();

  tracer() << "publisher = " << thePublisher->serviceName() << endl;
  tracer() << "status = " << theSubscription->status() << endl;
  tracer() << "subject = " << theSubscription->subject() << endl;
}

void Service2::handlePublisherNotification(
 OTCEV_PublisherNotification* theNotification
)
{
  OTC_Tracer tracer("Service2::handlePublisherNotification()");

  OTC_ServiceBinding* thePublisher;
  thePublisher = theNotification->publisher();

  OTC_ServiceBinding* theSubscriber;
  theSubscriber = theNotification->subscriber();

  tracer() << "publisher = " << thePublisher->serviceName() << endl;
  tracer() << "subscriber = " << theSubscriber->serviceName() << endl;
  tracer() << "status = " << theNotification->status() << endl;
}

void Service2::unexpectedReport(
 OTC_String const& theServiceName,
 OTC_String const& theSubjectName,
 OTC_ROPayload const& theContent
)
{
  OTC_Tracer tracer("Service2::unexpectedReport()");

  tracer() << "serviceName = " << theServiceName << endl;
  tracer() << "subjectName = " << theSubjectName << endl;
  tracer() << "content = " << theContent << endl;
}

void test2()
{
  OTC_Tracer tracer("void test1()");

  OTC_Dispatcher::initialise();

  OTCEV_Shutdown::monitor(SIGINT);

  Service2 service2("SERVICE","*");
  service2.scheduleStartup();

  OTC_Dispatcher::run();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
