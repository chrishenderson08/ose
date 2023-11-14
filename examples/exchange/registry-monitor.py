import netsvc
import signal
import string
import sys

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: registry-monitor -connect server [ -domain name ]"

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


class Monitor(netsvc.Monitor):

  def __init__(self):
    netsvc.Monitor.__init__(self)
    self.subscribeRegistry(self.callback1)

  def callback1(self,binding,group,status):
    if group == None:
      action = "WITHDRAWN"
      if status == netsvc.SERVICE_AVAILABLE:
	action = "AVAILABLE"
      name = binding.serviceName()
      identity = binding.agentIdentity()
      print "SERVICE-%s: %s (%s)" % (action,`name`,identity)
    else:
      action = "LEAVE"
      if status == netsvc.SERVICE_AVAILABLE:
	action = "JOIN"
      name = binding.serviceName()
      identity = binding.agentIdentity()
      print "%s-GROUP[%s]: %s (%s)" % (action,`group`,`name`,identity)


dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

monitor = Monitor()

dispatcher.run()
