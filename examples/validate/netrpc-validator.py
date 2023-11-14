import netrpc

url = "http://localhost:8111/netrpc/validator"
service = netrpc.RemoteService(url)

import ValidatorTests

print ValidatorTests.test1(service)
print ValidatorTests.test2(service)
print ValidatorTests.test3(service)
print ValidatorTests.test4(service)
print ValidatorTests.test5(service)
print ValidatorTests.test6(service)
print ValidatorTests.test7(service)
print ValidatorTests.test8(service)
