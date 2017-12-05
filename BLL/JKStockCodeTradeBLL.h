#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeModel.h"


class JKStockCodeTradeBLL : public JKBaseBLL
{
	friend class JKStockCodeBLL;

	JK_BLL_INIT(JKStockCodeTrade);

public:
	/** 设置参数 */
	void setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice);
	/** 卖出 */
	void sell(double price) = delete;
	/** 卖出股票 */
	void sell(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission);

	TradeType getType();
	JKString getDate();
	JKUInt64 getCount();
	JKUInt64 getSoldCount();
	JKUInt64 getCouldSellCount();
	//获取买入单价
	double getBuyPrice();
	//获取卖出单价
	double getSellPrice();
	//获取买入纯成本
	double getBuyPureCost();

};


