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
	/** ������ӿ� */
	void upgradeDatabase(JKString fullFileName);

	/** ��ȡ���ݱ��е����ݰ汾�� */
	int getDbDataVersion();
	/** ��ȡ��������ݰ汾�� */
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




