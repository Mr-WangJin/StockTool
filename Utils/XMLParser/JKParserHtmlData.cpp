#include "stdafx.h"
#include "JKParserHtmlData.h"
#include <libxml/HTMLparser.h>
#include "Utils/WebRequest/JKRequestWebData.h"
#include "Utils/TransferData/JKWebData.h"
#include <iostream>
#include <string.h>  
#include <JKUtil/JKStringUtil.h>
#include "Utils/XMLParser/JKTagTextContext.h"

#ifdef _MSC_VER
#define COMPARE(a, b) (!_stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif


static void StartElement(void *voidContext,	const xmlChar *name, const xmlChar **attributes)
{
	JKTagTextContext *context = (JKTagTextContext *)voidContext;

	//assert(!COMPARE((char *)name, "strong"));
		
	if (COMPARE((char *)name, context->getTag().c_str())) {
		context->setState(true);
	}
	(void)attributes;
}

//  libxml end element callback function
static void EndElement(void *voidContext, const xmlChar *name)
{
	JKTagTextContext *context = (JKTagTextContext *)voidContext;

	if (COMPARE((char *)name, context->getTag().c_str()))
		context->setState(false);
}

//  Text handling helper function
static void handleCharacters(JKTagTextContext *context, const xmlChar *chars, int length)
{
	if (context->getState())
		context->pushText((char *)chars, length);
}

//  libxml PCDATA callback function
static void Characters(void *voidContext, const xmlChar *chars, int length)
{
	JKTagTextContext *context = (JKTagTextContext *)voidContext;

	handleCharacters(context, chars, length);
}

//  libxml CDATA callback function
static void cdata(void *voidContext, const xmlChar *chars, int length)
{
	//JKTagTextContext *context = (JKTagTextContext *)voidContext;

	//handleCharacters(context, chars, length);
}

//  libxml SAX callback structure
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


JKParserHtmlData::JKParserHtmlData()
{

}

JKParserHtmlData::~JKParserHtmlData()
{

}

bool JKParserHtmlData::parserTagTextAttribute(JKHtmlData * htmlData, JKTagTextContext * tagContext)
{
	if (htmlData == nullptr || tagContext == nullptr)
		return false;

	htmlParserCtxtPtr ctxt;

	ctxt = htmlCreatePushParserCtxt(&saxHandler, tagContext, "", 0, "", XML_CHAR_ENCODING_UTF8);

	htmlParseChunk(ctxt, htmlData->getMemory(), htmlData->getSize(), 0);
	//htmlParseChunk(ctxt, "", 0, 1);

	htmlFreeParserCtxt(ctxt);

	return true;
}


