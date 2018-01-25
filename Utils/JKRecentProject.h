#pragma once

#include <json/value.h>

class JKRecentProject
{
	typedef struct StrProjectItem
	{
		JKString name;
		JKString filePath;
	} ProjectItem;
public:
	JKRecentProject(const JKString &_recentFilePath);
	~JKRecentProject();

	void addRecentProject(const JKString &name, const JKString &filePath);
	void saveToFile(const JKString &file);

	int getCount() { return listItems.size(); };
	JKString getNameByIndex(int count);
	JKString getFilePathByIndex(int count);

private:
	void initialize();

	void serializable(Json::Value& result) noexcept;
	void deSerializable(Json::Value &) noexcept;

private:
	std::list<ProjectItem> listItems;
	JKString recentFilePath;
};
