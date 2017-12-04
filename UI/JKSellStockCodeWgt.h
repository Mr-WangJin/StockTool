#ifndef JKSELLSTOCKCODEWGT_H
#define JKSELLSTOCKCODEWGT_H

#include <QDialog>
#include "ui_JKSellStockCodeWgt.h"

class JKStockCodeBLL;
class JKStockCodeTradeBLL;

class JKSellStockCodeWgt : public QDialog
{
	Q_OBJECT

public:
	JKSellStockCodeWgt(JKRef_Ptr<JKStockCodeBLL> _refStockCode, std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> _vecStockTrade, QWidget *parent = 0);
	~JKSellStockCodeWgt();

private:
	void initUI();

	private slots:
	void onOK();
	void onCancel();

private:
	Ui::JKSellStockCodeWgt ui;

	JKRef_Ptr<JKStockCodeBLL> refStockCode;
	std::vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecStockTrade;

};

#endif // JKSELLSTOCKCODEWGT_H
