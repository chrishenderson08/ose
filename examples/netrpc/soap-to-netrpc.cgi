#! /usr/bin/env python

import netrpc
import netrpc.soap
import time

def dispatch(method,params):
  import cgi, os
  qs = os.environ.get("QUERY_STRING","")
  options = cgi.parse_qs(qs)

  if not options.has_key("url"):
    raise netrpc.ServiceFailure(2,"Remote URL not specified.","proxy")

  url = options["url"][0]

  try:
    file = open("/logs/soap-to-netrpc.log","a")
    date = time.asctime(time.gmtime(time.time()))
    file.write(str((date,url,method)))
    file.write("\n")
    file.close()
  except:
    pass

  service = netrpc.RemoteService(url)

  try:
    return apply(service.__getattr__(method),params)

  except netrpc.ServiceFailure, exception:
    if exception.origin == "netrpc":
      exception.origin = "proxy"
    raise exception


netrpc.soap.asCGI(dispatch)
