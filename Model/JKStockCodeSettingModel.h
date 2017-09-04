#pragma once
#include "JKBaseModel.h"

class JKStockCodeSettingModel : public JKBaseModel
{
	friend class hiberlite::Database;
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

