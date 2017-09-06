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

	//ui.lEditDate->set

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(cancleClicked()));


}

JKBuyStockCodeWgt::~JKBuyStockCodeWgt()
{

}

void JKBuyStockCodeWgt::okClicked()
{
	QString buyDate = ui.lEditDate->date().toString(Qt::ISODate);
	QString buyCount = ui.lEdtCount->text();
	QString buyPrice = ui.lEditPrice->text();

	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = refStockCode->newStockCodeTrade();
	_refStockCodeTrade->setParams(TradeType::BUY, buyDate.toStdString(), 
		buyCount.toULongLong(), buyPrice.toULongLong());

	this->accept();

}
void JKBuyStockCodeWgt::cancleClicked()
{
	this->reject();
}