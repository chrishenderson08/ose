import netsvc
import netsvc.client
import string
import sys

count = len(sys.argv)-1
server = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: client-request -connect server [ -domain name ]"

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

try:
  dispatcher = netsvc.Dispatcher()
  dispatcher.task().start()

  exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT)

  delay = 5
  host,port = string.splitfields(server,':')
  port = int(port)
  exchange.connect(host,port,delay)

  registry = netsvc.client.Registry()
  bindings = registry.locateService("service")

  if bindings != []:
    service = netsvc.client.RemoteService(bindings[0])
    print service.echo(netsvc.DateTime())
  else:
    print "timeout"

finally:
  dispatcher.task().stop()
  dispatcher.task().wait()
