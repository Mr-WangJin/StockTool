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


	JKRef_Ptr<JKStockCodeTradeBLL> newStockCodeTrade();

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> getAllTrades();

	//���ù�Ʊ����
	void setParams(JKString name, JKString code, double latestPrice);


};


