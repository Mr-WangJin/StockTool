#include "stdafx.h"
#include "JKStockTableWidget.h"
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL\JKStockCodeTradeBLL.h"
#include "BLL\JKBLLContainer.h"
#include <QModelIndex>
#include "BLL/JKStockTradeUtil.h"
#include "BLL/JKStockCodeTradeItemBLL.h"
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
	mapHeader.insert(std::make_pair(c++, QStringLiteral("部分收益")));
	columnCount = c;
}

JKBuyStockTableAdapter::~JKBuyStockTableAdapter()
{

}

int JKBuyStockTableAdapter::getItemsCount(BaseObjectConstRefPtr parent)
{
	BaseObjectPtr _baseObject = getValue(parent);
	if (_baseObject == nullptr)
		return 0;
	StockCodeBLLPtr _stockCodeBll = _baseObject->toTypeObject<JKStockCodeBLL*>();
	if (_stockCodeBll)
	{
		//typedef int (JKStockCodeBLL::*ptr_fun)(int);
		//ptr_fun f = static_cast<ptr_fun>(&JKStockCodeBLL::getTradeCountByType);
		//return (_stockCodeBll.get()->*f)((int)tradeType);
		int itemCount = 0;
		itemCount += _stockCodeBll->getTradeCountByType((int)tradeType);

		return itemCount;
	}

	QString name = _baseObject->getClassName();
	StockCodeTradeBLLPtr _stockCodeTrade = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
	if (_stockCodeTrade)
	{
		return _stockCodeTrade->getTradeItemsCount();
	}


	return 0;
}

BaseObjectPtr JKBuyStockTableAdapter::getItem(BaseObjectConstRefPtr parent, int index)
{
	BaseObjectPtr _baseObject = getValue(parent);
	StockCodeBLLPtr _stockCodeBll = _baseObject->toTypeObject<JKStockCodeBLL*>();
	if (_stockCodeBll)
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
					item = var->toBaseObject();
					break;
				}
				else
					iterIndex++;
			}
		}

		return item;
	}

	StockCodeTradeBLLPtr _stockCodeTrade = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
	if (_stockCodeTrade)
	{
		return _stockCodeTrade->getTradeItem(index);
	}

	return BaseObjectPtr();
}

QVariant JKBuyStockTableAdapter::data(BaseObjectConstRefPtr item, int role, const QModelIndex &index)
{
	auto _baseObject = getValue(item);
	if (role == Qt::DisplayRole)
	{
		auto _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
		if (_stockCodeTradeBll)
		{
			TradeType type = _stockCodeTradeBll->getType();
			JKStockTradeUtil tradeUtil(projectBll);
			double latestPrice = projectBll->getCurStockCode()->getLatestPrice();

			switch (index.column())
			{
			case 0:
				if (type == TradeType::BUY)
					return QStringLiteral("买入");
				else if (type == TradeType::PART)
					return QStringLiteral("部分");
				break;
			case 1:
				return _stockCodeTradeBll->getCouldSellCount();
				break;
			case 2:
				return _stockCodeTradeBll->getBuyPrice();
				break;
			case 3:
				return tradeUtil.getTradeBuyCostPrice(_stockCodeTradeBll);
				break;
			case 4:
				return QString::fromStdString(_stockCodeTradeBll->getDate());
				break;
			case 5:
				return _stockCodeTradeBll->getCouldSellCount()* _stockCodeTradeBll->getBuyPrice();
				break;
			case 6:
				return (int)((tradeUtil.getExpactEarning(latestPrice, _stockCodeTradeBll, _stockCodeTradeBll->getCouldSellCount())+0.005)*100) / 100.0;
				break;
			case 7:
				return QString("%1%").arg(QString::number(tradeUtil.getExpactEarningPercent(latestPrice, _stockCodeTradeBll) * 100, 'f', 2));
				break;
			case 8:
				if (type == TradeType::PART)
				{
					return (int)((_stockCodeTradeBll->getRealEarning() + 0.005) * 100) / 100.0f;
				}
				break;
			default:
				break;
			}
		}

		StockCodeTradeItemBLLPtr _stockCodeTradeItem = _baseObject->toTypeObject<JKStockCodeTradeItemBLL*>();
		if (_stockCodeTradeItem)
		{
			return 1;
		}
	}
	else if (role == Qt::BackgroundColorRole)
	{
		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
		if (_stockCodeTradeBll)
		{
			JKStockTradeUtil tradeUtil(projectBll);
			double latestPrice = projectBll->getCurStockCode()->getLatestPrice();
			TradeType type = _stockCodeTradeBll->getType();
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
			case 8:
				if (type == TradeType::PART)
				{
					double earning = (int)((_stockCodeTradeBll->getRealEarning() + 0.005) * 100) / 100.0f;
					if (earning >= 0)
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
	
	StockCodeTradeBLLPtr _stockCodeTradeBll = item->toTypeObject<JKStockCodeTradeBLL*>();
	if (_stockCodeTradeBll)
	{
		StockCodeBLLPtr _stockCode = LoadBLL(JKStockCodeBLL, JKStockCodeModel, _stockCodeTradeBll->getParentID(), -1);
		return _stockCode->toBaseObject();
	}
	StockCodeTradeItemBLLPtr _stockCodeTradeItem = item->toTypeObject<JKStockCodeTradeItemBLL*>();
	if (_stockCodeTradeItem)
	{
		StockCodeTradeBLLPtr _stockCodeTrade = LoadBLL(JKStockCodeTradeBLL, JKStockCodeTradeModel, _stockCodeTradeItem->getParentID(), -1);
		return _stockCodeTrade->toBaseObject();
	}

	return nullptr;
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
				break;
			case 1:
				return _stockCodeTradeBll->getSoldCount();
				break;
			case 2:
				return _stockCodeTradeBll->getBuyPrice();
				break;
			case 3:
				return (int)((tradeUtil.getTradeBuyCostPrice(_stockCodeTradeBll)+0.005)*100)/100.0f;
				break;
			case 4:
				return QString::fromStdString(_stockCodeTradeBll->getDate());
				break;
			case 5:
				return _stockCodeTradeBll->getBuyAmount();
				break;
			case 6:
				return _stockCodeTradeBll->getSellPrice();
				break;
			case 7:
				return QString::fromStdString(_stockCodeTradeBll->getSoldDate());
				break;
			case 8:
				return _stockCodeTradeBll->getSoldAmount();
				break;
			case 9:
				return (int)((_stockCodeTradeBll->getRealEarning() + 0.005) * 100) / 100.0f;
				break;
			case 10:
				return QString("%1%").arg(QString::number(tradeUtil.getRealEarningPercent(_stockCodeTradeBll)*100, 'f', 2));
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
