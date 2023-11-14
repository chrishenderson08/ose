# COPYRIGHT 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

from common import *
from common import _escape

import types
import string
import urllib
import httplib
import socket
import StringIO
import ZSI
import ZSI.TC
import os


# Register typecode objects for ZSI.

class _TCBoolean(ZSI.TC.Boolean):

  parselist = ZSI.TC.Boolean.parselist
  seriallist = [ Boolean ]

  def parse(self,elt,ps):
    val = ZSI.TC.Boolean.parse(self,elt,ps)
    return Boolean(val)


class _TCBinary(ZSI.TC.Base64String):

  parselist = ZSI.TC.Base64String.parselist
  seriallist = [ Binary ]

  def parse(self,elt,ps):
    val = ZSI.TC.Base64String.parse(self,elt,ps)
    return Binary(val)

  def serialize(self,sw,pyobj,**kw):
    ZSI.TC.Base64String.serialize(self,sw,str(pyobj),**kw)


class _TCDuration(ZSI.TC.Duration):

  parselist = ZSI.TC.Duration.parselist
  seriallist = [ Duration ]

  def parse(self,elt,ps):
    return Duration(ZSI.TC.Duration.parse(self,elt,ps))

  def serialize(self,sw,pyobj,**kw):
    ZSI.TC.Duration.serialize(self,sw,pyobj.value,**kw)


class _TCDate(ZSI.TC.gDate):

  parselist = ZSI.TC.gDate.parselist
  seriallist = [ Date ]

  def parse(self,elt,ps):
    return Date(ZSI.TC.gDate.parse(self,elt,ps))

  def serialize(self,sw,pyobj,**kw):
    ZSI.TC.gDate.serialize(self,sw,pyobj.value,**kw)


class _TCTime(ZSI.TC.gTime):

  parselist = ZSI.TC.gTime.parselist
  seriallist = [ Time ]

  def parse(self,elt,ps):
    return Time(ZSI.TC.gTime.parse(self,elt,ps))

  def serialize(self,sw,pyobj,**kw):
    ZSI.TC.gTime.serialize(self,sw,pyobj.value,**kw)


class _TCDateTime(ZSI.TC.gDateTime):

  parselist = ZSI.TC.gDateTime.parselist
  seriallist = [ DateTime ]

  def parse(self,elt,ps):
    return DateTime(ZSI.TC.gDateTime.parse(self,elt,ps))

  def serialize(self,sw,pyobj,**kw):
    ZSI.TC.gDateTime.serialize(self,sw,pyobj.value,**kw)

ZSI.TC.RegisterType(_TCBoolean,clobber=1)
ZSI.TC.RegisterType(_TCBinary,clobber=1)
ZSI.TC.RegisterType(_TCDuration,clobber=1)
ZSI.TC.RegisterType(_TCDate,clobber=1)
ZSI.TC.RegisterType(_TCTime,clobber=1)
ZSI.TC.RegisterType(_TCDateTime,clobber=1)


# Endpoint which handles the call.

class _EndPoint:

  def __init__(self,url,login=None,password=None,**kw):
    self._url = url
    self._login = login
    self._password = password
    self._type,url = urllib.splittype(url)
    self._host,self._path = urllib.splithost(url)
    self._ns = kw.get("ns",None)
    self._soapaction = kw.get("soapaction",None)
    if not self._path:
      self._path = "/"

  def call(self,method,params,kw):
    try:
      if RemoteService._debug:
	RemoteService._request = ""
	RemoteService._response = ""
      if self._type != "http":
	error = CLIENT_SERVICE_ADDRESS_INVALID
	description = CLIENT_SERVICE_ADDRESS_INVALID_MESSAGE
	origin = "netrpc.soap"
	details = self._url
	raise AddressInvalid(error,description,origin,details)

      try:
	ns = kw.get("ns",self._ns)
	soapaction = kw.get("soapaction",self._soapaction)
	if soapaction == None:
	  soapaction = '""'
	d = {}
	if ns:
	  d[''] = ns
	s = StringIO.StringIO()
        sw = ZSI.SoapWriter(s,nsdict=d)
        #sw.serialize(params,ZSI.TC.Any(method))
        sw.serialize(params,ZSI.TC.Any(method,aslist=1))
        sw.close()
        body = s.getvalue()
      except ServiceFailure:
        raise
      except:
	error = CLIENT_REQUEST_ENCODING_ERROR
	description = CLIENT_REQUEST_ENCODING_ERROR_MESSAGE
	origin = "netrpc.soap"
	details = exceptionDescription()
	raise ServiceFailure(error,description,origin,details)

      try:
	request = httplib.HTTP(self._host)
	request.putrequest("POST",self._path)
	request.putheader("Host",self._host)
	request.putheader("Connection","Close")
	request.putheader("Content-Type","text/xml; charset=utf-8")
	request.putheader("Content-Length",str(len(body)))
	request.putheader("User-Agent","OSE/SOAP(ZSI)")
	request.putheader("SOAPAction",soapaction)
	if self._login != None and self._password != None:
	  info = "%s:%s" % (self._login,self._password)
	  info = base64.encodestring(info)
	  info = "Basic %s" % string.strip(info)
	  request.putheader("Authorization",info)
	request.endheaders()
	if RemoteService._debug:
	  headers = "POST %s HTTP/1.0" % self._path
	  headers = "%s\nHost: %s" % (headers,self._host)
	  headers = "%s\nConnection: Close" % headers
	  headers = "%s\nContent-Type: text/xml; charset=utf-8" % headers
	  headers = "%s\nContent-Length: %s" % (headers,str(len(body)))
	  headers = "%s\nUser-Agent: OSE/SOAP(ZSI)" % headers
	  headers = "%s\nSOAPAction: %s" % (headers,soapaction)
	  RemoteService._request = headers + "\n\n" + body
	request.send(body)
      except socket.error,exception:
	error = CLIENT_SERVICE_UNAVAILABLE
	description = CLIENT_SERVICE_UNAVAILABLE_MESSAGE
	origin = "netrpc.soap"
	details = exceptionDescription()
	raise ServiceUnavailable(error,description,origin,details)

      error,description,headers = request.getreply()
      if RemoteService._debug:
	RemoteService._response = "HTTP/1.0 %d %s\r\n" % (error,description)
        for line in headers.headers:
	  RemoteService._response = RemoteService._response + line;
	RemoteService._response = string.replace(
	    RemoteService._response,"\r\n","\n")
      if error != 200:
	if error == 500:
	  if headers.has_key("content-type") and \
	      string.split(headers["content-type"],';')[0] == "text/xml":
	    error = 200

	if error != 200:
	  origin = "netrpc.soap"
	  response = request.getfile()

	  # This alone was loosing data
	  # under Mac OS for some reason.
	  #details = response.read()

	  details = ""
	  content = []
	  while 1:
	    try:
	      # Large read was still loosing
	      # data under Mac OS. Small read
	      # works okay though. Strange!!!
	      data = response.read(64)
	      if data:
		content.append(data)
	      else:
		break
	    except:
	      break
	  details = string.join(content,'')

	  if RemoteService._debug:
	    RemoteService._response = RemoteService._response + "\n" + details
	  response.close()
	  if error == 401:
	    raise AuthenticationFailure(error,description,origin,details)
	  elif error == 404:
	    raise ServiceUnavailable(error,description,origin,details)
	  else:
	    raise TransportFailure(error,description,origin,details)

      error = CLIENT_RESPONSE_HEADERS_INVALID
      description = CLIENT_RESPONSE_HEADERS_INVALID_MESSAGE
      origin = "netrpc.soap"
      if not headers.has_key("content-type"):
	details = "The Content-Type header does not exist."
	raise ServiceFailure(error,description,origin,details)
      if string.split(headers["content-type"],';')[0] != "text/xml":
	details = "Unexpected value of %s for Content-Type."
	details = details % `headers["content-type"]`
	raise ServiceFailure(error,description,origin,details)
      if not headers.has_key("content-length"):
	details = "The Content-Length header does not exist."
	raise ServiceFailure(error,description,origin,details)
      try:
	expect = int(headers["content-length"])
      except:
	details = "Content length not an integer, but %s."
	details = details % `headers["content-length"]`
	raise ServiceFailure(error,description,origin,details)

      try:
	response = request.getfile()
	actual = 0
	data = []
	if RemoteService._debug:
	  RemoteService._response = RemoteService._response + "\n"
	while 1:
	  content = response.read(4096)
	  if not content:
	    break
	  data.append(content)
          actual = actual + len(content)
	  if RemoteService._debug:
	    RemoteService._response = RemoteService._response + content
	if expect != actual:
	  error = CLIENT_RESPONSE_HEADERS_INVALID
	  description = CLIENT_RESPONSE_HEADERS_INVALID_MESSAGE
	  origin = "netrpc.soap"
	  details = "Length of content was %d, expected %d."
	  details = details % (actual,expect)
	  raise ServiceFailure(error,description,origin,details)
	data = string.join(data,"")
	result = ZSI.ParsedSoap(data)
      except ServiceFailure:
        raise
      except:
	error = CLIENT_RESPONSE_DECODING_ERROR
	description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	origin = "netrpc.soap"
	details = exceptionDescription()
	raise ServiceFailure(error,description,origin,details)

      if result.IsAFault():
	try:
	  fault = ZSI.FaultFromFaultMessage(result)
          error = SERVER_UNKNOWN_ERROR
          description = fault.string
          origin = "netrpc.soap"
          details = ""
          if type(fault.detail) in ZSI._seqtypes:
	    if len(fault.detail) != 0:
	      elt = fault.detail[0]
              if elt.nodeName == "OSE:ServiceFailure":
		tc = ZSI.TC.Struct(None,[ZSI.TC.Integer('error'),
                    ZSI.TC.String('description'),ZSI.TC.String('origin'),
		    ZSI.TC.String('details')],'ServiceFailure')
                detail = tc.parse(elt,result)
		error = detail['error']
		description = detail['description']
		origin = detail['origin']
		details = detail['details']
              elif elt.nodeName == "ZSI:FaultDetail":
		tc = ZSI.TC.Struct(None,[ZSI.TC.String('string'),
                    ZSI.TC.String('trace')],'FaultDetail')
                detail = tc.parse(elt,result)
                details = detail['string'] + '\n\n' + detail['trace']
              elif elt.nodeName == "ZSI:ParseFaultDetail":
		tc = ZSI.TC.Struct(None,[ZSI.TC.String('string'),
                    ZSI.TC.String('trace')],'FaultDetail')
                detail = tc.parse(elt,result)
                details = detail['string'] + '\n\n' + detail['trace']
              else:
		details = fault.detail
          elif type(fault.detail) in ZSI._stringtypes:
	    details = fault.detail
        except:
	  error = CLIENT_RESPONSE_DECODING_ERROR
          description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
          origin = "netrpc.soap"
          details = exceptionDescription()
          raise ServiceFailure(error,description,origin,details)
        raise ServiceFailure(error,description,origin,details)

      tc = ZSI.TC.Any()
      data = ZSI._child_elements(result.body_root)
      if len(data) == 0:
	return None
      return tc.parse(data[0],result)

    except ServiceFailure:
      raise

    except:
      error = CLIENT_UNKNOWN_ERROR
      description = CLIENT_UNKNOWN_ERROR_MESSAGE
      origin = "netrpc.soap"
      details = exceptionDetails()
      raise ServiceFailure(error,description,origin,details)


# Define remote service handle and magic method dispatcher.

class _Method:

  def __init__(self,endpoint,method):
    self._endpoint = endpoint
    self._method = method

  def __call__(self,*args,**kw):
    return self._endpoint.call(self._method,args,kw)

  def __getattr__(self,method):
    return _Method(self._endpoint,"%s.%s" % (self._method,method))


class RemoteService:

  _debug = 0
  _request = ""
  _response = ""

  def __init__(self,url,login=None,password=None,**kw):
    self._endpoint = apply(_EndPoint,(url,login,password),kw)

  def __getattr__(self,method):
    return _Method(self._endpoint,method)


# Define handler for SOAP requests inside CGI script.

SCHEMA_URI = "http://www.dscpl.com.au/schemas/OSE/"
 
FAULT_DETAIL = """<OSE:ServiceFailure xmlns:OSE="%s">
<OSE:error>%d</OSE:error>
<OSE:description>%s</OSE:description>
<OSE:origin>%s</OSE:origin>
<OSE:details>%s</OSE:details>
</OSE:ServiceFailure>
"""

class _CGI:

  import BaseHTTPServer
  _responses = BaseHTTPServer.BaseHTTPRequestHandler.responses
  _format = BaseHTTPServer.DEFAULT_ERROR_MESSAGE

  def __init__(self,callback=None):
    self._callback = callback

  def _error(self,code,message=None):
    try:
      short,long = _CGI._responses[code]
    except KeyError:
      short,long = "???","???"
    if not message:
      message = short
    explain = long

    print "Status: %d" % code
    print "Content-Type: text/html"
    print
    print _CGI._format % \
	{"code": code, "message": message, "explain": explain}

  def dispatch(self,method,params):
    if self._callback:
      return self._callback(method,params)

  def execute(self):
    try:
      if os.environ["REQUEST_METHOD"] != "POST":
	self._error(400,"Only POST method supported.")
	return

      ct = string.split(os.environ.get("CONTENT_TYPE","text/xml"),';')[0]
      if ct != "text/xml":
	self._error(400,"Invalid content type.")
	return

      length = int(os.environ["CONTENT_LENGTH"])
      content = sys.stdin.read(length)

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
 
      method = ps.body_root.localName

      result = self.dispatch(method,params)

      result = [result]
      reply = StringIO.StringIO()
      ZSI.SoapWriter(reply).serialize(result,
          ZSI.TC.Any(aslist=1,pname=method + 'Response'))

      body = reply.getvalue()

      print "Status: 200"
      print "Content-Type: text/xml"
      print "Content-Length: %d" % len(body)
      print
      sys.stdout.write(body)

    except SystemExit:
      pass

    except ServiceFailure, exception:
      result = {}
      result["error"] = exception.error
      result["description"] = exception.description
      result["origin"] = exception.origin
      result["details"] = exception.details

      detail = FAULT_DETAIL % (SCHEMA_URI,
	  exception.error,_escape(exception.description),
	  _escape(exception.origin),_escape(exception.details))

      if exception.origin == "netsvc" and \
	  (exception.error == netrpc.SERVER_METHOD_UNAVAILABLE or \
	  exception.error == netrpc.SERVER_PARAMETERS_INCORRECT or \
	  exception.error == netrpc.SERVER_REQUEST_DECODING_ERROR):
	fault = ZSI.Fault(ZSI.Fault.Client,
	    "Request Failed",None,detail,None)
      else:
	fault = ZSI.Fault(ZSI.Fault.Server,
	    "Request Failed",None,detail,None)

      body = fault.AsSOAP()

      print "Status: 500"
      print "Content-Type: text/xml"
      print "Content-Length: %d" % len(body)
      print
      sys.stdout.write(body)

    except:
      self._error(500,_escape(exceptionDetails()))


def asCGI(callback):
  _CGI(callback).execute()
