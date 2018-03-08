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

	//��ȡ����ɱ�
	double getTradeBuyCost(StockCodeTradeBLLConstRefPtr);
	//��ȡ����ɱ�����
	double getTradeBuyCostPrice(StockCodeTradeBLLConstRefPtr);
	////��ȡ�����ɱ�
	//double getTradeSellCost(double latestPrice, StockCodeTradeBLLConstRefPtr);
	//��ȡԤ����������
	double getExpactEarning(double latestPrice, StockCodeTradeBLLConstRefPtr);
	double getExpactEarning(double latestPrice, StockCodeTradeBLLConstRefPtr, size_t count);
	//��ȡԤ����������ٷֱ�
	double getExpactEarningPercent(double latestPrice, StockCodeTradeBLLConstRefPtr refStockCodeTrade);
	//��ȡ������ʵ����
	double getRealEarning(StockCodeTradeBLLConstRefPtr);
	//��ȡ������ʵ����ٷֱ�
	double getRealEarningPercent(StockCodeTradeBLLConstRefPtr);

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
	float stampTax = 0;			//ӡ��˰
	float transfer = 0;			//����
	float commission = 0;		//Ӷ��


};

