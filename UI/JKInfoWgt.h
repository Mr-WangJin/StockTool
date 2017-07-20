#ifndef JKINFOWGT_H
#define JKINFOWGT_H

#include <QWidget>
#include "ui_JKInfoWgt.h"

class JKInfoWgt : public QWidget
{
	Q_OBJECT

public:
	JKInfoWgt(QWidget *parent = 0);
	~JKInfoWgt();

private:
	Ui::JKInfoWgt ui;
};

#endif // JKINFOWGT_H
