#include "stdafx.h"
#include "JKNewStockCodeWgt.h"
#include "bll/JKStockCodeBLL.h"
#include "bll/JKProjectBLL.h"

JKNewStockCodeWgt::JKNewStockCodeWgt(JKRef_Ptr<JKProjectBLL> project, QWidget *parent)
	: QDialog(parent)
	, refProject(project)
{
	ui.setupUi(this);

	connect(ui.pBtnOK, SIGNAL(clicked()), this, SLOT(slotOKClicked()));
}

JKNewStockCodeWgt::~JKNewStockCodeWgt()
{

}

JKRef_Ptr<JKStockCodeBLL> JKNewStockCodeWgt::getStockCode()
{
	return refStockCode;
}


void JKNewStockCodeWgt::slotOKClicked()
{
	refStockCode = refProject->newStockCode();
	QString name = ui.lEdtName->text();
	QString code = ui.lEdtCode->text();
	QString latestPrice = ui.lEdtLatestPrice->text();

	refStockCode->setParams(name.toStdString(), code.toStdString(), latestPrice.toDouble());
	
	this->accept();
}

