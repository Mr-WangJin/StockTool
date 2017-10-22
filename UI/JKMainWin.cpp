#include "stdafx.h"

#include "JKMainWin.h"
#include "JKNewStockCodeWgt.h"
#include "JKBuyStockCodeWgt.h"
#include "BLL\JKStockCodeTradeBLL.h"

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
		QLabel* pQLabel = new QLabel(QStringLiteral("当前显示股票：")+ QString(_refStockCode->getName().c_str()));
		ui.statusBar->addWidget(pQLabel);

		pQLabel = new QLabel(QStringLiteral("当前股票最新交易价：") + QString::number(_refStockCode->getLatestPrice()));
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
		{
			ui.trendChartWgt->updateTrendChart();
			this->updateTableWidget();
		}
	}
}

void JKMainWin::stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (!_refStockCode.valid())
		return;
	this->updateStatusBar(_refStockCode);
	ui.trendChartWgt->setStockCode(_refStockCode);
	this->updateTableWidget();
}

void JKMainWin::updateTableWidget()
{
	ui.tableWidget->clearContents();

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _refStockCodeTradeBLL = refProject->getCurStockCode()->getAllTrades();
	double latestPrice = refProject->getCurStockCode()->getLatestPrice();

	ui.tableWidget->setRowCount(_refStockCodeTradeBLL.size());
	int i = 0;
	for (auto var : _refStockCodeTradeBLL)
	{
		int j = 0;
		QTableWidgetItem* tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, (int)var->getType());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getDate().c_str());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getCount());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getPrice());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getEarning(latestPrice));
		ui.tableWidget->setItem(i, j++, tbItem);

		i++;
	}
}


