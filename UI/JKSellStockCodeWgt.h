#ifndef JKSELLSTOCKCODEWGT_H
#define JKSELLSTOCKCODEWGT_H

#include <QDialog>
#include "ui_JKSellStockCodeWgt.h"

class JKStockCodeBLL;

class JKSellStockCodeWgt : public QDialog
{
	Q_OBJECT

public:
	JKSellStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, QWidget *parent = 0);
	~JKSellStockCodeWgt();

private:
	void initUI();

	private slots:
	void onOK();
	void onCancel();


private:
	Ui::JKSellStockCodeWgt ui;

	JKRef_Ptr<JKStockCodeBLL> refStockCode;

};

#endif // JKSELLSTOCKCODEWGT_H
