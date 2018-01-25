#include "stdafx.h"
#include "JKRecentProject.h"
#include <json/reader.h>
#include <JKFile/JKFileIO.h>


JKRecentProject::JKRecentProject(const JKString &_recentFilePath)
{
	recentFilePath = _recentFilePath;
	this->initialize();
}

JKRecentProject::~JKRecentProject()
{
}

void JKRecentProject::addRecentProject(const JKString & name, const JKString & filePath)
{
	ProjectItem projectItem = { "", "" };

	projectItem.name = name;
	projectItem.filePath = filePath;

	listItems.push_back(projectItem);
}

void JKRecentProject::saveToFile(const JKString & file)
{
	Json::Value root;

	this->serializable(root);

	std::string out = root.toStyledString();
	JKFileIO::WriteFile(file, JKFileIO::Write_Plus, out, out.size());
}

JKString JKRecentProject::getNameByIndex(int count)
{
	std::list<ProjectItem>::iterator iter = listItems.begin();
	for (int i = 0; i< count; ++i)
	{
		iter++;
	}
	if (iter != listItems.end())
		return iter->name;

	return "";
}

JKString JKRecentProject::getFilePathByIndex(int count)
{
	std::list<ProjectItem>::iterator iter = listItems.begin();
	for (int i = 0; i < count; ++i)
	{
		iter++;
	}
	if (iter != listItems.end())
		return iter->filePath;

	return "";
}

void JKRecentProject::initialize()
{
	JKString data;
	size_t dataLen = 0;

	if (JKFileIO::ReadFile(recentFilePath, JKFileIO::Read, data, dataLen))
	{
		Json::Reader reader;
		Json::Value result;
		reader.parse(data, result);

		this->deSerializable(result);
	}
}

void JKRecentProject::serializable(Json::Value & result)noexcept
{
	Json::Value arrayObj;
	std::list<ProjectItem>::iterator iter = listItems.begin();
	int count = 0;
	for (; iter != listItems.end(); ++iter)
	{
		Json::Value item;
		item["name"] = iter->name;
		item["filePath"] = iter->filePath;

		arrayObj.append(item);
		if (count > 5)
			break;;
	}

	result["recentProject"] = arrayObj;
}

void JKRecentProject::deSerializable(Json::Value &result)noexcept
{
	Json::Value vRecentProject = result["recentProject"];
	//version = result["version"].asUInt();
	for (int i = 0; i < vRecentProject.size(); ++i)
	{
		Json::Value item = vRecentProject[i];
		ProjectItem projectItem = { "", "" };

		projectItem.name = item["name"].asString();
		projectItem.filePath = item["filePath"].asString();

		listItems.push_back(projectItem);
	}
}
