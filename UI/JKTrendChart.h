#ifndef JKTRENDCHART_H
#define JKTRENDCHART_H

#include <QWidget>
#include "ui_JKTrendChart.h"

class JKTrendChart : public QWidget
{
	Q_OBJECT

public:
	JKTrendChart(QWidget *parent = 0);
	~JKTrendChart();

private:
	Ui::JKTrendChart ui;
};

#endif // JKTRENDCHART_H
