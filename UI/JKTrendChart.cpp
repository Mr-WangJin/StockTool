#include "stdafx.h"
#include "JKTrendChart.h"
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QCandlestickSet>
#include <QVBoxLayout>
#include "BLL\JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"


JKTrendChart::JKTrendChart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	this->initUi();
}

JKTrendChart::~JKTrendChart()
{

}

void JKTrendChart::setStockCode(JKRef_Ptr<JKStockCodeBLL> _refStockCode)
{
	refStockCode = _refStockCode;
	this->updateTrendChart();
}

void JKTrendChart::updateTrendChart()
{
	if (!refStockCode.valid())
		return;

	vector<JKRef_Ptr<JKStockCodeTradeBLL>> _refStockCodeTradeBLL = refStockCode->getAllTrades();

	chart->removeAllSeries();
	
	acmeSeries = new QCandlestickSeries();
	acmeSeries->setName("Acme Ltd");
	acmeSeries->setIncreasingColor(QColor(Qt::green));
	acmeSeries->setDecreasingColor(QColor(Qt::red));
	acmeSeries->setMaximumColumnWidth(1);
	acmeSeries->setUseOpenGL(true);

	QDateTime minDate(QDate(2030, 1,1), QTime(1,1,1));
	QDateTime maxDate(QDate(1999, 1, 1), QTime(1, 1, 1));

	for (auto &var : _refStockCodeTradeBLL)
	{
		JKString dateStr = var->getDate();
		QDateTime dt;
		dt.setDate(QDate::fromString(QString::fromStdString(dateStr), Qt::ISODate));

		JKUInt64 price = var->getBuyPrice();
		JKUInt64 count = var->getCount();

		if (dt < minDate)
			minDate = dt;
		if (dt > maxDate)
			maxDate = dt;

		QCandlestickSet *candlestickSet = new QCandlestickSet(dt.toMSecsSinceEpoch());
		candlestickSet->setOpen(price);
		candlestickSet->setHigh(price + .001);
		candlestickSet->setLow(price);
		candlestickSet->setClose(price + .001);

		switch (var->getType())
		{
		case TradeType::BUY:
		{
			candlestickSet->setBrush(Qt::blue);
		}
			break;
		case TradeType::SELL:
		{
			candlestickSet->setBrush(Qt::green);
		}
			break;
		default:
			break;
		}

		acmeSeries->append(candlestickSet);
	}

	chart->removeAxis(axisX);

	axisX = new QDateTimeAxis;
	axisX->setTickCount(1);
	axisX->setFormat("yyyy-MM-dd");
	axisX->setTitleText(QStringLiteral("日期"));
	//if (minDate == maxDate)
	//	maxDate.addDays(1);
	//axisX->setRange(minDate, maxDate);
	chart->addAxis(axisX, Qt::AlignBottom);

	axisX->setTickCount(_refStockCodeTradeBLL.size()+2);
	//axisX->setTickCount(5);
	chart->addSeries(acmeSeries);
	acmeSeries->attachAxis(axisX);
	acmeSeries->attachAxis(axisY);


	//acmeSeries = new QCandlestickSeries();
	//acmeSeries->setName("Acme Ltd");
	//acmeSeries->setIncreasingColor(QColor(Qt::green));
	//acmeSeries->setDecreasingColor(QColor(Qt::red));
	//acmeSeries->setMaximumColumnWidth(1);
	//acmeSeries->setUseOpenGL(true);
	////acmeSeries->setBodyWidth(1);

	//for (int i = 0; i< 9; ++i)
	//{
	//	QDateTime pp;
	//	pp.setDate(QDate(2010, 10, i + 1));
	//	pp.setTime(QTime(12, 0));

	//	qint64 bbb = pp.toMSecsSinceEpoch();

	//	QCandlestickSet *candlestickSet = new QCandlestickSet(bbb);
	//	candlestickSet->setOpen(0);
	//	candlestickSet->setHigh(i);
	//	candlestickSet->setLow(0);
	//	candlestickSet->setClose(1);

	//	acmeSeries->append(candlestickSet);
	//}


	//chart->addSeries(acmeSeries);
	//acmeSeries->attachAxis(axisX);
	//acmeSeries->attachAxis(axisY);


}

void JKTrendChart::initUi()
{
	chart = new QChart();
	chart->setAnimationOptions(QChart::SeriesAnimations);
	chart->legend()->hide();
	chart->setTitle(QStringLiteral("交易图"));

	axisX = new QDateTimeAxis;
	axisX->setTickCount(1);
	axisX->setFormat("yyyy-MM-dd");
	axisX->setTitleText(QStringLiteral("日期"));
	chart->addAxis(axisX, Qt::AlignBottom);

	axisY = new QValueAxis;
	axisY->setLabelFormat("%f");
	axisY->setTitleText(QStringLiteral("交易价钱"));
	chart->addAxis(axisY, Qt::AlignLeft);

	chartView = new QChartView();
	chartView->setChart(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	//chartView->setFixedWidth(400);

	QVBoxLayout* pVLayout = new QVBoxLayout;
	pVLayout->setSpacing(0);
	pVLayout->setContentsMargins(0, 0, 0, 0);
	pVLayout->addWidget(chartView);
	this->setLayout(pVLayout);

}
