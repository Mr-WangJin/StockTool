#pragma once

#include "JKTagContextBase.h"

//  libxml callback context structure
class JKTagTextContext : public JKTagContextBase
{
public:
	JKTagTextContext(const JKString &_tag);
	virtual ~JKTagTextContext();

	const JKString &getTag();

	const std::list<JKString> &getListTagText() { return listTagText; };
	void pushText(const JKString &text);
	void pushText(const char* text, size_t size);

private:
	JKString tag;

	std::list<JKString> listTagText;
};