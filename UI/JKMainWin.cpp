#include "stdafx.h"
#include <QFileDialog>
#include <QMessageBox>

#include "JKMainWin.h"
#include "JKNewStockCodeWgt.h"
#include "JKBuyStockCodeWgt.h"
#include "JKSettingLatestPrice.h"
#include "JKSellStockCodeWgt.h"
#include "BLL\JKStockCodeTradeBLL.h"

#include "bll/JKProjectBLL.h"
#include "bll/JKStockCodeBLL.h"


JKMainWin::JKMainWin(/*JKProjectBLL* _projectBLL,*/ QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	refProject = nullptr;

	this->initUI();
	this->updateUIEnable();

//	this->stockCodeChanged(refProject->getCurStockCode());
}

JKMainWin::~JKMainWin()
{

}

void JKMainWin::updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (_refStockCode.valid())
	{
		lblShowCurStock->setText(QStringLiteral("当前显示股票：") + QString(_refStockCode->getName().c_str()));

		lblLatestPrice->setText(QStringLiteral("当前股票最新交易价：") + QString::number(_refStockCode->getLatestPrice()));
	}
	
}

void JKMainWin::newProject()
{
	QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择新建工程路径......"));
	if (path.isEmpty())
		return;
	QDir dir(path);
	QStringList fileNames = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	if (fileNames.count() > 0)
	{
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("文件夹不为空！"));
		return;
	}
	else
	{
		refProject = JKProjectBLL::newProject(path.toStdString());
		this->updateUIEnable();
		//this->stockCodeChanged(refProject->getCurStockCode());
	}
}

void JKMainWin::openProject()
{
	QString fullName = QFileDialog::getOpenFileName(this, QStringLiteral("请选择工程文件......"));
	if (fullName.isEmpty())
	{
		return;
	}
	else
	{
		refProject = JKProjectBLL::openProject(fullName.toStdString());
		this->updateUIEnable();
		this->stockCodeChanged(refProject->getCurStockCode());
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
		this->updateCmbBoxSwitch();
		this->updateInputUIEnable();
	}
}

void JKMainWin::buyStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (!_refStockCode.valid())
	{
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("请新建股票代码"));
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

void JKMainWin::sellStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (!_refStockCode.valid())
	{
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("请新建股票代码"));
	}
	else
	{
		JKSellStockCodeWgt sellStockCodeWgt(_refStockCode);
		if (sellStockCodeWgt.exec() == QDialog::Accepted)
		{
			//ui.trendChartWgt->updateTrendChart();
			this->updateTableWidget();
		}
	}
}

void JKMainWin::onSwitchCode()
{
	QString code = ui.cmbBxSwitch->currentData(Qt::UserRole).toString();

	vector<JKRef_Ptr<JKStockCodeBLL>> vecRefStockCodeBLL = refProject->getAllStockCode();
	for (auto var : vecRefStockCodeBLL)
	{
		if (QString(var->getCode().c_str()) == code)
		{
			refProject->setCurStockCode(var);
			this->stockCodeChanged(var);
		}
	}
}

void JKMainWin::setCurrentStockPrice()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (_refStockCode.valid())
	{
		JKSettingLatestPrice setting(_refStockCode->getLatestPrice(), this);
		if (setting.exec() == QDialog::Accepted)
		{
			_refStockCode->setLatestPrice(setting.getLatestPrice());
			this->latestPriceChanged(_refStockCode);
		}
	}
}

void JKMainWin::onTableWgtPopMenu(QPoint pos)
{
	tableWgtPopMenu->exec(QCursor::pos());
}

void JKMainWin::onDeleteTrade()
{
	if (!refProject.valid())
		return;

	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (_refStockCode.valid())
	{
		vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLL = _refStockCode->getAllTrades();
		int rowNum = ui.tableWidget->currentRow();
		if (_refStockCode->deleteTrade(vecRefStockCodeTradeBLL[rowNum]))
		{
			this->stockCodeChanged(_refStockCode);
		}
	}
}

void JKMainWin::onSellTrade()
{
	if (!refProject.valid())
		return;

	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (_refStockCode.valid())
	{
		JKSellStockCodeWgt sellStockCodeWgt(_refStockCode);
		if (sellStockCodeWgt.exec() == QDialog::Accepted)
		{
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
	this->updateCmbBoxSwitch();
}

void JKMainWin::latestPriceChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (!_refStockCode.valid())
		return;
	this->updateStatusBar(_refStockCode);
	this->updateTableWidget();
}

void JKMainWin::updateTableWidget()
{
	ui.tableWidget->clearContents();

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLL = refProject->getCurStockCode()->getAllTrades();
	double latestPrice = refProject->getCurStockCode()->getLatestPrice();

	ui.tableWidget->setRowCount(vecRefStockCodeTradeBLL.size());
	int i = 0;
	for (auto var : vecRefStockCodeTradeBLL)
	{
		int j = 0;
		QTableWidgetItem* tbItem = new QTableWidgetItem();
		switch ((int)var->getType())
		{
		case 1:
			tbItem->setData(Qt::DisplayRole, QStringLiteral("买入"));
			break;
		case 2:
			tbItem->setData(Qt::DisplayRole, QStringLiteral("卖出"));
			break;
		default:
			break;
		}
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getDate().c_str());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getCount());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getBuyPrice());
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getEarning(latestPrice));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, QString("%1%").arg(var->getEarningPercent(latestPrice)));
		ui.tableWidget->setItem(i, j++, tbItem);

		i++;
	}
}

void JKMainWin::initUI()
{

	tableWgtPopMenu = new QMenu(ui.tableWidget);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	QAction* actDel = new QAction(QStringLiteral("删除"), this);
	tableWgtPopMenu->addAction(actDel);
	connect(actDel, SIGNAL(triggered()), this, SLOT(onDeleteTrade()));
	QAction* actSell = new QAction(QStringLiteral("卖出"), this);
	tableWgtPopMenu->addAction(actSell);
	connect(actSell, SIGNAL(triggered()), this, SLOT(onSellTrade()));

	lblShowCurStock = new QLabel(QStringLiteral("当前显示股票："));
	ui.statusBar->addWidget(lblShowCurStock);

	lblLatestPrice = new QLabel(QStringLiteral("当前股票最新交易价："));
	ui.statusBar->addWidget(lblLatestPrice);

	ui.m_pHSplitter->setSizes(QList<int>() << 200 << 400);
	this->updateCmbBoxSwitch();

	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));
	connect(ui.actSellStock, SIGNAL(triggered()), this, SLOT(sellStockCode()));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableWgtPopMenu(QPoint)));
	connect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()), Qt::UniqueConnection);
	connect(ui.pBtnSetCurPrice, SIGNAL(clicked()), this, SLOT(setCurrentStockPrice()));
	connect(ui.actNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
	connect(ui.actOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui.actExit, SIGNAL(triggered()), this, SLOT(close()));

}

void JKMainWin::updateCmbBoxSwitch()
{
	if (!refProject.valid())
		return;

	disconnect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()));

	ui.cmbBxSwitch->clear();
	vector<JKRef_Ptr<JKStockCodeBLL>> vecRefStockCodeBLL = refProject->getAllStockCode();
	for (auto var : vecRefStockCodeBLL)
	{
		ui.cmbBxSwitch->addItem(QString(var->getName().c_str()), QVariant(QString(var->getCode().c_str())));
	}

	connect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()));

}

void JKMainWin::updateUIEnable()
{
	bool bUIEnable = false;
	if (refProject.valid())
		bUIEnable = true;
	ui.trendChartWgt->setEnabled(bUIEnable);
	ui.tableWidget->setEnabled(bUIEnable);
	ui.cmbBxSwitch->setEnabled(bUIEnable);
	ui.pBtnSetCurPrice->setEnabled(bUIEnable);

	ui.actNewStockCode->setEnabled(bUIEnable);

	this->updateInputUIEnable();
}

void JKMainWin::updateInputUIEnable()
{
	bool bInputEnable = refProject.valid();
	if (bInputEnable)
	{
		JKRef_Ptr<JKStockCodeBLL> refCurStockCode = refProject->getCurStockCode();
		if (!refCurStockCode.valid())
			bInputEnable = false;
	}

	ui.actBuyStock->setEnabled(bInputEnable);
	ui.actSellStock->setEnabled(bInputEnable);
}


