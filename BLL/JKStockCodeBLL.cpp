#include "stdafx.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeModel.h"
#include "JKStockCodeTradeBLL.h"



JKString JKStockCodeBLL::getName()
{
	return refJKStockCodeModel->name;
}

JKRef_Ptr<JKStockCodeTradeBLL> JKStockCodeBLL::newStockCodeTrade()
{
	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = new JKStockCodeTradeBLL();

	refJKStockCodeModel->addStockCodeTrade(_refStockCodeTrade->getModel());
	return _refStockCodeTrade;
}

vector<JKRef_Ptr<JKStockCodeTradeBLL>> JKStockCodeBLL::getAllTrades()
{
	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecTrades;

	for (auto &var : refJKStockCodeModel->vecCodeTrade)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = new JKStockCodeTradeBLL(var);
		vecTrades.push_back(_refStockCodeTradeBLL);
	}
	
	return vecTrades;	 
}

void JKStockCodeBLL::setParams(JKString name, JKString code, double latestPrice)
{
	refJKStockCodeModel->name = name;
	refJKStockCodeModel->code = code;
	refJKStockCodeModel->latestPrice = latestPrice;
}



//HIBERLITE_EXPORT_CLASS(JKStockCodeBLL)
