#pragma once
#include "JKBaseModel.h"

class JKStockCodeTradeModel : public JKBaseModel
{
	friend class JKStockCodeTradeBLL;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(type);
		ar & HIBERLITE_NVP(date);
		ar & HIBERLITE_NVP(tradingPrice);
		ar & HIBERLITE_NVP(tradingCount);
	}

public:
	JKStockCodeTradeModel() {};

public:

	int type;
	JKString date;
	int tradingPrice;
	int tradingCount;
};

