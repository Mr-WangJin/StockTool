#include "stdafx.h"
#include "JKStockTradeDetail.h"
#include "BLL\JKStockTradeUtil.h"
#include "BLL\JKStockCodeTradeBLL.h"

JKStockTradeDetail::JKStockTradeDetail(JKRef_Ptr<JKStockTradeUtil> refUtil, JKRef_Ptr<JKStockCodeTradeBLL> refStockTrade, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::WindowCloseButtonHint);

	ui.lblBuyPrice->setText(QStringLiteral("买入价钱：") + QString("%1").arg(refStockTrade->getBuyPrice()));
	ui.lblBuyCount->setText(QStringLiteral("买入数量：") + QString("%1").arg(refStockTrade->getCount()));
	ui.lblSellPrice->setText(QStringLiteral("卖出价钱：") + QString("%1").arg(refStockTrade->getSellPrice()));

	if (refStockTrade->getType() == TradeType::SELL)
	{
		double buyCost = refStockTrade->getBuyPureCost();
		double sellCost = refStockTrade->getSellPrice()*refStockTrade->getCount();

		ui.lblCommission->setText(QStringLiteral("佣金税费：") + 
			QString("%1 -- %2").arg(refUtil->getCommission(buyCost)).arg(refUtil->getCommission(sellCost)));
		ui.lblTransfer->setText(QStringLiteral("过户税费：") + 
			QString("%1 -- %2").arg(refUtil->getTransfer(buyCost)).arg(refUtil->getTransfer(sellCost)));
		ui.lblStampTax->setText(QStringLiteral("印花税费：") + QString("%1").arg(refUtil->getStampTax(sellCost)));
	}
}

JKStockTradeDetail::~JKStockTradeDetail()
{

}
