#include "stdafx.h"
#include "JKStockCodeModel.h"
#include "JKModelFactory.h"



void JKStockCodeModel::addStockCodeTrade(bean_ptr<JKStockCodeTradeModel> _refStockCodeTradeModel)
{
	vecCodeTrade.push_back(_refStockCodeTradeModel);
}


HIBERLITE_EXPORT_CLASS(JKStockCodeModel)


