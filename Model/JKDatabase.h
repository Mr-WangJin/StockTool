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
	void upgradeDatabase(JKString fullFileName);
private:
	int getDbVersion(JKString _dbFullFileName);

public:
	hiberlite::Database* db = nullptr;
	static const int databaseVersion = 2;
	static const JKString dbName;

};




