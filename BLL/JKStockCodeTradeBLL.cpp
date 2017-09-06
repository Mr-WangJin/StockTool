#include "stdafx.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKStockCodeTradeModel.h"


void JKStockCodeTradeBLL::setParams(TradeType type, JKString date, JKUInt64 count, JKUInt64 price)
{
	refJKStockCodeTradeModel->type = (JKUInt)type;
	refJKStockCodeTradeModel->date = date;
	refJKStockCodeTradeModel->tradingCount = count;
	refJKStockCodeTradeModel->tradingPrice = price;
}

JKString JKStockCodeTradeBLL::getDate()
{
	return refJKStockCodeTradeModel->date;
}
JKUInt64 JKStockCodeTradeBLL::getCount()
{
	return refJKStockCodeTradeModel->tradingCount;
}
JKUInt64 JKStockCodeTradeBLL::getPrice()
{
	return refJKStockCodeTradeModel->tradingPrice;
}