import random
import string
import netrpc

def test1(service):
  array = []
  for i in range(0,20):
    struct = {}
    struct["moe"] = int(1000*random.random())
    struct["larry"] = int(1000*random.random())
    struct["curly"] = int(1000*random.random())
    array.append(struct)
    total = 0
    for struct in array:
      total = total + struct["curly"]
  try:
    result = service.arrayOfStructsTest(array)
    if result != total:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test2(service):
  ascii = ""
  chars = "0123456789<>&'\""
  for i in range(0,100):
    ascii = string.join((ascii,chars[int(len(chars)*random.random())]),"")
  totals = { '<':0, '>':0, '&':0, "'": 0, '"':0 }
  for c in ascii:
    if totals.has_key(c):
      totals[c] = totals[c] + 1
  try:
    result = service.countTheEntities(ascii)
    if result["ctLeftAngleBrackets"] != totals['<']:
      return "FAILED"
    if result["ctRightAngleBrackets"] != totals['>']:
      return "FAILED"
    if result["ctAmpersands"] != totals['&']:
      return "FAILED"
    if result["ctApostrophes"] != totals["'"]:
      return "FAILED"
    if result["ctQuotes"] != totals['"']:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test3(service):
  struct = {}
  struct["moe"] = int(1000*random.random())
  struct["larry"] = int(1000*random.random())
  struct["curly"] = int(1000*random.random())
  total = struct["moe"] + struct["larry"] + struct["curly"]
  try:
    result = service.easyStructTest(struct)
    if result != total:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    print "FAILED"

def test4(service):
  struct = {}
  struct["moe"] = int(1000*random.random())
  struct["larry"] = int(1000*random.random())
  struct["curly"] = int(1000*random.random())
  try:
    result = service.echoStructTest(struct)
    if result != struct:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test5(service):
  array = []
  array.append(int(1000*random.random()))
  array.append(netrpc.Boolean(int(1000*random.random())%2))
  array.append(str(netrpc.DateTime()))
  array.append(float(int(1000*random.random()))/2)
  array.append(netrpc.DateTime())
  array.append(netrpc.Binary(str(netrpc.DateTime())))
  try:
    result = service.manyTypesTest(
        array[0],array[1],array[2],array[3],array[4],array[5])
    # This may fail sometimes due to float comparison test.
    if result != array:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test6(service):
  array = []
  for i in range(1,100+int(100*random.random())):
    array.append(string.zfill(str(i),int(20*random.random())))
  try:
    result = service.moderateSizeArrayCheck(array)
    if result != array[0]+array[-1]:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test7(service):
  struct = {}
  struct["moe"] = int(1000*random.random())
  struct["larry"] = int(1000*random.random())
  struct["curly"] = int(1000*random.random())
  total = struct["moe"] + struct["larry"] + struct["curly"]
  struct = { "2000": { "04" : { "01" : struct } } }
  try:
    result = service.nestedStructTest(struct)
    if result != total:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"

def test8(service):
  number = int(1000*random.random())
  struct = {}
  struct["times10"] = 10*number
  struct["times100"] = 100*number
  struct["times1000"] = 1000*number
  try:
    result = service.simpleStructReturnTest(number)
    if result != struct:
      return "FAILED"
    return "PASSED"
  except:
    print netrpc.exceptionDetails()
    return "FAILED"
