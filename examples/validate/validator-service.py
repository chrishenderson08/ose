import signal
import netsvc
import netsvc.xmlrpc

# Use following URL:
#    NET-RPC --> http://localhost:8111/netrpc/validator
#    XML-RPC --> http://localhost:8111/xmlrpc/validator

class Validator(netsvc.Service):

  def __init__(self):
    netsvc.Service.__init__(self,"validator")
    self.exportMethod(self.arrayOfStructsTest)
    self.exportMethod(self.countTheEntities)
    self.exportMethod(self.easyStructTest)
    self.exportMethod(self.echoStructTest)
    self.exportMethod(self.manyTypesTest)
    self.exportMethod(self.moderateSizeArrayCheck)
    self.exportMethod(self.nestedStructTest)
    self.exportMethod(self.simpleStructReturnTest)

  def arrayOfStructsTest(self,array):
    total = 0
    for struct in array:
      total = total + struct["curly"]
    return total

  def countTheEntities(self,string):
    totals = { '<':0, '>':0, '&':0, "'": 0, '"':0 }
    for c in string:
      if totals.has_key(c):
        totals[c] = totals[c] + 1
    result = {
        "ctLeftAngleBrackets":totals['<'],
        "ctRightAngleBrackets":totals['>'],
        "ctAmpersands":totals['&'],
        "ctApostrophes":totals["'"],
        "ctQuotes":totals['"']
    }
    return result

  def easyStructTest(self,struct):
    total = struct["moe"] + struct["larry"] + struct["curly"]
    return total

  def echoStructTest(self,struct):
    return struct

  def manyTypesTest(self,a1,a2,a3,a4,a5,a6):
    return [a1,a2,a3,a4,a5,a6]

  def moderateSizeArrayCheck(self,array):
    return array[0]+array[-1]

  def nestedStructTest(self,struct):
    entry = struct["2000"]["04"]["01"]
    total = entry["moe"] + entry["larry"] + entry["curly"]
    return total

  def simpleStructReturnTest(self,value):
    struct = {}
    struct["times10"] = 10*value
    struct["times100"] = 100*value
    struct["times1000"] = 1000*value
    return struct


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

validator = Validator()

httpd = netsvc.HttpDaemon(8111)

rpcgw1 = netsvc.RpcGateway(
    validator.serviceAddress(),netsvc.SERVICE_ADDRESS)
httpd.attach("/netrpc/validator",rpcgw1)

rpcgw2 = netsvc.xmlrpc.RpcGateway(
    validator.serviceAddress(),netsvc.SERVICE_ADDRESS)
httpd.attach("/xmlrpc/validator",rpcgw2)

exchange = netsvc.Exchange(netsvc.EXCHANGE_SERVER)
exchange.listen(11111)

httpd.start()

dispatcher.run()
