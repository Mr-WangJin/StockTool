#include "stdafx.h"
#include "JKStockTableViewer.h"
#include <QHeaderView>
#include "JKStockTableModel.h"
#include "BLL/JKStockCodeTradeBLL.h"


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
	this->setColumnWidth(3, 70);
	this->setColumnWidth(4, 70);
}

void JKStockTableViewer::getSelectedStockTradeIds(std::vector<JKString>& _vecStockTradeIDs)
{
	JKStockTableModel* _tableModel = qobject_cast<JKStockTableModel*>(model());
	QModelIndexList indexList = selectionModel()->selectedIndexes();

	for each(auto& index in indexList)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockTrade = _tableModel->getStockTradeByRow(index.row());
		if (!_refStockTrade.valid())
			continue;
		JKString id = _refStockTrade->getId();

		if (std::find(_vecStockTradeIDs.begin(), _vecStockTradeIDs.end(), id) == _vecStockTradeIDs.end())
			_vecStockTradeIDs.push_back(id);
	}

}

void JKStockTableViewer::getSelectedStockTrade(std::vector<JKRef_Ptr<JKStockCodeTradeBLL>>& _vecStockTrade)
{
	JKStockTableModel* _tableModel = qobject_cast<JKStockTableModel*>(model());
	QModelIndexList indexList = selectionModel()->selectedIndexes();

	for each(auto& index in indexList)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockTrade = _tableModel->getStockTradeByRow(index.row());
		if (!_refStockTrade.valid())
			continue;

		if (std::find(_vecStockTrade.begin(), _vecStockTrade.end(), _refStockTrade) == _vecStockTrade.end())
			_vecStockTrade.push_back(_refStockTrade);
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

