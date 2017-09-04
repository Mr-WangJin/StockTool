#include "stdafx.h"
#include "JKBuyStockCodeWgt.h"
#include "BLL\JKProjectBLL.h"
#include "BLL\JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"

JKBuyStockCodeWgt::JKBuyStockCodeWgt(JKRef_Ptr<JKProjectBLL> _refProject, QWidget* parent)
	: QDialog(parent)
	, refProject(_refProject)
{
	ui.setupUi(this);

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(okClicked()));
	connect(ui.pBtnCancel, SIGNAL(clicked()), this, SLOT(cancleClicked()));


}

JKBuyStockCodeWgt::~JKBuyStockCodeWgt()
{

}

void JKBuyStockCodeWgt::okClicked()
{
	QString buyDate = ui.lEditDate->text();
	QString buyCount = ui.lEdtCount->text();
	QString buyPrice = ui.lEditPrice->text();

	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	JKRef_Ptr<JKStockCodeTradeBLL> _refStockCodeTrade = _refStockCode->newStockCodeTrade();
	_refStockCodeTrade->setParams(TradeType::BUY, buyDate.toStdString(), 
		buyCount.toULongLong(), buyPrice.toULongLong());

	this->accept();

}
void JKBuyStockCodeWgt::cancleClicked()
{
	this->reject();
}