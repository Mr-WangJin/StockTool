#pragma once
#include "JKBaseModel.h"

class JKStockCodeTradeModel : public JKBaseModel
{
	friend class JKStockCodeTradeBLL;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(id);
		ar & HIBERLITE_NVP(type);
		ar & HIBERLITE_NVP(date);
		ar & HIBERLITE_NVP(buyPrice);
		ar & HIBERLITE_NVP(buyCount);
		ar & HIBERLITE_NVP(sellPrice);
	}

public:
	JKStockCodeTradeModel() {};

public:
	JKString id;
	int type;
	JKString date;
	double buyPrice;
	JKUInt buyCount;
	double sellPrice = 0.0f;
};

