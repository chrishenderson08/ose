import sys
import netrpc

def test1(service):
  try:
    result = service.listMethods()
  except:
    print "Introspection Failed"
    print "--------------------"
    print ""
    print netrpc.exceptionDetails()
    sys.exit(1)

  info = {}
  try:
    for name in result:
      info[name] = {}
      info[name]["signatures"] = service.methodSignature(name)
      info[name]["help"] = service.methodHelp(name)
  except:
    print "Introspection Failed"
    print "--------------------"
    print ""
    print netrpc.exceptionDetails()
    sys.exit(1)

  if info.keys() == []:
    print "No Methods Described"
    print "--------------------"

  for name in info.keys():
    print name
    print len(name) * "-"
    print
    signatures = info[name]["signatures"]
    if signatures == "undef" or signatures == []:
      print "Signature Unknown"
    else:
      print "Signatures:\n"
      for prototype in signatures:
	string = "  %s %s(" % (prototype[0],name)
	params = prototype[1:]
	if params != []:
	  string = string + params[0]
	  params = params[1:]
	  if params != []:
	    for param in params:
	      string = string + ", " + param
	string = string + ")"
	print string
    print
    help = info[name]["help"]
    if help == None or help == "":
      print "No Documentation Available"
    else:
      print "Documentation:\n"
      print help
    print
