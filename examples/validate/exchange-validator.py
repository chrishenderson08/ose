import netsvc
import netsvc.client
 
import ValidatorTests

try:
  dispatcher = netsvc.Dispatcher()
  dispatcher.task().start()

  exchange = netsvc.Exchange(netsvc.EXCHANGE_CLIENT)
  exchange.connect("localhost",11111)

  registry = netsvc.client.Registry()
  bindings = registry.locateService("validator")

  if bindings != []:
    service = netsvc.client.RemoteService(bindings[0])
    print ValidatorTests.test1(service)
    print ValidatorTests.test2(service)
    print ValidatorTests.test3(service)
    print ValidatorTests.test4(service)
    print ValidatorTests.test5(service)
    print ValidatorTests.test6(service)
    print ValidatorTests.test7(service)
    print ValidatorTests.test8(service)

finally:
  dispatcher.task().stop()
  dispatcher.task().wait()
