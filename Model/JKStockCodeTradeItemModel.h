#pragma once

#include "JKBaseModel.h"

class JKStockCodeTradeItemModel : public JKBaseModel
{
	friend class JKStockCodeTradeBLL;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(id);
		ar & HIBERLITE_NVP(sellPrice);
		ar & HIBERLITE_NVP(sellCount);
		ar & HIBERLITE_NVP(sellSumCount);
		ar & HIBERLITE_NVP(stampTax);
		ar & HIBERLITE_NVP(transfer);
		ar & HIBERLITE_NVP(commission);
	}

public:
	JKStockCodeTradeItemModel() {};

public:
	JKString id;
	double sellPrice;		//卖出单价
	size_t sellCount;		//卖出数量
	size_t sellSumCount;	//总计卖出数量

	float stampTax;			//印花税
	float transfer;			//过户
	float commission;		//佣金
};


