# COPYRIGHT 2001-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

from common import *
from common import _escape

import string
import base64
import urllib
import httplib
import socket
import os
import re

# Determine what the best XML parser available is.

try:
  import sgmlop

except ImportError:

  try:
    from xml.parsers import expat
    if not hasattr(expat, "ParserCreate"):
        raise ImportError

  except ImportError:

    import xmllib

    class _XmlSlowParser(xmllib.XMLParser):

      def __init__(self,target):
	self.unknown_starttag = target.start
	self.handle_data = target.data
	self.unknown_endtag = target.end
	xmllib.XMLParser.__init__(self)

    _XmlParser = _XmlSlowParser

  else:

    class _XmlExpatParser:

      def __init__(self,target):
	self._parser = parser = expat.ParserCreate(None,None)
	self._target = target
	parser.StartElementHandler = target.start
	parser.EndElementHandler = target.end
	parser.CharacterDataHandler = target.data
	encoding = None
	if not parser.returns_unicode:
	  encoding = "utf-8"
	target.xml(encoding,None)

      def feed(self, data):
	self._parser.Parse(data,0)

      def close(self):
	self._parser.Parse("",1)
	del self._target, self._parser

    _XmlParser = _XmlExpatParser

else:

  class _XmlSgmlopParser:

    def __init__(self, target):
      self.finish_starttag = target.start
      self.finish_endtag = target.end
      self.handle_data = target.data
      self.handle_xml = target.xml
      self.parser = sgmlop.XMLParser()
      self.parser.register(self)
      self.feed = self.parser.feed
      self.entity = { "amp": "&", "gt": ">", "lt": "<",
	  "apos": "'", "quot": '"' }

    def close(self):
      try:
	self.parser.close()
      finally:
	self.parser = self.feed = None

    def handle_proc(self,tag,attr):
      m = re.search("encoding\s*=\s*['\"]([^\"']+)[\"']", attr)
      if m:
	self.handle_xml(m.group(1),1)

    def handle_entityref(self,entity):
      try:
	self.handle_data(self.entity[entity])
      except KeyError:
	self.handle_data("&%s;"% entity)

  _XmlParser = _XmlSgmlopParser


# Define decoder/encoder helper classes for the NET-RPC protocol.

class _Encoder:

  def __init__(self,encode=None):
    self._objects = {}
    self._body = []
    self._encode = encode or encodeObject

  def convert(self,object):
    self._objects = {}
    self._body = []
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

  def _append(self,string):
    self._body.append(string)

  def _encodeObject(self,object,name=None):
    if type(object) == types.TupleType:
      self._encodeArray(object,name)
    elif type(object) == types.ListType:
      self._encodeArray(object,name)
    elif type(object) == types.DictionaryType:
      self._encodeIndex(object,name)
    else:
      self._encodeScalar(object,name)

  def _encodeArray(self,array,name=None):
    self._track(array)
    self._append("<array")
    if name != None:
      name = _escape(name,1)
      self._append(" name=\"%s\">\n" % name)
    else:
      self._append(">\n")
    for object in array:
      self._encodeObject(object)
    self._append("</array>\n")

  def _encodeIndex(self,index,name=None):
    self._track(index)
    self._append("<index")
    if name != None:
      name = _escape(name,1)
      self._append(" name=\"%s\">\n" % name)
    else:
      self._append(">\n")
    for key in index.keys():
      if type(key) != types.StringType:
	raise RuntimeError("dictionary keys must be strings")
      self._encodeObject(index[key],key)
    self._append("</index>\n")

  def _encodeScalar(self,scalar,name=None):
    valueType,string = self._encode(scalar)
    if valueType == None:
      self._encodeObject(string)
    else:
      if valueType == "xsd:base64Binary":
	string = base64.encodestring(string)
      else:
	string = _escape(string)
      self._append("<scalar")
      valueType = _escape(valueType,1)
      if valueType != "":
	if name != None:
	  name = _escape(name,1)
	  self._append(" name=\"%s\" type=\"%s\">" % (name,valueType))
	else:
	  self._append(" type=\"%s\">" % valueType)
      else:
	if name != None:
	  name = _escape(name,1)
	  self._append(" name=\"%s\">" % name)
	else:
	  self._append(">")
      self._append(string)
      self._append("</scalar>\n")


class _Decoder:

  def __init__(self,decode=None):
    self._level = 0
    self._data = []
    self._root = []
    self._last = self._root
    self._stack = [self._root]
    self._type = None
    self._name = None
    self._encoding = None
    self._parser = _XmlParser(self)
    self._decode = decode or decodeValue

  def _fail(self):
    raise RuntimeError("badly formatted message")

  def feed(self,data):
    self._parser.feed(data)

  def result(self):
    self._parser.close()
    self._parser = None
    if len(self._root) != 1:
      self._fail()
    return self._root[0]

  def _push(self,object,name=None):
    if name != None:
      if type(self._last) != types.DictType:
	self._fail()
      else:
	self._last[name] = object
    else:
      if type(self._last) != types.ListType:
	self._fail()
      else:
	self._last.append(object)
    self._last = object
    self._stack.append(object)

  def _pop(self):
    del self._stack[-1]
    self._last = self._stack[-1]

  def xml(self, encoding, standalone):
    self._encoding = encoding or "utf-8"

  def start(self,tag,attrs):
    name = None
    if attrs.has_key("name"):
      name = attrs["name"]
    if tag == "scalar":
      if attrs.has_key("type"):
	self._type = attrs["type"]
      self._name = name
    elif tag == "array":
      self._push([],name)
    elif tag == "index":
      self._push({},name)
    self._data = []
    self._level = self._level + 1

  def data(self,content):
    self._data.append(content)

  def end(self,tag):
    self._level = self._level - 1
    if tag == "scalar":
      if self._type == None:
        self._type = ""
      data = string.join(self._data,"")
      if self._type == "xsd:base64Binary":
        if data != "":
          data = base64.decodestring(data)
      scalar = self._decode(self._type,data)
      self._push(scalar,self._name)
      self._pop()
      self._type = None
      self._data = []
    elif tag == "array":
      self._pop()
    elif tag == "index":
      self._pop()
    else:
      self._fail()


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
	origin = "netrpc"
	details = self._url
	raise AddressInvalid(error,description,origin,details)

      try:
	body = self._encoder.convert(params)
      except ServiceFailure:
        raise
      except:
	error = CLIENT_REQUEST_ENCODING_ERROR
	description = CLIENT_REQUEST_ENCODING_ERROR_MESSAGE
	origin = "netrpc"
	details = exceptionDescription()
	raise ServiceFailure(error,description,origin,details)

      try:
	request = httplib.HTTP(self._host)
	request.putrequest("POST",self._path)
	request.putheader("Host",self._host)
	request.putheader("Connection","Close")
	request.putheader("Content-Type","application/x-rpc")
	request.putheader("Content-Length",str(len(body)))
	request.putheader("User-Agent","OSE/NET-RPC")
	if self._login != None and self._password != None:
	  info = "%s:%s" % (self._login,self._password)
	  info = base64.encodestring(info)
	  info = "Basic %s" % string.strip(info)
	  request.putheader("Authorization",info)
	request.putheader("X-RPC-Version","1.1")
	request.putheader("X-RPC-Content-Type","request")
	request.putheader("X-RPC-Request-Method",method)
	request.endheaders()
	if RemoteService._debug:
	  headers = "POST %s HTTP/1.0" % self._path
	  headers = "%s\nHost: %s" % (headers,self._host)
	  headers = "%s\nConnection: Close" % headers
	  headers = "%s\nContent-Type: application/x-rpc" % headers
	  headers = "%s\nContent-Length: %s" % (headers,str(len(body)))
	  headers = "%s\nUser-Agent: OSE/NET-RPC" % headers
	  headers = "%s\nX-RPC-Version: 1.1" % headers
	  headers = "%s\nX-RPC-Content-Type: request" % headers
	  headers = "%s\nX-RPC-Request-Method: %s" % (headers,method)
	  RemoteService._request = headers + "\n\n" + body
	request.send(body)
      except socket.error,exception:
	error = CLIENT_SERVICE_UNAVAILABLE
	description = CLIENT_SERVICE_UNAVAILABLE_MESSAGE
	origin = "netrpc"
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
	origin = "netrpc"
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
      origin = "netrpc"
      if not headers.has_key("content-type"):
	details = "The Content-Type header does not exist."
	raise ServiceFailure(error,description,origin,details)
      if headers["content-type"] != "application/x-rpc":
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
      if not headers.has_key("x-rpc-content-type"):
	details = "The X-RPC-Content-Type header does not exist."
	raise ServiceFailure(error,description,origin,details)
      if headers["x-rpc-content-type"] not in ["response","failure"]:
	details = "Unexpected value of %s for X-RPC-Content-Type."
	details = details % `headers["x-rpc-content-type"]`
	raise ServiceFailure(error,description,origin,details)
      if not headers.has_key("x-rpc-version"):
	details = "The X-RPC-Version header does not exist."
	raise ServiceFailure(error,description,origin,details)
      if headers["x-rpc-version"] not in ["1.1"]:
	details = "Unexpected value of %s for X-RPC-Version."
	details = details % `headers["x-rpc-version"]`
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
	  origin = "netrpc"
	  details = "Length of content was %d, expected %d."
	  details = details % (actual,expect)
	  raise ServiceFailure(error,description,origin,details)
	result = decoder.result()
	response.close()
      except ServiceFailure:
        raise
      except:
	error = CLIENT_RESPONSE_DECODING_ERROR
	description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	origin = "netrpc"
	details = exceptionDescription()
	raise ServiceFailure(error,description,origin,details)

      if headers["x-rpc-content-type"] == "failure":
	try:
	  error = result["error"]
	  description = result["description"]
	  origin = result["origin"]
	  details = result["details"]
	except:
	  error = CLIENT_RESPONSE_DECODING_ERROR
	  description = CLIENT_RESPONSE_DECODING_ERROR_MESSAGE
	  origin = "netrpc"
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
      origin = "netrpc"
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

  _debug = 0
  _request = ""
  _response = ""

  def __init__(self,url,login=None,password=None,**kw):
    self._endpoint = apply(_EndPoint,(url,login,password),kw)

  def __getattr__(self,method):
    return _Method(self._endpoint,method)


# Define handler for NET-RPC requests inside CGI script.

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

      if os.environ["CONTENT_TYPE"] != "application/x-rpc":
	self._error(400,"Invalid content type.")
	return

      if os.environ["HTTP_X_RPC_VERSION"] not in ["1.1"]:
	self._error(400,"Invalid NET-RPC version.")
	return

      if os.environ["HTTP_X_RPC_CONTENT_TYPE"] != "request":
	self._error(400,"Invalid NET-RPC content type.")
	return

      method = os.environ["HTTP_X_RPC_REQUEST_METHOD"]

      length = int(os.environ["CONTENT_LENGTH"])
      content = sys.stdin.read(length)

      decoder = _Decoder()
      decoder.feed(content)
      params = decoder.result()

      result = self.dispatch(method,params)

      encoder = _Encoder()
      body = encoder.convert(result)

      print "Status: 200"
      print "Content-Type: application/x-rpc"
      print "X-RPC-Version: 1.1"
      print "X-RPC-Content-Type: response"
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

      encoder = _Encoder()
      body = encoder.convert(result)

      print "Status: 200"
      print "Content-Type: application/x-rpc"
      print "X-RPC-Version: 1.1"
      print "X-RPC-Content-Type: failure"
      print "Content-Length: %d" % len(body)
      print
      sys.stdout.write(body)

    except:
      self._error(500,_escape(exceptionDetails()))
