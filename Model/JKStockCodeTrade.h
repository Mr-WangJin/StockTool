#pragma once
#include "JKBaseModel.h"

class JKStockCodeTrade : public JKBaseModel
{
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
	enum class TradeType
	{
		BUY = 1,
		SELL
	};


	uint32_t type;
	JKString date;
	double tradingPrice;
	uint64_t tradingCount;
};

//HIBERLITE_EXPORT_CLASS(JKStockCodeTrade)
