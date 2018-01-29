#include "stdafx.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeModel.h"
#include "JKStockCodeTradeBLL.h"
#include <iostream>
#include <sstream>


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
	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = new JKStockCodeTradeBLL(refContext);

	refJKStockCodeModel->addStockCodeTrade(_refStockCodeTrade->getModel());
	return _refStockCodeTrade;
}

bool JKStockCodeBLL::deleteTrade(JKRef_Ptr<JKStockCodeTradeBLL> _refTradeBll)
{
	if (!_refTradeBll.valid())
		return false;

	return refJKStockCodeModel->delStockCodeTrade(_refTradeBll->getModel());
}

void JKStockCodeBLL::upgradeDataVersion(int dataVersion)
{
	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockCodeTrade;
	this->getAllTrades(_vecStockCodeTrade);
	for (auto &var : _vecStockCodeTrade)
	{
		var->upgradeDataVersion(dataVersion);
	}
}

void JKStockCodeBLL::getAllTrades(vector<JKRef_Ptr<JKStockCodeTradeBLL>> &_vecStockTrade)
{
	for (auto &var : refJKStockCodeModel->vecCodeTrade)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = new JKStockCodeTradeBLL(var, refContext);
		_vecStockTrade.push_back(_refStockCodeTradeBLL);
	}
}

void JKStockCodeBLL::getTradesByType(int type, vector<JKRef_Ptr<JKStockCodeTradeBLL>>& _vecStockTrade)
{
	for (auto &var : refJKStockCodeModel->vecCodeTrade)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = new JKStockCodeTradeBLL(var, refContext);
		if ((int)_refStockCodeTradeBLL->getType() & type)
			_vecStockTrade.push_back(_refStockCodeTradeBLL);
	}
}

JKRef_Ptr<JKStockCodeTradeBLL> JKStockCodeBLL::getStockTradeById(const JKString &id)
{
	for (auto &var : refJKStockCodeModel->vecCodeTrade)
	{
		if (var->id != id)
			continue;
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = new JKStockCodeTradeBLL(var, refContext);
		return _refStockCodeTradeBLL;
	}
	return nullptr;
}

bool JKStockCodeBLL::batchSellTrade(std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockTrade, size_t sellCount, float sellPrice)
{
	int canSellCount = 0;
	for (auto &var : _vecStockTrade)
	{
		canSellCount += var->getCount();
	}
	if (canSellCount < sellCount)
	{
		std::ostringstream o;
		o << "sell count " << sellCount << " larger than " << canSellCount;
		throw std::exception(o.str().c_str());
	}

	return false;
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