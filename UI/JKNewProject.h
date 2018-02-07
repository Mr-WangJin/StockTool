#ifndef JKNEWPROJECT_H
#define JKNEWPROJECT_H

#include <QDialog>
#include "ui_JKNewProject.h"

class JKProjectBLL;

class JKNewProject : public QDialog
{
	Q_OBJECT

public:
	JKNewProject(QWidget *parent = 0);
	~JKNewProject();

	double getStampTax();
	double getTransfer();
	double getCommission();
	double getUpAlertPercent();
	double getDownAlertPercent();

	void setProject(JKRef_Ptr<JKProjectBLL> _refProject);

	private slots:
	void onOkClick();
	void onCancelClick();

private:
	Ui::JKNewProject ui;
};

#endif // JKNEWPROJECT_H
