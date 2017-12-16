#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeModel.h"

class JKStockCodeTradeItemBLL;


class JKStockCodeTradeBLL : public JKBaseBLL
{
	friend class JKStockCodeBLL;

	JK_BLL_INIT(JKStockCodeTrade);

public:
	/** ���ò��� */
	void setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice);
	/** ���� */
	void sell(double price) = delete;
	/** ������Ʊ */
	void sell(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission);

	TradeType getType();
	JKString getDate();
	JKUInt64 getCount();
	//��ȡ�Ѿ�����������
	JKUInt64 getSoldCount();
	//
	JKUInt64 getCouldSellCount();
	//��ȡ���뵥��
	double getBuyPrice();
	//��ȡ��������
	double getSellPrice();
	//��ȡ���봿�ɱ�
	double getBuyPureCost();
	//��ȡ��ʵ����
	double getRealEarning();

	void getTradeItems(std::vector<JKRef_Ptr<JKStockCodeTradeItemBLL>> &vecTradeItems);

	virtual void upgradeDataVersion(int dataVersion) override;


private:
	void updateType();

};


