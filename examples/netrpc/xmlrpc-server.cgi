#! /usr/bin/env python

import netrpc
import netrpc.xmlrpc
import functions

def _dispatch(method,params):
  if method[:1] == '_' or not hasattr(functions,method):
    raise netrpc.ServiceFailure(netrpc.SERVER_METHOD_UNAVAILABLE,
     netrpc.SERVER_METHOD_UNAVAILABLE_MESSAGE,"netrpc-server","")
  callback = getattr(functions,method)
  return apply(callback,params)

netrpc.xmlrpc.asCGI(_dispatch)
