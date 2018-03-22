#ifndef JKMAINWIN_H
#define JKMAINWIN_H

#include <QMainWindow>
#include "ui_JKMainWin.h"
#include <QSystemTrayIcon>
#include <JKFramework/SmartPtr/JKRef_Ptr.h>
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKStockCodeTradeBLL.h"
#include "Utils/JKCrawlPrice.h"

class QLabel;
class QSystemTrayIcon;
class JKBuyStockTableAdapter;
class JKSellStockTableAdapter;
class JKTreeModel;

class JKMainWin : public QMainWindow
{
	Q_OBJECT
public:
	JKMainWin(/*JKProjectBLL* _projectBLL, */QWidget *parent = 0);
	~JKMainWin();

	void updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

	/** 激活windows窗口 */
	void setActivateWindow();

	/** 设置开机自动启动 */
	void setAutoStartExe(bool is_auto_start);

	void showAbout();
	JKRef_Ptr<JKProjectBLL> getProjectBLL();
signals:
	void beforeProjectChanged();
	void afterProjectChanged(JKRef_Ptr<JKProjectBLL>);
	void beforeStockCodeChanged();
	void afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>);


	private slots:
	void newProject();
	void openRecentProject();
	void openProject();
	void newStockCode();
	void save();
	void deleteCurrentStock();
	void buyStockCode();
	void sellStockCode();
	void onSwitchCode();
	void onShowBuyOnly(bool checked);
	void onShowSellOnly(bool checked);
	void onShowAll(bool checked);
	void projectTaxSetting();
	void setTradeProperty();
	void crawlerOptChanged(bool);
	void alertChanged(bool);
	void about();

	void onSystemTrayIconActive(QSystemTrayIcon::ActivationReason);
	void onExitApp();
	void onShowApp();
	
	void onTableWgtPopMenu(QPoint pos);
	void onDeleteTrade();
	void onSellTrade();
	void onShowTradeInfo();

	void onBeforeProjectChanged();
	void onAfterProjectChanged(JKRef_Ptr<JKProjectBLL>);

	void onAfterStockChanged(JKRef_Ptr<JKStockCodeBLL>);
	void stockCodePriceChanged(JKString);

	void refreshCrawler(JKRef_Ptr<JKProjectBLL>);

protected:
	virtual void resizeEvent(QResizeEvent*event) override;

	virtual void moveEvent(QMoveEvent *event) override;

	virtual void closeEvent(QCloseEvent *event) override;

private:
	void initUI();
	void updateInfoWgt(JKRef_Ptr<JKStockCodeBLL>);
	void updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL>);
	void updateUIEnable(JKRef_Ptr<JKProjectBLL>);
	void updateCmbBoxSwitch(JKRef_Ptr<JKProjectBLL>);
	void updateTableWidget();

	void addedCmbBoxSwitch(JKRef_Ptr<JKStockCodeBLL>);
	void addedNewStockCode(JKRef_Ptr<JKStockCodeBLL>);

	void setCurrentStockPrice();

private:
	Ui::JKMainWin ui;
	QSystemTrayIcon* systemTrayIcon = nullptr;
	/** 窗口坐标 */
	QRect curGemRect;
	QMenu* tableWgtPopMenu;
	QLabel* lblShowCurStock;
	QLabel* lblLatestPrice;

	JKRef_Ptr<JKProjectBLL> project;

	JKCrawlPrice* crawlPrice = nullptr;

	JKTreeModel* stockTableModel;
	//JKVirtualTreeModel* stockTableModel;



	std::shared_ptr<JKBuyStockTableAdapter> buyStockTableAdapter;
	std::shared_ptr<JKSellStockTableAdapter> sellStockTableAdapter;
	//std::shared_ptr<JKVirtualModelAdapter> curStockTableAdapter;

	

};

#endif // JKMAINWIN_H
