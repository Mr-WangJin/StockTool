#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeModel.h"

class JKStockCodeTradeItemBLL;


class JKStockCodeTradeBLL : public JKBaseBLL<JKStockCodeTradeModel>
{
	friend class JKStockCodeBLL;

	JK_BLL_INIT(JKStockCodeTrade);

public:
	/** ���ò��� */
	void setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice);
	/** ���� */
	void sell(double price) = delete;
	/** ������Ʊ */
	void sell(double sellPrice, size_t sellCount, size_t sellSumCount, JKString soldDate,
		float stampTax, float transfer, float commission);

	TradeType getType();
	HoldStockType getHoldType();
	void setHoldType(HoldStockType type);
	JKString getDate();
	JKUInt64 getCount();
	//��ȡ�Ѿ�����������
	JKUInt64 getSoldCount();
	//��ȡ����������
	JKUInt64 getCouldSellCount();
	//��ȡ����ʱ��
	JKString getSoldDate();
	//��ȡ���뵥��
	double getBuyPrice();
	//��ȡ�����ܼ�
	double getBuyAmount();
	//��ȡ��������
	double getSellPrice();
	//��ȡ�����ܼ�
	double getSellAmount();
	//��ȡ��ʵ����
	double getRealEarning();

	void getTradeItems(std::vector<JKRef_Ptr<JKStockCodeTradeItemBLL>> &vecTradeItems);

	virtual void upgradeDataVersion(int dataVersion) override;


protected:
	virtual void save() override;
	virtual void destroy() override;

private:
	void updateType();

};


