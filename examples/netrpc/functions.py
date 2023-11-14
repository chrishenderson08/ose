import netrpc

def returnZero():
  return 0

def returnOne():
  return 1

def returnTrue():
  return netrpc.True

def returnFalse():
  return netrpc.False

def returnNone():
  return None

def returnArrayNone():
  return [None]

def returnDictNone():
  return {"a":None}

def returnComplexNone():
  return [None,{"a":None,"b":[None,{"c":None}]}]

def returnArray1():
  return [1]

def returnArray2():
  return [1,2]

def returnDict1():
  return {"a":1}

def returnDict2():
  return {"a":1,"b":2}

def echo(*args):
  return args

def echoSingle(arg):
  return arg
