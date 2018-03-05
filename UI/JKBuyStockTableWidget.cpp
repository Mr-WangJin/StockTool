#include "stdafx.h"
#include "JKBuyStockTableWidget.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"
#include "BLL\JKBLLContainer.h"


///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
JKBuyStockTableAdapter::JKBuyStockTableAdapter(BaseObjectConstRefPtr _root)
	: JKVirtualModelAdapter()
{
	root = _root;
}

JKBuyStockTableAdapter::JKBuyStockTableAdapter(const JKBuyStockTableAdapter *)
{

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
		//typedef int (JKStockCodeBLL::*ptr_fun)(int);
		//ptr_fun f = static_cast<ptr_fun>(&JKStockCodeBLL::getTradeCountByType);
		//return (_stockCodeBll.get()->*f)((int)tradeType);

		return _stockCodeBll->getTradeCountByType((int)tradeType);
	}
}

BaseObjectPtr JKBuyStockTableAdapter::getItem(BaseObjectConstRefPtr parent, int index)
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
	return BaseObjectPtr();
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
		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = dynamic_cast<JKStockCodeTradeBLL*>(_baseObject.get());
		if (_stockCodeTradeBll)
		{
			if (index.column() == 1)
				return _stockCodeTradeBll->getBuyPrice();
			else
				return _stockCodeTradeBll->getCount();

		}
		return QVariant("aaaa");

	}
	else
	{
		return QVariant();
	}
}

QVariant JKBuyStockTableAdapter::headerData(int section, Qt::Orientation orientation, int role)
{
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0:
		{
			return QVariant(QStringLiteral("买卖"));
		}
		case 1:
		{
			return QVariant(QStringLiteral("时间"));
		}
		case 2:
		{
			return QVariant(QStringLiteral("数量"));
		}
		case 3:
		{
			return QVariant(QStringLiteral("买入价"));
		}
		case 4:
		{
				return QVariant(QStringLiteral("成本价"));
		}
		case 5:
		{
			return QVariant(QStringLiteral("预计收益"));
		}
		case 6:
		{
			return QVariant(QStringLiteral("收益百分比"));
		}
		case 7:
		{
			return QVariant(QStringLiteral("卖出数量"));
		}
		case 8:
		{
			return QVariant(QStringLiteral("卖出收益"));
		}
		default:
			break;
		}
	}

	return QVariant();
}

BaseObjectPtr JKBuyStockTableAdapter::getItemParent(BaseObjectConstRefPtr item)
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

int JKBuyStockTableAdapter::getColumnCount()
{
	return 9;
}

BaseObjectPtr JKBuyStockTableAdapter::getValue(BaseObjectConstRefPtr data)
{
	return data ? data : root;
}
