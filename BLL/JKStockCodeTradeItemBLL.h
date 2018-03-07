#pragma once

#include "JKBLLCommon.h"
#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeItemModel.h"


class JKStockCodeTradeItemBLL : public JKBaseBLL<JKStockCodeTradeItemModel>
{
	friend class JKStockCodeTradeBLL;

	JK_BLL_INIT(JKStockCodeTradeItem);

public:
	/** 设置参数 */
	void setParams(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission);

	PROPERTY(JKString, soldDate);

	/** 设置真实收益 */
	void setRealEarning(double _realEarning);

protected:
	virtual void save() override;
	virtual void destroy() override;
};


