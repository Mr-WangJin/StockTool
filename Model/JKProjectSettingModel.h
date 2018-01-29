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
	}


public:
	JKString id;
	int isStartCrawl;
	int isAlert;
};

