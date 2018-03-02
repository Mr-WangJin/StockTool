#include "stdafx.h"
#include "JKDatabase.h"
#include "Model/JKProjectModel.h"
#include "Model/JKStockCodeModel.h"
#include "Model/JKStockCodeSettingModel.h"
#include "Model/JKStockCodeTradeModel.h"
#include "Model/JKProjectVersionModel.h"
#include "Model\JKStockCodeTradeItemModel.h"
#include <iostream>
#include <QFile>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlError"
#include "QtSql/QSqlQuery"
#include "QVariant"


const JKString JKDatabase::dbName = "JKDatabase.jk";

JKDatabase::JKDatabase()
{
	
}

JKDatabase::~JKDatabase()
{
	if (db)
		db->close();
}

bool JKDatabase::newDatabase(JKString fileName)
{
	JKString fullFileName = fileName;// path + "\\" + dbName;
	//QFile f(QString::fromStdString(fullFileName));
	//if (f.exists())
	//	return false;
	//else
	{
		if (db)
		{
			db->close();
			JK_FreeAndNullptr(db);
		}

		db = new hiberlite::Database();
		db->open(fullFileName);

		db->registerBeanClass<JKProjectVersionModel>();
		db->registerBeanClass<JKProjectModel>();
		db->registerBeanClass<JKProjectSettingModel>();
		db->registerBeanClass<JKStockCodeModel>();
		db->registerBeanClass<JKStockCodeSettingModel>();
		db->registerBeanClass<JKStockCodeTradeModel>();
		db->registerBeanClass<JKStockCodeTradeItemModel>();

		db->dropModel();
		db->createModel();

		bean_ptr<JKProjectVersionModel> refProjectVersion = db->createBean<JKProjectVersionModel>();
		refProjectVersion->version = databaseVersion;
		refProjectVersion->dataVersion = dataVersion;
	}
	return true;
}

bool JKDatabase::openDatabase(JKString fullName)
{
	JKString fullFileName = fullName;
	QFile f(QString::fromStdString(fullFileName));
	if (f.exists())
	{
		this->upgradeDatabase(fullFileName);

		JK_FreeAndNullptr(db);
		db = new hiberlite::Database();
		db->open(fullFileName);

		db->registerBeanClass<JKProjectVersionModel>();
		db->registerBeanClass<JKProjectModel>();
		db->registerBeanClass<JKProjectSettingModel>();
		db->registerBeanClass<JKStockCodeModel>();
		db->registerBeanClass<JKStockCodeSettingModel>();
		db->registerBeanClass<JKStockCodeTradeModel>();
		db->registerBeanClass<JKStockCodeTradeItemModel>();

		this->checkModel();


		return true;
	}
	else
		return false;
}

void JKDatabase::checkModel()
{
	vector<string> msg = db->checkModel();
	for (size_t ci = 0; ci<msg.size(); ci++)
		std::cout << "model check reported: " << msg[ci] << endl;
}

void JKDatabase::upgradeDatabase(JKString fullFileName)
{
	int maxVersion = JKDatabase::databaseVersion;
	int version = this->getDbVersion(fullFileName);
	if (maxVersion == version)
		return;

	QSqlDatabase _db;
	_db = QSqlDatabase::addDatabase("QSQLITE");
	_db.setDatabaseName(QString::fromStdString(fullFileName));
	if (!_db.open())
	{
		QSqlError sqlError = _db.lastError();
		QString szError = sqlError.text();
		throw std::exception(szError.toStdString().c_str());
	}
	else
	{
		try
		{
			if (version > maxVersion)
			{
				throw std::exception("Project version is bigger then program support version, you need update program!");
			}
			/** 按照版本更新依次添加 */
			if (version < 3)
			{
				//创建表  
				QString sqlStr = "CREATE TABLE JKStockCodeTradeItemModel(commission REAL, hiberlite_id INTEGER PRIMARY KEY AUTOINCREMENT, id TEXT, sellCount INTEGER, sellPrice REAL, sellSumCount INTEGER, stampTax REAL, transfer REAL);";
				QSqlQuery query(_db);
				query.prepare(sqlStr);		
				if (!query.exec())			
				{
					JKString error = "upgrade to 3 error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
				sqlStr = "CREATE TABLE JKStockCodeTradeModel_vecSellItem_items (hiberlite_entry_indx INTEGER, hiberlite_id INTEGER PRIMARY KEY AUTOINCREMENT, hiberlite_parent_id INTEGER, item_id INTEGER);";
				query.prepare(sqlStr);
				if (!query.exec())			
				{
					JKString error = "upgrade to 3 error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}
			if (version < 4)
			{
				//添加字段DataVersion
				QString sqlStr = "alter table JKProjectVersionModel add column dataVersion INTEGER default 0;";
				QSqlQuery query(_db);
				query.prepare(sqlStr);		
				if (!query.exec())			
				{
					JKString error = "add DataVersion Field error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}
			if (version < 5)
			{
				QString sqlStr = "alter table JKStockCodeTradeItemModel add column realEarning REAL default -1;";
				QSqlQuery query(_db);
				query.prepare(sqlStr);
				if (!query.exec())
				{
					JKString error = "add DataVersion Field error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}
			if (version < 6)
			{
				QString sqlStr = "alter table JKStockCodeTradeModel add column holdType integer default 0;";
				QSqlQuery query(_db);
				query.prepare(sqlStr);
				if (!query.exec())
				{
					JKString error = "add holdType Field error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}
			if (version < 7)
			{
				QString sqlStr = "CREATE TABLE JKProjectSettingModel (alertPercent REAL, hiberlite_id INTEGER PRIMARY KEY AUTOINCREMENT, id TEXT, isAlert INTEGER, isStartCrawl INTEGER);";
				QSqlQuery query(_db);
				query.prepare(sqlStr);
				if (!query.exec())
				{
					JKString error = "create table JKProjectSettingModel is error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
				sqlStr = "alter table JKProjectModel add column projectSetting_id integer default -1;";
				query.prepare(sqlStr);
				if (!query.exec())
				{
					JKString error = "add projectSetting_id Field error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}
			if (version < 8)
			{
				QString sqlStr = "alter table JKProjectSettingModel add column tableShowType integer default 1;";
				QSqlQuery query(_db);
				query.prepare(sqlStr);
				if (!query.exec())
				{
					JKString error = "add tableShowType Field error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}
			if (version < 9)
			{
				QString sqlStr = "alter table JKProjectSettingModel add column downAlertPercent float default -0.03;";
				QSqlQuery query(_db);
				query.prepare(sqlStr);
				if (!query.exec())
				{
					JKString error = "add downAlertPercent Field error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
			}

			// 更新数据表格式版本号
			QString querySqlStr = QString("update JKProjectVersionModel Set version = %1").arg(maxVersion);
			QSqlQuery query(querySqlStr, _db);
			if (!query.exec())
			{
				throw std::exception("update ProjectVersion error!");
			}
			/**
			CREATE TABLE temp (hiberlite_id INTEGER PRIMARY KEY AUTOINCREMENT, version INTEGER); --创建表
			insert into temp(version) values(3);					--插入表
			alter table temp rename to JKProjectVersionModel;		--重命名
			drop table JKProjectVersionModel;						--删除表
			
			*/
			
		}
		catch (const std::exception& e)
		{
			_db.close();

			throw e;
		}
		catch (...)
		{
			_db.close();
		}

		_db.close();
	}
	
}

int JKDatabase::getDbDataVersion()
{
	assert(db);
	std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<JKProjectVersionModel>();
	if (vecIds.size() > 0)
	{
		hiberlite::bean_ptr<JKProjectVersionModel> ptrProjectVersionModel = SingleDB->loadBean<JKProjectVersionModel>(vecIds[0]);
		return ptrProjectVersionModel->dataVersion;
	}
}

int JKDatabase::getDataVersion()
{
	return dataVersion;
}

void JKDatabase::updateDbDataVersion()
{
	assert(db);
	std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<JKProjectVersionModel>();
	if (vecIds.size() > 0)
	{
		hiberlite::bean_ptr<JKProjectVersionModel> ptrProjectVersionModel = SingleDB->loadBean<JKProjectVersionModel>(vecIds[0]);

		ptrProjectVersionModel->dataVersion = dataVersion;
	}
}

int JKDatabase::getDbVersion(JKString _dbFullFileName)
{
	QSqlDatabase _db;
	_db = QSqlDatabase::addDatabase("QSQLITE");
	_db.setDatabaseName(QString::fromStdString(_dbFullFileName));
	if (!_db.open())
	{
		QSqlError sqlError = _db.lastError();
		QString szError = sqlError.text();
		throw std::exception(szError.toStdString().c_str());
	}
	int version = -1;
	try {
		JKString querySqlStr = "select version from JKProjectVersionModel";
		QSqlQuery query(QString::fromStdString(querySqlStr), _db);
		if (query.exec())
		{
			query.next();
			version = query.value(0).toInt();
		}
		else
		{
			JKString error = JKString("exec sql error!") + querySqlStr;
			throw std::exception(error.c_str());
		}
	}
	catch (std::exception &e)
	{

		_db.close();
		throw e;
	}
	
	_db.close();
	return version;
}