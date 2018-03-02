#pragma once
#include "JKBaseModel.h"
#include "JKStockCodeModel.h"
#include "JKProjectSettingModel.h"


class JKProjectModel : public JKBaseModel
{
	friend class JKProjectBLL;
	friend class hiberlite::Database;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(id);
		ar & HIBERLITE_NVP(name);
		ar & HIBERLITE_NVP(createDate);
		ar & HIBERLITE_NVP(vecStockCode);
		ar & HIBERLITE_NVP(stampTax);
		ar & HIBERLITE_NVP(transfer);
		ar & HIBERLITE_NVP(commission);
		ar & HIBERLITE_NVP(projectSetting);
	}

public:
	void addStockCode(bean_ptr<JKStockCodeModel> stockCode);
	void deleteStockCode(bean_ptr<JKStockCodeModel> stockCode);

public:
	JKString id;
	JKString name;
	JKString createDate;

	float stampTax;			//印花税
	float transfer;			//过户
	float commission;		//佣金

	vector<bean_ptr<JKStockCodeModel>> vecStockCode;
	bean_ptr<JKProjectSettingModel> projectSetting;
};

