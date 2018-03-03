#include "stdafx.h"
#include "JKBuyStockTableWidget.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"
#include "BLL\JKBLLContainer.h"

JKBuyStockTableViewer::JKBuyStockTableViewer(QWidget* parent/* = nullptr*/)
{

}
JKBuyStockTableViewer::~JKBuyStockTableViewer()
{

}

void JKBuyStockTableViewer::setModel(QAbstractItemModel * model)
{
	QTableView::setModel(model);

}

///////////////////////////////////////////////////////////////////////
JKBuyStockTableAdapter::JKBuyStockTableAdapter(BaseObjectConstRefPtr _root)
	: JKVirtualModelAdapter()
{
	root = _root;
}

JKBuyStockTableAdapter::~JKBuyStockTableAdapter()
{

}

int JKBuyStockTableAdapter::getItemsCount(BaseObjectConstRefPtr parent)
{
	BaseObjectConstRefPtr _baseObject = getValue(parent);
	StockCodeBLLConstRefPtr _stockCodeBll = dynamic_cast<JKStockCodeBLL*>(_baseObject.get());
	if (_stockCodeBll != nullptr)
	{
		return _stockCodeBll->getTradeCountByType((int)tradeType);
	}

}


BaseObjectConstRefPtr JKBuyStockTableAdapter::getItem(BaseObjectConstRefPtr parent, int index)
{
	BaseObjectConstRefPtr _baseObject = getValue(parent);
	StockCodeBLLConstRefPtr _stockCodeBll = dynamic_cast<JKStockCodeBLL*>(_baseObject.get());
	if (_stockCodeBll != nullptr)
	{
		vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecStockCodeTrade;
		_stockCodeBll->getTradesByType(tradeType, vecStockCodeTrade);

		BaseObjectPtr item;
		int iterIndex = 0;
		for (auto var : vecStockCodeTrade)
		{
			if ((int)var->getType() & tradeType)
			{
				if (iterIndex == index)
				{
					item = dynamic_cast<JKBaseObject*>(var.get());
					break;
				}
				else
					iterIndex++;
			}
		}

		return item;
	}
}


QVariant JKBuyStockTableAdapter::data(BaseObjectConstRefPtr item, int role, const QModelIndex &index)
{
	auto _baseObject = getValue(item);
	if (role == Qt::DisplayRole)
	{
		StockCodeBLLConstRefPtr _stockCodeBll = dynamic_cast<JKStockCodeBLL*>(_baseObject.get());
		if (_stockCodeBll != nullptr)
		{
			return _stockCodeBll->getClassName();
		}
		return QVariant("aaaa");

	}
	else
	{
		return QVariant();
	}
}

BaseObjectConstRefPtr JKBuyStockTableAdapter::getItemParent(BaseObjectConstRefPtr item)
{
	if (item == root)
		return nullptr;
	
	StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = dynamic_cast<JKStockCodeTradeBLL*>(item.get());
	if (_stockCodeTradeBll != nullptr)
	{
		StockCodeBLLConstRefPtr _stockCode = LoadBLL(JKStockCodeBLL, JKStockCodeModel, _stockCodeTradeBll->getParentID(), -1);
		return _stockCode->toBaseObject();
	}
	return nullptr;
	//return item ? reinterpret_cast<Part*>(item)->parent : nullptr;
}

BaseObjectConstRefPtr JKBuyStockTableAdapter::getValue(BaseObjectConstRefPtr data)
{
	return data ? data : root;
}
