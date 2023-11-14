# COPYRIGHT 2001-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED

import string
import types
import base64
import traceback
import operator
import time
import sys
import re

# Define HTML escape routine to avoid dragging
# in the whole of the cgi module which is slow.

def _escape(s, quote=None):
  s = string.replace(s,"&","&amp;")
  s = string.replace(s,"<","&lt;")
  s = string.replace(s,">","&gt;",)
  if quote:
    s = string.replace(s,'"',"&quot;")
  return s


# ISO 8601 date/time parsing and formatting routines. Note, support only
# the extended date/time format as presented by ISO 8601, except that
# truncated components not allowed. Also ignore fractional seconds and
# timezone information for now.

_year = r'(?P<Y>\d\d\d\d+)'
_month = r'(?P<M>\d\d)'
_day = r'(?P<D>\d\d)'
_hour = r'(?P<h>\d\d)'
_minute = r'(?P<m>\d\d)'
_second = r'(?P<s>\d\d(?:\.\d+)?)'
_sign = r'(?P<sign>[-+])'
_zone = r'(?P<neg>[+-]\d\d:\d\d|Z)'

_date = _sign + '?' + _year + '-' + _month + '-' + _day
_time = _hour + ':' + _minute + ':' + _second
_datetime = _date + 'T' + _time

_duration = r'(?P<neg>-?)P' r'((?P<Y>\d+)Y)?' r'((?P<M>\d+)M)?' \
    r'((?P<D>\d+)D)?' r'(?P<T>T?)' r'((?P<h>\d+)H)?' r'((?P<m>\d+)M)?' \
    r'((?P<s>\d*(\.\d+)?)S)?'

_date_re = re.compile('^' + _date + '(?:' + _zone + ')?' + '$')
_time_re = re.compile('^' + _time + '(?:' + _zone + ')?' + '$')
_datetime_re = re.compile('^' + _datetime + '(?:' + _zone + ')?' + '$')
_duration_re = re.compile('^' + _duration + '$')


# Function which returns a detailed description of the current Python
# exception. This function should only be called from within the context
# of a Python "except" clause.

def exceptionDescription():
  info = sys.exc_info()[:2]
  exc_type = info[0]
  exc_value = info[1]
  return str(exc_type) + ": " + str(exc_value)

def exceptionDetails(skip=0):
  info = sys.exc_info()
  exc_type = info[0]
  exc_value = info[1]
  exc_traceback = info[2]
  trace = traceback.extract_tb(exc_traceback)
  trace = trace[skip:]
  error = "Python Exception\n"
  try:
    where = "Module: " + str(callback.im_class.__module__) + "\n"
    where = where + "Method: " + str(callback.im_class.__name__)
    where = where + "." + str(callback.__name__) + "\n"
  except:
    try:
      where = "Function: " + str(callback.__name__) + "\n"
    except:
      where = ""
  error = error + where
  error = error + "Traceback (innermost last):\n"
  for filename,lineno,function,message in trace:
    error = error + "  File \"" + filename +  "\""
    error = error + ", line " + str(lineno)
    error = error + ", in " + function + "\n"
    if message:
      error = error + "    " + message + "\n"
  error = error + str(exc_type) + ": " + str(exc_value)
  return error


# Define some high level error types related to sending requests.

CLIENT_UNKNOWN_ERROR = 40
CLIENT_RESOURCE_UNAVAILABLE = 41
CLIENT_REQUEST_ENCODING_ERROR = 42
CLIENT_RESPONSE_HEADERS_INVALID = 43
CLIENT_RESPONSE_DECODING_ERROR = 44
CLIENT_SERVICE_ADDRESS_INVALID = 45
CLIENT_SERVICE_UNAVAILABLE = 46
CLIENT_REQUEST_TIMEOUT = 47
CLIENT_REQUEST_ABORTED = 48

CLIENT_UNKNOWN_ERROR_MESSAGE = "Unknown Error"
CLIENT_RESOURCE_UNAVAILABLE_MESSAGE = "Resource Unavailable"
CLIENT_REQUEST_ENCODING_ERROR_MESSAGE = "Request Encoding Error"
CLIENT_RESPONSE_HEADERS_INVALID_MESSAGE = "Response Headers Invalid"
CLIENT_RESPONSE_DECODING_ERROR_MESSAGE = "Response Decoding Error"
CLIENT_SERVICE_ADDRESS_INVALID_MESSAGE = "Service Address Invalid"
CLIENT_SERVICE_UNAVAILABLE_MESSAGE = "Service Unavailable"
CLIENT_REQUEST_TIMEOUT_MESSAGE = "Request Timeout"
CLIENT_REQUEST_ABORTED_MESSAGE = "Request Aborted"

# For backwards compatibility only.
CLIENT_SERVER_ADDRESS_INVALID = 45
CLIENT_SERVER_UNAVAILABLE = 46
CLIENT_SERVER_ADDRESS_INVALID_MESSAGE = "Service Address Invalid"
CLIENT_SERVER_UNAVAILABLE_MESSAGE = "Service Unavailable"

SERVER_UNKNOWN_ERROR = 50
SERVER_RESOURCE_UNAVAILABLE = 51
SERVER_RESPONSE_ENCODING_ERROR = 52
SERVER_REQUEST_HEADERS_INVALID = 53
SERVER_REQUEST_DECODING_ERROR = 54
SERVER_METHOD_UNAVAILABLE = 55
SERVER_PARAMETERS_INCORRECT = 56
SERVER_APPLICATION_ERROR = 57
SERVER_RESPONSE_ABORTED = 58

SERVER_UNKNOWN_ERROR_MESSAGE = "Unknown Error"
SERVER_RESOURCE_UNAVAILABLE_MESSAGE = "Resource Unavailable"
SERVER_RESPONSE_ENCODING_ERROR_MESSAGE = "Response Encoding Error"
SERVER_REQUEST_HEADERS_INVALID_MESSAGE = "Request Headers Invalid"
SERVER_REQUEST_DECODING_ERROR_MESSAGE = "Request Decoding Error"
SERVER_METHOD_UNAVAILABLE_MESSAGE = "Method Unavailable"
SERVER_PARAMETERS_INCORRECT_MESSAGE = "Parameters Incorrect"
SERVER_APPLICATION_ERROR_MESSAGE = "Application Error"
SERVER_RESPONSE_ABORTED_MESSAGE = "Response Aborted"


class ServiceFailure(Exception):

  def __init__(self,error,description,origin="",details=""):
    self.error = error
    self.description = description
    self.origin = origin
    self.details = details

  def __repr__(self):
    return ("<ServiceFailure %s, description=%s, origin=%s, details=%s>" %
        (self.error,`self.description`,`self.origin`,`self.details`))

  __str__ = __repr__


class AddressInvalid(ServiceFailure):

  def __init__(self,error,description,origin="",details=""):
    ServiceFailure.__init__(self,error,description,origin,details)


class ServiceUnavailable(ServiceFailure):

  def __init__(self,error,description,origin="",details=""):
    ServiceFailure.__init__(self,error,description,origin,details)


class TransportFailure(ServiceFailure):

  def __init__(self,error,description,origin="",details=""):
    ServiceFailure.__init__(self,error,description,origin,details)


class AuthenticationFailure(ServiceFailure):

  def __init__(self,error,description,origin="",details=""):
    ServiceFailure.__init__(self,error,description,origin,details)


# Define classes for the standard scalar value types which are not
# supported natively to Python. An Opaque type is also defined to
# cater for value types when no class has been specifically defined
# for a value type.
#
# Note that the class for DateTime always expects a date/time value
# string to be in the format "YYYY-MM-DDThh:mm:ss". This is classified
# as the extended date/time format in ISO 8601 and is also the format
# used in the XML Schema Data Types specification. Similarly, the
# Date and Duration classes also adhere the XML Schema Data Types
# specification. Where the transport being used dictates some other
# format, a conversion will done at the point a message is sent or
# received.

try:

  Boolean = bool
  False = False
  True = True

except:

  class Boolean:

    def __init__(self,value=0):
      self.value = operator.truth(value)

    def __cmp__(self,other):
      if isinstance(other,Boolean):
	other = other.value
      return cmp(self.value,other)

    def __repr__(self):
      if self.value:
	return "True"
      else:
	return "False"

    def __int__(self):
      return self.value

    def __nonzero__(self):
      return self.value

  False = Boolean(0)
  True = Boolean(1)


class Binary:

  def __init__(self,data=None):
    self.data = data

  def __cmp__(self,other):
    if isinstance(other,Binary):
      other = other.data
    return cmp(self.data,other)

  def __str__(self):
    return self.data

  def __repr__(self):
    return "Binary(%s)" % `self.data`


class Date:

  def __init__(self,value=None):
    if value == None:
      value = time.time()
    t = type(value)
    if t is types.StringType:
      value = string.strip(value)
      result = _date_re.match(value)
      if not result:
	raise ValueError("wrong format, use YYYY-MM-DD")
      atoi = string.atoi
      year,month,day = map(string.atoi,result.groups()[1:4])
      sign = result.groups()[0]
      if sign == '-':
	year = -year
    elif t is types.IntType or t is types.LongType:
      value = time.localtime(value)
      year,month,day = value[0:3]
    elif t is types.FloatType:
      value = time.localtime(value)
      year,month,day = value[0:3]
    else:
      year,month,day = value[0:3]
    self.value = 9 * [0]
    self.value[0:3] = [year,month,day]

  def __cmp__(self,other):
    if isinstance(other,Date):
      other = other.value
    return cmp(self.value,other)

  def __str__(self):
    year,month,day = self.value[0:3]
    sign = ['','-'][year < 0]
    return "%s%04d-%02d-%02d" % (sign,abs(year),month,day)

  def __repr__(self):
    return "Date(%s)" % `str(self)`


class DateTime:

  def __init__(self,value=None):
    if value == None:
      value = time.time()
    t = type(value)
    if t is types.StringType:
      value = string.strip(value)
      result = _datetime_re.match(value)
      if not result:
	raise ValueError("wrong format, use YYYY-MM-DDThh:mm:ss")
      atoi = string.atoi
      year,month,day = map(string.atoi,result.groups()[1:4])
      hours,minutes,seconds = map(string.atoi,result.groups()[4:7])
      sign = result.groups()[0]
      if sign == '-':
	year = -year
    elif t is types.IntType or t is types.LongType:
      value = time.localtime(value)
      year,month,day,hours,minutes,seconds = value[0:6]
    elif t is types.FloatType:
      value = time.localtime(value)
      year,month,day,hours,minutes,seconds = value[0:6]
    else:
      year,month,day,hours,minutes,seconds = value[0:6]
    self.value = 9 * [0]
    self.value[0:6] = [year,month,day,hours,minutes,seconds]

  def __cmp__(self,other):
    if isinstance(other,DateTime):
      other = other.value
    return cmp(self.value,other)

  def __str__(self):
    year,month,day,hours,minutes,seconds = self.value[0:6]
    sign = ['','-'][year < 0]
    return "%s%04d-%02d-%02dT%02d:%02d:%02d" % \
	(sign,abs(year),month,day,hours,minutes,seconds)

  def __repr__(self):
    return "DateTime(%s)" % `str(self)`


class Time:

  def __init__(self,value=None):
    if value == None:
      value = time.time()
    t = type(value)
    if t is types.StringType:
      value = string.strip(value)
      result = _time_re.match(value)
      if not result:
	raise ValueError("wrong format, use hh:mm:ss")
      hours,minutes,seconds = map(string.atoi,result.groups()[:3])
    elif t is types.IntType or t is types.LongType:
      value = time.localtime(value)
      hours,minutes,seconds = value[3:6]
    elif t is types.FloatType:
      value = time.localtime(value)
      hours,minutes,seconds = value[3:6]
    else:
      hours,minutes,seconds = value[3:6]
    self.value = 9 * [0]
    self.value[3:6] = [hours,minutes,seconds]

  def __cmp__(self,other):
    if isinstance(other,Time):
      other = other.value
    return cmp(self.value,other)

  def __str__(self):
    hours,minutes,seconds = self.value[3:6]
    return "%02d:%02d:%02d" % (hours,minutes,seconds)

  def __repr__(self):
    return "Time(%s)" % `str(self)`


class Duration:

  def __init__(self,value=None):
    if value == None:
      value = 9 * [0]
    t = type(value)
    if t is types.StringType:
      value = string.strip(value)
      result = _duration_re.match(value)
      if not result:
	raise ValueError("wrong format, use PnYnMnDTnHnMnS")
      dict = result.groupdict()
      value = 9 * [0]
      for k,i in (('Y',0),('M',1),('D',2),('h',3),('m',4)):
	if dict.get(k,None):
	  value[i] = int(dict[k])
      if dict.get('s',None):
	value[5] = float(dict['s'])
      if dict.get('neg',0):
	value = map(operator.__neg__,value)
    elif t in [types.IntType,types.LongType,types.FloatType]:
      neg = value < 0
      value = abs(value)
      seconds = value - long(value / 60) * 60
      minutes = long(value / 60)
      hours = long(minutes / 60)
      minutes = minutes - (hours * 60)
      days = long(hours / 24)
      hours = hours - (days * 24)
      value = [0,0,days,hours,minutes,seconds,0,0]
      if neg:
	value = map(operator.__neg__,value)
    self.value = value

  def __cmp__(self,other):
    if isinstance(other,Duration):
      other = other.value
    return cmp(self.value,other)

  def __str__(self):
    if 1 in map(lambda x: x < 0, self.value):
      value = map(abs,self.value)
      neg = '-'
    else:
      value = self.value
      neg = ''
    return "%sP%dY%dM%dDT%dH%dM%gS" % \
        (neg,value[0],value[1],value[2],value[3],value[4],value[5])

  def __repr__(self):
    return "Duration(%s)" % `str(self)`


class Opaque:

  def __init__(self,type,data):
      self.type = type
      self.data = data

  def __cmp__(self,other):
    if isinstance(other,Opaque):
      other = (other.type,other.data)
    return cmp((self.type,self.data),other)

  def __repr__(self):
    return "Opaque(%s,%s)" % (`self.type`,`self.data`)


# Define routines for encoding and decoding of scalar value types.

_encoders = {}
_decoders = {}

def encoder(pythonType,encoder):
  if type(pythonType) == types.ClassType:
    valueType = pythonType.__name__
    moduleName = pythonType.__module__
  else:
    valueType = pythonType.__name__
    moduleName = ""
  _encoders[(moduleName,valueType)] = encoder

def decoder(valueType,decoder):
  _decoders[valueType] = decoder

def encodeObject(object):
  if type(object) == types.InstanceType:
    pythonType = object.__class__
    typeName = pythonType.__name__
    moduleName = pythonType.__module__
  else:
    typeName = type(object).__name__
    moduleName = ""
  lookupKey = (moduleName,typeName)
  if _encoders.has_key(lookupKey):
    callback = _encoders[lookupKey]
    return callback(object)
  else:
    raise TypeError("no encoder for type " + str(lookupKey))

def decodeValue(valueType,string):
  if _decoders.has_key(valueType):
    callback = _decoders[valueType]
    return callback(valueType,string)
  else:
    return Opaque(valueType,string)


# Define decoder/encoder routines for scalar builtin types.

def _encode_None(object):
  return ("","")

def _decode_None(name,object):
  return None

encoder(types.NoneType,_encode_None)
decoder("",_decode_None)

def _encode_Int(object):
  return ("xsd:int",str(object))

def _decode_Int(name,string):
  try:
    return int(string)
  except ValueError:
    return long(string)

encoder(types.IntType,_encode_Int)
decoder("xsd:byte",_decode_Int)
decoder("xsd:short",_decode_Int)
decoder("xsd:int",_decode_Int)
decoder("xsd:long",_decode_Int)
decoder("xsd:unsignedByte",_decode_Int)
decoder("xsd:unsignedShort",_decode_Int)
decoder("xsd:unsignedInt",_decode_Int)
decoder("xsd:unsignedLong",_decode_Int)
decoder("xsd:integer",_decode_Int)

def _encode_Long(object):
  return ("xsd:long",str(object))

encoder(types.LongType,_encode_Long)

def _encode_Float(object):
  return ("xsd:double",str(object))

def _decode_Float(name,string):
  return float(string)

encoder(types.FloatType,_encode_Float)
decoder("xsd:float",_decode_Float)
decoder("xsd:double",_decode_Float)
decoder("xsd:real",_decode_Float)

def _encode_String(object):
  return ("xsd:string",object)

def _decode_String(name,string):
  return string

encoder(types.StringType,_encode_String)
decoder("xsd:string",_decode_String)

if hasattr(types,"UnicodeType"):
  encoder(types.UnicodeType,_encode_String)

# Define decoder/encoder routines for scalar extended types.

def _encode_Boolean(object):
  #return ("xsd:boolean",str(operator.truth(object.value)))
  return ("xsd:boolean",string.lower(str(operator.truth(object))))

def _decode_Boolean(name,string):
  if string in ["false","0"]:
    return False
  elif string in ["true","1"]:
    return True
  raise TypeError("invalid encoding for boolean type")

encoder(Boolean,_encode_Boolean)
decoder("xsd:boolean",_decode_Boolean)

def _encode_Binary(object):
  return ("xsd:base64Binary",object.data)

def _decode_Binary(name,string):
  return Binary(string)

encoder(Binary,_encode_Binary)
decoder("xsd:base64Binary",_decode_Binary)

def _encode_Date(object):
  return ("xsd:date",str(object))

def _decode_Date(name,string):
  return Date(string)

encoder(Date,_encode_Date)
decoder("xsd:date",_decode_Date)

def _encode_DateTime(object):
  return ("xsd:dateTime",str(object))

def _decode_DateTime(name,string):
  return DateTime(string)

encoder(DateTime,_encode_DateTime)
decoder("xsd:dateTime",_decode_DateTime)

def _encode_Time(object):
  return ("xsd:time",str(object))

def _decode_Time(name,string):
  return Time(string)

encoder(Time,_encode_Time)
decoder("xsd:time",_decode_Time)

def _encode_Duration(object):
  return ("xsd:duration",str(object))

def _decode_Duration(name,string):
  return Duration(string)

encoder(Duration,_encode_Duration)
decoder("xsd:duration",_decode_Duration)

def _encode_Opaque(object):
  return (object.type,object.data)

encoder(Opaque,_encode_Opaque)


# Define decoder/encoder routines for some other scalar types specified
# in Python which which aren't in the basic set of supported scalar types.

def _encode_Complex(object):
  return ("python:complex",repr(object))

def _decode_Complex(name,string):
  object = eval(string,{},{})
  if type(object) != types.ComplexType:
    raise TypeError("invalid encoding for complex type")
  return object

encoder(types.ComplexType,_encode_Complex)
decoder("python:complex",_decode_Complex)
