#include "stdafx.h"
#include "JKWebData.h"

JKHtmlData::JKHtmlData()
{
	memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	size = 0;					/* no data at this point */
}
JKHtmlData::~JKHtmlData()
{
	free(memory);
}