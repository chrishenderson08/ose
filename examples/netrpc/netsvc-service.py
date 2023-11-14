import signal
import netsvc
import netsvc.xmlrpc
import netsvc.soap

# Note, you need to have the complete OSE installed for
# this service to work.

# Use the following URLs:
#
#    NET-RPC --> http://localhost:8000/netrpc/service
#    XML-RPC --> http://localhost:8000/xmlrpc/service
#    SOAP --> http://localhost:8000/soap/service
#
# Each map to the same service but with a different URL.

class Service(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"service")
    self.joinGroup("public")
    self.exportMethod(self.returnZero)
    self.exportMethod(self.returnOne)
    self.exportMethod(self.returnTrue)
    self.exportMethod(self.returnFalse)
    self.exportMethod(self.returnNone)
    self.exportMethod(self.returnArrayNone)
    self.exportMethod(self.returnDictNone)
    self.exportMethod(self.returnComplexNone)
    self.exportMethod(self.returnArray1)
    self.exportMethod(self.returnArray2)
    self.exportMethod(self.returnDict1)
    self.exportMethod(self.returnDict2)
    self.exportMethod(self.echo)
    self.exportMethod(self.echoSingle)

  def returnZero(self):
    return 0

  def returnOne(self):
    return 1

  def returnTrue(self):
    return netsvc.True

  def returnFalse(self):
    return netsvc.False

  def returnNone(self):
    return None

  def returnArrayNone(self):
    return [None]

  def returnDictNone(self):
    return {"a":None}

  def returnComplexNone(self):
    return [None,{"a":None,"b":[None,{"c":None}]}]

  def returnArray1(self):
    return [1]

  def returnArray2(self):
    return [1,2]

  def returnDict1(self):
    return {"a":1}

  def returnDict2(self):
    return {"a":1,"b":2}

  def echo(self,*args):
    return args

  def echoSingle(self,arg):
    return arg


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

service = Service()

httpd = netsvc.HttpDaemon(8000)

rpcgw1 = netsvc.RpcGateway("public")
httpd.attach("/netrpc",rpcgw1)

rpcgw2 = netsvc.xmlrpc.RpcGateway("public")
httpd.attach("/xmlrpc",rpcgw2)

rpcgw3 = netsvc.soap.RpcGateway("public")
httpd.attach("/soap",rpcgw3)

httpd.start()

dispatcher.run()
