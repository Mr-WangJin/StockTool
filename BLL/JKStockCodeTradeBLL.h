#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeTradeModel.h"

class JKStockCodeTradeItemBLL;


class JKStockCodeTradeBLL : public JKBaseBLL<JKStockCodeTradeModel>
{
	friend class JKStockCodeBLL;

	JK_BLL_INIT(JKStockCodeTrade);

public:
	/** 设置参数 */
	void setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice);
	/** 卖出 */
	void sell(double price) = delete;
	/** 卖出股票 */
	void sell(double sellPrice, size_t sellCount, size_t sellSumCount, JKString soldDate,
		float stampTax, float transfer, float commission);

	TradeType getType();
	HoldStockType getHoldType();
	void setHoldType(HoldStockType type);
	JKString getDate();
	JKUInt64 getCount();
	//获取已经卖出的数量
	JKUInt64 getSoldCount();
	//获取可卖出数量
	JKUInt64 getCouldSellCount();
	//获取卖出时间
	JKString getSoldDate();
	//获取买入单价
	double getBuyPrice();
	//获取买入总价
	double getBuyAmount();
	//获取卖出单价
	double getSellPrice();
	//获取买入总价
	double getSellAmount();
	//获取真实收益
	double getRealEarning();

	void getTradeItems(std::vector<JKRef_Ptr<JKStockCodeTradeItemBLL>> &vecTradeItems);

	virtual void upgradeDataVersion(int dataVersion) override;


protected:
	virtual void save() override;
	virtual void destroy() override;

private:
	void updateType();

};


