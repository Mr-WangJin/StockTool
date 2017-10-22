#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeModel.h"

class JKStockCodeTradeBLL;

class JKStockCodeBLL : public JKBaseBLL
{
	friend class JKProjectBLL;
	
	JK_BLL_INIT(JKStockCode)

public:
	JKString getName();

	double getLatestPrice();

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> getAllTrades();

	//���ù�Ʊ����
	void setParams(JKString name, JKString code, double latestPrice);

	JKRef_Ptr<JKStockCodeTradeBLL> newStockCodeTrade();

};


