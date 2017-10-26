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
				+QString("--")+ QString::number(var->getBuyPrice())
				+QString("--")+ QString::number(var->getCount());
			ui.cmbBx->insertItem(0, text);
		}
	}

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}

void JKSellStockCodeWgt::onOK()
{


	this->reject();
}

void JKSellStockCodeWgt::onCancel()
{
	this->reject();
}
