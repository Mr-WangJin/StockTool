#pragma once

#include "JKBaseModel.h"
#include "JKStockCodeSettingModel.h"
#include "JKStockCodeTradeModel.h"

class JKStockCodeModel : public JKBaseModel
{
	friend class JKStockCodeBLL;
	friend class hiberlite::Database;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(name);
		ar & HIBERLITE_NVP(code);
		ar & HIBERLITE_NVP(latestPrice);
		ar & HIBERLITE_NVP(codeSetting);
		ar & HIBERLITE_NVP(vecCodeTrade);
	}

	void addStockCodeTrade(bean_ptr<JKStockCodeTradeModel> _refStockCodeTradeModel);
	bool delStockCodeTrade(bean_ptr<JKStockCodeTradeModel> _refStockCodeTradeModel);

public:
	JKString name;
	JKString code;
	double latestPrice;
	bean_ptr<JKStockCodeSettingModel> codeSetting;

	vector<bean_ptr<JKStockCodeTradeModel>> vecCodeTrade;

};

