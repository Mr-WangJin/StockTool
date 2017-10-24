#include "stdafx.h"
#include "JKSettingLatestPrice.h"

JKSettingLatestPrice::JKSettingLatestPrice(double _prePrice, QWidget *parent)
	: QDialog(parent)
	, prePrice(_prePrice)
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
	ui.dSpBxPrice->setValue(prePrice);

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(onCancel()));
}
