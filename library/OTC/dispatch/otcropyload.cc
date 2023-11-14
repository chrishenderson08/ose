/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcropyload.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/ropyload.hh"
#endif
#endif

#include <OTC/dispatch/ropyload.hh>

#include <OTC/text/osstream.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
static OTC_NRMutex MODULE_TRACETAG_MUTEX;
static OTC_TraceTag& MODULE_TRACETAG()
{
  static OTC_TraceTag* TAG = 0;
  MODULE_TRACETAG_MUTEX.lock();
  if (TAG == 0)
    TAG = new OTC_TraceTag("OTC_ROPayload");
  MODULE_TRACETAG_MUTEX.unlock();
  return *TAG;
}

#define MODULE MODULE_TRACETAG()

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_ROPayload::mutex_;
OTC_Cluster* OTC_ROPayload::gCluster_ = 0;
OTC_DOMNode* OTC_ROPayload::gNullValue_ = 0;
OTC_DOMNode* OTC_ROPayload::gNullArray_ = 0;
OTC_DOMNode* OTC_ROPayload::gNullStruct_ = 0;

/* ------------------------------------------------------------------------- */
OTC_DOMString OTC_ROPayload::nodeType() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return "invalid";

  return theNode->name();
}

/* ------------------------------------------------------------------------- */
OTC_DOMString OTC_ROPayload::value() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  return theNode->content();
}

/* ------------------------------------------------------------------------- */
bool OTC_ROPayload::hasProperty(char const* theName) const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return false;

  return theNode->property(theName) != 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMString OTC_ROPayload::property(char const* theName) const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  OTC_DOMProperty const* theProperty;
  theProperty = theNode->property(theName);

  if (theProperty == 0)
    return 0;

  return theProperty->value();
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::findProperty(char const* theName) const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->findProperty(theName);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::findProperty(
 char const* theName,
 char const* theValue,
 size_t theLength
) const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->findProperty(theName,theValue,theLength);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::sibling(
 char const* thePath,
 size_t theLength
) const
{
  OTCLIB_ENSURE_FN((thePath != 0 || theLength == 0),
   "OTC_ROPayload::sibling(char const*,size_t)",
   "invalid path");

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  return OTC_ROPayload(theNode,thePath,theLength,-3);
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::operator[](char const* thePath) const
{
  OTCLIB_ENSURE_FN((thePath != 0),
   "OTC_ROPayload::operator[](char const*)",
   "invalid path");

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  return OTC_ROPayload(theNode,thePath,strlen(thePath),-2);
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::operator[](int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex >= -1),
   "OTC_ROPayload::operator[](int) const",
   "invalid index");

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  if (theNode->name() != "array")
    return 0;

  if (theIndex >= int(theNode->numChildren()))
    return 0;

  return OTC_ROPayload(theNode,0,0,theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::parent() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->parent();

  return theNode;
}

/* ------------------------------------------------------------------------- */
u_int OTC_ROPayload::numChildren() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    return theNode->numChildren();

  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::firstChild() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->firstChild();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::lastChild() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->lastChild();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::prevSibling() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->prevSibling();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::nextSibling() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->nextSibling();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_ROPayload::encode() const
{
  OTCLIB_MARKBLOCK(MODULE,"OTC_ROPayload::encode()");

  // We use the amount of memory which has been
  // allocated to define how much capacity to
  // preallocate within the string into which
  // the payload object is encoded. This
  // actually can give us a lot more than we
  // need dependent on whether the payload is
  // node heavy or data heavy. There is however
  // the issue of binary data increasing in
  // size when it is encoded into base64 in
  // which case there may not be enough memory
  // still. Assume for the time being that
  // memory is plentiful and live with the
  // extra memory for the normal case. It is
  // better to do this than have to do
  // reallocations and memory copies if buffer
  // isn't sufficient size.

  size_t tmpCapacity = 0;

  if (root_ != 0)
  {
    OTC_Cluster* tmpCluster;
    tmpCluster = OTC_Cluster::of(root_);

    if (tmpCluster != 0)
      tmpCapacity = tmpCluster->allocated();
  }

  OTCLIB_TRACER(MODULE) << "capacity = " << tmpCapacity << endl;

  OTC_Capacity theCapacity(tmpCapacity);
  OTC_String theString(theCapacity);

  OTC_OSStream theStream(theString,OTCLIB_BUFFERED);

  encode(theStream);

  theStream << flush;

  OTCLIB_TRACER(MODULE) << "length = " << theString.length() << endl;

  return theString;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_ROPayload::peek() const
{
  if (root_ == 0)
    return 0;

  if (path_ == 0 && index_ < -1)
    return root_;

  OTC_DOMNode* theNode;
  theNode = resolve_(root_,path_,length_,index_);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_ROPayload::resolve() const
{
  if (root_ == 0)
    return 0;

  if (path_ == 0 && index_ < -1)
    return root_;

  OTC_DOMNode* theNode;
  theNode = resolve_(root_,path_,length_,index_);

  OTC_ROPayload* theSelf;
  theSelf = (OTC_ROPayload*)this;

  theSelf->root_ = theNode;
  theSelf->path_ = 0;
  theSelf->index_ = -2;

  return root_;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_ROPayload::install() const
{
  if (root_ == 0)
    return 0;

  if (path_ == 0 && index_ < -1)
    return root_;

  if (path_ != 0)
  {
    if (root_->name() != "index")
    {
      root_->removeAllChildren();
      resetProperties(root_);
      root_->setContent(0);

      root_->setName("index");
    }
  }
  else if (index_ >= -1)
  {
    if (root_->name() != "array")
    {
      root_->removeAllChildren();
      root_->setName("array");
      resetProperties(root_);
    }
  }

  OTC_DOMNode* theNode;
  theNode = install_(root_,path_,length_,index_);

  if (theNode != 0)
  {
    OTC_ROPayload* theSelf;
    theSelf = (OTC_ROPayload*)this;

    theSelf->root_ = theNode;
    theSelf->path_ = 0;
    theSelf->index_ = -2;
  }

  return root_;
}

/* ------------------------------------------------------------------------- */
bool OTC_ROPayload::validProperty(char const* theName) const
{
  char const* tmpName;
  tmpName = theName;

  if (tmpName == 0 || *tmpName == EOS)
    return false;

  if (!isalpha(*tmpName) &&
   *tmpName != '_' && *tmpName != ':')
  {
    return false;
  }

  tmpName++;

  while (*tmpName != EOS)
  {
    if (!isalpha(*tmpName) && !isdigit(*tmpName) &&
     *tmpName != '_' && *tmpName != ':' &&
     *tmpName != '-' && *tmpName != '.')
    {
      return false;
    }

    tmpName++;
  }

  return true;
}

/* ------------------------------------------------------------------------- */
void OTC_ROPayload::resetProperties(OTC_DOMNode* theNode) const
{
  if (theNode != 0)
  {
    OTC_DOMProperty* theProperty;
    theProperty = theNode->properties();
    while (theProperty != 0)
    {
      OTC_DOMProperty* tmpProperty = 0;

      if (theProperty->name() != "name" &&
       theProperty->name() != "index")
      {
	tmpProperty = theProperty;
      }

      theProperty = theProperty->next();

      if (tmpProperty != 0)
	tmpProperty->destroy();
    }
  }
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::nullValue()
{
  mutex_.lock();

  if (gCluster_ == 0)
  {
    gCluster_ = new OTC_Cluster;
    OTCLIB_ASSERT_M(gCluster_ != 0);

    gNullValue_ = new (*gCluster_) OTC_DOMNode;
    gNullValue_->setName("scalar");

    gNullArray_ = new (*gCluster_) OTC_DOMNode;
    gNullArray_->setName("array");

    gNullStruct_ = new (*gCluster_) OTC_DOMNode;
    gNullStruct_->setName("index");
  }

  mutex_.unlock();

  return gNullValue_;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::nullArray()
{
  mutex_.lock();

  if (gCluster_ == 0)
  {
    gCluster_ = new OTC_Cluster;
    OTCLIB_ASSERT_M(gCluster_ != 0);

    gNullValue_ = new (*gCluster_) OTC_DOMNode;
    gNullValue_->setName("scalar");

    gNullArray_ = new (*gCluster_) OTC_DOMNode;
    gNullArray_->setName("array");

    gNullStruct_ = new (*gCluster_) OTC_DOMNode;
    gNullStruct_->setName("index");
  }

  mutex_.unlock();

  return gNullArray_;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_ROPayload::nullIndex()
{
  mutex_.lock();

  if (gCluster_ == 0)
  {
    gCluster_ = new OTC_Cluster;
    OTCLIB_ASSERT_M(gCluster_ != 0);

    gNullValue_ = new (*gCluster_) OTC_DOMNode;
    gNullValue_->setName("scalar");

    gNullArray_ = new (*gCluster_) OTC_DOMNode;
    gNullArray_->setName("array");

    gNullStruct_ = new (*gCluster_) OTC_DOMNode;
    gNullStruct_->setName("index");
  }

  mutex_.unlock();

  return gNullStruct_;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_ROPayload::resolve_(
 OTC_DOMNode* theNode,
 char const* thePath,
 size_t theLength,
 int theIndex
) const
{
  size_t tmpLength = 0;
  char const* tmpTail = 0;

  // Found target node.

  if (thePath == 0 && theIndex < -1)
    return root_;

  // Array, reference.

  if (thePath == 0 && theIndex >= -1)
  {
    // Make sure last node is an array.

    if (theNode->name() != "array")
      return 0;

    // Want last element in an array.

    if (theIndex == -1)
    {
      OTC_DOMNode* theChild;
      theChild = theNode->lastChild();

      return theChild;
    }

    return theNode->child(theIndex);
  }

  if (theIndex == -2)
    tmpTail = (char*)memchr(thePath,'.',theLength);

  if (tmpTail != 0)
    tmpLength = tmpTail - thePath;
  else
    tmpLength = theLength;

  if (tmpTail != 0)
    tmpTail++;

  OTC_DOMNode* theChild;

  theChild = theNode->firstChild();
  while (theChild != 0)
  {
    if (theChild->property("name",thePath,tmpLength))
    {
      if (tmpTail == 0)
	return theChild;

      return resolve_(theChild,tmpTail,theLength-tmpLength-1,theIndex);
    }

    theChild = theChild->nextSibling();
  }
  
  return 0;
}

/* ------------------------------------------------------------------------- */
OTC_DOMNode* OTC_ROPayload::install_(
 OTC_DOMNode* theNode,
 char const* thePath,
 size_t theLength,
 int theIndex
) const
{
  size_t tmpLength = 0;
  char const* tmpTail = 0;

  // Found target node.

  if (thePath == 0 && theIndex < -1)
    return root_;

  // Array, reference.

  if (thePath == 0 && theIndex >= -1)
  {
    // Make sure last node is an array.

    if (theNode->name() != "array")
    {
      theNode->removeAllChildren();
      resetProperties(theNode);
      theNode->setContent(0);

      theNode->setName("array");
    }

    // Need to create new last element.

    if (theIndex == -1)
    {
      OTC_DOMNode* theChild;
      theChild = theNode->createChild();

      // Nothing about it has been set.
      // We rely on value being assigned
      // to it straight away.

      return theChild;
    }

    return theNode->child(theIndex);
  }

  if (theIndex == -2)
    tmpTail = (char*)memchr(thePath,'.',theLength);

  if (tmpTail != 0)
    tmpLength = tmpTail - thePath;
  else
    tmpLength = theLength;

  if (tmpTail != 0)
    tmpTail++;

  OTC_DOMNode* theChild;
  theChild = theNode->firstChild();
  while (theChild != 0)
  {
    if (theChild->property("name",thePath,tmpLength))
    {
      if (tmpTail == 0)
	return theChild;

      if (theNode->name() != "index")
      {
	theNode->removeAllChildren();
	resetProperties(theNode);
	theNode->setContent(0);

	theNode->setName("index");
      }

      return install_(theChild,tmpTail,theLength-tmpLength-1,theIndex);
    }

    theChild = theChild->nextSibling();
  }

  if (tmpTail == 0 && theIndex >= -1)
  {
    theChild = theNode->createChild("array");
    theChild->setProperty("name",thePath,tmpLength);

    return install_(theChild,0,0,theIndex);
  }
  else
  {
    theChild = theNode->createChild("index");
    theChild->setProperty("name",thePath,tmpLength);
  }

  if (tmpTail == 0)
    return theChild;

  return install_(theChild,tmpTail,theLength-tmpLength-1,theIndex);
}

/* ------------------------------------------------------------------------- */
void OTC_ROPayload::encodeString_(
 ostream& theStream,
 char const* theString,
 size_t theLength
) const
{
  char const* theStart;
  size_t theActive = 0;

  theStart = theString;

  for (size_t i=0; i<theLength; i++)
  {
    char c = theString[i];

    switch (c)
    {
      case '<':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&lt;";
	break;
      }

      case '>':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&gt;";
	break;
      }

      case '&':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&amp;";
	break;
      }

      case '"':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&quot;";
	break;
      }

      case '\'':
      {
	if (theActive != 0)
	{
	  theStream.write(theStart,theActive);
	  theActive = 0;
	}

	theStart = &theString[i+1];

	theStream << "&apos;";
	break;
      }

      default:
      {
	// theStream << c;

	theActive++;
      }
    }
  }

  if (theActive != 0)
    theStream.write(theStart,theActive);
}

/* ------------------------------------------------------------------------- */
static char b64Encode_table[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

/* ------------------------------------------------------------------------- */
void OTC_ROPayload::encodeBinary_(
 ostream& theStream,
 char const* theString,
 size_t theLength
) const
{
  u_char const* tmpString;
  tmpString = (u_char const*)theString;
  size_t theCount = 0;

  while (theLength > 2)
  {
    theStream << b64Encode_table[tmpString[0]>>2];
    theStream << b64Encode_table[((tmpString[0]&0x03)<<4)+(tmpString[1]>>4)];
    theStream << b64Encode_table[((tmpString[1]&0x0f)<<2)+(tmpString[2]>>6)];
    theStream << b64Encode_table[tmpString[2]&0x3f];

    tmpString += 3;
    theLength -= 3;

    theCount += 3;

    if ((theCount % 57) == 0)
      theStream << "\n";
  }

  if (theLength != 0)
  {
    theStream << b64Encode_table[tmpString[0]>>2];

    if (theLength > 1)
    {
      theStream << b64Encode_table[((tmpString[0]&0x03)<<4)+(tmpString[1]>>4)];
      theStream << b64Encode_table[(tmpString[1]&0x0f)<<2];
      theStream << '=';
    }
    else
    {
      theStream << b64Encode_table[(tmpString[0]&0x03)<<4];
      theStream << '=';
      theStream << '=';
    }

    theCount += theLength;

    if ((theCount % 57) == 0)
      theStream << "\n";
  }

  if ((theCount % 57) != 0)
    theStream << "\n";
}

/* ------------------------------------------------------------------------- */
void OTC_ROPayload::encodeNode_(
 ostream& theStream,
 OTC_DOMNode const* theNode
) const
{
  if (theNode == 0)
    return;

  if (theNode->hasChildren())
  {
    theStream << "<";
    theStream << theNode->name();

    OTC_DOMProperty const* theProperty;
    theProperty = theNode->properties();
    while (theProperty != 0)
    {
      if (validProperty(theProperty->name().string()))
      {
	theStream << " ";

	theStream << theProperty->name();
	theStream << "=\"";
	encodeString_(theStream,theProperty->value());
	theStream << "\"";
      }

      theProperty = theProperty->next();
    }

    theStream << ">\n";

    OTC_DOMNode const* theChild;
    theChild = theNode->firstChild();

    while (theChild != 0)
    {
      encodeNode_(theStream,theChild);

      theChild = theChild->nextSibling();
    }

    theStream << "</";
    theStream << theNode->name();
    theStream << ">\n";
  }
  else
  {
    if (!theNode->name().isEmpty())
    {
      theStream << "<";
      theStream << theNode->name();

      OTC_DOMProperty const* theProperty;
      theProperty = theNode->properties();
      while (theProperty != 0)
      {
	if (validProperty(theProperty->name().string()))
	{
	  theStream << " ";

	  theStream << theProperty->name();
	  theStream << "=\"";
	  encodeString_(theStream,theProperty->value());
	  theStream << "\"";
        }

	theProperty = theProperty->next();
      }

      theStream << ">";

      theProperty = theNode->property("type");

      if (theProperty != 0 && theProperty->value() == "xsd:base64Binary")
	encodeBinary_(theStream,theNode->content());
      else
	encodeString_(theStream,theNode->content());

      theStream << "</";
      theStream << theNode->name();
      theStream << ">\n";
    }
    else
    {
      encodeString_(theStream,theNode->content());
    }
  }
}

/* ------------------------------------------------------------------------- */
