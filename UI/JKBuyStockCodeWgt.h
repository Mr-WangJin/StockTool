#pragma once

#include <QDialog>
#include "ui_JKBuyStockCodeWgt.h"


class JKBuyStockCodeWgt : public QDialog
{
	Q_OBJECT
public:
	JKBuyStockCodeWgt(QWidget* parent = nullptr);
	~JKBuyStockCodeWgt();


private:
	Ui::JKBuyStockCodeWgt ui;


};



