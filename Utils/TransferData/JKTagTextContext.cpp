#include "stdafx.h"
#include "JKTagTextContext.h"

JKTagTextContext::JKTagTextContext(const JKString & _tag)
	: JKTagContextBase()
	, tag(_tag)
{}

JKTagTextContext::~JKTagTextContext()
{
}

const JKString & JKTagTextContext::getTag()
{
	return tag;
}

void JKTagTextContext::pushText(const JKString & text)
{
	listTagText.push_back(text);
}

void JKTagTextContext::pushText(const char * text, size_t size)
{
	JKString tempText = JKString(text);
	listTagText.push_back(tempText);
}
