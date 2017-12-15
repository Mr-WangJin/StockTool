#include "stdafx.h"
#include "JKStockTableViewer.h"
#include <QHeaderView>


JKStockTableViewer::JKStockTableViewer(QWidget* parent/* = nullptr*/)
	: QTableView(parent)
{
	this->initUI();
	
}

JKStockTableViewer::~JKStockTableViewer()
{
}

void JKStockTableViewer::setModel(QAbstractItemModel * model)
{
	QTableView::setModel(model);

	this->setColumnWidth(0, 40);
	this->setColumnWidth(1, 80);
	this->setColumnWidth(2, 50);
}

void JKStockTableViewer::getSelectedStockCode(std::vector<JKString>& _vecStockTradeIDs)
{
	QModelIndexList indexList = this->selectedIndexes();
	
	for each(auto& index in indexList)
	{
		JKString id = index.data().toString().toStdString();
		if (std::find(_vecStockTradeIDs.begin(), _vecStockTradeIDs.end(), id) == _vecStockTradeIDs.end())
			_vecStockTradeIDs.push_back(id);
	}

}

void JKStockTableViewer::initHeader()
{
	QHeaderView *headerGoods = this->horizontalHeader();
	//SortIndicator为水平标题栏文字旁边的三角指示器
	headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	headerGoods->setSortIndicatorShown(true);
	//headerGoods->setClickable(true);
	connect(headerGoods, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));

	this->sortByColumn(1, Qt::SortOrder::AscendingOrder);
}

void JKStockTableViewer::initUI()
{
	this->initHeader();

	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
}

