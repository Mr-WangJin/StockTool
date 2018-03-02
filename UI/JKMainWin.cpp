#include "stdafx.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStyledItemDelegate>

#include "JKMainWin.h"
#include "JKNewStockCodeWgt.h"
#include "JKBuyStockCodeWgt.h"
#include "JKSettingLatestPrice.h"
#include "JKSellStockCodeWgt.h"
#include "JKNewProject.h"
#include "BLL/JKStockTradeUtil.h"
#include "JKStockTradeDetail.h"
#include "JKAbout.h"
#include "Utils/JKRecentProject.h"
#include "JKUiContext.h"
#include "JKSetTradeProperty.h"
#include "BLL/JKProjectSettingBLL.h"
#include <QSettings>

#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"



JKMainWin::JKMainWin(/*JKProjectBLL* _projectBLL,*/ QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<JKString>("JKString");

	ui.setupUi(this);
	ui.trendChartWgt->setVisible(false);
	refProject = nullptr;

	JKSingleton<JKUiContext>::GetInstance().setMainWin(this);
	this->initUI();
	this->updateInputUIEnable(nullptr);
	this->setAutoStartExe(true);
	
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
//		lblShowCurStock->setText(QStringLiteral("当前显示股票：") + QString(_refStockCode->getName().c_str()));

		lblLatestPrice->setText(QStringLiteral("当前股票最新交易价：") + QString::number(_refStockCode->getLatestPrice()));
	}
	
}

void JKMainWin::setActivateWindow()
{
	Qt::WindowStates winStatus = Qt::WindowNoState;
	if (windowState() & Qt::WindowMaximized)
	{
		winStatus = Qt::WindowMaximized;
	}
	setWindowState(Qt::WindowMinimized);
	setWindowState(Qt::WindowActive | winStatus);
	//setGeometry(curGemRect);
	activateWindow();
	raise();
}

void JKMainWin::setAutoStartExe(bool is_auto_start)
{
	QString application_name = QApplication::applicationName();
	QSettings *settings = new QSettings(REG_RUN, QSettings::NativeFormat);
	if (is_auto_start)
	{
		QString application_path = QApplication::applicationFilePath();
		settings->setValue(application_name, application_path.replace("/", "\\"));
	}
	else
	{
		settings->remove(application_name);
	}
	delete settings;
}

void JKMainWin::showAbout()
{
	this->about();
}

JKRef_Ptr<JKProjectBLL> JKMainWin::getProjectBLL()
{
	return refProject;
}

void JKMainWin::newProject()
{
	QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("新建工程......"), "", "JKProject (*.jk)");
	if (fileName.isEmpty())
		return;

	//QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择新建工程路径......"));
	//if (path.isEmpty())
	//	return;
	//QDir dir(path);
	//QStringList fileNames = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	//if (fileNames.count() > 0)
	//{
	//	QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("文件夹不为空！"));
	//	return;
	//}
	//else
	{
		JKNewProject newProject(this);
		if (newProject.exec() == QDialog::Accepted)
		{
			emit beforeProjectChanged();

			refProject = JKProjectBLL::newProject(fileName.toStdString());
			refProject->setStampTax(newProject.getStampTax());
			refProject->setTransfer(newProject.getTransfer());
			refProject->setCommission(newProject.getCommission());

			JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = refProject->getProjectSetting();
			_refProjectSetting->setUpAlertPercent(newProject.getUpAlertPercent());
			_refProjectSetting->setUpAlertPercent(newProject.getDownAlertPercent());

			emit afterProjectChanged(refProject);
		}
	}
}

void JKMainWin::openRecentProject()
{
	QAction* pAct = dynamic_cast<QAction*>(sender());
	if (pAct == nullptr)
		return;

	emit beforeProjectChanged();

	refProject = JKProjectBLL::openProject(pAct->data().toString().toStdString());

	emit afterProjectChanged(refProject);
	emit afterStockCodeChanged(refProject->getCurStockCode());
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

		/** 更新最近打开的项目 */
		JKRecentProject recentProject(JKSingleton<JKUiContext>::GetInstance().getRecentFilePath());
		recentProject.addRecentProject(QFileInfo(fullName).baseName().toStdString(), fullName.toStdString());
		recentProject.saveToFile(JKSingleton<JKUiContext>::GetInstance().getRecentFilePath());

		emit afterProjectChanged(refProject);
		emit afterStockCodeChanged(refProject->getCurStockCode());
	}
}

void JKMainWin::newStockCode()
{
	JKNewStockCodeWgt newStockCodeWgt(refProject);
	if (QDialog::Accepted == newStockCodeWgt.exec())
	{

		JKRef_Ptr<JKStockCodeBLL> refCurStockCode = newStockCodeWgt.getStockCode();
		refProject->setCurStockCode(refCurStockCode);

		addedNewStockCode(refCurStockCode);
		emit afterStockCodeChanged(refCurStockCode);
	}
}

void JKMainWin::deleteCurrentStock()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
	if (_refStockCode.valid() == false)
		return;

	refProject->deleteStockCode(_refStockCode);

	this->updateCmbBoxSwitch(refProject);
	if (ui.cmbBxSwitch->count() >0 )
		this->onSwitchCode();
	else
	{
		onAfterStockChanged(nullptr);
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
		QMessageBox::information(this, QStringLiteral("提示！"), QStringLiteral("请新建股票代码"));
	}
	else
	{
		std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecStockTrade;
		JKSellStockCodeWgt sellStockCodeWgt(refProject, vecStockTrade);
		if (sellStockCodeWgt.exec() == QDialog::Accepted)
		{
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

			if (crawlPrice)
			{
				crawlPrice->clearStoclCode();
				crawlPrice->addStockCode(var);
			}
			
			lblLatestPrice->setText(QString("%1%2").arg(QStringLiteral("当前股票最新交易价：")).arg(var->getLatestPrice()));

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

		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = refProject->getProjectSetting();
		_refProjectSetting->setUpAlertPercent(newProject.getUpAlertPercent());
		_refProjectSetting->setDownAlertPercent(newProject.getDownAlertPercent());

		emit afterProjectChanged(refProject);
	}
}

void JKMainWin::setTradeProperty()
{
	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecRefStockCodeTrade;
	ui.tableView->getSelectedStockTrade(_vecRefStockCodeTrade);

	if (_vecRefStockCodeTrade.size() == 0)
		return;


	JKSetTradeProperty dlg(this);
	dlg.setStockCodeTrade(_vecRefStockCodeTrade[0]);
	dlg.exec();
}

void JKMainWin::crawlerOptChanged(bool checked)
{
	if (refProject)
	{
		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = refProject->getProjectSetting();
		_refProjectSetting->setIsStartCrawl(checked);
	}
	if (crawlPrice)
	{
		if (checked)
			crawlPrice->startRunCraw();
		else
			crawlPrice->stopRunCraw();
	}
}

void JKMainWin::alertChanged(bool checked)
{
	if (refProject)
	{
		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = refProject->getProjectSetting();
		_refProjectSetting->setIsAlert(checked);
	}
}

void JKMainWin::about()
{
	JKAbout about(this);
	about.exec();
}

void JKMainWin::onSystemTrayIconActive(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
// 		systemTrayIcon->showMessage(QStringLiteral("JKStockTool"), 
// 			QStringLiteral("感谢您的支持......"), QSystemTrayIcon::Information, 1000);
		break;
	case QSystemTrayIcon::DoubleClick:
		this->show();
		break;
	default:
		break;
	}
}

void JKMainWin::onExitApp()
{
	exit(0);
}

void JKMainWin::onShowApp()
{
	this->show();
}

void JKMainWin::onTableWgtPopMenu(QPoint pos)
{
	tableWgtPopMenu->exec(QCursor::pos());
}

void JKMainWin::onDeleteTrade()
{
	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecRefStockCodeTrade;
	ui.tableView->getSelectedStockTrade(_vecRefStockCodeTrade);

	if (_vecRefStockCodeTrade.size() > 0)
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
		for (auto &var : _vecRefStockCodeTrade)
		{
			_refStockCode->deleteTrade(var);
		}
		emit afterStockCodeChanged(_refStockCode);
	}
}

void JKMainWin::onSellTrade()
{
	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecRefStockCodeTrade;
	ui.tableView->getSelectedStockTrade(_vecRefStockCodeTrade);

	if (_vecRefStockCodeTrade.size() > 0)
	{
		JKSellStockCodeWgt sellStockCodeWgt(refProject, _vecRefStockCodeTrade, this);
		if (sellStockCodeWgt.exec() == QDialog::Accepted)
		{
			this->updateTableWidget();
		}
	}
}

void JKMainWin::onShowTradeInfo()
{
	if (!refProject.valid())
		return;

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecRefStockCodeTrade;
	ui.tableView->getSelectedStockTrade(_vecRefStockCodeTrade);
	
	if (_vecRefStockCodeTrade.size() == 0)
		return;

	JKRef_Ptr<JKStockTradeUtil> refTradeUtil = new JKStockTradeUtil(refProject);
	JKStockTradeDetail detail(refTradeUtil, _vecRefStockCodeTrade[0], refProject->getCurStockCode()->getLatestPrice());
	detail.exec();
}

void JKMainWin::onBeforeProjectChanged()
{
	//ui.actCrawlerOpt->setChecked(false);
}

void JKMainWin::onAfterProjectChanged(JKRef_Ptr<JKProjectBLL> _refProject)
{
	updateUIEnable(_refProject);
	updateCmbBoxSwitch(_refProject);
	refreshCrawler(_refProject);
	this->updateTableWidget();

	if (_refProject == nullptr)
		return;
	JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = _refProject->getProjectSetting();
	if (_refProjectSetting->getIsStartCrawl())
	{
		ui.actCrawlerOpt->setChecked(Qt::Checked);
		crawlerOptChanged(true);
	}
	else
	{
		ui.actCrawlerOpt->setChecked(Qt::Unchecked);
		crawlerOptChanged(false);
	}

	if (_refProjectSetting->getIsAlert())
	{
		ui.actAlert->setChecked(Qt::Checked);
		alertChanged(true);
	}
	else
	{
		ui.actAlert->setChecked(Qt::Unchecked);
		alertChanged(false);
	}

	TableShowType tableShowType = (TableShowType)_refProjectSetting->getTableShowType();
	switch (tableShowType)
	{
	case Show_Buy_Only:
		ui.actOnlyShowBuy->setChecked(true);
		this->onShowBuyOnly(true);
		break;
	case Show_Sell_Only:
		ui.actOnlyShowSold->setChecked(true);
		this->onShowSellOnly(true);
		break;
	case Show_All:
		ui.actShowAll->setChecked(true);
		this->onShowAll(true);
		break;
	default:
		break;
	}
}

void JKMainWin::onShowBuyOnly(bool checked)
{
	if (!checked)
		return;
	tableModel->setShowType(Show_Buy_Only);
	this->updateTableWidget();
}
void JKMainWin::onShowSellOnly(bool checked)
{
	if (!checked)
		return;
	tableModel->setShowType(Show_Sell_Only);
	this->updateTableWidget();
}
void JKMainWin::onShowAll(bool checked)
{
	if (!checked)
		return;
	tableModel->setShowType(Show_All);
	this->updateTableWidget();
}

void JKMainWin::updateTableWidget()
{
	tableModel->setProject(refProject);
// 	ui.tableWidget->clearContents();
// 
// 	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLLTemp = refProject->getCurStockCode()->getAllTrades();
// 	double latestPrice = refProject->getCurStockCode()->getLatestPrice();
// 
// 	vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLL;
// 	for (auto &var :vecRefStockCodeTradeBLLTemp)
// 	{
// 		if (type & Show_Buy_Only && var->getType() == TradeType::BUY)
// 		{
// 			vecRefStockCodeTradeBLL.push_back(var);
// 		}
// 		if (type & Show_Sell_Only && var->getType() == TradeType::SELL)
// 		{
// 			vecRefStockCodeTradeBLL.push_back(var);
// 		}
// 		
// 	}
// 
// 	ui.tableWidget->setRowCount(vecRefStockCodeTradeBLL.size());
// 	int i = 0;
// 	JKStockTradeUtil tradeUtil(refProject);
// 	for (auto &var : vecRefStockCodeTradeBLL)
// 	{
// 		int j = 0;
// 
// 		QTableWidgetItem* tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		switch ((int)var->getType())
// 		{
// 		case 1:
// 			tbItem->setData(Qt::DisplayRole, QStringLiteral("买入"));
// 			break;
// 		case 2:
// 			tbItem->setData(Qt::DisplayRole, QStringLiteral("卖出"));
// 			break;
// 		default:
// 			break;
// 		}
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 		tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::DisplayRole, var->getDate().c_str()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 		tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::DisplayRole, var->getCount()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 		tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::DisplayRole, var->getBuyPrice()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 		tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::DisplayRole, tradeUtil.getTradeBuyCost(var)); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 		tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::DisplayRole, var->getSellPrice()); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 
// 		tbItem = new QTableWidgetItem();
// 		double preRealEarning = 0;
// 		if (var->getType() == TradeType::BUY)
// 			preRealEarning = tradeUtil.getRealEarning(latestPrice, var);
// 		else if (var->getType() == TradeType::SELL)
// 			preRealEarning = tradeUtil.getRealEarning(var->getSellPrice(), var);
// 		tbItem->setData(Qt::DisplayRole, preRealEarning); tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 		tbItem = new QTableWidgetItem();
// 		tbItem->setData(Qt::DisplayRole, QString("%1%").arg(tradeUtil.getRealEarningPercent(latestPrice, var)*100));
// 		tbItem->setData(Qt::UserRole, QString::fromStdString(var->getId()));
// 		ui.tableWidget->setItem(i, j++, tbItem);
// 
// 		i++;
// 	}
// 	ui.tableWidget->sortByColumn(1, Qt::SortOrder::AscendingOrder);
// 
}

void JKMainWin::onAfterStockChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	//if (_refStockCode.valid())
	{
		this->updateInfoWgt(_refStockCode);
		
		this->updateStatusBar(_refStockCode);
		this->updateTableWidget();

		this->updateInputUIEnable(_refStockCode);

		if (systemTrayIcon)
		{
			systemTrayIcon->setToolTip(QString("JKStockTool\n%1%2").arg(QStringLiteral("最新价：")).arg(_refStockCode->getLatestPrice()));
		}
	}
	
}

void JKMainWin::addedNewStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	addedCmbBoxSwitch(_refStockCode);
	updateInputUIEnable(_refStockCode);
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
		std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockCodeTrade;
		_refStockCode->getAllTrades(_vecStockCodeTrade);
		for (auto &var : _vecStockCodeTrade)
		{
			if (TradeType::BUY == var->getType())
			{
				buySumPrice += var->getBuyPureCost();
				buySumCount += var->getCount();
				double preEarning = trackUtil.getExpactEarning(latestPrice, var);

				buySumEarning += preEarning;
			}
			else if (TradeType::SELL == var->getType())
			{
				sellSumPrice += var->getSoldCount()*var->getBuyPrice() + var->getRealEarning();
				sellSumCount += var->getSoldCount();
				sellSumEarning += var->getRealEarning();
			}
			else if (TradeType::PART == var->getType())
			{
				buySumPrice += var->getCouldSellCount()*var->getBuyPrice();
				buySumCount += var->getCouldSellCount();
				double preEarning = trackUtil.getExpactEarning(latestPrice, var);
				buySumEarning += preEarning;

				sellSumPrice += var->getSoldCount()*var->getBuyPrice() + var->getRealEarning();
				sellSumCount += var->getSoldCount();
				sellSumEarning += var->getRealEarning();
			}
		}
	}

	ui.lblBuySumPrice->setText(QStringLiteral("买入总额：") + QString::number(buySumPrice));
	ui.lblBuySumCount->setText(QStringLiteral("买入数量：") + QString::number(buySumCount));
	ui.lblBuySumEarning->setText(QStringLiteral("预计收益：") + QString::number(buySumEarning));

	ui.lblSellSumPrice->setText(QStringLiteral("卖出总额：") + QString::number(sellSumPrice));
	ui.lblSellSumCount->setText(QStringLiteral("卖出数量：") + QString::number(sellSumCount));
	ui.lblSellSumEarning->setText(QStringLiteral("卖出收益：") + QString::number(sellSumEarning));

}

void JKMainWin::initUI()
{
	/** 初始化最新打开工程 */
	JKRecentProject recentProject(JKSingleton<JKUiContext>::GetInstance().getRecentFilePath());
	int itemCount = recentProject.getCount();
	for (int i = 0; i< itemCount; ++i)
	{
		QAction* pAct = new QAction(QString::fromStdString(recentProject.getNameByIndex(i)));
		pAct->setData(QString::fromStdString(recentProject.getFilePathByIndex(i)));
		connect(pAct, SIGNAL(triggered()), this, SLOT(openRecentProject()));
		ui.menuRecentOpen->addAction(pAct);
	}

	JKSingleton<JKUiContext>::GetInstance();

	tableWgtPopMenu = new QMenu(ui.tableView);
	ui.tableView->setContextMenuPolicy(Qt::CustomContextMenu);

	QAction* actDel = new QAction(QStringLiteral("删除"), this);
	tableWgtPopMenu->addAction(actDel);
	connect(actDel, SIGNAL(triggered()), this, SLOT(onDeleteTrade()));
	QAction* actSell = new QAction(QStringLiteral("卖出"), this);
	tableWgtPopMenu->addAction(actSell);
	connect(actSell, SIGNAL(triggered()), this, SLOT(onSellTrade()));
	QAction* actShowDetail = new QAction(QStringLiteral("显示详情"), this);
	tableWgtPopMenu->addAction(actShowDetail);
	connect(actShowDetail, SIGNAL(triggered()), this, SLOT(onShowTradeInfo()));
	tableWgtPopMenu->addAction(ui.actSetProperty);
	connect(ui.actSetProperty, SIGNAL(triggered()), this, SLOT(setTradeProperty()));

	lblLatestPrice = new QLabel(QStringLiteral("当前股票最新交易价："));
	QPalette p = lblLatestPrice->palette();
	p.setColor(QPalette::WindowText, Qt::red);
	lblLatestPrice->setPalette(p);
	ui.statusBar->addWidget(lblLatestPrice);

	ui.m_pHSplitter->setSizes(QList<int>() << 100 << 500);
	
	tableModel = new JKStockTableModel(refProject, this);
	ui.tableView->setModel(tableModel);
	
// 	QItemSelectionModel *selectionModel = new QItemSelectionModel(stockTableModel);
// 	ui.tableView->setSelectionModel(selectionModel);
// 	
// 	QStyledItemDelegate* pTableDelegate = new QStyledItemDelegate;
// 	ui.tableView->setItemDelegate(pTableDelegate);

	
	
	connect(ui.tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableWgtPopMenu(QPoint)));
	connect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()), Qt::UniqueConnection);
	connect(ui.actNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
	connect(ui.actOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui.actExit, SIGNAL(triggered()), this, SLOT(close()));
	/** 编辑 */
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));
	connect(ui.actSellStock, SIGNAL(triggered()), this, SLOT(sellStockCode()));
	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actDeleteStockCode, SIGNAL(triggered()), this, SLOT(deleteCurrentStock()));

	/** 设置 */
	connect(ui.actTaxSetting, SIGNAL(triggered()), this, SLOT(projectTaxSetting()));
	connect(ui.actCrawlerOpt, SIGNAL(triggered(bool)), this, SLOT(crawlerOptChanged(bool)));
	connect(ui.actAlert, SIGNAL(triggered(bool)), this, SLOT(alertChanged(bool)));
	/** 显示 */
	connect(ui.actOnlyShowBuy, SIGNAL(triggered(bool)), this, SLOT(onShowBuyOnly(bool)));
	connect(ui.actOnlyShowSold, SIGNAL(triggered(bool)), this, SLOT(onShowSellOnly(bool)));
	connect(ui.actShowAll, SIGNAL(triggered(bool)), this, SLOT(onShowAll(bool)));

	QActionGroup* _showActionGrp = new QActionGroup(this);
	_showActionGrp->addAction(ui.actOnlyShowBuy);
	_showActionGrp->addAction(ui.actOnlyShowSold);
	_showActionGrp->addAction(ui.actShowAll);
	ui.actShowAll->setChecked(true);

	/** 关于 */
	connect(ui.actAbout, SIGNAL(triggered()), this, SLOT(about()));

	connect(this, SIGNAL(beforeProjectChanged()), this, SLOT(onBeforeProjectChanged()));
	connect(this, SIGNAL(afterProjectChanged(JKRef_Ptr<JKProjectBLL>)), this, SLOT(onAfterProjectChanged(JKRef_Ptr<JKProjectBLL>)));

	connect(this, SIGNAL(afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>)), this, SLOT(onAfterStockChanged(JKRef_Ptr<JKStockCodeBLL>)));
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

void JKMainWin::stockCodePriceChanged(JKString price)
{
	float latestPrice = QString(price.c_str()).toFloat();
	if (refProject.valid())
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCode = refProject->getCurStockCode();
		if (_refStockCode.valid())
		{
			if (_refStockCode->getLatestPrice() == latestPrice)
				return;
			_refStockCode->setLatestPrice(latestPrice);

			emit afterStockCodeChanged(_refStockCode);

			JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = refProject->getProjectSetting();
			if (_refProjectSetting->getIsAlert())
			{
				vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLLTemp;
				_refStockCode->getTradesByType((int)TradeType::BUY | (int)TradeType::PART, vecRefStockCodeTradeBLLTemp);
				if (vecRefStockCodeTradeBLLTemp.size() > 0)
				{
					std::sort(vecRefStockCodeTradeBLLTemp.begin(), vecRefStockCodeTradeBLLTemp.end(),
						[&](JKRef_Ptr<JKStockCodeTradeBLL> first, JKRef_Ptr<JKStockCodeTradeBLL> second) ->bool {
						bool bSort = false;

						if (first->getDate() == second->getDate())
						{
							if (first->getBuyPrice() < second->getBuyPrice())
								return true;
							else
								return false;
						}
						bSort = first->getDate() > second->getDate();
						return bSort;
					});
					JKStockTradeUtil tradeUtil(refProject);
					double percent = tradeUtil.getExpactEarningPercent(latestPrice, vecRefStockCodeTradeBLLTemp[0]);
					//percent = fabs(percent);
					if (percent > _refProjectSetting->getUpAlertPercent() || percent < _refProjectSetting->getDownAlertPercent())
					{
						this->show();
						this->activateWindow();
						this->raise();
					}
				}
			}
		}
	}
}

void JKMainWin::resizeEvent(QResizeEvent*event)
{
	curGemRect = geometry();

	QMainWindow::resizeEvent(event);
}

void JKMainWin::moveEvent(QMoveEvent *event)
{
	curGemRect = geometry();

	QMainWindow::moveEvent(event);
}

void JKMainWin::closeEvent(QCloseEvent * event)
{
	if (this->sender() == ui.actExit)
	{
		QMainWindow::closeEvent(event);
	}
	else
	{
		this->hide();
		if (!systemTrayIcon)
		{
			systemTrayIcon = new QSystemTrayIcon(this);
			systemTrayIcon->setToolTip(QStringLiteral("JKStockTool"));
			QIcon icon = QIcon(":/StockTool/JKStockTool.ico");
			systemTrayIcon->setIcon(icon);

			QMenu* menuTray = new QMenu(this);
			menuTray->addAction(ui.actShowWindows);
			menuTray->addSeparator();
			menuTray->addAction(ui.actExit);
			systemTrayIcon->setContextMenu(menuTray);

			systemTrayIcon->show();

			connect(ui.actShowWindows, SIGNAL(triggered()), this, SLOT(onShowApp()));
			connect(systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
				this, SLOT(onSystemTrayIconActive(QSystemTrayIcon::ActivationReason)));
		}
		event->ignore();
	}
}

void JKMainWin::updateUIEnable(JKRef_Ptr<JKProjectBLL> _refProject)
{
	bool bUIEnable = _refProject.valid();
	//ui.trendChartWgt->setEnabled(bUIEnable);
	ui.tableView->setEnabled(bUIEnable);
	ui.cmbBxSwitch->setEnabled(bUIEnable);

	ui.actNewStockCode->setEnabled(bUIEnable);
	ui.actDeleteStockCode->setEnabled(bUIEnable);
	ui.actTaxSetting->setEnabled(bUIEnable);
}

void JKMainWin::updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	bool bInputEnable = _refStockCode.valid();

	ui.actBuyStock->setEnabled(bInputEnable);
	ui.actSellStock->setEnabled(bInputEnable);
}


