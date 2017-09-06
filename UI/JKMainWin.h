#ifndef JKMAINWIN_H
#define JKMAINWIN_H

#include <QMainWindow>
#include "ui_JKMainWin.h"
#include <JKFramework/SmartPtr/JKRef_Ptr.h>
#include "bll/JKStockCodeBLL.h"

class JKProjectBLL;


class JKMainWin : public QMainWindow
{
	Q_OBJECT
public:
	JKMainWin(JKProjectBLL* _projectBLL, QWidget *parent = 0);
	~JKMainWin();


	void updateStatusBar(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

	private slots:
	void newStockCode();
	void buyStockCode();

	void stockCodeChanged(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

private:
	Ui::JKMainWin ui;

	JKRef_Ptr<JKProjectBLL> refProject;


};

#endif // JKMAINWIN_H
