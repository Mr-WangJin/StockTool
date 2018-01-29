#pragma once
#include "JKBaseModel.h"
#include "JKStockCodeTradeItemModel.h"

class JKStockCodeTradeModel : public JKBaseModel
{
	friend class JKStockCodeTradeBLL;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(id);
		ar & HIBERLITE_NVP(type);
		ar & HIBERLITE_NVP(holdType);
		ar & HIBERLITE_NVP(date);
		ar & HIBERLITE_NVP(buyPrice);
		ar & HIBERLITE_NVP(buyCount);
		ar & HIBERLITE_NVP(sellPrice);
		ar & HIBERLITE_NVP(vecSellItem);

	}

public:
	JKStockCodeTradeModel() {};

	void addStockCodeTradeItem(bean_ptr<JKStockCodeTradeItemModel> _refItem)
	{
		vecSellItem.push_back(_refItem);
	}

public:
	JKString id;
	int type;							//买卖类型
	JKString date;
	double buyPrice;
	JKUInt buyCount;
	double sellPrice = 0.0f;			//废弃
	int holdType;						//持有类型

	vector<bean_ptr<JKStockCodeTradeItemModel>> vecSellItem;
};
