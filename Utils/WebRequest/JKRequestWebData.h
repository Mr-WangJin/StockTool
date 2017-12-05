#pragma once

#include "json\json.h"
#include <JKFramework\SmartPtr\JKReferenced.h>

class curl_slist;
class JKHtmlData;

class JKRequestWebData : public JKReferenced
{
public:
	JKRequestWebData();
	virtual ~JKRequestWebData();

	JKHtmlData *getHtmlData(const JKString &url);
	int get(
		std::string url,
		std::map<std::string, std::string> const * params,
		std::map<std::string, std::string> const * headers,
		std::string * response) const;

	int post(
		std::string url,
		std::map<std::string, std::string> const * params,
		const std::string & body,
		std::map<std::string, std::string> const * headers,
		std::string * response) const;
	
	int post(
		std::string url,
		std::map<std::string, std::string> const * params,
		std::map<std::string, std::string> const & data,
		std::map<std::string, std::string> const * headers,
		std::string * response) const;
	
	int post(
		std::string url,
		std::map<std::string, std::string> const * params,
		Json::Value const & data,
		std::map<std::string, std::string> const * headers,
		std::string * response) const;

	int post(
		std::string url,
		std::map<std::string, std::string> const * params,
		std::map<std::string, std::string> const * headers,
		std::string * response) const;

	void setConnectTimeout(int connect_timeout)
	{
		this->connect_timeout = connect_timeout;
	}

	void setSocketTimeout(int socket_timeout)
	{
		this->socket_timeout = socket_timeout;
	}

	void setDebug(bool debug)
	{
		this->debug = debug;
	}

private:
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

	void makeUrlencodedForm(std::map<std::string, std::string> const & params, std::string * content) const;
	void appendUrlParams(std::map<std::string, std::string> const & params, std::string* url) const;
	void appendHeaders(std::map<std::string, std::string> const & headers, curl_slist ** slist) const;

private:
	int connect_timeout = 10000;
	int socket_timeout = 10000;
	bool debug = false;
};
