#include "stdafx.h"
#include "JKParserHtmlData.h"
#include <libxml/HTMLparser.h>
#include "../WebRequest/JKRequestWebData.h"
#include <iostream>


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
	htmlDocPtr doc;
	xmlNode *roo_element = NULL;

	//if (argc != 2)
	//{
	//	printf("\nInvalid argument\n");
	//	return(1);
	//}

	/* Macro to check API for match with the DLL we are using */
	LIBXML_TEST_VERSION

	doc = htmlParseDoc(xmlCharStrndup(htmlData->memory, htmlData->size), "");
	if (doc == NULL)
	{
		fprintf(stderr, "Document not parsed successfully.\n");
		return ;
	}

	roo_element = xmlDocGetRootElement(doc);

	if (roo_element == NULL)
	{
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return ;
	}

	printf("Root Node is %s\n", roo_element->name);
	traverse_dom_trees(roo_element);

	xmlFreeDoc(doc);       // free document
	xmlCleanupParser();    // Free globals
}


