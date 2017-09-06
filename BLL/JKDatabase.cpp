#include "stdafx.h"
#include "JKDatabase.h"
#include "Model/JKProjectModel.h"
#include "Model/JKStockCodeModel.h"
#include "Model/JKStockCodeSettingModel.h"
#include "Model/JKStockCodeTradeModel.h"
#include <iostream>



JKDatabase::JKDatabase()
{
	db.open("test.db");

	db.registerBeanClass<JKProjectModel>();
	db.registerBeanClass<JKStockCodeModel>();
	db.registerBeanClass<JKStockCodeSettingModel>();
	db.registerBeanClass<JKStockCodeTradeModel>();

	//db.dropModel();
	//db.createModel();

}

JKDatabase::~JKDatabase()
{
}

void JKDatabase::dropAllTable()
{
	db.dropModel();
}

void JKDatabase::checkModel()
{
	vector<string> msg = db.checkModel();
	for (size_t ci = 0; ci<msg.size(); ci++)
		std::cout << "model check reported: " << msg[ci] << endl;
}