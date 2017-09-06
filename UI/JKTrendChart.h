#ifndef JKTRENDCHART_H
#define JKTRENDCHART_H

#include <QWidget>
#include "ui_JKTrendChart.h"
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QDateTimeAxis>

QT_CHARTS_USE_NAMESPACE

class JKStockCodeBLL;

class JKTrendChart : public QWidget
{
	Q_OBJECT

public:
	JKTrendChart(QWidget *parent = 0);
	~JKTrendChart();

	void setStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode);

	public slots:
	void updateTrendChart();

private:
	void initUi();

private:
	Ui::JKTrendChart ui;
	QChartView* chartView;
	QCandlestickSeries* acmeSeries;
	QChart* chart;
	QDateTimeAxis *axisX;
	QValueAxis *axisY;

	JKRef_Ptr<JKStockCodeBLL> refStockCode;
};

#endif // JKTRENDCHART_H
