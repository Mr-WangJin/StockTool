#include "stdafx.h"
#include "JKStockTableWidget.h"
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"
#include "BLL\JKBLLContainer.h"
#include <QModelIndex>
#include "BLL/JKStockTradeUtil.h"
#include "JKUiCommon.h"


///////////////////////////////////////////////////////////////////////
JKBuyStockTableAdapter::JKBuyStockTableAdapter(ProjectBLLConstRefPtr _projectBll, BaseObjectConstRefPtr _root)
	: JKVirtualModelAdapter(_projectBll, _root)
{
	projectBll = _projectBll;
	root = _root;

	int c = 0;
	mapHeader.insert(std::make_pair(c++, QStringLiteral("类型")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("数量")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("成本价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入时间")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入合价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("预计收益")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("收益%")));
	columnCount = c;
}

// JKBuyStockTableAdapter::JKBuyStockTableAdapter(const JKBuyStockTableAdapter *)
// {
// 
// }

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
	return 0;
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
		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
		JKStockTradeUtil tradeUtil(projectBll);
		double latestPrice = projectBll->getCurStockCode()->getLatestPrice();
		if (_stockCodeTradeBll)
		{
			switch (index.column())
			{
			case 0:
				return QStringLiteral("买入");
			case 1:
				return _stockCodeTradeBll->getCouldSellCount();
			case 2:
				return _stockCodeTradeBll->getBuyPrice();
			case 3:
				return tradeUtil.getTradeBuyCostPrice(_stockCodeTradeBll);
			case 4:
				return QString::fromStdString(_stockCodeTradeBll->getDate());
			case 5:
				return _stockCodeTradeBll->getBuyAmount();
			case 6:
				return tradeUtil.getExpactEarning(latestPrice, _stockCodeTradeBll, _stockCodeTradeBll->getCouldSellCount());
			case 7:
				return QString("%1%").arg(tradeUtil.getExpactEarningPercent(latestPrice, _stockCodeTradeBll) * 100);
			default:
				break;
			}
		}
	}
	else if (role == Qt::BackgroundColorRole)
	{
		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
		if (_stockCodeTradeBll)
		{
			JKStockTradeUtil tradeUtil(projectBll);
			double latestPrice = projectBll->getCurStockCode()->getLatestPrice();
			QVariant variant;
			switch (index.column())
			{
			case 6:
			case 7:
			{
				double expactEarning = tradeUtil.getExpactEarning(latestPrice, _stockCodeTradeBll, _stockCodeTradeBll->getCouldSellCount());
				if (expactEarning >= 0)
					variant.setValue(UP_EARNING);
				else
					variant.setValue(DOWN_EARNING);
			}
			break;
			}
			return variant;
		}
	}
	return QVariant();
}

QVariant JKBuyStockTableAdapter::headerData(int section, Qt::Orientation orientation, int role)
{
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		if (mapHeader.find(section) != mapHeader.end())
		{
			QVariant v = mapHeader[section];
			return mapHeader[section];
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
	return columnCount;
}

BaseObjectPtr JKBuyStockTableAdapter::getValue(BaseObjectConstRefPtr data)
{
	return data ? data : root;
}


//////////////////////////////////////////////////////////////////////////
JKSellStockTableAdapter::JKSellStockTableAdapter(ProjectBLLConstRefPtr _projectBll, BaseObjectConstRefPtr _root)
	: JKVirtualModelAdapter(_projectBll, _root)
{

	int c = 0;
	mapHeader.insert(std::make_pair(c++, QStringLiteral("类型")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("数量")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("成本价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入时间")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入合价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("卖出价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("卖出时间")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("卖出合价")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("收益")));
	mapHeader.insert(std::make_pair(c++, QStringLiteral("收益%")));
	columnCount = c;
}

JKSellStockTableAdapter::~JKSellStockTableAdapter()
{

}

int JKSellStockTableAdapter::getItemsCount(BaseObjectConstRefPtr parent)
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
	return 0;
}

BaseObjectPtr JKSellStockTableAdapter::getItem(BaseObjectConstRefPtr parent, int index)
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

QVariant JKSellStockTableAdapter::data(BaseObjectConstRefPtr item, int role, const QModelIndex &index)
{
	auto _baseObject = getValue(item);
	if (role == Qt::DisplayRole)
	{
		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
		JKStockTradeUtil tradeUtil(projectBll);
		double latestPrice = projectBll->getCurStockCode()->getLatestPrice();
		if (_stockCodeTradeBll)
		{
			switch (index.column())
			{
			case 0:
				return QStringLiteral("卖出");
			case 1:
				return _stockCodeTradeBll->getSoldCount();
			case 2:
				return _stockCodeTradeBll->getBuyPrice();
			case 3:
				return (int)((tradeUtil.getTradeBuyCostPrice(_stockCodeTradeBll)+0.005)*100)/100.0f;
			case 4:
				return QString::fromStdString(_stockCodeTradeBll->getDate());
			case 5:
				return _stockCodeTradeBll->getBuyAmount();
			case 6:
				return _stockCodeTradeBll->getSellPrice();
			case 7:
				break;
			case 8:
				return _stockCodeTradeBll->getSellAmount();
			case 9:
				return _stockCodeTradeBll->getRealEarning();
			case 10:
				return tradeUtil.getRealEarningPercent(_stockCodeTradeBll);
				break;
			default:
				break;
			}
		}
	}
	else if (role == Qt::BackgroundColorRole)
	{
		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
		if (_stockCodeTradeBll)
		{
			JKStockTradeUtil tradeUtil(projectBll);
			double latestPrice = projectBll->getCurStockCode()->getLatestPrice();
			QVariant variant;
			switch (index.column())
			{
			case 9:
			{
				if (_stockCodeTradeBll->getRealEarning() >= 0)
					variant.setValue(UP_EARNING);
				else
					variant.setValue(DOWN_EARNING);
			}
			break;
			}
			return variant;
		}
	}
	return QVariant();
}

QVariant JKSellStockTableAdapter::headerData(int section, Qt::Orientation orientation, int role)
{
	if (orientation != Qt::Horizontal)
		return QVariant();

	if (role == Qt::DisplayRole)
	{
		if (mapHeader.find(section) != mapHeader.end())
		{
			QVariant v = mapHeader[section];
			return mapHeader[section];
		}
	}

	return QVariant();
}

BaseObjectPtr JKSellStockTableAdapter::getItemParent(BaseObjectConstRefPtr item)
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

int JKSellStockTableAdapter::getColumnCount()
{
	return columnCount;
}

BaseObjectPtr JKSellStockTableAdapter::getValue(BaseObjectConstRefPtr data)
{
	return data ? data : root;
}
