#pragma once
#include "JKBaseModel.h"
#include "JKStockCode.h";

class JKProjectModel : public JKBaseModel
{
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(name);
		ar & HIBERLITE_NVP(createDate);
		ar & HIBERLITE_NVP(vecStockCode);
	}

private:
	JKString name;
	JKString createDate;

	vector<bean_ptr<JKStockCode>> vecStockCode;

};

