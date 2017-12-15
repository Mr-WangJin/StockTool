#pragma once

#include <JKFramework\JKSingleton.h>

#include <hiberlite.h>


class JKDatabase
{
public:
	JKDatabase();
	~JKDatabase();

	bool newDatabase(JKString path);
	bool openDatabase(JKString fullName);

	void checkModel();
	/** 升级表接口 */
	void upgradeDatabase(JKString fullFileName);

	/** 获取数据表中的数据版本号 */
	int getDbDataVersion();
	/** 获取软件的数据版本号 */
	int getDataVersion();
	void updateDbDataVersion();
private:
	int getDbVersion(JKString _dbFullFileName);

public:
	hiberlite::Database* db = nullptr;
	static const int databaseVersion = 4;
	static const int dataVersion = 1;
	static const JKString dbName;

};




