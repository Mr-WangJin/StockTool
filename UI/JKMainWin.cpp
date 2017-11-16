#include "stdafx.h"
#include <QFileDialog>
#include <QMessageBox>

#include "JKMainWin.h"
#include "JKNewStockCodeWgt.h"
#include "JKBuyStockCodeWgt.h"
#include "JKSettingLatestPrice.h"
#include "JKSellStockCodeWgt.h"
#include "JKNewProject.h"
#include "BLL/JKStockTradeUtil.h"
#include "JKStockTradeDetail.h"


JKMainWin::JKMainWin(/*JKProjectBLL* _projectBLL,*/ QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<JKString>("JKString");

	ui.setupUi(this);
	ui.trendChartWgt->setVisible(false);
	refProject = nullptr;

	this->initUI();
	this->updateInputUIEnable(nullptr);
	
	emit afterProjectChanged(refProject);
}

JKMainWin::~JKMainWin()
{
	JK_FreeAndNullptr(crawlPrice);
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
	QString fullName = QFileDialog::getOpenFileName(this, QStringLiteral("请选择工程文件......"));
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
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("请新建股票代码"));
	}
	else
	{
		JKBuyStockCodeWgt buyStockCodeWgt(_refStockCode);
		if (buyStockCodeWgt.exec() == QDialog::Accepted)
		{
			//ui.trendChartWgt->updateTrendChart();
			this->updateTableWidget(tbShowType);
			this->updateInfoWgt(_refStockCode);
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
			this->updateTableWidget(tbShowType);
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

			if (crawlPrice)
			{
				crawlPrice->clearStoclCode();
				crawlPrice->addStockCode(var);
			}
			break;
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

			emit afterStockCodeChanged(_refStockCode);
		}
	}
}

void JKMainWin::projectTaxSetting()
{
	JKNewProject newProject(this);
	newProject.setProject(refProject);
	if (newProject.exec() == QDialog::Accepted)
	{
		emit beforeProjectChanged();

		refProject->setStampTax(newProject.getStampTax());
		refProject->setTransfer(newProject.getTransfer());
		refProject->setCommission(newProject.getCommission());

		emit afterProjectChanged(refProject);
	}
}

void JKMainWin::crawlerOptChanged()
{
	if (crawlPrice)
	{
		if (ui.actCrawlerOpt->isChecked())
			crawlPrice->startRunCraw();
		else
			crawlPrice->stopRunCraw();
	}
}

void JKMainWin::onTableWgtPopMenu(QPoint pos)
{
	tableWgtPopMenu->exec(QCursor::pos());
}

void JKMainWin::onDeleteTrade()
{
	//删除有问题，需要改
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
			this->updateTableWidget(tbShowType);
		}
	}
}

void JKMainWin::onShowTradeInfo()
{
	if (!refProject.valid())
		return;

	JKString stockTradeId = ui.tableWidget->currentItem()->data(Qt::UserRole).toString().toStdString();
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (_refStockCode.valid())
	{
		JKRef_Ptr<JKStockCodeTradeBLL> refStockTrade = _refStockCode->getTradeById(stockTradeId);

		JKRef_Ptr<JKStockTradeUtil> refTradeUtil = new JKStockTradeUtil(refProject);
		JKStockTradeDetail detail(refTradeUtil, refStockTrade);
		detail.exec();
	}

}

void JKMainWin::onBeforeProjectChanged()
{
	ui.actCrawlerOpt->setChecked(false);
}

void JKMainWin::onAfterProjectChanged(JKRef_Ptr<JKProjectBLL> _refProject)
{
	updateUIEnable(_refProject);
	updateCmbBoxSwitch(_refProject);
	refreshCrawler(_refProject);
	crawlerOptChanged();
}

void JKMainWin::onShowBuyOnly()
{
	tbShowType = Show_Buy_Only;
	this->updateTableWidget(tbShowType);
}
void JKMainWin::onShowSellOnly()
{
	tbShowType = Show_Sell_Only;
	this->updateTableWidget(tbShowType);

}
void JKMainWin::onShowAll()
{
	tbShowType = Show_All;
	this->updateTableWidget(tbShowType);

}

void JKMainWin::stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (!_refStockCode.valid())
		return;
	this->updateStatusBar(_refStockCode);
	//ui.trendChartWgt->setStockCode(_refStockCode);
	this->updateTableWidget(tbShowType);
}

void JKMainWin::updateTableWidget(TableShowType type /*= Show_All*/)
{
	ui.tableWidget->clearContents();

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLLTemp = refProject->getCurStockCode()->getAllTrades();
	double latestPrice = refProject->getCurStockCode()->getLatestPrice();

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLL;
	for (auto &var :vecRefStockCodeTradeBLLTemp)
	{
		if (type & Show_Buy_Only && var->getType() == TradeType::BUY)
		{
			vecRefStockCodeTradeBLL.push_back(var);
		}
		if (type & Show_Sell_Only && var->getType() == TradeType::SELL)
		{
			vecRefStockCodeTradeBLL.push_back(var);
		}
		
	}

	ui.tableWidget->setRowCount(vecRefStockCodeTradeBLL.size());
	int i = 0;
	JKStockTradeUtil tradeUtil(refProject);
	for (auto &var : vecRefStockCodeTradeBLL)
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
		tbItem->setData(Qt::DisplayRole, var->getDate().c_str()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getCount()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getBuyPrice()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, tradeUtil.getTradeBuyCost(var)); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, var->getSellPrice()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);

		tbItem = new QTableWidgetItem();
		double preRealEarning = 0;
		if (var->getType() == TradeType::BUY)
			preRealEarning = tradeUtil.getRealEarning(latestPrice, var);
		else
			preRealEarning = tradeUtil.getRealEarning(var->getSellPrice(), var);
		tbItem->setData(Qt::DisplayRole, preRealEarning); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);
		tbItem = new QTableWidgetItem();
		tbItem->setData(Qt::DisplayRole, QString("%1%").arg(tradeUtil.getRealEarningPercent(latestPrice, var)*100));
		tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
		ui.tableWidget->setItem(i, j++, tbItem);

		i++;
	}
	ui.tableWidget->sortByColumn(1, Qt::SortOrder::AscendingOrder);

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

		JKStockTradeUtil trackUtil(refProject);
		for (auto &var : _refStockCode->getAllTrades())
		{
			if (TradeType::BUY == var->getType())
			{
				buySumPrice += var->getBuyPureCost();
				buySumCount += var->getCount();
				double preEarning = trackUtil.getRealEarning(latestPrice, var);

				buySumEarning += preEarning;
			}
			else
			{
				sellSumPrice += var->getBuyPrice();
				sellSumCount += var->getCount();
				sellSumEarning += trackUtil.getRealEarning(var->getSellPrice(), var);
			}
		}
	}

	ui.lblBuySumPrice->setText(QStringLiteral("买入总支出：") + QString::number(buySumPrice));
	ui.lblBuySumCount->setText(QStringLiteral("买入总数量：") + QString::number(buySumCount));
	ui.lblBuySumEarning->setText(QStringLiteral("预计总收益：") + QString::number(buySumEarning));

	ui.lblSellSumPrice->setText(QStringLiteral("卖出支出：") + QString::number(sellSumPrice));
	ui.lblSellSumCount->setText(QStringLiteral("卖出数量：") + QString::number(sellSumCount));
	ui.lblSellSumEarning->setText(QStringLiteral("卖出收益：") + QString::number(sellSumEarning));

}

void JKMainWin::initUI()
{
	tableWgtPopMenu = new QMenu(ui.tableWidget);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);

	QAction* actDel = new QAction(QStringLiteral("删除"), this);
	tableWgtPopMenu->addAction(actDel);
	connect(actDel, SIGNAL(triggered()), this, SLOT(onDeleteTrade()));
	QAction* actSell = new QAction(QStringLiteral("卖出"), this);
	tableWgtPopMenu->addAction(actSell);
	connect(actSell, SIGNAL(triggered()), this, SLOT(onSellTrade()));
	QAction* actShowDetail = new QAction(QStringLiteral("显示详情"), this);
	tableWgtPopMenu->addAction(actShowDetail);
	connect(actShowDetail, SIGNAL(triggered()), this, SLOT(onShowTradeInfo()));
	QAction* actShowBuyOnly = new QAction(QStringLiteral("只显示买入"), this);
	tableWgtPopMenu->addAction(actShowBuyOnly);
	connect(actShowBuyOnly, SIGNAL(triggered()), this, SLOT(onShowBuyOnly()));
	QAction* actShowSellOnly = new QAction(QStringLiteral("只显示卖入"), this);
	tableWgtPopMenu->addAction(actShowSellOnly);
	connect(actShowSellOnly, SIGNAL(triggered()), this, SLOT(onShowSellOnly()));
	QAction* actShowAll = new QAction(QStringLiteral("全部显示"), this);
	tableWgtPopMenu->addAction(actShowAll);
	connect(actShowAll, SIGNAL(triggered()), this, SLOT(onShowAll()));

	lblShowCurStock = new QLabel(QStringLiteral("当前显示股票："));
	ui.statusBar->addWidget(lblShowCurStock);

	lblLatestPrice = new QLabel(QStringLiteral("当前股票最新交易价："));
	ui.statusBar->addWidget(lblLatestPrice);

	ui.m_pHSplitter->setSizes(QList<int>() << 100 << 500);
	QHeaderView *headerGoods = ui.tableWidget->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	//headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), ui.tableWidget, SLOT(sortByColumn(int)));



	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));
	connect(ui.actSellStock, SIGNAL(triggered()), this, SLOT(sellStockCode()));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableWgtPopMenu(QPoint)));
	connect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()), Qt::UniqueConnection);
	//connect(ui.pBtnSetCurPrice, SIGNAL(clicked()), this, SLOT(setCurrentStockPrice()));
	connect(ui.actNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
	connect(ui.actOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui.actExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actTaxSetting, SIGNAL(triggered()), this, SLOT(projectTaxSetting()));
	connect(ui.actCrawlerOpt, SIGNAL(triggered()), this, SLOT(crawlerOptChanged()));


	connect(this, SIGNAL(beforeProjectChanged()), this, SLOT(onBeforeProjectChanged()));
	connect(this, SIGNAL(afterProjectChanged(JKRef_Ptr<JKProjectBLL>)), this, SLOT(onAfterProjectChanged(JKRef_Ptr<JKProjectBLL>)));

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

void JKMainWin::refreshCrawler(JKRef_Ptr<JKProjectBLL> _refProject)
{
	JK_FreeAndNullptr(crawlPrice);
	if (!_refProject.valid())
		return;

	crawlPrice = new JKCrawlPrice(this);
	crawlPrice->addStockCode(_refProject->getCurStockCode());

	connect(crawlPrice, SIGNAL(stockCodePriceChanged(JKString)), this, SLOT(stockCodePriceChanged(JKString)));
}

void JKMainWin::stockCodePriceChanged(JKString v)
{
	float laterPrice = QString(v.c_str()).toFloat();
	if (refProject.valid())
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
		if (_refStockCode.valid())
		{
			_refStockCode->setLatestPrice(laterPrice);

			emit afterStockCodeChanged(_refStockCode);
		}
	}
}

void JKMainWin::updateUIEnable(JKRef_Ptr<JKProjectBLL> _refProject)
{
	bool bUIEnable = _refProject.valid();
	//ui.trendChartWgt->setEnabled(bUIEnable);
	ui.tableWidget->setEnabled(bUIEnable);
	ui.cmbBxSwitch->setEnabled(bUIEnable);

	ui.actNewStockCode->setEnabled(bUIEnable);
	ui.actTaxSetting->setEnabled(bUIEnable);
}

void JKMainWin::updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	bool bInputEnable = _refStockCode.valid();

	ui.actBuyStock->setEnabled(bInputEnable);
	ui.actSellStock->setEnabled(bInputEnable);
}


