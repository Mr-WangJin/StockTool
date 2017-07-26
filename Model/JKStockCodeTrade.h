#pragma once
#include "JKBaseModel.h"

#include "JKUtil/JKDateUtil.h"


class JKStockCodeTrade : public JKBaseModel
{
	friend class JKStockCodeTradeDAL;
public:
	enum class TradeType
	{
		BUY = 1,
		SELL
	};

private:
	JKStockCodeTrade();
	~JKStockCodeTrade();


	JK_DISABLE_COPY(JKStockCodeTrade)


private:
	TradeType m_tradeType = TradeType::BUY;
	JKRef_Ptr<JKDateUtil> m_Date;
	double m_TradingPrice;
	uint m_TradingCount;


};

