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
#include "JKStockTableModel.h"

class QLabel;
class QSystemTrayIcon;


class JKMainWin : public QMainWindow
{
	Q_OBJECT
public:
	JKMainWin(/*JKProjectBLL* _projectBLL, */QWidget *parent = 0);
	~JKMainWin();

	void updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

	/** 激活windows窗口 */
	void setActivateWindow();
	

	void showAbout();
signals:
	void beforeProjectChanged();
	void afterProjectChanged(JKRef_Ptr<JKProjectBLL>);
	void beforeStockCodeChanged();
	void afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>);


	private slots:
	void newProject();
	void openProject();
	void newStockCode();
	void deleteCurrentStock();
	void buyStockCode();
	void sellStockCode();
	void onSwitchCode();
	void setCurrentStockPrice();
	void projectTaxSetting();
	void crawlerOptChanged();
	void about();
	void onSystemTrayIconActive(QSystemTrayIcon::ActivationReason);
	void onExitApp();
	void onShowApp();

	void onTableWgtPopMenu(QPoint pos);
	void onDeleteTrade();
	void onSellTrade();
	void onShowBuyOnly(bool checked);
	void onShowSellOnly(bool checked);
	void onShowAll(bool checked);
	void onShowTradeInfo();
	void onBeforeProjectChanged();
	void onAfterProjectChanged(JKRef_Ptr<JKProjectBLL>);

	void updateTableWidget();
	void onAfterStockChanged(JKRef_Ptr<JKStockCodeBLL>);

	void updateUIEnable(JKRef_Ptr<JKProjectBLL>);
	void updateCmbBoxSwitch(JKRef_Ptr<JKProjectBLL>);

	void refreshCrawler(JKRef_Ptr<JKProjectBLL>);
	void stockCodePriceChanged(JKString);

protected:
	virtual void resizeEvent(QResizeEvent*event) override;

	virtual void moveEvent(QMoveEvent *event) override;

	virtual void closeEvent(QCloseEvent *event) override;

private:
	void initUI();
	void updateInfoWgt(JKRef_Ptr<JKStockCodeBLL>);
	void updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL>);

	void addedCmbBoxSwitch(JKRef_Ptr<JKStockCodeBLL>);
	void addedNewStockCode(JKRef_Ptr<JKStockCodeBLL>);


private:
	Ui::JKMainWin ui;

	QSystemTrayIcon* systemTrayIcon = nullptr;
	/** 窗口坐标 */
	QRect curGemRect;

	QMenu* tableWgtPopMenu;
	QLabel* lblShowCurStock;
	QLabel* lblLatestPrice;
	JKStockTableModel* tableModel;
	JKRef_Ptr<JKProjectBLL> refProject;

	JKCrawlPrice* crawlPrice = nullptr;
	//TableShowType tbShowType;


};

#endif // JKMAINWIN_H
