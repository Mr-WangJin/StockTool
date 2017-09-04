#pragma once

#include <QDialog>
#include "ui_JKBuyStockCodeWgt.h"

class JKProjectBLL;


class JKBuyStockCodeWgt : public QDialog
{
	Q_OBJECT
public:
	JKBuyStockCodeWgt(JKRef_Ptr<JKProjectBLL> _refProject, QWidget* parent = nullptr);
	~JKBuyStockCodeWgt();

	private slots:
	void okClicked();
	void cancleClicked();

private:
	Ui::JKBuyStockCodeWgt ui;

	JKRef_Ptr<JKProjectBLL> refProject;
};



