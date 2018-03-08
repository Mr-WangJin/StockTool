#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>

class JKProjectBLL;
class JKStockCodeTradeBLL;

class JKStockTradeUtil : public JKReferenced
{
public:
	JKStockTradeUtil();
	JKStockTradeUtil(JKRef_Ptr<JKProjectBLL> _refProject);
	JKStockTradeUtil(float _stampTax, float _transfer, float _commission);
	virtual ~JKStockTradeUtil();

	//获取买入成本
	double getTradeBuyCost(StockCodeTradeBLLConstRefPtr);
	//获取买入成本单价
	double getTradeBuyCostPrice(StockCodeTradeBLLConstRefPtr);
	////获取卖出成本
	//double getTradeSellCost(double latestPrice, StockCodeTradeBLLConstRefPtr);
	//获取预期卖出收益
	double getExpactEarning(double latestPrice, StockCodeTradeBLLConstRefPtr);
	double getExpactEarning(double latestPrice, StockCodeTradeBLLConstRefPtr, size_t count);
	//获取预期卖出收益百分比
	double getExpactEarningPercent(double latestPrice, StockCodeTradeBLLConstRefPtr refStockCodeTrade);
	//获取卖出真实收益
	double getRealEarning(StockCodeTradeBLLConstRefPtr);
	//获取卖出真实收益百分比
	double getRealEarningPercent(StockCodeTradeBLLConstRefPtr);

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

