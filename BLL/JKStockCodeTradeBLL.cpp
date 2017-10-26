#include "stdafx.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeTradeModel.h"


void JKStockCodeTradeBLL::setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice)
{
	refJKStockCodeTradeModel->type = (JKUInt)type;
	refJKStockCodeTradeModel->date = date;
	refJKStockCodeTradeModel->buyCount = count;
	refJKStockCodeTradeModel->buyPrice = buyPrice;
}

void JKStockCodeTradeBLL::sell(double price)
{
	refJKStockCodeTradeModel->type = (JKUInt)TradeType::SELL;
	refJKStockCodeTradeModel->sellPrice = price;
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
double JKStockCodeTradeBLL::getBuyPrice()
{
	return refJKStockCodeTradeModel->buyPrice;
}

double JKStockCodeTradeBLL::getSellPrice()
{
	return refJKStockCodeTradeModel->sellPrice;
}

double JKStockCodeTradeBLL::getEarning(const double & latestPrice)
{
	switch ((TradeType)refJKStockCodeTradeModel->type)
	{
	case TradeType::BUY:
	{
		double diff = latestPrice - refJKStockCodeTradeModel->buyPrice;
		return diff * refJKStockCodeTradeModel->buyCount;
	}
	break;
	case TradeType::SELL:
	{
		double diff = refJKStockCodeTradeModel->sellPrice - refJKStockCodeTradeModel->buyPrice;
		return diff * refJKStockCodeTradeModel->buyCount;
	}
	default:
		break;
	}
	
}

double JKStockCodeTradeBLL::getEarningPercent(const double & latestPrice)
{
	double diff = latestPrice - refJKStockCodeTradeModel->buyPrice;
	return diff / refJKStockCodeTradeModel->buyPrice * 100.0;
}