import netsvc
import sys
import signal

count = len(sys.argv)-1
port = None
domain = ""

args = sys.argv[1:]

def usage():
  print "Usage: exchange -listen port [ -domain name ]"

try:
  i = 0
  while i < count:
    if args[i] == "-listen":
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

if port == None:
  usage()
  sys.exit(1)

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_SERVER,domain)
exchange.listen(port)

dispatcher.run()
