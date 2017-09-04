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
	this->updateStatusBar();

	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));

}

JKMainWin::~JKMainWin()
{

}

void JKMainWin::updateStatusBar()
{
	JKRef_Ptr<JKStockCodeBLL> refStockCode = refProject->getCurStockCode();
	if (refStockCode.valid())
	{
		QLabel* pQLabel = new QLabel(QString(refStockCode->getName().c_str()));
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
		this->updateStatusBar();
	}
	
}

void JKMainWin::buyStockCode()
{
	JKBuyStockCodeWgt buyStockCodeWgt(refProject);
	buyStockCodeWgt.exec();
}


