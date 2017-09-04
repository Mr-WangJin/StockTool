#include "stdafx.h"
#include "JKTrendChart.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QCandlestickSet>
#include <QVBoxLayout>

QT_CHARTS_USE_NAMESPACE

JKTrendChart::JKTrendChart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->initUi();
}

JKTrendChart::~JKTrendChart()
{

}

void JKTrendChart::initUi()
{
	//QLineSeries *series = new QLineSeries();

	QCandlestickSeries *acmeSeries = new QCandlestickSeries();
	acmeSeries->setName("Acme Ltd");
	acmeSeries->setIncreasingColor(QColor(Qt::green));
	acmeSeries->setDecreasingColor(QColor(Qt::red));



	for (int i = 1; i < 10; ++i)
	{
		QDateTime d(QDate(1989, 1, i));

		QCandlestickSet *candlestickSet = new QCandlestickSet(d.toMSecsSinceEpoch());
		candlestickSet->setOpen(1);
		candlestickSet->setHigh(i+1);
		candlestickSet->setLow(1);
		candlestickSet->setClose(i+1);

		acmeSeries->append(candlestickSet);
	}

	QChart *chart = new QChart();
	chart->addSeries(acmeSeries);
	chart->legend()->hide();
	chart->setTitle("Sunspots count (by Space Weather Prediction Center)");
	//![3]

	//![4]
	QDateTimeAxis *axisX = new QDateTimeAxis;
	axisX->setTickCount(5);
	axisX->setFormat("yyyy-MM-dd");
	axisX->setTitleText(QStringLiteral("日期"));
	//axisX->setMax((QDateTime::currentDateTime()).toMSecsSinceEpoch());
	chart->addAxis(axisX, Qt::AlignBottom);
	acmeSeries->attachAxis(axisX);

	QValueAxis *axisY = new QValueAxis;
	axisY->setLabelFormat("%i");
	axisY->setTitleText(QStringLiteral("交易价钱"));
	chart->addAxis(axisY, Qt::AlignLeft);
	acmeSeries->attachAxis(axisY);
	//![4]

	//![5]
	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QVBoxLayout* pVLayout = new QVBoxLayout;
	pVLayout->setSpacing(0);
	pVLayout->setContentsMargins(0, 0, 0, 0);
	pVLayout->addWidget(chartView);
	this->setLayout(pVLayout);
}
