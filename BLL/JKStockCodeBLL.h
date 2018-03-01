#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model/JKStockCodeModel.h"

class JKStockCodeTradeBLL;

class JKStockCodeBLL : public JKBaseBLL<JKStockCodeModel>
{
	friend class JKProjectBLL;
	JK_BLL_INIT(JKStockCode)

public:
	JKString getName();
	JKString getCode();
	double getLatestPrice();

	void setLatestPrice(double latestPrice);

	void getAllTrades(vector<JKRef_Ptr<JKStockCodeTradeBLL>> &_vecStockTrade);
	void getTradesByType(int type, vector<JKRef_Ptr<JKStockCodeTradeBLL>> &_vecStockTrade);
	
	JKRef_Ptr<JKStockCodeTradeBLL> getStockTradeById(const JKString &id);
	bool batchSellTrade(std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockTrade, size_t sellCount, float sellPrice);


	//设置股票参数
	void setParams(JKString name, JKString code, double latestPrice);

	JKRef_Ptr<JKStockCodeTradeBLL> newStockCodeTrade();
	bool deleteTrade(JKRef_Ptr<JKStockCodeTradeBLL>);

	virtual void upgradeDataVersion(int dataVersion) override;

};


