# COPYRIGHT 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import netsvc
import string
import StringIO

from netrpc.common import _escape
import netrpc.soap

# The ZSI module is from Zolera, found at:
#    http://www.zolera.com

SCHEMA_URI = "http://www.dscpl.com.au/schemas/OSE/"

FAULT_DETAIL = """<OSE:ServiceFailure xmlns:OSE="%s">
<OSE:error>%d</OSE:error>
<OSE:description>%s</OSE:description>
<OSE:origin>%s</OSE:origin>
<OSE:details>%s</OSE:details>
</OSE:ServiceFailure>
"""

import ZSI

class RpcServlet(netsvc.HttpServlet,netsvc.Service):

  def __init__(self,session,binding):
    netsvc.HttpServlet.__init__(self,session)
    netsvc.Service.__init__(self,"","",netsvc.SERVICE_HIDDEN)
    self._binding = binding
    self._method = None
    self._content = []
    self._contentLength = 0

  def destroyServlet(self):
    netsvc.HttpServlet.destroyServlet(self)
    netsvc.Service.destroyReferences(self)

  def processRequest(self):
    try:
      if self.requestMethod() != "POST":
	self.sendError(400,"Only POST method supported.")
	return
      if self.contentLength() <= 0:
	self.sendError(400,"Invalid content length.")
	return
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def processContent(self,content):
    try:
      try:
        self._content.append(content)
	self._contentLength = self._contentLength + len(content)
	if self._contentLength < self.contentLength():
	  return
	if self._contentLength > self.contentLength():
	  error = netsvc.SERVER_REQUEST_DECODING_ERROR
	  description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	  origin = "netsvc.soap"
	  details = "Invalid content length."
	  raise netsvc.ServiceFailure(error,description,origin,details)
	content = string.join(self._content,"")

	ps = ZSI.ParsedSoap(content)

	data = ZSI._child_elements(ps.body_root)
        if len(data) == 0:
	  params = []
        else:
	  try:
	    tc = ZSI.TC.Any()
	    params = []
	    for e in data:
	      params.append(tc.parse(e,ps))
	  except ZSI.EvaluateException:
	    raise

	self._method = ps.body_root.localName

      except netsvc.ServiceFailure:
	raise
      except ZSI.EvaluateException:
	raise
      except:
	error = netsvc.SERVER_REQUEST_DECODING_ERROR
	description = netsvc.SERVER_REQUEST_DECODING_ERROR_MESSAGE
	origin = "netsvc.soap"
	details = netsvc.exceptionDescription()
	raise netsvc.ServiceFailure(error,description,origin,details)

      service = self.serviceEndPoint(self._binding)
      id = apply(service.__getattr__(self._method),params)
      self.processResponse(self.processResult,id)
      self.processFailure(self.processResult,id)

    except ZSI.EvaluateException, e:
      fault = ZSI.FaultFromZSIException(e)
      self.sendResult(500,fault.AsSOAP())
    except netsvc.ServiceFailure,exception:
      try:
	detail = FAULT_DETAIL % (SCHEMA_URI,
	    exception.error,_escape(exception.description),
	    _escape(exception.origin),_escape(exception.details))
	fault = ZSI.Fault(ZSI.Fault.Client,
	    "Request Failed",None,detail,None)
	body = fault.AsSOAP()
	self.sendResult(500,body)
      except:
	self.sendError(500,_escape(netsvc.exceptionDetails()))
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def processResult(self,params):
    try:
      if self.failureOccurred():
	exception = self.currentFailure()
        error = exception.error()
        description = exception.description()
        origin = exception.origin()
        details = exception.details()
	raise netsvc.ServiceFailure(error,description,origin,details)

      params = [params]
      reply = StringIO.StringIO()
      ZSI.SoapWriter(reply).serialize(params,
	  ZSI.TC.Any(aslist=1,pname=self._method + 'Response'))

      self.sendResult(200,reply.getvalue())
    except netsvc.ServiceFailure,exception:
      try:
	detail = FAULT_DETAIL % (SCHEMA_URI,
	    exception.error,_escape(exception.description),
	    _escape(exception.origin),_escape(exception.details))

	if exception.origin == "netsvc" and \
	    (exception.error == netsvc.SERVER_METHOD_UNAVAILABLE or \
	    exception.error == netsvc.SERVER_PARAMETERS_INCORRECT or \
	    exception.error == netsvc.SERVER_REQUEST_DECODING_ERROR):
	  fault = ZSI.Fault(ZSI.Fault.Client,
	      "Request Failed",None,detail,None)
	else:
	  fault = ZSI.Fault(ZSI.Fault.Server,
	      "Request Failed",None,detail,None)

	body = fault.AsSOAP()
	self.sendResult(500,body)
      except:
	self.sendError(500,_escape(netsvc.exceptionDetails()))
    except:
      self.sendError(500,_escape(netsvc.exceptionDetails()))

  def sendResult(self,status,body):
    self.sendResponse(status)
    self.sendHeader("Content-Type","text/xml")
    self.sendHeader("Content-Length",str(len(body)))
    self.endHeaders()
    self.sendContent(body)
    self.endContent()


"""
class RpcGateway(netsvc.HttpServer,netsvc.Monitor):

  def __init__(self,selection,type=netsvc.SERVICE_GROUP):
    netsvc.HttpServer.__init__(self)
    netsvc.Monitor.__init__(self)
    self._members = {}
    self._bindings = {}
    self._selection = selection
    self._type = type
    if type == netsvc.SERVICE_REGISTRY:
      self.subscribeRegistry(self.register)
    elif type == netsvc.SERVICE_GROUP:
      self.subscribeServiceGroup(self.register,selection)
    elif type == netsvc.SERVICE_NAME:
      self.subscribeServiceName(self.register,selection)
    elif type == netsvc.SERVICE_ADDRESS:
      self.subscribeServiceAddress(self.register,selection)

  def register(self,binding,group,status):
    if self._type == netsvc.SERVICE_GROUP or group == None:
      serviceName = binding.serviceName()
      agentIdentity = binding.agentIdentity()
      processAddress = binding.processAddress()
      serviceAudience = binding.serviceAudience()
      if self._type != netsvc.SERVICE_REGISTRY or self._selection == "*" \
       or (self._selection == "" and processAddress == "") \
       or (self._selection != "" and self._selection == serviceAudience):
	if status == netsvc.SERVICE_AVAILABLE:
	  if not self._bindings.has_key(agentIdentity):
	    self._bindings[agentIdentity] = binding
	    if self._members.has_key(serviceName):
	      self._members[serviceName].append(agentIdentity)
	    else:
	      self._members[serviceName] = [agentIdentity]
	else:
	  if self._bindings.has_key(agentIdentity):
	    del self._bindings[agentIdentity]
	    members = self._members[serviceName]
	    del members[members.index(agentIdentity)]
	    if len(members) == 0:
	      del self._members[serviceName]

  def servlet(self,session):
    serviceName = session.servletPath()
    if self._type == netsvc.SERVICE_NAME or \
     self._type == netsvc.SERVICE_ADDRESS:
        if serviceName == "" and len(self._members) != 0:
	  agentIdentity = self._members.values()[0][0]
	  binding = self._bindings[agentIdentity]
	  return RpcServlet(session,binding)
    else:
	if self._members.has_key(serviceName):
	  agentIdentity = self._members[serviceName][0]
	  binding = self._bindings[agentIdentity]
	  return RpcServlet(session,binding)
    return netsvc.ErrorServlet(session,404)
"""

class RpcGateway(netsvc.HttpBridge):

  def create(self,session,binding):
    return RpcServlet(session,binding)
