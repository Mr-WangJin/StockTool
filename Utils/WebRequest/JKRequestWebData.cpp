#include "stdafx.h"
#include "JKRequestWebData.h"
#include "Utils/TransferData/JKWebData.h"
#include <curl/curl.h>
#include <sstream>
#include <string>


JKRequestWebData::JKRequestWebData()
{
	
}

JKRequestWebData::~JKRequestWebData()
{

}

size_t JKRequestWebData::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	JKHtmlData *mem = (JKHtmlData *)userp;

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
		CURL *curl_handle = curl_easy_init();
		

		curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 3000);		//连接超时
		curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, 3000);			//超时
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, FALSE);
		curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());			//设置url

		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, JKRequestWebData::WriteMemoryCallback);	//write function
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)data);

		/* some servers don't like requests that are made without a user-agent field, so we provide one */
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		/* get it! */
		CURLcode res = curl_easy_perform(curl_handle);

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

inline size_t onWriteData(void * buffer, size_t size, size_t nmemb, void * userp)
{
	std::string * str = dynamic_cast<std::string *>((std::string *)userp);
	str->append((char *)buffer, size * nmemb);
	return nmemb;
}

int JKRequestWebData::get(std::string url, std::map<std::string, std::string> const * params, std::map<std::string, std::string> const * headers, std::string * response) const
{
	CURL * curl = curl_easy_init();
	struct curl_slist * slist = NULL;
	if (headers) {
		this->appendHeaders(*headers, &slist);
	}
	if (params) {
		this->appendUrlParams(*params, &url);
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, true);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, this->connect_timeout);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, this->socket_timeout);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, this->debug);

	int status_code = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_slist_free_all(slist);

	return status_code;
}

int JKRequestWebData::post(std::string url, std::map<std::string, std::string> const * params, const std::string & body, std::map<std::string, std::string> const * headers, std::string * response) const
{
	struct curl_slist * slist = NULL;
	CURL * curl = curl_easy_init();
	if (headers) {
		this->appendHeaders(*headers, &slist);
	}
	if (params) {
		this->appendUrlParams(*params, &url);
	}

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
	curl_easy_setopt(curl, CURLOPT_POST, true);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, true);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, this->connect_timeout);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, this->socket_timeout);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, this->debug);

	int status_code = curl_easy_perform(curl);

	curl_easy_cleanup(curl);
	curl_slist_free_all(slist);

	return status_code;
}

int JKRequestWebData::post(std::string url, std::map<std::string, std::string> const * params, std::map<std::string, std::string> const & data, std::map<std::string, std::string> const * headers, std::string * response) const
{
	std::string body;
	this->makeUrlencodedForm(data, &body);
	return this->post(std::move(url), params, body, headers, response);
}

int JKRequestWebData::post(std::string url, std::map<std::string, std::string> const * params, Json::Value const & data, std::map<std::string, std::string> const * headers, std::string * response) const
{
	std::string body;
	Json::StreamWriterBuilder swb;
	std::unique_ptr<Json::StreamWriter> writer(swb.newStreamWriter());
	std::ostringstream os;
	writer->write(data, &os);
	body = os.str();
	std::map<std::string, std::string> temp_headers;
	if (headers) {
		std::map<std::string, std::string> temp_headers(*headers);
	}

	temp_headers["Content-Type"] = "application/json";
	return this->post(url.c_str(), params, body, &temp_headers, response);
}

int JKRequestWebData::post(std::string url, std::map<std::string, std::string> const * params, std::map<std::string, std::string> const * headers, std::string * response) const
{
	const static std::string EMPTY_STRING;
	return this->post(std::move(url), params, EMPTY_STRING, headers, response);
}

void JKRequestWebData::makeUrlencodedForm(std::map<std::string, std::string> const & params, std::string * content) const
{
	content->clear();
	std::map<std::string, std::string>::const_iterator it;
	for (it = params.begin(); it != params.end(); it++)
	{
		char * key = curl_escape(it->first.c_str(), (int)it->first.size());
		char * value = curl_escape(it->second.c_str(), (int)it->second.size());
		*content += key;
		*content += '=';
		*content += value;
		*content += '&';
		curl_free(key);
		curl_free(value);
	}
}

void JKRequestWebData::appendUrlParams(std::map<std::string, std::string> const & params, std::string* url) const
{
	if (params.empty()) {
		return;
	}
	std::string content;
	this->makeUrlencodedForm(params, &content);
	bool url_has_param = false;
	for (const auto& ch : *url) {
		if (ch == '?') {
			url_has_param = true;
			break;
		}
	}
	if (url_has_param) {
		url->append("&");
	}
	else {
		url->append("?");
	}
	url->append(content);
}

void JKRequestWebData::appendHeaders(std::map<std::string, std::string> const & headers, curl_slist ** slist) const
{
	std::ostringstream ostr;
	std::map<std::string, std::string>::const_iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
	{
		ostr << it->first << ":" << it->second;
		*slist = curl_slist_append(*slist, ostr.str().c_str());
		ostr.str("");
	}
}