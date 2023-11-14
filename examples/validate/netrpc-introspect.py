import sys
import netrpc
import IntrospectTests

url = "http://localhost:8111/netrpc/service-1"
service = netrpc.RemoteService(url)

IntrospectTests.test1(service)

url = "http://localhost:8111/netrpc/service-2"
service = netrpc.RemoteService(url)

IntrospectTests.test1(service)
