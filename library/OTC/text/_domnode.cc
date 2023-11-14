/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     text/_domnode.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/text/domnode.hh>

#include <OTC/memory/cluster.hh>
#include <OTC/debug/tracer.hh>

#include <stdlib.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */

void escape(ostream& theStream, char const* theString, u_int theLength)
{
  for (u_int i=0; i<theLength; i++)
  {
    char c = theString[i];

    switch (c)
    {
      case '<':
      {
	theStream << "&lt;";
	break;
      }

      case '>':
      {
	theStream << "&gt;";
	break;
      }

      case '&':
      {
	theStream << "&amp;";
	break;
      }

      case '"':
      {
	theStream << "&quot;";
	break;
      }

      case '\'':
      {
	theStream << "&apos;";
	break;
      }

      default:
      {
	//if (u_char(c) > 127)
	if (!isprint(c))
	{
	  char buf[16];
	  sprintf(buf,"%X",u_int(u_char(c)));

	  theStream << "&#x";
	  theStream << buf;
	  theStream << ";";
	}
        else
	  theStream << c;
      }
    }
  }
}

void escape(ostream& theStream, char const* theString)
{
  escape(theStream,theString,strlen(theString));
}

void escape(ostream& theStream, OTC_DOMString const& theString)
{
  escape(theStream,theString.string(),theString.length());
}

ostream& operator<<(ostream& theStream, OTC_DOMNode const& theNode)
{
  if (theNode.hasChildren())
  {
    theStream << "<";
    theStream << theNode.name();

    OTC_DOMProperty const* theProperty;
    theProperty = theNode.properties();
    while (theProperty != 0)
    {
      theStream << " ";

      theStream << theProperty->name();
      theStream << "=\"";
      escape(theStream,theProperty->value());
      theStream << "\"";

      theProperty = theProperty->next();
    }

    theStream << ">";

    OTC_DOMNode const* theChild;
    theChild = theNode.firstChild();

    while (theChild != 0)
    {
      theStream << *theChild;

      theChild = theChild->nextSibling();
    }

    theStream << "</";
    theStream << theNode.name();
    theStream << ">";
  }
  else
  {
    if (*theNode.name().string() != EOS)
    {
      if (*theNode.content().string() != EOS)
      {
	theStream << "<";
	theStream << theNode.name();

	OTC_DOMProperty const* theProperty;
	theProperty = theNode.properties();
	while (theProperty != 0)
	{
	  theStream << " ";

	  theStream << theProperty->name();
	  theStream << "=\"";
	  escape(theStream,theProperty->value());
	  theStream << "\"";

	  theProperty = theProperty->next();
	}

	theStream << ">";

	escape(theStream,theNode.content());

	theStream << "</";
	theStream << theNode.name();
	theStream << ">";
      }
      else
      {
	theStream << "<";
	theStream << theNode.name();
	theStream << "/>";
      }
    }
    else
    {
      escape(theStream,theNode.content());
    }
  }

  return theStream;
}

void test2()
{
  OTC_Tracer tracer("void test2()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode;

  tracer() << *theNode << endl;
}

void test3()
{
  OTC_Tracer tracer("void test3()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode("body");

  tracer() << *theNode << endl;;
}

void test4()
{
  OTC_Tracer tracer("void test4()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode("","text");

  tracer() << *theNode << endl;;
}

void test5()
{
  OTC_Tracer tracer("void test5()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode("body","text");

  tracer() << *theNode << endl;;
}

void test6()
{
  OTC_Tracer tracer("void test6()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode("body");
  theNode->createChild("","text");

  tracer() << *theNode << endl;;
}

void test7()
{
  OTC_Tracer tracer("void test7()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode("html");
  theNode->createChild("body","text");

  tracer() << *theNode << endl;;
}

void test8()
{
  OTC_Tracer tracer("void test8()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;
  OTC_DOMNode* theHead;
  OTC_DOMNode* theTitle;

  theNode = new (theCluster) OTC_DOMNode("html");

  theHead = theNode->createChild("head");
  theTitle = theHead->createChild("title","text");

  theNode->createChild("body","text");

  tracer() << *theNode << endl;;
}

void test9()
{
  OTC_Tracer tracer("void test9()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;
  OTC_DOMNode* theHead;
  OTC_DOMNode* theTitle;

  theNode = new (theCluster) OTC_DOMNode("html");

  theNode->setProperty("xmlns","http://www.w3.org/1999/xhtml");
  theNode->setProperty("xml:lang","en");
  theNode->setProperty("lang","en");

  theHead = theNode->createChild("head");
  theTitle = theHead->createChild("title","text");

  theNode->createChild("body","text");

  tracer() << *theNode << endl;;
}

void test10()
{
  OTC_Tracer tracer("void test10()");

  OTC_Cluster theCluster1;
  OTC_DOMNode* theNode;

  OTC_DOMNode* theHead;
  OTC_DOMNode* theTitle;
  OTC_DOMNode* theBody;
  OTC_DOMNode* theParagraph;

  theNode = new (theCluster1) OTC_DOMNode("html");

  theNode->setProperty("xmlns","http://www.w3.org/1999/xhtml");
  theNode->setProperty("xml:lang","en");
  theNode->setProperty("lang","en");

  theHead = theNode->createChild("head");
  theTitle = theHead->createChild("title","text");

  theBody = theNode->createChild("body","text");

  theParagraph = theBody->createChild("p");
  theParagraph->createChild("","text");
  theParagraph->createChild("b","text");
  theParagraph->createChild("","text");

  OTC_Cluster theCluster2;

  theBody->insertChild(theParagraph->clone());
  theBody->insertChild(theParagraph->clone(&theCluster1));
  theBody->insertChild(theParagraph->clone(&theCluster2));

  tracer() << *theNode << endl;;
}

void test11()
{
  OTC_Tracer tracer("void test11()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;
  OTC_DOMNode* theHead;
  OTC_DOMNode* theTitle;
  OTC_DOMNode* theBody;

  theNode = new (theCluster) OTC_DOMNode("html");

  theHead = theNode->createChild("head");
  theTitle = theHead->createChild("title","text");
  theBody = theNode->createChild("body","text");

  theTitle->setProperty("id","1");
  theBody->setProperty("id","2");

  tracer() << *theNode << endl;;

  OTC_DOMNode* theResult;

  theResult = theNode->findNode("head");

  if (theResult != 0)
    tracer() << *theResult << endl;

  theResult = theNode->findNode("titletitle",5);

  if (theResult != 0)
    tracer() << *theResult << endl;

  theResult = theNode->findProperty("id","2");

  if (theResult != 0)
    tracer() << *theResult << endl;

  theResult = theNode->findProperty("id");

  if (theResult != 0)
    tracer() << *theResult << endl;
}

void test12()
{
  OTC_Tracer tracer("void test12()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;
  OTC_DOMNode* theBody;

  theNode = new (theCluster) OTC_DOMNode("html");

  theBody = theNode->createChild("body","text");
  
  theBody->createChild("p","text1");
  theBody->createChild("b","text2");
  theBody->createChild("p","text3");
  theBody->createChild("b","text4");
  theBody->createChild("p","text5");

  OTC_DOMNode* theResult;

  theResult = theBody->findChild("p");

  if (theResult != 0)
    tracer() << *theResult << endl;

  theResult = theBody->findChild("pp",1);

  if (theResult != 0)
    tracer() << *theResult << endl;

  theResult = theResult->findSibling("p");

  if (theResult != 0)
    tracer() << *theResult << endl;

  theResult = theResult->findSibling("pp",1);

  if (theResult != 0)
    tracer() << *theResult << endl;
}

void test13()
{
  OTC_Tracer tracer("void test13()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;
  OTC_DOMNode* theHead;
  OTC_DOMNode* theTitle;

  theNode = new (theCluster) OTC_DOMNode("html");

  theNode->setProperty("markup"," <>&\"\' ");
  theNode->setProperty("escape"," \001\n\176\177\200 ");

  theHead = theNode->createChild("head");
  theTitle = theHead->createChild("title","text");

  theNode->createChild("body","text <>&\"\' \001\n\176\177\200 ");

  tracer() << *theNode << endl;;
}

void test14()
{
  OTC_Tracer tracer("void test14()");

  OTC_Cluster theCluster;
  OTC_DOMNode* theNode;

  theNode = new (theCluster) OTC_DOMNode("root");

  tracer() << "numChildren=" << theNode->numChildren() << endl;

  theNode->createChild("0","0");
  theNode->createChild("1","1");
  theNode->createChild("2","2");

  u_int i;

  tracer() << *theNode << endl;;

  tracer() << "numChildren=" << theNode->numChildren() << endl;

  for (i=0; i<theNode->numChildren(); i++)
    tracer() << *theNode->child(i) << endl;;

  theNode->child(1)->destroy();

  tracer() << *theNode << endl;;

  tracer() << "numChildren=" << theNode->numChildren() << endl;

  for (i=0; i<theNode->numChildren(); i++)
    tracer() << *theNode->child(i) << endl;;

  theNode = theNode->clone();

  tracer() << *theNode << endl;;

  tracer() << "numChildren=" << theNode->numChildren() << endl;

  for (i=0; i<theNode->numChildren(); i++)
    tracer() << *theNode->child(i) << endl;;

  theNode->removeAllChildren();

  tracer() << "numChildren=" << theNode->numChildren() << endl;
}

void test1()
{
  test2();
  test3();
  test4();
  test5();
  test6();
  test7();
  test8();
  test9();
  test10();
  test11();
  test12();
  test13();
  test14();
}

typedef void (*testFunc)();

testFunc tests[] =
{
  test1,
  test2,
  test3,
  test4,
  test5,
  test6,
  test7,
  test8,
  test9,
  test10,
  test11,
  test12,
  test13,
  test14
};

/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
  u_int const numTests = sizeof(tests)/sizeof(tests[0]);

  if (argc != 2)
  {
    cout << numTests << endl;
    return 1;
  }
  else
  {
    int testNum = atoi(argv[1]);
    if (testNum > 0 && u_int(testNum) <= numTests)
    {
      tests[testNum-1]();
      return 0;
    }
    else
      return 1;
  }
}

/* ------------------------------------------------------------------------- */
