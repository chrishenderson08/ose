#! /usr/bin/env python

import netrpc
import netrpc.soap
import netrpc.xmlrpc
import time

def dispatch(method,params):
  import cgi, os
  qs = os.environ.get("QUERY_STRING","")
  options = cgi.parse_qs(qs)

  if not options.has_key("url"):
    raise netrpc.ServiceFailure(2,"Remote URL not specified.","proxy")

  url = options["url"][0]
  ns = options.get("ns",[None])[0]
  soapaction = options.get("soapaction",[None])[0]

  try:
    file = open("/logs/xmlrpc-to-soap.log","a")
    date = time.asctime(time.gmtime(time.time()))
    file.write(str((date,url,method,ns,soapaction)))
    file.write("\n")
    file.close()
  except:
    pass

  service = netrpc.soap.RemoteService(url,ns=ns,soapaction=soapaction)

  try:
    return apply(service.__getattr__(method),params)

  except netrpc.ServiceFailure, exception:
    if exception.origin == "netrpc.soap":
      exception.origin = "proxy"
    raise exception


netrpc.xmlrpc.asCGI(dispatch)
