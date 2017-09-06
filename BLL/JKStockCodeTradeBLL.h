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
	void setParams(TradeType type, JKString date, JKUInt64 count, JKUInt64 price);

	JKString getDate();
	JKUInt64 getCount();
	JKUInt64 getPrice();

};


