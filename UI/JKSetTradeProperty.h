#ifndef JKSETTRADEPROPERTY_H
#define JKSETTRADEPROPERTY_H

#include <QDialog>
#include "ui_JKSetTradeProperty.h"

class JKStockCodeTradeBLL;

class JKSetTradeProperty : public QDialog
{
	Q_OBJECT

public:
	JKSetTradeProperty(QWidget *parent = 0);
	~JKSetTradeProperty();

	void setStockCodeTrade(StockCodeTradeBLLConstRefPtr _refStockCodeTrade);

	private slots:
	void onOkClick();
	void onCancelClick();

private:
	void initialize();

private:
	Ui::JKSetTradeProperty ui;

	JKRef_Ptr<JKStockCodeTradeBLL> refStockCodeTrade;
};

#endif // JKSETTRADEPROPERTY_H
