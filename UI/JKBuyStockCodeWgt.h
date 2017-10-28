#pragma once

#include <QDialog>
#include "ui_JKBuyStockCodeWgt.h"

class JKStockCodeBLL;
class JKStockCodeTradeBLL;


class JKBuyStockCodeWgt : public QDialog
{
	Q_OBJECT
public:
	JKBuyStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget* parent = nullptr);
	~JKBuyStockCodeWgt();

	JKRef_Ptr<JKStockCodeTradeBLL> getNewStockCodeTrade();

	private slots:
	void okClicked();
	void cancleClicked();

private:
	void initUI();

private:
	Ui::JKBuyStockCodeWgt ui;

	JKRef_Ptr<JKStockCodeBLL> refStockCode;
	JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade;
};



