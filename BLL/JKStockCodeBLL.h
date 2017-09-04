#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeModel.h"

class JKStockCodeTradeBLL;

class JKStockCodeBLL : public JKBaseBLL
{
	friend class JKProjectBLL;
public:
	JKString getName();


	JKRef_Ptr<JKStockCodeTradeBLL> newStockCodeTrade();

	////购买交易
	//bool buyStockCodeTrade(JKString date, JKUInt64 count, JKUInt64 price);
	//bool cellStockCodeTrade(JKString date, JKUInt64 count, JKUInt64 price);


	//设置股票参数
	void setParams(JKString name, JKString code, double latestPrice);


	JK_BLL_INIT(JKStockCode)


};


