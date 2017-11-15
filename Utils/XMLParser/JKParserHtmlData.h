#pragma once

class JKHtmlData;
class JKTagTextContext;

class JKParserHtmlData
{
public:
	JKParserHtmlData();
	virtual ~JKParserHtmlData();

	JK_DISABLE_COPY(JKParserHtmlData);

	static bool parserTagTextAttribute(JKHtmlData* htmlData, JKTagTextContext* tagContext);

};
