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
  print "Usage: service-reports -connect server [ -domain name ]"

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

class Subscriber(netsvc.Service):
  
  def __init__(self):
    netsvc.Service.__init__(self,"monitor-reports","*",netsvc.SERVICE_HIDDEN)
    self.subscribeStartup(self.startup)

  def startup(self):
    self.announceAvailability()
    self.monitorReports(self.report,"service","currentTime")

  def report(self,content):
    print content

dispatcher = netsvc.Dispatcher()
dispatcher.monitor(signal.SIGINT)

exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT,domain)

delay = 5
host,port = string.splitfields(server,':')
port = int(port)
exchange.connect(host,port,delay)

subscriber = Subscriber()
subscriber.scheduleStartup()

dispatcher.run()
