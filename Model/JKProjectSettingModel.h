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
		ar & HIBERLITE_NVP(downAlertPercent);
		ar & HIBERLITE_NVP(tableShowType);
	}


public:
	JKString id;
	int isStartCrawl = 0;				//是否开启爬虫
	int isAlert = 0;					//是否预警
	double alertPercent = 0.03f;		//上升百分比
	double downAlertPercent = -0.03f;	//下降百分比
	int tableShowType = 1;				//显示类型
};

