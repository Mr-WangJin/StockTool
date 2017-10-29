#ifndef JKSETTINGLATESTPRICE_H
#define JKSETTINGLATESTPRICE_H

#include <QDialog>
#include "ui_JKSettingLatestPrice.h"

class JKStockCodeBLL;

class JKSettingLatestPrice : public QDialog
{
	Q_OBJECT

public:
	JKSettingLatestPrice(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget *parent = 0);
	~JKSettingLatestPrice();

	double getLatestPrice();

	private slots:
	void onOK();
	void onCancel();

private:
	void initUI();

private:
	Ui::JKSettingLatestPrice ui;

	JKRef_Ptr<JKStockCodeBLL> refStockCode;
};

#endif // JKSETTINGLATESTPRICE_H
