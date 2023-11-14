import netsvc
import sys
import signal
import string
import random

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: service -connect server [ -domain name ]"

try:
  i = 0
  while i < count:
    if args[i] == "-connect":
      server = args[i+1]
      i = i + 2
    elif args[i] == "-domain":
      domain = args[i+1]
      i = i + 2
    else:
      usage()
      sys.exit(1)
except:
  usage()
  sys.exit(1)

if server == None:
  usage()
  sys.exit(1)

class Publisher(netsvc.Service):
  
  def __init__(self):
    netsvc.Service.__init__(self,"service","*",netsvc.SERVICE_HIDDEN)
    self.subscribeStartup(self.startup)

  def startup(self):
    self.exportMethod(self.multiply)
    self.exportMethod(self.wait)
    self.exportMethod(self.echo)
    self.announceAvailability()
    self.joinGroup("group")
    self.publishReport("creationTime",netsvc.DateTime(),-1)
    self.startTimer(self.publish,1.0)

  def publish(self,name):
    self.publishReport("currentTime",netsvc.DateTime())
    self.startTimer(self.publish,1.0)

  def multiply(self,x,y):
    return x*y

  def wait(self,period):
    self.startTimer(self.resumeResponse,period,self.conversationId())
    self.suspendResponse()

  def echo(self,value):
    return value

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

publisher = Publisher()
publisher.scheduleStartup()

dispatcher.run()
