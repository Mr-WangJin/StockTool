#include "stdafx.h"
#include "JKStockCodeTradeBLL.h"
#include "JKStockCodeTradeItemBLL.h"
#include "Model/JKStockCodeTradeModel.h"


void JKStockCodeTradeBLL::setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice)
{
	refJKStockCodeTradeModel->type = (JKUInt)type;
	refJKStockCodeTradeModel->date = date;
	refJKStockCodeTradeModel->buyCount = count;
	refJKStockCodeTradeModel->buyPrice = buyPrice;
}

// void JKStockCodeTradeBLL::sell(double price)
// {
// 	refJKStockCodeTradeModel->type = (JKUInt)TradeType::SELL;
// 	refJKStockCodeTradeModel->sellPrice = price;
// }

void JKStockCodeTradeBLL::sell(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission)
{
	if (this->getCouldSellCount() == 0 || sellCount <= 0)
		return;

	JKRef_Ptr<JKStockCodeTradeItemBLL> _refStockCodeTradeItem = new JKStockCodeTradeItemBLL();
	_refStockCodeTradeItem->setParams(sellPrice, sellCount, sellSumCount, stampTax, transfer, commission);
	refJKStockCodeTradeModel->addStockCodeTradeItem(_refStockCodeTradeItem->getModel());
}

TradeType JKStockCodeTradeBLL::getType()
{
	return (TradeType)refJKStockCodeTradeModel->type;
}

JKString JKStockCodeTradeBLL::getDate()
{
	return refJKStockCodeTradeModel->date;
}
JKUInt64 JKStockCodeTradeBLL::getCount()
{
	return refJKStockCodeTradeModel->buyCount;
}
JKUInt64 JKStockCodeTradeBLL::getSoldCount()
{
	JKUInt64 count = 0;
	for (auto &var : refJKStockCodeTradeModel->vecSellItem)
	{
		count += var->sellCount;
	}
	return count;
}
JKUInt64 JKStockCodeTradeBLL::getCouldSellCount()
{
	JKUInt64 count = 0;
	for (auto &var : refJKStockCodeTradeModel->vecSellItem)
	{
		count += var->sellCount;
	}
	return refJKStockCodeTradeModel->buyCount - count;
}
double JKStockCodeTradeBLL::getBuyPrice()
{
	return refJKStockCodeTradeModel->buyPrice;
}

double JKStockCodeTradeBLL::getSellPrice()
{
	return refJKStockCodeTradeModel->sellPrice;
}

double JKStockCodeTradeBLL::getBuyPureCost()
{
	return refJKStockCodeTradeModel->buyPrice * refJKStockCodeTradeModel->buyCount;
}
