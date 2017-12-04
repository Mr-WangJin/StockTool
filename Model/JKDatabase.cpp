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
}

bool JKDatabase::newDatabase(JKString path)
{
	JKString fullFileName = path + "\\" + dbName;
	QFile f(QString::fromStdString(fullFileName));
	if (f.exists())
		return false;
	else
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
		db->registerBeanClass<JKStockCodeModel>();
		db->registerBeanClass<JKStockCodeSettingModel>();
		db->registerBeanClass<JKStockCodeTradeModel>();
		db->registerBeanClass<JKStockCodeTradeItemModel>();

		db->dropModel();
		db->createModel();

		bean_ptr<JKProjectVersionModel> refProjectVersion = db->createBean<JKProjectVersionModel>();
		refProjectVersion->version = databaseVersion;
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
		db->registerBeanClass<JKStockCodeModel>();
		db->registerBeanClass<JKStockCodeSettingModel>();
		db->registerBeanClass<JKStockCodeTradeModel>();
		db->registerBeanClass<JKStockCodeTradeItemModel>();


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
			if (version < maxVersion)//3
			{
				QString sqlStr = "CREATE TABLE JKStockCodeTradeItemModel(commission REAL, hiberlite_id INTEGER PRIMARY KEY AUTOINCREMENT, id TEXT, sellCount INTEGER, sellPrice REAL, sellSumCount INTEGER, sellSumTax REAL, stampTax REAL, transfer REAL);";
				QSqlQuery query(_db);
				query.prepare(sqlStr);		//创建表  
				if (!query.exec())			//查看创建表是否成功  
				{
					JKString error = "upgrade to 3 error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}
				sqlStr = "CREATE TABLE JKStockCodeTradeModel_vecSellItem_items (hiberlite_entry_indx INTEGER, hiberlite_id INTEGER PRIMARY KEY AUTOINCREMENT, hiberlite_parent_id INTEGER, item_id INTEGER);";
				query.prepare(sqlStr);
				if (!query.exec())			//查看创建表是否成功  
				{
					JKString error = "upgrade to 3 error! " + query.lastError().text().toStdString();
					throw std::exception(error.c_str());
				}

			}
			
			QString querySqlStr = QString("update JKProjectVersionModel Set version = %1").arg(maxVersion);
			QSqlQuery query(querySqlStr, _db);
			if (!query.exec())
			{
				throw std::exception("update ProjectVersion error!");
			}
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
		JKString querySqlStr = "select * from JKProjectVersionModel";
		QSqlQuery query(QString::fromStdString(querySqlStr), _db);
		if (query.exec())
		{
			query.next();
			version = query.value(1).toInt();
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