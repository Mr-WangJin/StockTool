#pragma once

#include "JKBLLCommon.h"
#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeItemModel.h"


class JKStockCodeTradeItemBLL : public JKBaseBLL
{
	friend class JKStockCodeTradeBLL;

	JK_BLL_INIT(JKStockCodeTradeItem);

public:
	/** …Ë÷√≤Œ ˝ */
	void setParams(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission);


};


