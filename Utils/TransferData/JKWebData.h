#pragma once


#include "JKTransferDataBase.h"
#include "JKCommon/JKDef.h"

class JKRequestWebData;

class JKHtmlData : public JKTransferDataBase
{
	friend class JKRequestWebData;
public:
	JKHtmlData();
	virtual ~JKHtmlData();

	JK_DISABLE_COPY(JKHtmlData);

	const char* getMemory() { return memory; };
	size_t getSize() { return size; };

private:
	char *memory;
	size_t size;
};