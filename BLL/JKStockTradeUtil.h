#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>

class JKProjectBLL;
class JKStockCodeTradeBLL;

class JKStockTradeUtil : public JKReferenced
{
public:
	JKStockTradeUtil(JKRef_Ptr<JKProjectBLL> _refProject);
	JKStockTradeUtil(float _stampTax, float _transfer, float _commission);
	virtual ~JKStockTradeUtil();

	//获取买入成本
	double getTradeBuyCost(JKRef_Ptr<JKStockCodeTradeBLL>);
	//获取买入成本单价
	double getTradeBuyCostPrice(JKRef_Ptr<JKStockCodeTradeBLL>);
	////获取卖出成本
	//double getTradeSellCost(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL>);
	//获取卖出真实收益
	double getRealEarning(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL>);
	//获取卖出真实收益百分比
	double getRealEarningPercent(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL>);

	//获取佣金
	double getCommission(double buyCost);
	//获取印花
	double getStampTax(double buyCost);
	//获取过户
	double getTransfer(double buyCost);
private:
	double getBuyTax(double buyCost);
	double getSellTax(double sellCost);

private:
	float stampTax = 0;			//印花税
	float transfer = 0;			//过户
	float commission = 0;		//佣金


};

