import signal
import random
import string

import netrpc

root = "http://localhost:8000"
admin = netrpc.RemoteService(root+"/admin")

users = admin.list()
print users

for i in range(0,int(0.75*len(users))):
  if len(users) != 0:
    user = users[int(len(users)*random.random())]
    print "del %s" % user
    admin.delete(user)
  users = admin.list()

print users

users = ["ulrick","ulrica","ulysses","una","ursula","ursy"]
delay = 10

for user in users:
  print "add %s %s" % (user,delay)
  admin.create(user,delay)
  delay = 2*delay

users = admin.list()
print users

for user in users:
  try:
    service = netrpc.RemoteService(root+"/user/"+user)
    print "data %s %s" % (user,`service.data()`)
  except netrpc.ServiceUnavailable:
    print "dead %s" % user

users = admin.list()
print users

for i in range(0,int(0.75*len(users))):
  if len(users) != 0:
    user = users[int(len(users)*random.random())]
    service = netrpc.RemoteService(root+"/user/"+user)
    data = string.zfill(str(int(1000*random.random())),4)
    print "set %s %s" % (user,`data`)
    try:
      service.set(data)
    except netrpc.ServiceUnavailable:
      print "dead %s" % user

users = admin.list()
print users

for user in users:
  service = netrpc.RemoteService(root+"/user/"+user)
  try:
    data = service.data()
    print "data %s %s" % (user,`data`)
  except netrpc.ServiceUnavailable:
    print "dead %s" % user

users = admin.list()
print users

for i in range(0,int(0.75*len(users))):
  if len(users) != 0:
    user = users[int(len(users)*random.random())]
    print "del %s" % user
    admin.delete(user)

print admin.list()

for user in users:
  service = netrpc.RemoteService(root+"/user/"+user)
  try:
    name = service.name()
    print "okay %s" % user
  except netrpc.ServiceUnavailable:
    print "dead %s" % user

users = admin.list()
print users
