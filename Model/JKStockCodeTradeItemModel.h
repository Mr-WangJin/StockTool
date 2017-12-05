#pragma once

#include "JKBaseModel.h"

class JKStockCodeTradeItemModel : public JKBaseModel
{
	friend class JKStockCodeTradeBLL;
	friend class hiberlite::access;
	template<class Archive>
	void hibernate(Archive & ar)
	{
		ar & HIBERLITE_NVP(id);
		ar & HIBERLITE_NVP(sellPrice);
		ar & HIBERLITE_NVP(sellCount);
		ar & HIBERLITE_NVP(sellSumCount);
		ar & HIBERLITE_NVP(stampTax);
		ar & HIBERLITE_NVP(transfer);
		ar & HIBERLITE_NVP(commission);
	}

public:
	JKStockCodeTradeItemModel() {};

public:
	JKString id;
	double sellPrice;		//��������
	size_t sellCount;		//��������
	size_t sellSumCount;	//�ܼ���������

	float stampTax;			//ӡ��˰
	float transfer;			//����
	float commission;		//Ӷ��
};


