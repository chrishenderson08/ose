import sys
import netrpc.xmlrpc
import IntrospectTests

url = "http://localhost:8111/xmlrpc/service-1"
service = netrpc.xmlrpc.RemoteService(url)

IntrospectTests.test1(service)

url = "http://localhost:8111/xmlrpc/service-2"
service = netrpc.xmlrpc.RemoteService(url)

IntrospectTests.test1(service)
