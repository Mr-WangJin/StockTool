#pragma once
#include "JKBaseModel.h"

#include "JKUtil/JKDateUtil.h"


class JKStockCodeSetting : public JKBaseModel
{
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(taxPercent);
		ar & HIBERLITE_NVP(offsetPercent);
		ar & HIBERLITE_NVP(targetPrice);
	}

public:
	float taxPercent;
	float offsetPercent;
	double targetPrice;
};

