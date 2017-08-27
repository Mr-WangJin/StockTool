#include "stdafx.h"
#include "JKDatabase.h"
#include "Model/JKProjectModel.h"
#include "Model/JKStockCode.h"
#include "Model/JKStockCodeSetting.h"
#include "Model/JKStockCodeTrade.h"
#include <iostream>



JKDatabase::JKDatabase()
{
	db.open("test.db");

	db.registerBeanClass<JKProjectModel>();
	db.registerBeanClass<JKStockCode>();
	db.registerBeanClass<JKStockCodeSetting>();
	db.registerBeanClass<JKStockCodeTrade>();

	vector<string> msg = db.checkModel();
	for (size_t ci = 0; ci<msg.size(); ci++)
		std::cout << "model check reported: " << msg[ci] << endl;

	//db.dropModel();
	//db.createModel();


}

JKDatabase::~JKDatabase()
{
}
