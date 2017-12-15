#include "stdafx.h"
#include "JKStockTradeDetail.h"
#include "BLL\JKStockTradeUtil.h"
#include "BLL\JKStockCodeTradeBLL.h"

JKStockTradeDetail::JKStockTradeDetail(JKRef_Ptr<JKStockTradeUtil> refUtil, JKRef_Ptr<JKStockCodeTradeBLL> refStockTrade, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::WindowCloseButtonHint);

	ui.lblBuyPrice->setText(QStringLiteral("�����Ǯ��") + QString("%1").arg(refStockTrade->getBuyPrice()));
	ui.lblBuyCount->setText(QStringLiteral("����������") + QString("%1").arg(refStockTrade->getCount()));
	ui.lblSellPrice->setText(QStringLiteral("������Ǯ��") + QString("%1").arg(refStockTrade->getSellPrice()));

	if (refStockTrade->getType() == TradeType::SELL)
	{
		double buyCost = refStockTrade->getBuyPureCost();
		double sellCost = refStockTrade->getSellPrice()*refStockTrade->getCount();

		ui.lblCommission->setText(QStringLiteral("Ӷ��˰�ѣ�") + 
			QString("%1 -- %2").arg(refUtil->getCommission(buyCost)).arg(refUtil->getCommission(sellCost)));
		ui.lblTransfer->setText(QStringLiteral("����˰�ѣ�") + 
			QString("%1 -- %2").arg(refUtil->getTransfer(buyCost)).arg(refUtil->getTransfer(sellCost)));
		ui.lblStampTax->setText(QStringLiteral("ӡ��˰�ѣ�") + QString("%1").arg(refUtil->getStampTax(sellCost)));
	}
}

JKStockTradeDetail::~JKStockTradeDetail()
{

}
