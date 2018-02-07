#ifndef JKSTOCKTRADEDETAIL_H
#define JKSTOCKTRADEDETAIL_H

#include <QDialog>
#include "ui_JKStockTradeDetail.h"

class JKStockTradeUtil;
class JKStockCodeTradeBLL;

class JKStockTradeDetail : public QDialog
{
	Q_OBJECT

public:
	JKStockTradeDetail(JKRef_Ptr<JKStockTradeUtil> refUtil, JKRef_Ptr<JKStockCodeTradeBLL> refStockTrade,double latestPrice, QWidget *parent = 0);
	~JKStockTradeDetail();


private:
	Ui::JKStockTradeDetail ui;
};

#endif // JKSTOCKTRADEDETAIL_H
