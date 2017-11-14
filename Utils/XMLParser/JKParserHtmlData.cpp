#include "stdafx.h"
#include "JKParserHtmlData.h"
#include <libxml/HTMLparser.h>
#include "../WebRequest/JKRequestWebData.h"
#include <iostream>
#include <string.h>  
#include <JKUtil/JKStringUtil.h>


#ifdef _MSC_VER
#define COMPARE(a, b) (!_stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

//
//  libxml callback context structure
//
struct Context
{
	Context() : addTitle(false) { }

	bool addTitle;
	std::string title;
};

//
//  libcurl write callback function
//
static int writer(char *data, size_t size, size_t nmemb,
	std::string *writerData)
{
	if (writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}


static void StartElement(void *voidContext,
	const xmlChar *name,
	const xmlChar **attributes)
{
	Context *context = (Context *)voidContext;

	//assert(!COMPARE((char *)name, "strong"));
		
	if (COMPARE((char *)name, "strong")) {
		context->title = "";
		context->addTitle = true;
	}
	(void)attributes;
}

//
//  libxml end element callback function
//

static void EndElement(void *voidContext,
	const xmlChar *name)
{
	Context *context = (Context *)voidContext;

	if (COMPARE((char *)name, "strong"))
		context->addTitle = false;
}

//
//  Text handling helper function
//

static void handleCharacters(Context *context,
	const xmlChar *chars,
	int length)
{
	if (context->addTitle)
		context->title.append((char *)chars, length);
}

//
//  libxml PCDATA callback function
//

static void Characters(void *voidContext,
	const xmlChar *chars,
	int length)
{
	Context *context = (Context *)voidContext;

	handleCharacters(context, chars, length);
}

//
//  libxml CDATA callback function
//

static void cdata(void *voidContext,
	const xmlChar *chars,
	int length)
{
	Context *context = (Context *)voidContext;

	handleCharacters(context, chars, length);
}

//
//  libxml SAX callback structure
//

static htmlSAXHandler saxHandler =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	StartElement,
	EndElement,
	NULL,
	Characters,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	cdata,
	NULL
};

//
//  Parse given (assumed to be) HTML text and return the title
//

static void parseHtml(const std::string &html, std::string &title)
{
	htmlParserCtxtPtr ctxt;
	Context context;

	ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "", XML_CHAR_ENCODING_UTF8);

	htmlParseChunk(ctxt, html.c_str(), html.size(), 0);
	//htmlParseChunk(ctxt, "", 0, 1);

	htmlFreeParserCtxt(ctxt);

	title = context.title;
}

JKParserHtmlData::JKParserHtmlData()
{

}

JKParserHtmlData::~JKParserHtmlData()
{

}

void traverse_dom_attr(xmlAttr* attr)
{
	if (NULL == attr)
	{
		return;
	}

	
}

void traverse_dom_trees(xmlNodePtr currentNode)
{
	if (NULL == currentNode)
	{
		return;
	}

	//xmlNodePtr currentNode = (xmlNodePtr)_htmlDocument;
	bool beginOfNode = true;
	while (currentNode) 
	{
		// output node if it is an element 
		if (beginOfNode) 
		{
			if (currentNode->type == XML_ELEMENT_NODE) 
			{
				for (xmlAttrPtr attrNode = currentNode->properties; attrNode; attrNode = attrNode->next) 
				{ 
					xmlNodePtr contents = attrNode->children;   
					std::cout << attrNode->name << contents->content<<std::endl;
				}  
				std::cout << currentNode->name << std::endl;
				std::cout << currentNode->content << std::endl;

			} else if (currentNode->type == XML_TEXT_NODE) 
			{
				std::cout << currentNode->content << std::endl;
			} else if (currentNode->type == XML_COMMENT_NODE) 
			{ 
				std::cout << currentNode->name << std::endl;
			} 
		}   
		if (beginOfNode && currentNode->children)
		{
			currentNode = currentNode->children; 
			beginOfNode = true; 
		}
		else if (beginOfNode && currentNode->next) 
		{ 
			currentNode = currentNode->next; beginOfNode = true; 
		}
		else 
		{
			currentNode = currentNode->parent; beginOfNode = false;

			if (currentNode && currentNode->type == XML_ELEMENT_NODE) 
			{
				std::cout << currentNode->name << std::endl;
			} 
		}
	}

	//xmlChar *name, *value;
	//xmlNodePtr root, node = a_node, detail;
	//for (node = node->children; node; node = node->next) {
	//	name = xmlGetProp(node, BAD_CAST"id");
	//	value = xmlNodeGetContent(node);
	//	printf("this is %s:\n%s\n", (char*)name, (char*)value);   //get value, CDATA is not parse and don't take into value

	//	xmlFree(name);
	//	xmlFree(value);
	//	
	//}

	//std::stack<xmlNode*> nodeStack;
	//nodeStack.push(a_node);
	//while (!nodeStack.empty())
	//{
	//	xmlNode *curNode = nodeStack.top();
	//	nodeStack.pop();

	//	value = xmlNodeGetContent(node);
	//	if (value != nullptr)
	//		std::cout << "value:" << value << std::endl;
	//	xmlFree(value);

	//	if (curNode->name)
	//		std::cout << "name : " << curNode->name <<std::endl;
	//	if (curNode->content)
	//		std::cout << "content : " << curNode->content << std::endl;
	//	
	//	if (curNode->next)
	//		nodeStack.push(curNode->next);
	//	if (curNode->children)
	//		nodeStack.push(curNode->children);

	//	if (curNode->properties)
	//	{
	//		std::stack<xmlAttr*> attrNodeStack;
	//		attrNodeStack.push(curNode->properties);
	//		while (!attrNodeStack.empty())
	//		{
	//			xmlAttr *curAttr = attrNodeStack.top();
	//			attrNodeStack.pop();

	//			if (curAttr->name)
	//				std::cout << "name : " << curNode->name << std::endl;
	//			if (curAttr->doc)
	//				std::cout << "content : " << curNode->doc << std::endl;


	//			if (curAttr->next)
	//				attrNodeStack.push(curAttr->next);
	//			if (curAttr->children)
	//				nodeStack.push(curAttr->children);
	//		}
	//	}
	//	
	//}
}

void JKParserHtmlData::test(JKHtmlData* htmlData)
{
	if (htmlData == nullptr)
		return;

	JKString data = JKString(htmlData->memory);

	//char* p = g2u(htmlData->memory, htmlData->size);

	JKString title;
	parseHtml(data, title);

	std::wstring t = JKStringUtil::UTF8ToUnicode(title);
	std::string t1 = UTF8ToANSI(title);

	std::cout << title;



	//int nOutLen = 2 * size - 1;
	//char* szOut = (char*)malloc(nOutLen);
	//if (-1 == code_convert("utf-8", "gb2312", inbuf, size, szOut, nOutLen))
	//{
	//	free(szOut);
	//	szOut = NULL;
	//}
	//return szOut;

	
}


