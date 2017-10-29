#include "stdafx.h"
#include <QFileDialog>
#include <QMessageBox>

#include "JKMainWin.h"
#include "JKNewStockCodeWgt.h"
#include "JKBuyStockCodeWgt.h"
#include "JKSettingLatestPrice.h"
#include "JKSellStockCodeWgt.h"
#include "JKNewProject.h"



JKMainWin::JKMainWin(/*JKProjectBLL* _projectBLL,*/ QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	refProject = nullptr;

	this->initUI();
	this->updateInputUIEnable(nullptr);
	
	emit afterProjectChanged(refProject);

}

JKMainWin::~JKMainWin()
{

}

void JKMainWin::updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (_refStockCode.valid())
	{
		lblShowCurStock->setText(QStringLiteral("��ǰ��ʾ��Ʊ��") + QString(_refStockCode->getName().c_str()));

		lblLatestPrice->setText(QStringLiteral("��ǰ��Ʊ���½��׼ۣ�") + QString::number(_refStockCode->getLatestPrice()));
	}
	
}

void JKMainWin::newProject()
{
	QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("��ѡ���½�����·��......"));
	if (path.isEmpty())
		return;
	QDir dir(path);
	QStringList fileNames = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	if (fileNames.count() > 0)
	{
		QMessageBox::information(this, QStringLiteral("��ʾ��"), QStringLiteral("�ļ��в�Ϊ�գ�"));
		return;
	}
	else
	{
		JKNewProject newProject(this);
		if (newProject.exec() == QDialog::Accepted)
		{
			emit beforeProjectChanged();

			refProject = JKProjectBLL::newProject(path.toStdString());
			refProject->setStampTax(newProject.getStampTax());
			refProject->setTransfer(newProject.getTransfer());
			refProject->setCommission(newProject.getCommission());

			emit afterProjectChanged(refProject);
		}
	}
}

void JKMainWin::openProject()
{
	QString fullName = QFileDialog::getOpenFileName(this, QStringLiteral("��ѡ�񹤳��ļ�......"));
	if (fullName.isEmpty())
	{
		return;
	}
	else
	{
		emit beforeProjectChanged();

		refProject = JKProjectBLL::openProject(fullName.toStdString());

		emit afterProjectChanged(refProject);
		emit afterStockCodeChanged(refProject->getCurStockCode());
	}
}

void JKMainWin::newStockCode()
{
	JKNewStockCodeWgt newStockCodeWgt(refProject);
	if (QDialog::Accepted == newStockCodeWgt.exec())
	{
		emit beforeAddedNewStockCode();

		JKRef_Ptr<JKStockCodeBLL> refCurStockCode = newStockCodeWgt.getStockCode();
		refProject->setCurStockCode(refCurStockCode);

		emit afterAddedNewStockCode(refCurStockCode);
		emit afterStockCodeChanged(refCurStockCode);
	}
}

void JKMainWin::buyStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (!_refStockCode.valid())
	{
		QMessageBox::information(this, QStringLiteral("��ʾ��"), QStringLiteral("���½���Ʊ����"));
	}
	else
	{
		JKBuyStockCodeWgt buyStockCodeWgt(_refStockCode);
		if (buyStockCodeWgt.exec() == QDialog::Accepted)
		{
			ui.trendChartWgt->updateTrendChart();
			this->updateTableWidget();
			this->updateInfoWgt(_refStockCode);
		}
	}
}

void JKMainWin::sellStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (!_refStockCode.valid())
	{
		QMessageBox::information(this, QStringLiteral("��ʾ��"), QStringLiteral("���½���Ʊ����"));
	}
	else
	{
		JKSellStockCodeWgt sellStockCodeWgt(_refStockCode);
		if (sellStockCodeWgt.exec() == QDialog::Accepted)
		{
			ui.trendChartWgt->updateTrendChart();
			this->updateTableWidget();
			this->updateInfoWgt(_refStockCode);
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
			emit beforeStockCodeChanged();

			refProject->setCurStockCode(var);

			emit afterStockCodeChanged(var);
		}
	}
}

void JKMainWin::setCurrentStockPrice()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (_refStockCode.valid())
	{
		JKSettingLatestPrice setting(_refStockCode, this);
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
	for (auto &var : vecRefStockCodeTradeBLL)
	{
		int j = 0;
		QTableWidgetItem* tbItem = new QTableWidgetItem();
		switch ((int)var->getType())
		{
		case 1:
			tbItem->setData(Qt::DisplayRole, QStringLiteral("����"));
			break;
		case 2:
			tbItem->setData(Qt::DisplayRole, QStringLiteral("����"));
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
		tbItem->setData(Qt::DisplayRole, refProject->getCostPrice(var));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getSellPrice());
		ui.tableWidget->setItem(i, j++, tbItem);

		tbItem = new QTableWidgetItem();
		double preEarning = var->getEarning(latestPrice) - refProject->getBuyTaxes(var) - refProject->getPredictSellTaxes(var, latestPrice);
		tbItem->setData(Qt::DisplayRole, preEarning);
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, QString("%1%").arg(preEarning/var->getInputPrice()*100));
		ui.tableWidget->setItem(i, j++, tbItem);

		i++;
	}
}

void JKMainWin::updateInfoWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	double buySumPrice = 0;
	double buySumCount = 0;
	double buySumEarning = 0;
	double sellSumPrice = 0;
	double sellSumCount = 0;
	double sellSumEarning = 0;

	if (_refStockCode.valid())
	{
		double latestPrice = _refStockCode->getLatestPrice();

		for (auto &var : _refStockCode->getAllTrades())
		{
			if (TradeType::BUY == var->getType())
			{
				buySumPrice += var->getInputPrice();
				buySumCount += var->getCount();
				double preEarning = var->getEarning(latestPrice) - refProject->getBuyTaxes(var) - refProject->getPredictSellTaxes(var, latestPrice);

				buySumEarning += preEarning;
			}
			else
			{
				sellSumPrice += var->getBuyPrice();
				sellSumCount += var->getCount();
				sellSumEarning += var->getEarning(latestPrice);
			}
		}
	}

	ui.lblBuySumPrice->setText(QStringLiteral("������֧����") + QString::number(buySumPrice));
	ui.lblBuySumCount->setText(QStringLiteral("������������") + QString::number(buySumCount));
	ui.lblBuySumEarning->setText(QStringLiteral("Ԥ�������棺") + QString::number(buySumEarning));

	ui.lblSellSumPrice->setText(QStringLiteral("����֧����") + QString::number(sellSumPrice));
	ui.lblSellSumCount->setText(QStringLiteral("����������") + QString::number(sellSumCount));
	ui.lblSellSumEarning->setText(QStringLiteral("�������棺") + QString::number(sellSumEarning));

}

void JKMainWin::initUI()
{
	tableWgtPopMenu = new QMenu(ui.tableWidget);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);

	QAction* actDel = new QAction(QStringLiteral("ɾ��"), this);
	tableWgtPopMenu->addAction(actDel);
	connect(actDel, SIGNAL(triggered()), this, SLOT(onDeleteTrade()));
	QAction* actSell = new QAction(QStringLiteral("����"), this);
	tableWgtPopMenu->addAction(actSell);
	connect(actSell, SIGNAL(triggered()), this, SLOT(onSellTrade()));

	lblShowCurStock = new QLabel(QStringLiteral("��ǰ��ʾ��Ʊ��"));
	ui.statusBar->addWidget(lblShowCurStock);

	lblLatestPrice = new QLabel(QStringLiteral("��ǰ��Ʊ���½��׼ۣ�"));
	ui.statusBar->addWidget(lblLatestPrice);

	ui.m_pHSplitter->setSizes(QList<int>() << 100 << 500);


	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));
	connect(ui.actSellStock, SIGNAL(triggered()), this, SLOT(sellStockCode()));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableWgtPopMenu(QPoint)));
	connect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()), Qt::UniqueConnection);
	connect(ui.pBtnSetCurPrice, SIGNAL(clicked()), this, SLOT(setCurrentStockPrice()));
	connect(ui.actNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
	connect(ui.actOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui.actExit, SIGNAL(triggered()), this, SLOT(close()));


	connect(this, SIGNAL(afterProjectChanged(JKRef_Ptr<JKProjectBLL>)), this, SLOT(updateUIEnable(JKRef_Ptr<JKProjectBLL>)));
	connect(this, SIGNAL(afterProjectChanged(JKRef_Ptr<JKProjectBLL>)), this, SLOT(updateCmbBoxSwitch(JKRef_Ptr<JKProjectBLL>)));
	connect(this, SIGNAL(afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>)), this, SLOT(updateInfoWgt(JKRef_Ptr<JKStockCodeBLL>)));
	connect(this, SIGNAL(afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>)), this, SLOT(stockCodeChanged(JKRef_Ptr<JKStockCodeBLL>)));
	connect(this, SIGNAL(afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>)), this, SLOT(updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL>)));
	connect(this, SIGNAL(afterAddedNewStockCode(JKRef_Ptr<JKStockCodeBLL>)), this, SLOT(addedCmbBoxSwitch(JKRef_Ptr<JKStockCodeBLL>)));
	connect(this, SIGNAL(afterAddedNewStockCode(JKRef_Ptr<JKStockCodeBLL>)), this, SLOT(updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL>)));


}

void JKMainWin::updateCmbBoxSwitch(JKRef_Ptr<JKProjectBLL> _refProject)
{
	if (!_refProject.valid())
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

void JKMainWin::addedCmbBoxSwitch(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (!_refStockCode.valid())
		return;

	ui.cmbBxSwitch->addItem(QString(_refStockCode->getName().c_str()), QVariant(QString(_refStockCode->getCode().c_str())));
	ui.cmbBxSwitch->setCurrentText(QString(_refStockCode->getName().c_str()));

}

void JKMainWin::updateUIEnable(JKRef_Ptr<JKProjectBLL> _refProject)
{
	bool bUIEnable = _refProject.valid();
	ui.trendChartWgt->setEnabled(bUIEnable);
	ui.tableWidget->setEnabled(bUIEnable);
	ui.cmbBxSwitch->setEnabled(bUIEnable);
	ui.pBtnSetCurPrice->setEnabled(bUIEnable);

	ui.actNewStockCode->setEnabled(bUIEnable);

}

void JKMainWin::updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	bool bInputEnable = _refStockCode.valid();

	ui.actBuyStock->setEnabled(bInputEnable);
	ui.actSellStock->setEnabled(bInputEnable);
}


