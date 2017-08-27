#pragma once

#include <QWidget>
#include "ui_JKNewStockCodeWgt.h"

class JKProjectBLL;
class JKStockCodeBLL;

class JKNewStockCodeWgt : public QDialog
{
	Q_OBJECT

public:
	JKNewStockCodeWgt(JKRef_Ptr<JKProjectBLL> project, QWidget *parent = 0);
	~JKNewStockCodeWgt();


	JKRef_Ptr<JKStockCodeBLL> getStockCode();

	private slots:
	void slotOKClicked();

private:
	Ui::JKNewStockCodeWgt ui;

	JKRef_Ptr<JKProjectBLL> refProject;
	JKRef_Ptr<JKStockCodeBLL> refStockCode;
};

