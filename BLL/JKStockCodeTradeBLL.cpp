#include "stdafx.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeTradeModel.h"


void JKStockCodeTradeBLL::setParams(TradeType type, JKString date, JKUInt64 count, double price)
{
	refJKStockCodeTradeModel->type = (JKUInt)type;
	refJKStockCodeTradeModel->date = date;
	refJKStockCodeTradeModel->tradingCount = count;
	refJKStockCodeTradeModel->tradingPrice = price;
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
	return refJKStockCodeTradeModel->tradingCount;
}
double JKStockCodeTradeBLL::getPrice()
{
	return refJKStockCodeTradeModel->tradingPrice;
}

double JKStockCodeTradeBLL::getEarning(const double & latestPrice)
{
	double diff = latestPrice - refJKStockCodeTradeModel->tradingPrice;
	return diff * refJKStockCodeTradeModel->tradingCount;
}
