#pragma once

#include "JKBLLCommon.h"
#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeItemModel.h"


class JKStockCodeTradeItemBLL : public JKBaseBLL<JKStockCodeTradeItemModel>
{
	friend class JKStockCodeTradeBLL;

	JK_BLL_INIT(JKStockCodeTradeItem);

public:
	/** ���ò��� */
	void setParams(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission);

	/** ������ʵ���� */
	void setRealEarning(double _realEarning);
};


