import sys
import time

import ZSI.client
import ZSI.TC

import netrpc
import netrpc.soap

# The standard ZSI.client.Binding, although it is intended to be used for
# positional parameter calling, returns the result in a single element
# array. It also throws away the keys in dictionaries, instead returning
# just the values the keys pointed at. The following version of the Binding
# class for ZSI fixes these problems. If using this interface, the exceptions
# which can result will be those used by the ZSI package. Note that if you
# use this interface to call server side SOAP functions which use the
# "netrpc" CGI dispatcher or "netsvc" framework and a fault is returned,
# this interface will not be able to handle them and will generate an
# exception while handling the result. Blame on this point can be directed at
# the SOAP specification which doesn't actually say what has to be in a SOAP
# fault detail. Thus a service can use whatever it wants and if a client
# doesn't provide a graceful way of handling arbitrary fault detail, you are
# in trouble. You will be fine with this though if you intend to use a ZSI
# based server, where you are only using "netrpc" for the additional
# predefined types.

class _Caller:

  def __init__(self,binding,name):
    self.binding, self.name = binding, name

  def __call__(self,*args):
    return self.binding.RPC(None,self.name,args,ZSI.TC.Any(aslist=0), 
        requesttypecode=ZSI.TC.Any(self.name, aslist=1)).values()[0]

class Binding(ZSI.client.Binding):

  def __getattr__(self, name):
    if name[:2] == '__' and len(name) > 5 and name[-2:] == '__':
	if hasattr(self, name): return getattr(self, name)
	return getattr(self.__class__,name)
    return _Caller(self,name)

service = Binding(host="localhost",port=8000)

print service.returnTrue()
print service.returnFalse()
print service.returnZero()
print service.returnOne()
print service.returnNone()
print service.returnArrayNone()
print service.returnDictNone()
print service.returnComplexNone()

print service.echoSingle(None)
print service.echoSingle([None])
print service.echoSingle({"a":None,"b":[None,{"c":None, \
 "d":[None,{"a":None,"b":[None,{"c":None}]}]}]})
print service.echoSingle([None,{"a":None,"b":[None,{"c":None, \
 "d":[None,{"a":None,"b":[None,{"c":None}]}]}]}])

print service.echo(None)
print service.echo([None])
print service.echo({"a":None,"b":[None,{"c":None,"d":[None, \
 {"a":None,"b":[None,{"c":None}]}]}]})
print service.echo([None,{"a":None,"b":[None,{"c":None,"d":[None, \
 {"a":None,"b":[None,{"c":None}]}]}]}])

print service.returnArray1()
print service.returnArray2()
print service.returnDict1()
print service.returnDict2()

print service.echoSingle(netrpc.Boolean(0))
print service.echoSingle(netrpc.Boolean(1))

print service.echoSingle(netrpc.Binary("1234"))

print service.echoSingle(netrpc.Duration("P7Y6M5DT4H3M2.1S"))
print service.echoSingle(netrpc.Duration("P7Y6M5DT4H3M"))
print service.echoSingle(netrpc.Duration("P7Y6M5DT4H"))
print service.echoSingle(netrpc.Duration("P7Y6M5D"))
print service.echoSingle(netrpc.Duration("P7Y6M"))
print service.echoSingle(netrpc.Duration("P7Y"))
print service.echoSingle(netrpc.Duration("PT4H3M2.1S"))

print service.echoSingle(netrpc.Date())
print service.echoSingle(netrpc.Time())
print service.echoSingle(netrpc.DateTime())

now = time.gmtime(time.time())
print service.echoSingle(netrpc.Date(now))
print service.echoSingle(netrpc.Time(now))
print service.echoSingle(netrpc.DateTime(now))
