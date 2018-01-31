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

	/** ��ȡ���ݱ��е� ���� �汾�� */
	int getDbDataVersion();
	/** ��ȡ���ݱ�� ��ʽ �汾�� */
	int getDbVersion(JKString _dbFullFileName);

	/** ��ȡ����� ���� �汾�� */
	int getDataVersion();
	/** ���¹��̵� ���� �汾�� */
	void updateDbDataVersion();

public:
	hiberlite::Database* db = nullptr;
	/** ���ݸ�ʽ�汾 */
	static const int databaseVersion = 8;
	/** ���ݰ汾 */
	static const int dataVersion = 1;
	static const JKString dbName;

};




