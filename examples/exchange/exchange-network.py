import netsvc
import sys
import signal
import string

count = len(sys.argv)-1
servers = []
port = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: exchange-network -listen port " \
      "[ -connect server ][ -domain name ]"

try:
  i = 0
  while i < count:
    if args[i] == "-listen":
      port = int(args[i+1])
      i = i + 2
    elif args[i] == "-connect":
      servers.append(args[i+1])
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

if port == None:
  usage()
  sys.exit(1)

logger = netsvc.Logger()

class Exchange(netsvc.Exchange):

  def __init__(self,type,domain=""):
    netsvc.Exchange.__init__(self,type,domain)

  def handleConnection(self,announcement):
    state = "INACTIVE"
    if announcement.state() == netsvc.CONNECTION_ACTIVE:
      state = "ACTIVE"
    process = announcement.remoteProcess()
    address = announcement.remoteAddress()
    message = "%s %s (%s)" % (state,process,address)
    logger.notify(netsvc.LOG_NOTICE,message)

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = Exchange(netsvc.EXCHANGE_SERVER,domain)
exchange.listen(port)

delay = 5

for server in servers:
  host,port = string.splitfields(server,':')
  port = int(port)
  exchange.connect(host,port,delay)

dispatcher.run()
