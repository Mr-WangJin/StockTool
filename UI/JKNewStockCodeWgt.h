#ifndef JKINFOWGT_H
#define JKINFOWGT_H

#include <QWidget>
#include "ui_JKNewStockCodeWgt.h"

class JKNewStockCodeWgt : public QDialog
{
	Q_OBJECT

public:
	JKNewStockCodeWgt(QWidget *parent = 0);
	~JKNewStockCodeWgt();

private:
	Ui::JKNewStockCodeWgt ui;
};

#endif // JKINFOWGT_H
