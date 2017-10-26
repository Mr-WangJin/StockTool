#include "stdafx.h"
#include "JKExeVersion.h"
#include "JKFile/JKFileIO.h"
#include <json/reader.h>


const int exeVersion = 1;
const JKString exeVersionFileName = "version";


JKExeVersion::JKExeVersion()
{
	this->deSerializable();
}

JKExeVersion::~JKExeVersion()
{

}

bool JKExeVersion::serializable()
{
	return false;
}

bool JKExeVersion::deSerializable()
{
	JKString data;
	size_t dataLen = 0;

	if (JKFileIO::ReadFile(exeVersionFileName, JKFileIO::Read, data, dataLen))
	{
		Json::Reader reader;
		Json::Value result;
		reader.parse(data, result);

		Json::Value projects = result["projects"];
		for (int i = 0; i < projects.size(); ++i)
		{
			Json::Value project = projects[i];

			JKString path = project["path"].asString();

		}
	}

	return false;
}
