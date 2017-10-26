#pragma once
#include "JKBaseModel.h"
#include "JKStockCodeModel.h"


class JKProjectModel : public JKBaseModel
{
	friend class JKProjectBLL;
	friend class hiberlite::Database;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(name);
		ar & HIBERLITE_NVP(createDate);
		ar & HIBERLITE_NVP(vecStockCode);
	}


public:
	void addStockCode(bean_ptr<JKStockCodeModel> stockCode);

public:
	JKString id;
	JKString name;
	JKString createDate;

	vector<bean_ptr<JKStockCodeModel>> vecStockCode;

};

