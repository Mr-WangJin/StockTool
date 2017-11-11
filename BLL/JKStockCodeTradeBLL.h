#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeModel.h"


class JKStockCodeTradeBLL : public JKBaseBLL
{
	friend class JKStockCodeBLL;

	JK_BLL_INIT(JKStockCodeTrade);

public:
	/**
	 *	 ���ò���
	 */
	void setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice);
	/** 
	 *	����
	 */
	void sell(double price);

	TradeType getType();
	JKString getDate();
	JKUInt64 getCount();
	//��ȡ���뵥��
	double getBuyPrice();
	//��ȡ��������
	double getSellPrice();
	//��ȡ���봿�ɱ�
	double getBuyPureCost();

};


