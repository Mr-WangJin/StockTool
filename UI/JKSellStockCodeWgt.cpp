#include "stdafx.h"
#include "JKSellStockCodeWgt.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKStockCodeTradeBLL.h"

JKSellStockCodeWgt::JKSellStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget *parent)
	: QDialog(parent)
	, refStockCode(_refStockCode)
{
	ui.setupUi(this);

	this->initUI();
}

JKSellStockCodeWgt::~JKSellStockCodeWgt()
{

}

void JKSellStockCodeWgt::initUI()
{
	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefTrade = refStockCode->getAllTrades();
	for (auto &var : vecRefTrade)
	{
		if (var->getType() == TradeType::BUY)
		{
			QString text = QString::fromStdString(var->getDate())
				+ QString("--") + QString::number(var->getBuyPrice())
				+ QString("--") + QString::number(var->getCount());
			ui.cmbBx->addItem(text, QString::fromStdString(var->getId()));
		}
	}
	connect(ui.cmbBx, SIGNAL(currentIndexChanged(int)), this, SLOT(onCmbBoxChanged(int)));
	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

void JKSellStockCodeWgt::onOK()
{
	int idx = ui.cmbBx->currentIndex();
	JKString id = ui.cmbBx->itemData(idx).toString().toStdString();
	JKRef_Ptr<JKStockCodeTradeBLL> refTrade = refStockCode->getTradeById(id);
	if (refTrade.valid())
	{
		refTrade->sell(ui.dSpBxPrice->value());
	}

	this->accept();
}

void JKSellStockCodeWgt::onCancel()
{
	this->reject();
}

void JKSellStockCodeWgt::onCmbBoxChanged(int idx)
{
	JKString id = ui.cmbBx->itemData(idx).toString().toStdString();
	JKRef_Ptr<JKStockCodeTradeBLL> refTrade = refStockCode->getTradeById(id);
	if (refTrade.valid())
	{
		//ui.dSpBxPrice->setValue(refTrade->getBuyPrice());
		ui.spBxCount->setValue(refTrade->getCount());
	}
}