#include "stdafx.h"
#include "JKDatabase.h"
#include "Model/JKProjectModel.h"
#include "Model/JKStockCode.h"
#include "Model/JKStockCodeSetting.h"
#include "Model/JKStockCodeTrade.h"



JKDatabase::JKDatabase()
{
	db.open("test.db");

	db.registerBeanClass<JKProjectModel>();
	db.registerBeanClass<JKStockCode>();
	db.registerBeanClass<JKStockCodeSetting>();
	db.registerBeanClass<JKStockCodeTrade>();

	db.dropModel();
	db.createModel();


}

JKDatabase::~JKDatabase()
{
}
