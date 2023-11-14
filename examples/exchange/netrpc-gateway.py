import netsvc
import sys
import signal
import string

count = len(sys.argv)-1
server = None
domain = ""
port = None

args = sys.argv[1:]

def usage():
  print "Usage: netrpc-gateway -connect server " \
      "[ -domain name ] -listen port"

try:
  i = 0
  while i < count:
    if args[i] == "-connect":
      server = args[i+1]
      i = i + 2
    elif args[i] == "-listen":
      port = int(args[i+1])
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

if server == None or port == None:
  usage()
  sys.exit(1)

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

httpd = netsvc.HttpDaemon(port)
rpcgw = netsvc.RpcGateway("*",netsvc.SERVICE_REGISTRY)
httpd.attach("/",rpcgw)
httpd.start()

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

dispatcher.run()
