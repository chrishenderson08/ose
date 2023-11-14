import netsvc
import signal
import string
import sys

count = len(sys.argv)-1
server = None
name = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: service-monitor -connect server -name name [ -domain name ]"

try:
  i = 0
  while i < count:
    if args[i] == "-connect":
      server = args[i+1]
      i = i + 2
    elif args[i] == "-name":
      name = args[i+1]
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

if server == None or name == None:
  usage()
  sys.exit(1)


class Monitor(netsvc.Monitor):

  def __init__(self,name):
    netsvc.Monitor.__init__(self)
    self.subscribeServiceName(self.callback1,name)

  def callback1(self,binding,status):
    action = "WITHDRAWN"
    if status == netsvc.SERVICE_AVAILABLE:
      action = "AVAILABLE"
    name = binding.serviceName()
    identity = binding.agentIdentity()
    print "SERVICE-%s: %s (%s)" % (action,`name`,identity)


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

monitor = Monitor(name)

dispatcher.run()
