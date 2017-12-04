#ifndef JKABOUT_H
#define JKABOUT_H

#include <QDialog>
#include "ui_JKAbout.h"

class JKAbout : public QDialog
{
	Q_OBJECT

public:
	JKAbout(QWidget *parent = 0);
	~JKAbout();

private:
	Ui::JKAbout ui;
};

#endif // JKABOUT_H
