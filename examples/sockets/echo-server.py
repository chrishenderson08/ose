import sys
import netsvc
import signal
import socket

logger = netsvc.Logger()
dispatcher = netsvc.Dispatcher()

logger.enableLongFormat()

class Agent(netsvc.Agent):

  def __init__(self,port):

    netsvc.Agent.__init__(self)

    self.clients = {}

    self.sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    self.sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    self.sock.bind(("0.0.0.0",port))
    self.sock.listen(5)

    self.subscribeSocket(self.callback1,self.sock.fileno())

    self.sock.setblocking(1)

  def callback1(self,fileno,event):

    if fileno != self.sock.fileno():
      return

    if event == netsvc.SOCKET_POLLIN:
      (newsock,addr) = self.sock.accept()
      self.clients[newsock.fileno()] = newsock
      self.subscribeSocket(self.callback2,newsock.fileno())

  def callback2(self,fileno,event):

    if not self.clients.has_key(fileno):
      return

    newsock = self.clients[fileno]

    if event == netsvc.SOCKET_POLLIN:
      data = newsock.recv(1024)
      if len(data) == 0:
	self.unsubscribeSocket(newsock.fileno())
	newsock.close()
	del self.clients[fileno]
      else:
	newsock.send(data)
    else:
      self.unsubscribeSocket(newsock.fileno())
      newsock.close()
      del self.clients[fileno]


if len(sys.argv) != 2:
  print "Usage: echo-server port"
  sys.exit()

port = int(sys.argv[1])

dispatcher.monitor(signal.SIGINT)

object = Agent(port)

dispatcher.run()
