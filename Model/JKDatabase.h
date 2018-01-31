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

	/** 获取数据表中的 数据 版本号 */
	int getDbDataVersion();
	/** 获取数据表的 格式 版本号 */
	int getDbVersion(JKString _dbFullFileName);

	/** 获取软件的 数据 版本号 */
	int getDataVersion();
	/** 更新工程的 数据 版本号 */
	void updateDbDataVersion();

public:
	hiberlite::Database* db = nullptr;
	/** 数据格式版本 */
	static const int databaseVersion = 8;
	/** 数据版本 */
	static const int dataVersion = 1;
	static const JKString dbName;

};




