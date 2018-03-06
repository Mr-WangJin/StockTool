#include "stdafx.h"
#include "JKStockCodeTradeItemBLL.h"
#include "Model/JKStockCodeTradeModel.h"

void JKStockCodeTradeItemBLL::setParams(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission)
{
	ptrModel->sellPrice = sellPrice;
	ptrModel->sellCount = sellCount;
	ptrModel->sellSumCount = sellSumCount;
	ptrModel->stampTax = stampTax;
	ptrModel->transfer = transfer;
	ptrModel->commission = commission;
}

void JKStockCodeTradeItemBLL::setSoldDate(JKString soldDate)
{
	ptrModel->soldDate = soldDate;
}

void JKStockCodeTradeItemBLL::setRealEarning(double _realEarning)
{
	ptrModel->realEarning = _realEarning;
}

void JKStockCodeTradeItemBLL::save()
{

}

void JKStockCodeTradeItemBLL::destroy()
{

}