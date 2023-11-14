# COPYRIGHT 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
#
# This Python module provides a client implementation of the XML-RPC
# protocol. The programming interface is the same as the "netrpc"
# Python module provided with OSE. In essence, the only real difference is
# the RPC over HTTP protocol used. Do be aware however, that since the
# goal is compatibility with the "netrpc" module, some things may be a
# little bit different to what you might otherwise expect if using some
# other XML-RPC client. Also note that XML-RPC can't convey everything
# that is supported by the "netrpc" module. These issues are detailed below.
#
# Note that the parser doesn't complain when extra tags are added
# in some places and some tags can be quite out of place and it will
# still work. Too much mucking around to make it more precise. The
# PythonWare "xmlrpclib" doesn't do any better and it also can't handle
# struct member name and value tags being in reverse order whereas this can.
#
# Note that the Python None type will map to an empty string. The Date,
# Time, Duration and complex type, plus any other extended types will
# map to a string with the value being their encoded representation.
# Any type information will be lost in the transfer across the XML-RPC
# protocol. If a long int value is used, the XML-RPC client/server
# at the other end may blow up if the value is larger than the maximum
# allowed 32 bit quantity.
#
# Note that since XML-RPC faults only provide for a fault code and
# fault string, when using this client with OSE, the greater amount of
# error information available can't be transfered cleanly. The origin
# of a failure will be included as a prefix to the fault string followed
# by " -- ", with details of a failure included at the end preceeded
# by a blank line. This client will automaticaly extract this information
# back out of the fault description and present it as separate fields.
# If any other XML-RPC client besides the one here is used in conjunction
# with OSE, you will need to write code to extract the additional
# information. Any convention for the value of fault codes in XML-RPC is
# not adhered to and that used on the server side is used instead. If
# you use this client against some other XML-RPC server, the origin
# and details associated with a failure will always be empty.
#
# Note that the server side convention of encoding date and time is used.
# Ie., it is encoded as "YYYY-MM-DDThh:mm:ss". This is the extended
# date and time format of ISO 8601 and is what is used in the XML Schema
# Datatypes specification. The value will be converted to the appropriate
# XML-RPC format automatically and vice versa when creating requests and
# decoding responses. The client side conventions for Date and Duration
# are also the same as the server side, although in that case XML-RPC
# doesn't support them anyway and they are transfered as strings.
#
# If you are writing a client application which is only going to deal with
# OSE, you would probably be better off using the "netrpc" module. By
# using the "netrpc" module, you will not loose any type information. Also,
# the server side can be exclusively written in C++, where as the XML-RPC
# gateway requires that you be using Python or a mix of Python and C++.

import string
import types
import urllib
import socket
import httplib
import base64
import os

from common import *
from common import _escape
from remote import _XmlParser

# Define decoder/encoder helper classes for the XML-RPC protocol.

class _Encoder:

  _mapping = {
    "xsd:byte": "int",
    "xsd:short": "int",
    "xsd:int": "int",
    "xsd:long": "int",
    "xsd:unsignedByte": "int",
    "xsd:unsignedShort": "int",
    "xsd:unsignedInt": "int",
    "xsd:unsignedLong": "int",
    "xsd:integer": "int",
    "xsd:boolean": "boolean",
    "xsd:string": "string",
    "xsd:float": "double",
    "xsd:double": "double",
    "xsd:real": "double",
    "xsd:dateTime": "dateTime.iso8601",
    "xsd:base64Binary": "base64" }

  def __init__(self,encode=None):
    self._objects = {}
    self._body = []
    self._encode = encode or encodeObject

  def convert(self,object,type):
    self._objects = {}
    self._body = []
    if type == "request":
      self._encodeRequest(object)
    elif type == "response":
      self._encodeResponse(object)
    else:
      self._encodeObject(object)
    result = string.join(self._body,"")
    self._objects = {}
    self._body = []
    return result

  def _track(self,object):
    if object:
      i = id(object)
      if self._objects.has_key(i):
	raise TypeError("recursive data structure")
      self._objects[i] = None

  def _translate(self,type):
    if _Encoder._mapping.has_key(type):
      return _Encoder._mapping[type]
    return "string"

  def _append(self,string):
    self._body.append(string)

  def _encodeRequest(self,params):
    self._track(params)
    for object in params:
      self._append("<param>\n")
      self._encodeObject(object)
      self._append("</param>\n")

  def _encodeResponse(self,object):
    self._append("<param>\n")
    self._encodeObject(object)
    self._append("</param>\n")

  def _encodeObject(self,object,name=None):
    if type(object) == types.TupleType:
      self._encodeArray(object,name)
    elif type(object) == types.ListType:
      self._encodeArray(object,name)
    elif type(object) == types.DictionaryType:
      self._encodeStruct(object,name)
    else:
      self._encodeScalar(object,name)

  def _encodeArray(self,array,name=None):
    self._track(array)
    if name != None:
      self._append("<member>\n")
      self._append("<name>")
      self._append(_escape(name))
      self._append("</name>\n")
    self._append("<value>\n")
    self._append("<array>\n")
    self._append("<data>\n")
    for object in array:
      self._encodeObject(object)
    self._append("</data>\n")
    self._append("</array>\n")
    self._append("</value>\n")
    if name != None:
      self._append("</member>\n")

  def _encodeStruct(self,struct,name=None):
    self._track(struct)
    if name != None:
      self._append("<member>\n")
      self._append("<name>")
      self._append(_escape(name))
      self._append("</name>\n")
    self._append("<value>\n")
    self._append("<struct>\n")
    for key in struct.keys():
      if type(key) != types.StringType:
	raise RuntimeError("dictionary keys must be strings")
      self._encodeObject(struct[key],key)
    self._append("</struct>\n")
    self._append("</value>\n")
    if name != None:
      self._append("</member>\n")

  def _encodeScalar(self,value,name=None):
    if RemoteService._nil and type(value) == types.NoneType:
      if name != None:
	self._append("<member>\n")
	self._append("<name>")
	self._append(_escape(name))
	self._append("</name>\n")
      self._append("<value>")
      self._append("<nil/>")
      self._append("</value>\n")
      if name != None:
	self._append("</member>\n")
    else:
      valueType,string = self._encode(value)
      if valueType == None:
	self._encodeObject(string)
      else:
	if name != None:
	  self._append("<member>\n")
	  self._append("<name>")
	  self._append(_escape(name))
	  self._append("</name>\n")
	valueType = self._translate(valueType)
	if valueType == "base64":
	  string = base64.encodestring(string)
	elif valueType == "dateTime.iso8601":
	  string = _escape("%s%s%s" % (string[0:4],string[5:7],string[8:]))
	else:
	  string = _escape(string)
	self._append("<value>")
	self._append("<"+valueType+">")
	self._append(string)
	self._append("</"+valueType+">")
	self._append("</value>\n")
	if name != None:
	  self._append("</member>\n")


class _Decoder:

  _mapping = {
    "i4": "xsd:int",
    "int": "xsd:int",
    "boolean": "xsd:boolean",
    "string": "xsd:string",
    "double": "xsd:double",
    "dateTime.iso8601": "xsd:dateTime",
    "base64": "xsd:base64Binary" }

  def __init__(self,decode=None):
    self._content = None
    self._stack = []
    self._names = []
    self._marks = []
    self._data = []
    self._value = 0
    self._method = None
    self._encoding = None
    self._parser = _XmlParser(self)
    self._decode = decode or decodeValue

  def result(self):
    self._parser.close()
    self._parser = None
    if self._content == "params":
      if self._method != None:
        return ("request",self._method,self._stack)
      else:
        return ("response","",self._stack[0])
    elif self._content == "fault":
      return ("failure","",self._stack[0])
    else:
      raise RuntimeError("badly formed message")

  def feed(self,data):
    self._parser.feed(data)

  def xml(self, encoding, standalone):
    self._encoding = encoding or "utf-8"

  def start(self,tag,attrs):
    self._data = []
    if tag in ("array","struct"):
      self._value = 0
      self._marks.append(len(self._stack))
    elif tag == "value":
      self._value = 1

  def data(self,content):
    self._data.append(content)

  def end(self,tag):
    if tag == "value":
      if self._value:
	self._stack.append(string.join(self._data,""))
	self._value = 0
    elif self._value == 1:
      if RemoteService._nil and tag == "nil":
	self._stack.append(None)
	self._value = 0
      elif _Decoder._mapping.has_key(tag):
	type = _Decoder._mapping[tag]
	data = string.join(self._data,"")
	if type == "xsd:base64Binary":
	  if data != "":
	    data = base64.decodestring(data)
	elif type == "xsd:dateTime":
	  data = "%s-%s-%s" % (data[0:4],data[4:6],data[6:])
	value = decodeValue(type,data)
	self._stack.append(value)
	self._value = 0
      else:
        raise RuntimeError("unexpected value element tag")
    elif tag == "array":
      mark = self._marks[-1]
      del self._marks[-1]
      self._stack[mark:] = [self._stack[mark:]]
    elif tag == "struct":
      mark = self._marks[-1]
      del self._marks[-1]
      dict = {}
      names = []
      items = self._stack[mark:]
      count = len(items)
      if count != 0:
	names = self._names[-count:]
      for i in range(0,len(items)):
	dict[names[i]] = items[i]
      self._stack[mark:] = [dict]
      if count != 0:
	del self._names[-count:]
    elif tag == "name":
      self._names.append(string.join(self._data,""))
    elif tag in ("params","fault"):
      self._content = tag
    elif tag == "methodName":
      self._method = string.join(self._data,"")
    else:
      pass


# Define client endpoint for XML-RPC requests.

class _EndPoint:

  def __init__(self,url,login=None,password=None,**kw):
    self._encode = kw.get("encode",None)
    self._decode = kw.get("decode",None)
    self._encoder = _Encoder(self._encode)
    self._url = url
    self._login = login
    self._password = password
    self._type,url = urllib.splittype(url)
    self._host,self._path = urllib.splithost(url)
    if not self._path:
      self._path = "/"

  def call(self,method,params):
    try:
      if RemoteService._debug:
	RemoteService._request = ""
	RemoteService._response = ""
      if self._type != "http":
	error = CLIENT_SERVICE_ADDRESS_INVALID
	description = CLIENT_SERVICE_ADDRESS_INVALID_MESSAGE
	origin = "netrpc.xmlrpc"
	details = self._url
	raise AddressInvalid(error,description,origin,details)

      try:
	body = self._encoder.convert(params,"request")
	template = '<?xml version="1.0"?>\n<methodCall>\n' \
	           '<methodName>%s</methodName>\n' \
	           '<params>\n%s</params>\n</methodCall>\n'
	body = template % (_escape(method),body)
      except ServiceFailure:
        raise
      except:
	error = CLIENT_REQUEST_ENCODING_ERROR
	description = CLIENT_REQUEST_ENCODING_ERROR_MESSAGE
	origin = "netrpc.xmlrpc"
	details = exceptionDescription()
	raise ServiceFailure(error,description,origin,details)

      try:
	request = httplib.HTTP(self._host)
	request.putrequest("POST",self._path)
	request.putheader("Host",self._host)
	request.putheader("Connection","Close")
	request.putheader("Content-Type","text/xml")
	request.putheader("Content-Length",str(len(body)))
	request.putheader("User-Agent","OSE/XML-RPC")
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
	  headers = "%s\nContent-Type: text/xml" % headers
	  headers = "%s\nContent-Length: %s" % (headers,str(len(body)))
	  headers = "%s\nUser-Agent: OSE/XML-RPC" % headers
	  RemoteService._request = headers + "\n\n" + body
	request.send(body)
      except socket.error,exception:
	error = CLIENT_SERVICE_UNAVAILABLE
	description = CLIENT_SERVICE_UNAVAILABLE_MESSAGE
	origin = "netrpc.xmlrpc"
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
	origin = "netrpc.xmlrpc"
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
      origin = "netrpc.xmlrpc"
      if not headers.has_key("content-type"):
	details = "The Content-Type header does not exist."
	raise ServiceFailure(error,description,origin,details)
      if headers["content-type"] != "text/xml":
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
	decoder = _Decoder(self._decode)
	actual = 0
	if RemoteService._debug:
	  RemoteService._response = RemoteService._response + "\n"
	while 1:
	  content = response.read(4096)
	  if not content:
	    break
          actual = actual + len(content)
	  if RemoteService._debug:
	    RemoteService._response = RemoteService._response + content
	  decoder.feed(content)
	if expect != actual:
	  error = CLIENT_RESPONSE_HEADERS_INVALID
	  description = CLIENT_RESPONSE_HEADERS_INVALID_MESSAGE
	  origin = "netrpc.xmlrpc"
	  details = "Length of content was %d, expected %d."
	  details = details % (actual,expect)
	  raise ServiceFailure(error,description,origin,details)
	type,method,result = decoder.result()
	response.close()
      except ServiceFailure:
        raise
      except:
	error = CLIENT_RESPONSE_DECODING_ERROR
	description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	origin = "netrpc.xmlrpc"
	details = exceptionDescription()
	raise ServiceFailure(error,description,origin,details)

      if type not in ["response","failure"]:
	error = CLIENT_RESPONSE_DECODING_ERROR
	description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	origin = "netrpc.xmlrpc"
	raise ServiceFailure(error,description,origin)

      if type == "failure":
	try:
	  error = result["faultCode"]
	  description = result["faultString"]
	  origin = ""
	  details = ""
	  lines = string.split(description,'\n')
	  if len(lines) != 0:
	    try:
	      index = string.index(lines[0]," -- ")
	    except:
	      pass
	    else:
	      origin = lines[0][0:index]
	      description = lines[0][index+4:]
	      for index in range(1,len(lines)):
		if lines[index] == "":
		  details = string.join(lines[index+1:],"\n")
		  break
		else:
		  description = "%s\n%s" % (description,lines[index])
	  try:
	    index = string.rindex(description,prelude1)
	    details = description[index+len(prelude1):]
	    description = description[0:index]
	  except:
	    pass
	  try:
	    index = string.rindex(description,prelude2)
	    origin = description[index+len(prelude2):]
	    description = description[0:index]
	  except:
	    pass
	except:
	  error = CLIENT_RESPONSE_DECODING_ERROR
	  description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	  origin = "netrpc.xmlrpc"
	  details = exceptionDescription()
	  raise ServiceFailure(error,description,origin,details)
	raise ServiceFailure(error,description,origin,details)
      else:
	return result

    except ServiceFailure:
      raise

    except:
      error = CLIENT_UNKNOWN_ERROR
      description = CLIENT_UNKNOWN_ERROR_MESSAGE
      origin = "netrpc.xmlrpc"
      details = exceptionDetails()
      raise ServiceFailure(error,description,origin,details)


# Define remote service handle and magic method dispatcher.

class _Method:

  def __init__(self,endpoint,method):
    self._endpoint = endpoint
    self._method = method

  def __call__(self,*args):
    return self._endpoint.call(self._method,args)

  def __getattr__(self,method):
    return _Method(self._endpoint,"%s.%s" % (self._method,method))


class RemoteService:

  _nil = 0
  _debug = 0
  _request = ""
  _response = ""

  def __init__(self,url,login=None,password=None,**kw):
    self._endpoint = apply(_EndPoint,(url,login,password),kw)

  def __getattr__(self,method):
    return _Method(self._endpoint,method)


# Define handler for XML-RPC requests inside CGI script.

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

      if os.environ["CONTENT_TYPE"] != "text/xml":
	self._error(400,"Invalid content type.")
	return

      length = int(os.environ["CONTENT_LENGTH"])
      content = sys.stdin.read(length)

      decoder = _Decoder()
      decoder.feed(content)
      type,method,params = decoder.result()

      if type != "request":
	self._error(400,"Not a valid XML-RPC request.")
	return

      result = self.dispatch(method,params)

      encoder = _Encoder()
      body = encoder.convert(result,"response")
      template = '<?xml version="1.0"?>\n<methodResponse>\n' \
		 '<params>\n%s</params>\n</methodResponse>\n'
      body = template % body

      print "Status: 200"
      print "Content-Type: text/xml"
      print "Content-Length: %d" % len(body)
      print
      sys.stdout.write(body)

    except SystemExit:
      pass

    except ServiceFailure, exception:
      params = {}
      params["faultCode"] = exception.error
      description = exception.description
      if exception.origin != "":
	description = "%s -- %s" % (exception.origin,description)
      if exception.details != "":
	description = "%s\n\n%s" % (description,exception.details)
      params["faultString"] = description
      encoder = _Encoder()
      body = encoder.convert(params,"fault")
      template = '<?xml version="1.0"?>\n<methodResponse>\n' \
		 '<fault>\n%s</fault>\n</methodResponse>\n'
      body = template % body

      print "Status: 200"
      print "Content-Type: text/xml"
      print "Content-Length: %d" % len(body)
      print
      sys.stdout.write(body)

    except:
      self._error(500,_escape(exceptionDetails()))


def asCGI(callback):
  _CGI(callback).execute()
