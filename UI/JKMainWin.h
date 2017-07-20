#ifndef JKMAINWIN_H
#define JKMAINWIN_H

#include <QMainWindow>
#include "ui_JKMainWin.h"

class JKMainWin : public QMainWindow
{
	Q_OBJECT

public:
	JKMainWin(QWidget *parent = 0);
	~JKMainWin();

private:
	Ui::JKMainWin ui;
};

#endif // JKMAINWIN_H
