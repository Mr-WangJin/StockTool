#include "stdafx.h"
#include "JKStockCodeTradeItemBLL.h"
#include "Model/JKStockCodeTradeModel.h"

void JKStockCodeTradeItemBLL::setParams(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission)
{
	refJKStockCodeTradeItemModel->sellPrice = sellPrice;
	refJKStockCodeTradeItemModel->sellCount = sellCount;
	refJKStockCodeTradeItemModel->sellSumCount = sellSumCount;
	refJKStockCodeTradeItemModel->stampTax = stampTax;
	refJKStockCodeTradeItemModel->transfer = transfer;
	refJKStockCodeTradeItemModel->commission = commission;
}
