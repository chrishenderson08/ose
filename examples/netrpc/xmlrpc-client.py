import sys
import time
import netrpc
import netrpc.xmlrpc

service = netrpc.xmlrpc.RemoteService("http://localhost:8000/xmlrpc/service")

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
