#include "stdafx.h"
#include "JKStockCodeModel.h"
#include "JKModelFactory.h"



void JKStockCodeModel::addStockCodeTrade(bean_ptr<JKStockCodeTradeModel> _refStockCodeTradeModel)
{
	vecCodeTrade.push_back(_refStockCodeTradeModel);
}

bool JKStockCodeModel::delStockCodeTrade(bean_ptr<JKStockCodeTradeModel> _refStockCodeTradeModel)
{
	vector<bean_ptr<JKStockCodeTradeModel>>::iterator iterTemp = std::find(vecCodeTrade.begin(), vecCodeTrade.end(), _refStockCodeTradeModel);
	if (iterTemp != vecCodeTrade.end())
	{
		vecCodeTrade.erase(iterTemp);
		return true;
	}

	return false;
}


HIBERLITE_EXPORT_CLASS(JKStockCodeModel)


