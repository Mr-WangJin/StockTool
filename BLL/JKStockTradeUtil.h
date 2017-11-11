#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>

class JKProjectBLL;
class JKStockCodeTradeBLL;

class JKStockTradeUtil : public JKReferenced
{
public:
	JKStockTradeUtil(JKRef_Ptr<JKProjectBLL> _refProject);
	virtual ~JKStockTradeUtil();

	//��ȡ����ɱ�
	double getTradeBuyCost(JKRef_Ptr<JKStockCodeTradeBLL>);
	//��ȡ����ɱ�����
	double getTradeBuyCostPrice(JKRef_Ptr<JKStockCodeTradeBLL>);
	////��ȡ�����ɱ�
	//double getTradeSellCost(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL>);
	//��ȡ������ʵ����
	double getRealEarning(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL>);
	double getRealEarningPercent(double latestPrice, JKRef_Ptr<JKStockCodeTradeBLL>);

	//��ȡӶ��
	double getCommission(double buyCost);
	//��ȡӡ��
	double getStampTax(double buyCost);
	//��ȡ����
	double getTransfer(double buyCost);
private:
	double getBuyTax(double buyCost);
	double getSellTax(double sellCost);

private:
	JKRef_Ptr<JKProjectBLL> refProject;


};

