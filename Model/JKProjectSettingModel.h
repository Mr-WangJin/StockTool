#pragma once
#include "JKBaseModel.h"

class JKProjectSettingModel : public JKBaseModel
{
	friend class JKProjectSettingBLL;
	friend class hiberlite::Database;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(id);
		ar & HIBERLITE_NVP(isStartCrawl);
		ar & HIBERLITE_NVP(isAlert);
		ar & HIBERLITE_NVP(alertPercent);
		ar & HIBERLITE_NVP(tableShowType);
	}


public:
	JKString id;
	int isStartCrawl = 0;
	int isAlert = 0;
	double alertPercent = 0.05f;
	int tableShowType = 1;
};

