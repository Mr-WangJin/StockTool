#pragma once

#include "JKBaseModel.h"
#include "JKStockCodeSetting.h"
#include "JKStockCodeTrade.h"


class JKStockCode : public JKBaseModel
{
	friend class JKStockCodeBLL;
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

private:
	JKString name;
	JKString code;
	double latestPrice;
	bean_ptr<JKStockCodeSetting> codeSetting;

	vector<bean_ptr<JKStockCodeTrade>> vecCodeTrade;

};

