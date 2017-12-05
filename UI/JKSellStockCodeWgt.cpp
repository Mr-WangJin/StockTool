#include "stdafx.h"
#include "JKSellStockCodeWgt.h"
#include "BLL/JKProjectBll.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKStockCodeTradeBLL.h"

JKSellStockCodeWgt::JKSellStockCodeWgt(JKRef_Ptr<JKProjectBLL> _refProject, std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockTrade, QWidget *parent)
	: QDialog(parent)
	, refProject(_refProject)
	, vecStockTrade(_vecStockTrade)
{
	ui.setupUi(this);

	this->initUI();
}

JKSellStockCodeWgt::~JKSellStockCodeWgt()
{

}

void JKSellStockCodeWgt::initUI()
{
	int lblBuyCount = 0;
	for (auto &var : vecStockTrade)
	{
		lblBuyCount += var->getCount();
	}
	if (lblBuyCount == 0)
		ui.spBxCount->setEnabled(false);
	ui.lblBuyCount->setText(QString("%1").arg(lblBuyCount));

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

void JKSellStockCodeWgt::onOK()
{
	if (ui.spBxCount->value() == 0)
	{
		ui.lblInfo->setVisible(true);
		ui.lblInfo->setText(QStringLiteral("卖出数量不能为0！"));
		return;
	}
	else if (ui.lblBuyCount->text().toInt() < ui.spBxCount->value())
	{
		ui.lblInfo->setVisible(true);
		ui.lblInfo->setText(QStringLiteral("卖出数量不能大于买入数量！"));
		return;
	}
	int sellCount = ui.spBxCount->value();
	for (auto &var : vecStockTrade)
	{
		//if (var->getCount() <)
	}
	this->accept();
}

void JKSellStockCodeWgt::onCancel()
{
	this->reject();
}
