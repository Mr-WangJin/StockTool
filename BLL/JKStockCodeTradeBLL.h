#pragma once


#include "BLL/JKBaseBLL.h"
//#include "Model/JKStockCodeTradeModel.h"

class JKStockCodeTradeModel;

class JKStockCodeTradeBLL : public JKBaseBLL
{
	friend class JKStockCodeBLL;

	JK_BLL_INIT(JKStockCodeTrade);

public:
	/**
	 *	 …Ë÷√≤Œ ˝
	 */
	void setParams(TradeType type, JKString date, JKUInt64 count, double price);

	TradeType getType();
	JKString getDate();
	JKUInt64 getCount();
	double getPrice();

	double getEarning(const double & latestPrice);

};


