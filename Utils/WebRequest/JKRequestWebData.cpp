#include "stdafx.h"
#include "JKRequestWebData.h"

#include <curl/curl.h>


JKHtmlData::JKHtmlData()
{
	memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
	size = 0;    /* no data at this point */
}
JKHtmlData::~JKHtmlData()
{
	free(memory);
}

////////////////////////////////////////////////////////////////////////////////////////////

JKRequestWebData::JKRequestWebData()
{
	
}

JKRequestWebData::~JKRequestWebData()
{

}

size_t JKRequestWebData::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct JKHtmlData *mem = (struct JKHtmlData *)userp;

	mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

JKHtmlData * JKRequestWebData::getHtmlData(const JKString &url)
{
	JKHtmlData *data = new JKHtmlData();

	try {
		CURL *curl_handle;
		CURLcode res;
		/* init the curl session */
		curl_handle = curl_easy_init();

		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, FALSE);
		/* specify URL to get */
		curl_easy_setopt(curl_handle, CURLOPT_URL, "https://gupiao.baidu.com/stock/sh600100.html");

		/* send all data to this function  */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, JKRequestWebData::WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)data);

		/* some servers don't like requests that are made without a user-agent field, so we provide one */
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		/* get it! */
		res = curl_easy_perform(curl_handle);

		/* check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			throw std::exception(curl_easy_strerror(res));
		}
		else {
			printf("%lu bytes retrieved\n", (long)data->size);
		}

		/* cleanup curl stuff */
		curl_easy_cleanup(curl_handle);
	}
	catch (...)
	{
		JK_FreeAndNullptr(data);
	}
	return data;
}