#include "stdafx.h"
#include "JKSellStockCodeWgt.h"
#include "BLL/JKStockCodeBLL.h"

JKSellStockCodeWgt::JKSellStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget *parent)
	: QDialog(parent)
	, refStockCode(_refStockCode)
{
	ui.setupUi(this);
}

JKSellStockCodeWgt::~JKSellStockCodeWgt()
{

}

void JKSellStockCodeWgt::initUI()
{
	

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
