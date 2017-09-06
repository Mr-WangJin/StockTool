#include "stdafx.h"
#include "JKMainWin.h"
#include "JKNewStockCodeWgt.h"
#include "JKBuyStockCodeWgt.h"

#include "bll/JKProjectBLL.h"
#include "bll/JKStockCodeBLL.h"


JKMainWin::JKMainWin(JKProjectBLL* _projectBLL, QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	refProject = _projectBLL;
	this->stockCodeChanged(refProject->getCurStockCode());

	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));

}

JKMainWin::~JKMainWin()
{

}

void JKMainWin::updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	
	if (_refStockCode.valid())
	{
		QLabel* pQLabel = new QLabel(QString(_refStockCode->getName().c_str()));
		ui.statusBar->addWidget(pQLabel);
	}
}

void JKMainWin::newStockCode()
{
	JKNewStockCodeWgt newStockCodeWgt(refProject);
	if (QDialog::Accepted == newStockCodeWgt.exec())
	{
		JKRef_Ptr<JKStockCodeBLL> refCurStockCode = newStockCodeWgt.getStockCode();
		refProject->setCurStockCode(refCurStockCode);
		this->stockCodeChanged(refCurStockCode);
	}
	
}

void JKMainWin::buyStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (!_refStockCode.valid())
	{

	}
	else
	{
		JKBuyStockCodeWgt buyStockCodeWgt(_refStockCode);
		if (buyStockCodeWgt.exec() == QDialog::Accepted)
			ui.trendChartWgt->updateTrendChart();
	}
}

void JKMainWin::stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (!_refStockCode.valid())
		return;
	this->updateStatusBar(_refStockCode);
	ui.trendChartWgt->setStockCode(_refStockCode);
}


