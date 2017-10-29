#include "stdafx.h"
#include "JKSettingLatestPrice.h"
#include "BLL/JKStockCodeBLL.h"

JKSettingLatestPrice::JKSettingLatestPrice(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget *parent)
	: QDialog(parent)
	, refStockCode(_refStockCode)
{
	ui.setupUi(this);

	this->initUI();
}

JKSettingLatestPrice::~JKSettingLatestPrice()
{

}

double JKSettingLatestPrice::getLatestPrice()
{
	return ui.dSpBxPrice->value();
}

void JKSettingLatestPrice::onOK()
{
	this->accept();
}
void JKSettingLatestPrice::onCancel()
{
	this->reject();
}

void JKSettingLatestPrice::initUI()
{
	ui.dSpBxPrice->setValue(refStockCode->getLatestPrice());
	ui.lblCurStock->setText(QString::fromStdString(refStockCode->getName()));

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}
