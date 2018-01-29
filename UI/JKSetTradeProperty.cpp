#include "stdafx.h"
#include "JKSetTradeProperty.h"
#include "BLL/JKStockCodeTradeBLL.h"

JKSetTradeProperty::JKSetTradeProperty(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOkClick()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancelClick()));
}

JKSetTradeProperty::~JKSetTradeProperty()
{

}

void JKSetTradeProperty::setStockCodeTrade(JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade)
{
	refStockCodeTrade = _refStockCodeTrade;

	this->initialize();
}

void JKSetTradeProperty::onOkClick()
{

	if (refStockCodeTrade.valid())
	{
		int holdType = ui.cmbBxHoldType->currentIndex();
		refStockCodeTrade->setHoldType(HoldStockType(holdType));
	}

	this->accept();
}

void JKSetTradeProperty::onCancelClick()
{
	this->reject();
}

void JKSetTradeProperty::initialize()
{
	if (refStockCodeTrade.valid() == false)
		return;

	int holdType = (int)refStockCodeTrade->getHoldType();
	ui.cmbBxHoldType->setCurrentIndex(holdType);

}