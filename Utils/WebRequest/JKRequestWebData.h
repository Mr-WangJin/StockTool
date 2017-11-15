#pragma once

#include <JKFramework\SmartPtr\JKReferenced.h>

class JKHtmlData;

class JKRequestWebData : public JKReferenced
{
public:
	JKRequestWebData();
	virtual ~JKRequestWebData();

	JKHtmlData *getHtmlData(const JKString &url);

private:
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);


};
