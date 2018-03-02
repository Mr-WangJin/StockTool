#include "stdafx.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeModel.h"
#include "JKStockCodeTradeBLL.h"
#include "JKBLLContainer.h"
#include <iostream>
#include <sstream>


JKString JKStockCodeBLL::getName()
{
	return ptrModel->name;
}

JKString JKStockCodeBLL::getCode()
{
	return ptrModel->code;
}

JKRef_Ptr<JKStockCodeTradeBLL> JKStockCodeBLL::newStockCodeTrade()
{
	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = NewBLL(JKStockCodeTradeBLL, JKStockCodeTradeModel);

	ptrModel->addStockCodeTrade(_refStockCodeTrade->getModel());
	return _refStockCodeTrade;
}

bool JKStockCodeBLL::deleteTrade(JKRef_Ptr<JKStockCodeTradeBLL> _refTradeBll)
{
	if (!_refTradeBll.valid())
		return false;

	return ptrModel->delStockCodeTrade(_refTradeBll->getModel());
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
	for (auto &var : ptrModel->vecCodeTrade)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = LoadBLL( JKStockCodeTradeBLL,JKStockCodeTradeModel, var.get_id(), parentID);
		_vecStockTrade.push_back(_refStockCodeTradeBLL);
	}
}

void JKStockCodeBLL::getTradesByType(int type, vector<JKRef_Ptr<JKStockCodeTradeBLL>>& _vecStockTrade)
{
	for (auto &var : ptrModel->vecCodeTrade)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = LoadBLL(JKStockCodeTradeBLL, JKStockCodeTradeModel, var.get_id(), parentID);
		if ((int)_refStockCodeTradeBLL->getType() & type)
			_vecStockTrade.push_back(_refStockCodeTradeBLL);
	}
}

JKRef_Ptr<JKStockCodeTradeBLL> JKStockCodeBLL::getStockTradeById(const JKString &id)
{
	for (auto &var : ptrModel->vecCodeTrade)
	{
		if (var->id != id)
			continue;
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTradeBLL = LoadBLL(JKStockCodeTradeBLL, JKStockCodeTradeModel, var.get_id(), parentID);
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
	ptrModel->name = name;
	ptrModel->code = code;
	ptrModel->latestPrice = latestPrice;
}

double JKStockCodeBLL::getLatestPrice() 
{
	return ptrModel->latestPrice;
}

void JKStockCodeBLL::setLatestPrice(double latestPrice)
{
	ptrModel->latestPrice = latestPrice;
}

void JKStockCodeBLL::save()
{
	//SaveBean(JKStockCodeSettingBLL, JKStockCodeSettingModel, ptrModel->codeSetting);
	for (int i = 0; i < ptrModel->vecCodeTrade.size(); ++i)
	{
		SaveBean(JKStockCodeTradeBLL, JKStockCodeTradeModel, ptrModel->vecCodeTrade[i]);
	}
}

void JKStockCodeBLL::destroy()
{
	//DestroyBean(JKStockCodeSettingBLL, JKStockCodeSettingModel, ptrModel->codeSetting);

	for (int i = 0; i < ptrModel->vecCodeTrade.size(); ++i)
	{
		DestroyBean(JKStockCodeTradeBLL, JKStockCodeTradeModel, ptrModel->vecCodeTrade[i]);
	}
}