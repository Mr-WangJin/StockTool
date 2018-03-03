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


	void getAllTrades(vector<StockCodeTradeBLLPtr> &_vecStockTrade);
	void getTradesByType(int type, vector<StockCodeTradeBLLPtr> &_vecStockTrade);
	//StockCodeTradeBLLConstRefPtr getTradeByIndex(int index);
	int getTradeCountByType(int type);
	
	StockCodeTradeBLLConstRefPtr getStockTradeById(const JKString &id);
	bool batchSellTrade(std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockTrade, size_t sellCount, float sellPrice);


	//设置股票参数
	void setParams(JKString name, JKString code, double latestPrice);

	StockCodeTradeBLLConstRefPtr newStockCodeTrade();
	bool deleteTrade(StockCodeTradeBLLConstRefPtr);

	virtual void upgradeDataVersion(int dataVersion) override;

protected:
	virtual void save() override;
	virtual void destroy() override;

};


