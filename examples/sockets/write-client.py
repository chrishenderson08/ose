import sys
import netsvc
import signal
import socket

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()

class Agent(netsvc.Agent):

  def __init__(self,host,port):
    netsvc.Agent.__init__(self)
    self._host = host
    self._port = port
    self.scheduleAction(self.connect,netsvc.STANDARD_JOB,"connect")

  def connect(self,tag):
    self._sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    try:
      self._sock.connect((host,port))
    except:
      dispatcher.stop()
    else:
      self.subscribeSocket(self.read,self._sock.fileno())
      self.startTimer(self.write,1,"timeout")

  def read(self,fileno,event):
    if fileno != self._sock.fileno():
      return
    if event == netsvc.SOCKET_POLLIN:
      data = self._sock.recv(1024)
      if len(data) == 0:
	self.unsubscribeSocket(self._sock.fileno())
	self.cancelAllTimers()
	self._sock.close()
	dispatcher.stop()
      else:
	sys.stdout.write(data)

  def write(self,tag):
    self._sock.send(str(netsvc.DateTime())+"\n")
    self.startTimer(self.write,1,"timeout")


if len(sys.argv) != 3:
  print "Usage: read-client host port"
  sys.exit()

host = sys.argv[1]
port = int(sys.argv[2])

dispatcher.monitor(signal.SIGINT)

object = Agent(host,port)

dispatcher.run()
