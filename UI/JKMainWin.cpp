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
#include "JKStockTableWidget.h"
#include "JKVirtualTreeModel.h"


#define REG_RUN "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"



JKMainWin::JKMainWin(/*JKProjectBLL* _projectBLL,*/ QWidget *parent)
	: QMainWindow(parent)
{
	qRegisterMetaType<JKString>("JKString");

	ui.setupUi(this);
	ui.trendChartWgt->setVisible(false);
	project = nullptr;

	JKSingleton<JKUiContext>::GetInstance().setMainWin(this);
	this->initUI();
	this->updateInputUIEnable(nullptr);
	this->setAutoStartExe(true);
	
	emit afterProjectChanged(project);

}

JKMainWin::~JKMainWin()
{
	JK_FreeAndNullptr(crawlPrice);
}

void JKMainWin::updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	if (_refStockCode.valid())
	{
//		lblShowCurStock->setText(QStringLiteral("��ǰ��ʾ��Ʊ��") + QString(_refStockCode->getName().c_str()));

		lblLatestPrice->setText(QStringLiteral("��ǰ��Ʊ���½��׼ۣ�") + QString::number(_refStockCode->getLatestPrice()));
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
	return project;
}

void JKMainWin::newProject()
{
	QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("�½�����......"), "", "JKProject (*.jk)");
	if (fileName.isEmpty())
		return;

	//QString path = QFileDialog::getExistingDirectory(this, QStringLiteral("��ѡ���½�����·��......"));
	//if (path.isEmpty())
	//	return;
	//QDir dir(path);
	//QStringList fileNames = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	//if (fileNames.count() > 0)
	//{
	//	QMessageBox::information(this, QStringLiteral("��ʾ��"), QStringLiteral("�ļ��в�Ϊ�գ�"));
	//	return;
	//}
	//else
	{
		JKNewProject newProject(this);
		if (newProject.exec() == QDialog::Accepted)
		{
			emit beforeProjectChanged();

			project = JKProjectBLL::newProject(fileName.toStdString());
			project->setStampTax(newProject.getStampTax());
			project->setTransfer(newProject.getTransfer());
			project->setCommission(newProject.getCommission());

			JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = project->getProjectSetting();
			_refProjectSetting->setUpAlertPercent(newProject.getUpAlertPercent());
			_refProjectSetting->setUpAlertPercent(newProject.getDownAlertPercent());

			emit afterProjectChanged(project);
		}
	}
}

void JKMainWin::openRecentProject()
{
	QAction* pAct = dynamic_cast<QAction*>(sender());
	if (pAct == nullptr)
		return;

	emit beforeProjectChanged();

	project = JKProjectBLL::openProject(pAct->data().toString().toStdString());

	emit afterProjectChanged(project);
	emit afterStockCodeChanged(project->getCurStockCode());
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

		project = JKProjectBLL::openProject(fullName.toStdString());

		/** ��������򿪵���Ŀ */
		JKRecentProject recentProject(JKSingleton<JKUiContext>::GetInstance().getRecentFilePath());
		recentProject.addRecentProject(QFileInfo(fullName).baseName().toStdString(), fullName.toStdString());
		recentProject.saveToFile(JKSingleton<JKUiContext>::GetInstance().getRecentFilePath());

		emit afterProjectChanged(project);
		emit afterStockCodeChanged(project->getCurStockCode());
	}
}

void JKMainWin::newStockCode()
{
	JKNewStockCodeWgt newStockCodeWgt(project);
	if (QDialog::Accepted == newStockCodeWgt.exec())
	{

		JKRef_Ptr<JKStockCodeBLL> refCurStockCode = newStockCodeWgt.getStockCode();
		project->setCurStockCode(refCurStockCode);

		addedNewStockCode(refCurStockCode);
		emit afterStockCodeChanged(refCurStockCode);
	}
}

void JKMainWin::save()
{
// 	if (project)
// 		project->saveProject();
}

void JKMainWin::deleteCurrentStock()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = project->getCurStockCode();
	if (_refStockCode.valid() == false)
		return;

	project->deleteStockCode(_refStockCode);

	this->updateCmbBoxSwitch(project);
	if (ui.cmbBxSwitch->count() >0 )
		this->onSwitchCode();
	else
	{
		onAfterStockChanged(nullptr);
	}
}

void JKMainWin::buyStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = project->getCurStockCode();
	if (!_refStockCode.valid())
	{
		QMessageBox::information(this, QStringLiteral("��ʾ��"), QStringLiteral("���½���Ʊ����"));
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
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = project->getCurStockCode();
	if (!_refStockCode.valid())
	{
		QMessageBox::information(this, QStringLiteral("��ʾ��"), QStringLiteral("���½���Ʊ����"));
	}
	else
	{
		std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecStockTrade;
		JKSellStockCodeWgt sellStockCodeWgt(project, vecStockTrade);
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

	vector<JKRef_Ptr<JKStockCodeBLL>> vecRefStockCodeBLL = project->getAllStockCode();
	for (auto var : vecRefStockCodeBLL)
	{
		if (QString(var->getCode().c_str()) == code)
		{
			emit beforeStockCodeChanged();

			project->setCurStockCode(var);

			emit afterStockCodeChanged(var);

			if (crawlPrice)
			{
				crawlPrice->clearStoclCode();
				crawlPrice->addStockCode(var);
			}
			
			lblLatestPrice->setText(QString("%1%2").arg(QStringLiteral("��ǰ��Ʊ���½��׼ۣ�")).arg(var->getLatestPrice()));

			break;
		}
	}
}

void JKMainWin::setCurrentStockPrice()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = project->getCurStockCode();
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
	newProject.setProject(project);
	if (newProject.exec() == QDialog::Accepted)
	{
		emit beforeProjectChanged();

		project->setStampTax(newProject.getStampTax());
		project->setTransfer(newProject.getTransfer());
		project->setCommission(newProject.getCommission());

		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = project->getProjectSetting();
		_refProjectSetting->setUpAlertPercent(newProject.getUpAlertPercent());
		_refProjectSetting->setDownAlertPercent(newProject.getDownAlertPercent());

		emit afterProjectChanged(project);
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
	if (project)
	{
		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = project->getProjectSetting();
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
	if (project)
	{
		JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = project->getProjectSetting();
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
// 			QStringLiteral("��л����֧��......"), QSystemTrayIcon::Information, 1000);
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
		JKRef_Ptr<JKStockCodeBLL> _refStockCode = project->getCurStockCode();
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
		JKSellStockCodeWgt sellStockCodeWgt(project, _vecRefStockCodeTrade, this);
		if (sellStockCodeWgt.exec() == QDialog::Accepted)
		{
			this->updateTableWidget();
		}
	}
}

void JKMainWin::onShowTradeInfo()
{
	if (!project.valid())
		return;

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecRefStockCodeTrade;
	ui.tableView->getSelectedStockTrade(_vecRefStockCodeTrade);
	
	if (_vecRefStockCodeTrade.size() == 0)
		return;

	JKRef_Ptr<JKStockTradeUtil> refTradeUtil = new JKStockTradeUtil(project);
	JKStockTradeDetail detail(refTradeUtil, _vecRefStockCodeTrade[0], project->getCurStockCode()->getLatestPrice());
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

	if (buyStockTableAdapter == nullptr)
		buyStockTableAdapter.reset(new JKBuyStockTableAdapter(project, BaseObjectPtr()));

	curStockTableAdapter = buyStockTableAdapter;
	this->updateTableWidget();
}
void JKMainWin::onShowSellOnly(bool checked)
{
	if (!checked)
		return;
	if (sellStockTableAdapter == nullptr)
		sellStockTableAdapter.reset(new JKSellStockTableAdapter(project, BaseObjectPtr()));

	curStockTableAdapter = sellStockTableAdapter;
	this->updateTableWidget();
}
void JKMainWin::onShowAll(bool checked)
{
	if (!checked)
		return;
	//tableModel->setShowType(Show_All);
	//this->updateTableWidget();
}

void JKMainWin::updateTableWidget()
{
	//tableModel->setProject(refProject);
	if (project)
	{
		curStockTableAdapter->setRoot(project->getCurStockCode()->toBaseObject());
		stockTableModel->setModelAdapter(curStockTableAdapter);
		ui.tableView->resizeColumnsWidth();
	}
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
			systemTrayIcon->setToolTip(QString("JKStockTool\n%1%2").arg(QStringLiteral("���¼ۣ�")).arg(_refStockCode->getLatestPrice()));
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

		JKStockTradeUtil trackUtil(project);
		std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockCodeTrade;
		_refStockCode->getAllTrades(_vecStockCodeTrade);
		for (auto &var : _vecStockCodeTrade)
		{
			if (TradeType::BUY == var->getType())
			{
				buySumPrice += var->getBuyAmount();
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

	ui.lblBuySumPrice->setText(QStringLiteral("�����ܶ") + QString::number(buySumPrice));
	ui.lblBuySumCount->setText(QStringLiteral("����������") + QString::number(buySumCount));
	ui.lblBuySumEarning->setText(QStringLiteral("Ԥ�����棺") + QString::number(buySumEarning));

	ui.lblSellSumPrice->setText(QStringLiteral("�����ܶ") + QString::number(sellSumPrice));
	ui.lblSellSumCount->setText(QStringLiteral("����������") + QString::number(sellSumCount));
	ui.lblSellSumEarning->setText(QStringLiteral("�������棺") + QString::number(sellSumEarning));

}

void JKMainWin::initUI()
{
	/** ��ʼ�����´򿪹��� */
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

	QAction* actDel = new QAction(QStringLiteral("ɾ��"), this);
	tableWgtPopMenu->addAction(actDel);
	connect(actDel, SIGNAL(triggered()), this, SLOT(onDeleteTrade()));
	QAction* actSell = new QAction(QStringLiteral("����"), this);
	tableWgtPopMenu->addAction(actSell);
	connect(actSell, SIGNAL(triggered()), this, SLOT(onSellTrade()));
	QAction* actShowDetail = new QAction(QStringLiteral("��ʾ����"), this);
	tableWgtPopMenu->addAction(actShowDetail);
	connect(actShowDetail, SIGNAL(triggered()), this, SLOT(onShowTradeInfo()));
	tableWgtPopMenu->addAction(ui.actSetProperty);
	connect(ui.actSetProperty, SIGNAL(triggered()), this, SLOT(setTradeProperty()));

	lblLatestPrice = new QLabel(QStringLiteral("��ǰ��Ʊ���½��׼ۣ�"));
	QPalette p = lblLatestPrice->palette();
	p.setColor(QPalette::WindowText, Qt::red);
	lblLatestPrice->setPalette(p);
	ui.statusBar->addWidget(lblLatestPrice);

	ui.m_pHSplitter->setSizes(QList<int>() << 100 << 500);
	
	buyStockTableAdapter.reset(new JKBuyStockTableAdapter(project, BaseObjectPtr()));
	curStockTableAdapter = buyStockTableAdapter;
	stockTableModel = new JKVirtualTreeModel(curStockTableAdapter, this);
	ui.tableView->setModel(stockTableModel);
	ui.tableView->setSortingEnabled(true);
	ui.tableView->sortByColumn(1);
	
// 	QItemSelectionModel *selectionModel = new QItemSelectionModel(stockTableModel);
// 	ui.tableView->setSelectionModel(selectionModel);
// 	
// 	QStyledItemDelegate* pTableDelegate = new QStyledItemDelegate;
// 	ui.tableView->setItemDelegate(pTableDelegate);

	connect(ui.tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableWgtPopMenu(QPoint)));
	connect(ui.cmbBxSwitch, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchCode()), Qt::UniqueConnection);
	connect(ui.actNewProject, SIGNAL(triggered()), this, SLOT(newProject()));
	connect(ui.actOpenProject, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui.actSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actExit, SIGNAL(triggered()), this, SLOT(close()));
	/** �༭ */
	connect(ui.actBuyStock, SIGNAL(triggered()), this, SLOT(buyStockCode()));
	connect(ui.actSellStock, SIGNAL(triggered()), this, SLOT(sellStockCode()));
	connect(ui.actNewStockCode, SIGNAL(triggered()), this, SLOT(newStockCode()));
	connect(ui.actDeleteStockCode, SIGNAL(triggered()), this, SLOT(deleteCurrentStock()));

	/** ���� */
	connect(ui.actTaxSetting, SIGNAL(triggered()), this, SLOT(projectTaxSetting()));
	connect(ui.actCrawlerOpt, SIGNAL(triggered(bool)), this, SLOT(crawlerOptChanged(bool)));
	connect(ui.actAlert, SIGNAL(triggered(bool)), this, SLOT(alertChanged(bool)));
	/** ��ʾ */
	connect(ui.actOnlyShowBuy, SIGNAL(triggered(bool)), this, SLOT(onShowBuyOnly(bool)));
	connect(ui.actOnlyShowSold, SIGNAL(triggered(bool)), this, SLOT(onShowSellOnly(bool)));
	connect(ui.actShowAll, SIGNAL(triggered(bool)), this, SLOT(onShowAll(bool)));

	QActionGroup* _showActionGrp = new QActionGroup(this);
	_showActionGrp->addAction(ui.actOnlyShowBuy);
	_showActionGrp->addAction(ui.actOnlyShowSold);
	//_showActionGrp->addAction(ui.actShowAll);
	ui.actShowAll->setChecked(true);

	/** ���� */
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
	vector<JKRef_Ptr<JKStockCodeBLL>> vecRefStockCodeBLL = project->getAllStockCode();
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
	if (project.valid())
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCode = project->getCurStockCode();
		if (_refStockCode.valid())
		{
			if (_refStockCode->getLatestPrice() == latestPrice)
				return;
			_refStockCode->setLatestPrice(latestPrice);

			emit afterStockCodeChanged(_refStockCode);

			JKRef_Ptr<JKProjectSettingBLL> _refProjectSetting = project->getProjectSetting();
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
					JKStockTradeUtil tradeUtil(project);
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


