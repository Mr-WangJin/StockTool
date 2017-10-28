#ifndef JKNEWPROJECT_H
#define JKNEWPROJECT_H

#include <QDialog>
#include "ui_JKNewProject.h"

class JKNewProject : public QDialog
{
	Q_OBJECT

public:
	JKNewProject(QWidget *parent = 0);
	~JKNewProject();

	double getStampTax();
	double getTransfer();
	double getCommission();

	private slots:
	void onOkClick();
	void onCancelClick();

private:
	Ui::JKNewProject ui;
};

#endif // JKNEWPROJECT_H
