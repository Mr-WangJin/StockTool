#include "stdafx.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeModel.h"
#include "JKStockCodeTradeBLL.h"



JKString JKStockCodeBLL::getName()
{
	return refJKStockCodeModel->name;
}

JKString JKStockCodeBLL::getCode()
{
	return refJKStockCodeModel->code;
}

JKRef_Ptr<JKStockCodeTradeBLL> JKStockCodeBLL::newStockCodeTrade()
{
	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = new JKStockCodeTradeBLL();

	refJKStockCodeModel->addStockCodeTrade(_refStockCodeTrade->getModel());
	return _refStockCodeTrade;
}

bool JKStockCodeBLL::deleteTrade(JKRef_Ptr<JKStockCodeTradeBLL> _refTradeBll)
{
	if (!_refTradeBll.valid())
		return false;

	return refJKStockCodeModel->delStockCodeTrade(_refTradeBll->getModel());
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

JKRef_Ptr<JKStockCodeTradeBLL> JKStockCodeBLL::getTradeById(const JKString &id)
{
	for (auto &var : refJKStockCodeModel->vecCodeTrade)
	{
		if (var->id != id)
			continue;
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = new JKStockCodeTradeBLL(var);
		return _refStockCodeTradeBLL;
	}
	return nullptr;
}

void JKStockCodeBLL::setParams(JKString name, JKString code, double latestPrice)
{
	refJKStockCodeModel->name = name;
	refJKStockCodeModel->code = code;
	refJKStockCodeModel->latestPrice = latestPrice;
}

double JKStockCodeBLL::getLatestPrice() 
{
	return refJKStockCodeModel->latestPrice;
}

void JKStockCodeBLL::setLatestPrice(double latestPrice)
{
	refJKStockCodeModel->latestPrice = latestPrice;
}