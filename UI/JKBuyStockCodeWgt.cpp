#include "stdafx.h"
#include "JKBuyStockCodeWgt.h"
#include "BLL\JKProjectBLL.h"
#include "BLL\JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"
#include <qdatetime.h>

JKBuyStockCodeWgt::JKBuyStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget* parent)
	: QDialog(parent)
	, refStockCode(_refStockCode)
{
	ui.setupUi(this);

	this->initUI();
}

JKBuyStockCodeWgt::~JKBuyStockCodeWgt()
{

}

JKRef_Ptr<JKStockCodeTradeBLL> JKBuyStockCodeWgt::getNewStockCodeTrade()
{
	return refStockCodeTrade;
};

void JKBuyStockCodeWgt::okClicked()
{
	QString buyDate = ui.lEditDate->date().toString(Qt::ISODate);
	int buyCount = ui.spBxCount->value();
	QString buyPrice = ui.lEditPrice->text();

	double price = buyPrice.toDouble();
	JKUInt count = buyCount;

	refStockCodeTrade = refStockCode->newStockCodeTrade();
	refStockCodeTrade->setParams(TradeType::BUY, buyDate.toStdString(), count, price);

	this->accept();

}
void JKBuyStockCodeWgt::cancleClicked()
{
	this->reject();
}

void JKBuyStockCodeWgt::initUI()
{
	ui.lEditDate->setDate(QDate::currentDate());


	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(cancleClicked()));

}