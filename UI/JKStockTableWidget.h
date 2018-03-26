#pragma once

#include "JKVirtualTreeAdapter.h"
#include "JKTreeModelCustomItem.h"
#include "BLL/JKStockCodeTradeBLL.h"
#include "BLL/JKStockTradeUtil.h"


class StockBuyTableItem
	: public JKTreeModelCustomItem<StockCodeTradeBLLPtr>
{
public:
	StockBuyTableItem(StockCodeTradeBLLPtr data);
	~StockBuyTableItem();
	
};
