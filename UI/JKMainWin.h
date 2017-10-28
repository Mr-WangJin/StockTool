#ifndef JKMAINWIN_H
#define JKMAINWIN_H

#include <QMainWindow>
#include "ui_JKMainWin.h"
#include <JKFramework/SmartPtr/JKRef_Ptr.h>
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKStockCodeTradeBLL.h"

class QLabel;


class JKMainWin : public QMainWindow
{
	Q_OBJECT
public:
	JKMainWin(/*JKProjectBLL* _projectBLL, */QWidget *parent = 0);
	~JKMainWin();


	void updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

signals:
	void beforeProjectChanged();
	void afterProjectChanged(JKRef_Ptr<JKProjectBLL>);
	void beforeStockCodeChanged();
	void afterStockCodeChanged(JKRef_Ptr<JKStockCodeBLL>);

	void beforeAddedNewProject();
	void afterAddedNewProject(JKRef_Ptr<JKProjectBLL>);
	void beforeAddedNewStockCode();
	void afterAddedNewStockCode(JKRef_Ptr<JKStockCodeBLL>);
	/*void beforeAddedNewStockTrade();
	void afterAddedNewStockTrade(JKRef_Ptr<JKStockCodeTradeBLL>);*/

	


	private slots:
	void newProject();
	void openProject();
	void newStockCode();
	void buyStockCode();
	void sellStockCode();
	void onSwitchCode();
	void setCurrentStockPrice();

	void onTableWgtPopMenu(QPoint pos);
	void onDeleteTrade();
	void onSellTrade();

	void stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode);
	void latestPriceChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode);
	void updateTableWidget();
	void updateInfoWgt(JKRef_Ptr<JKStockCodeBLL>);
	void updateInputUIEnable(JKRef_Ptr<JKStockCodeBLL>);
	void updateUIEnable(JKRef_Ptr<JKProjectBLL>);
	void updateCmbBoxSwitch(JKRef_Ptr<JKProjectBLL>);


private:
	void initUI();

private:
	Ui::JKMainWin ui;
	QMenu* tableWgtPopMenu;
	QLabel* lblShowCurStock;
	QLabel* lblLatestPrice;


	JKRef_Ptr<JKProjectBLL> refProject;


};

#endif // JKMAINWIN_H
