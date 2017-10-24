#ifndef JKSETTINGLATESTPRICE_H
#define JKSETTINGLATESTPRICE_H

#include <QDialog>
#include "ui_JKSettingLatestPrice.h"

class JKSettingLatestPrice : public QDialog
{
	Q_OBJECT

public:
	JKSettingLatestPrice(double _prePrice, QWidget *parent = 0);
	~JKSettingLatestPrice();

	double getLatestPrice();

	private slots:
	void onOK();
	void onCancel();

private:
	void initUI();

private:
	Ui::JKSettingLatestPrice ui;


	double prePrice;
};

#endif // JKSETTINGLATESTPRICE_H
