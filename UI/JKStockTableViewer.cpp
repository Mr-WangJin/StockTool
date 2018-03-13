#include "stdafx.h"
#include "JKStockTableViewer.h"
#include <QHeaderView>
#include "BLL/JKStockCodeTradeBLL.h"
#include "JKVirtualTreeModel.h"


JKStockTableViewer::JKStockTableViewer(QWidget* parent/* = nullptr*/)
	: QTreeView(parent)
{
	this->initUI();	

}

JKStockTableViewer::~JKStockTableViewer()
{
}

void JKStockTableViewer::resizeColumnsWidth()
{
	this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	
	//this->resizeColumnToContents();
	//this->resizeColumnToContents();
	//setSectionResizeMode
	//this->resizeRowsToContents();


// 	for (int i = 0; i < this->header()->count(); i++)
// 	{
// 		this->resizeColumnToContents(i);
// 		this->setColumnWidth(i, this->columnWidth(i) + 20);  //多一些空余控件，不然每列内容很挤  
// 	}
}

void JKStockTableViewer::setModel(QAbstractItemModel * model)
{
	QTreeView::setModel(model);
}

void JKStockTableViewer::getSelectedStockTradeIds(std::vector<JKString>& _vecStockTradeIDs)
{
	JKVirtualTreeModel* _tableModel = qobject_cast<JKVirtualTreeModel*>(model());
	QModelIndexList indexList = selectionModel()->selectedIndexes();

	for each(auto& index in indexList)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockTrade = _tableModel->getItem(index)->toTypeObject<JKStockCodeTradeBLL*>();
		if (!_refStockTrade.valid())
			continue;
		JKString id = _refStockTrade->getId();

		if (std::find(_vecStockTradeIDs.begin(), _vecStockTradeIDs.end(), id) == _vecStockTradeIDs.end())
			_vecStockTradeIDs.push_back(id);
	}

}

void JKStockTableViewer::getSelectedStockTrade(std::vector<JKRef_Ptr<JKStockCodeTradeBLL>>& _vecStockTrade)
{
	JKVirtualTreeModel* _tableModel = qobject_cast<JKVirtualTreeModel*>(model());
	QModelIndexList indexList = selectionModel()->selectedIndexes();

	for each(auto& index in indexList)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> _refStockTrade = _tableModel->getItem(index)->toTypeObject<JKStockCodeTradeBLL*>();
		if (!_refStockTrade.valid())
			continue;

		if (std::find(_vecStockTrade.begin(), _vecStockTrade.end(), _refStockTrade) == _vecStockTrade.end())
			_vecStockTrade.push_back(_refStockTrade);
	}
}

void JKStockTableViewer::initHeader()
{
	//QHeaderView *headerGoods = this->horizontalHeader();
	////SortIndicator为水平标题栏文字旁边的三角指示器
	//headerGoods->setSortIndicator(0, Qt::AscendingOrder);
	//headerGoods->setSortIndicatorShown(true);
	////headerGoods->setClickable(true);
	//connect(headerGoods, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumn(int)));

	//this->sortByColumn(1, Qt::SortOrder::AscendingOrder);
}

void JKStockTableViewer::initUI()
{
	this->initHeader();

	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
}

