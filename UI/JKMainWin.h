#ifndef JKMAINWIN_H
#define JKMAINWIN_H

#include <QMainWindow>
#include "ui_JKMainWin.h"
#include <JKFramework/SmartPtr/JKRef_Ptr.h>

class JKProjectBLL;


class JKMainWin : public QMainWindow
{
	Q_OBJECT
public:
	JKMainWin(JKProjectBLL* _projectBLL, QWidget *parent = 0);
	~JKMainWin();

private:
	Ui::JKMainWin ui;

	JKRef_Ptr<JKProjectBLL> refProjectBLL;

};

#endif // JKMAINWIN_H
