#include "stdafx.h"
#include "JKStockTableWidget.h"
#include "JKFramework/JKSingleton.h"
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "JKUiContext.h"
#include "BLL/JKStockCodeTradeBLL.h"


///////////////////////////////////////////////////////////////////////
// JKBuyStockTableAdapter::JKBuyStockTableAdapter(ProjectBLLConstRefPtr _projectBll, BaseObjectConstRefPtr _root)
// 	: JKVirtualModelAdapter(_root)
// {
// 	projectBll = _projectBll;
// 	root = _root;
// 
// 	int c = 0;
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("类型")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("数量")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("成本价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入时间")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入合价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("预计收益")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("收益%")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("部分收益")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("备注")));
// 	columnCount = c;
// }
// 
// JKBuyStockTableAdapter::~JKBuyStockTableAdapter()
// {
// 
// }
// 
// int JKBuyStockTableAdapter::getChildItemsCount(BaseObjectConstRefPtr objectPtr)
// {
// 	BaseObjectPtr _baseObject = getValue(objectPtr);
// 	if (_baseObject == nullptr)
// 		return 0;
// 	StockCodeBLLPtr _stockCodeBll = _baseObject->toTypeObject<JKStockCodeBLL*>();
// 	if (_stockCodeBll)
// 	{
// 		//typedef int (JKStockCodeBLL::*ptr_fun)(int);
// 		//ptr_fun f = static_cast<ptr_fun>(&JKStockCodeBLL::getTradeCountByType);
// 		//return (_stockCodeBll.get()->*f)((int)tradeType);
// 		int itemCount = 0;
// 		itemCount += _stockCodeBll->getTradeCountByType((int)tradeType);
// 
// 		return itemCount;
// 	}
// 
// // 	StockCodeTradeBLLPtr _stockCodeTrade = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
// // 	if (_stockCodeTrade)
// // 	{
// // 		return _stockCodeTrade->getTradeItemsCount();
// // 	}
// 
// 	return 0;
// }
// 
// BaseObjectPtr JKBuyStockTableAdapter::getChildItem(BaseObjectConstRefPtr parent, int index)
// {
// 	BaseObjectPtr _baseObject = getValue(parent);
// 	StockCodeBLLPtr _stockCodeBll = _baseObject->toTypeObject<JKStockCodeBLL*>();
// 	if (_stockCodeBll)
// 	{
// 		vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecStockCodeTrade;
// 		_stockCodeBll->getTradesByType(tradeType, vecStockCodeTrade);
// 
// 		BaseObjectPtr item;
// 		int iterIndex = 0;
// 		for (auto var : vecStockCodeTrade)
// 		{
// 			if ((int)var->getType() & tradeType)
// 			{
// 				if (iterIndex == index)
// 				{
// 					item = var->toBaseObject();
// 					break;
// 				}
// 				else
// 					iterIndex++;
// 			}
// 		}
// 
// 		return item;
// 	}
// 
// // 	StockCodeTradeBLLPtr _stockCodeTrade = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
// // 	if (_stockCodeTrade)
// // 	{
// // 		return _stockCodeTrade->getTradeItem(index);
// // 	}
// 
// 	return BaseObjectPtr();
// }
// 
// QVariant JKBuyStockTableAdapter::data(BaseObjectConstRefPtr item, int role, const int &row, const int & column)
// {
// 	auto _baseObject = getValue(item);
// 	if (role == Qt::DisplayRole)
// 	{
// 		auto _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
// 		if (_stockCodeTradeBll)
// 		{
// 			TradeType type = _stockCodeTradeBll->getType();
// 			JKStockTradeUtil tradeUtil;
// 			auto projectBll = JKSingleton<JKUiContext>::GetInstance().getProjectBLL();
// 			double latestPrice = projectBll->getCurStockCode()->getLatestPrice();
// 
// 			switch (column)
// 			{
// 			case 0:
// 				if (type == TradeType::BUY)
// 					return QStringLiteral("买入");
// 				else if (type == TradeType::PART)
// 					return QStringLiteral("部分");
// 				break;
// 			case 1:
// 				return _stockCodeTradeBll->getCouldSellCount();
// 				break;
// 			case 2:
// 				return _stockCodeTradeBll->getBuyPrice();
// 				break;
// 			case 3:
// 				return (int)((tradeUtil.getTradeBuyCostPrice(_stockCodeTradeBll) + 0.005) * 100) / 100.0f;
// 				break;
// 			case 4:
// 				return QString::fromStdString(_stockCodeTradeBll->getDate());
// 				break;
// 			case 5:
// 				return _stockCodeTradeBll->getCouldSellCount()* _stockCodeTradeBll->getBuyPrice();
// 				break;
// 			case 6:
// 				return (int)((tradeUtil.getExpactEarning(latestPrice, _stockCodeTradeBll, _stockCodeTradeBll->getCouldSellCount()) + 0.005) * 100) / 100.0;
// 				break;
// 			case 7:
// 				return QString("%1%").arg(QString::number(tradeUtil.getExpactEarningPercent(latestPrice, _stockCodeTradeBll) * 100, 'f', 2));
// 				break;
// 			case 8:
// 				if (type == TradeType::PART)
// 				{
// 					return (int)((_stockCodeTradeBll->getRealEarning() + 0.005) * 100) / 100.0f;
// 				}
// 				break;
// 			case 9:
// 				return QString::fromStdString(_stockCodeTradeBll->getRemark());
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 
// 		// 		StockCodeTradeItemBLLPtr _stockCodeTradeItem = _baseObject->toTypeObject<JKStockCodeTradeItemBLL*>();
// 		// 		if (_stockCodeTradeItem)
// 		// 		{
// 		// 			switch (index.column())
// 		// 			{
// 		// 			case 5:
// 		// 				return _stockCodeTradeItem->getsellCount();
// 		// 				break;
// 		// 			default:
// 		// 				break;
// 		// 			}
// 		// 		}
// 	}
// 	else if (role == Qt::BackgroundColorRole)
// 	{
// 		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
// 		if (_stockCodeTradeBll)
// 		{
// 			JKStockTradeUtil tradeUtil;
// 			auto project = JKSingleton<JKUiContext>::GetInstance().getProjectBLL();
// 			double latestPrice = project->getCurStockCode()->getLatestPrice();
// 			TradeType type = _stockCodeTradeBll->getType();
// 			QVariant variant;
// 			switch (column)
// 			{
// 			case 6:
// 			case 7:
// 			{
// 				double expactEarning = tradeUtil.getExpactEarning(latestPrice, _stockCodeTradeBll, _stockCodeTradeBll->getCouldSellCount());
// 				if (expactEarning >= 0)
// 					variant.setValue(UP_EARNING);
// 				else
// 					variant.setValue(DOWN_EARNING);
// 			}
// 			break;
// 			case 8:
// 				if (type == TradeType::PART)
// 				{
// 					double earning = (int)((_stockCodeTradeBll->getRealEarning() + 0.005) * 100) / 100.0f;
// 					if (earning >= 0)
// 						variant.setValue(UP_EARNING);
// 					else
// 						variant.setValue(DOWN_EARNING);
// 				}
// 				break;
// 			}
// 			return variant;
// 		}
// 	}
// 
// 	return QVariant();
// }
// 
// QVariant JKBuyStockTableAdapter::headerData(int section, Qt::Orientation orientation, int role)
// {
// 	if (orientation != Qt::Horizontal)
// 		return QVariant();
// 
// 	if (role == Qt::DisplayRole)
// 	{
// 		if (mapHeader.find(section) != mapHeader.end())
// 		{
// 			QVariant v = mapHeader[section];
// 			return mapHeader[section];
// 		}
// 	}
// 
// 	return QVariant();
// }
// 
// bool JKBuyStockTableAdapter::setData(BaseObjectConstRefPtr item, const QVariant & value, int role /*= Qt::EditRole*/)
// {
// 	if (role & Qt::EditRole == 0)
// 		return false;
// 	StockCodeTradeBLLPtr stockTrade = item->toTypeObject<JKStockCodeTradeBLL*>();
// 	if (stockTrade)
// 	{
// 		stockTrade->setRemark(value.toString().toStdString());
// 		return true;
// 	}
// 	return false;
// }
// 
// Qt::ItemFlags JKBuyStockTableAdapter::flags(const QModelIndex & index) const
// {
// 	if (index.column() == mapHeader.size()-1)
// 		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
// 
// 	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
// }
// 
// BaseObjectPtr JKBuyStockTableAdapter::getItemParent(BaseObjectConstRefPtr item)
// {
// 	if (item == root)
// 		return nullptr;
// 	
// 	StockCodeTradeBLLPtr _stockCodeTradeBll = item->toTypeObject<JKStockCodeTradeBLL*>();
// 	if (_stockCodeTradeBll)
// 	{
// 		StockCodeBLLPtr _stockCode = LoadBLL(JKStockCodeBLL, JKStockCodeModel, _stockCodeTradeBll->getParentID(), -1);
// 		return _stockCode->toBaseObject();
// 	}
// // 	StockCodeTradeItemBLLPtr _stockCodeTradeItem = item->toTypeObject<JKStockCodeTradeItemBLL*>();
// // 	if (_stockCodeTradeItem)
// // 	{
// // 		StockCodeTradeBLLPtr _stockCodeTrade = LoadBLL(JKStockCodeTradeBLL, JKStockCodeTradeModel, _stockCodeTradeItem->getParentID(), -1);
// // 		return _stockCodeTrade->toBaseObject();
// // 	}
// 
// 	return nullptr;
// }
// 
// int JKBuyStockTableAdapter::getColumnCount()
// {
// 	return columnCount;
// }
// 
// BaseObjectPtr JKBuyStockTableAdapter::getValue(BaseObjectConstRefPtr objectPtr)
// {
// 	return objectPtr ? objectPtr : root;
// }
// 
// 
// //////////////////////////////////////////////////////////////////////////
// JKSellStockTableAdapter::JKSellStockTableAdapter(ProjectBLLConstRefPtr _projectBll, BaseObjectConstRefPtr _root)
// 	: JKVirtualModelAdapter(_root)
// {
// 
// 	int c = 0;
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("类型")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("数量")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("成本价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入时间")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("买入合价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("卖出价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("卖出时间")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("卖出合价")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("收益")));
// 	mapHeader.insert(std::make_pair(c++, QStringLiteral("收益%")));
// 	columnCount = c;
// }
// 
// JKSellStockTableAdapter::~JKSellStockTableAdapter()
// {
// 
// }
// 
// int JKSellStockTableAdapter::getChildItemsCount(BaseObjectConstRefPtr objectPtr)
// {
// 	BaseObjectConstRefPtr _baseObject = getValue(objectPtr);
// 	StockCodeBLLConstRefPtr _stockCodeBll = dynamic_cast<JKStockCodeBLL*>(_baseObject.get());
// 	if (_stockCodeBll != nullptr)
// 	{
// 		//typedef int (JKStockCodeBLL::*ptr_fun)(int);
// 		//ptr_fun f = static_cast<ptr_fun>(&JKStockCodeBLL::getTradeCountByType);
// 		//return (_stockCodeBll.get()->*f)((int)tradeType);
// 
// 		return _stockCodeBll->getTradeCountByType((int)tradeType);
// 	}
// 	return 0;
// }
// 
// BaseObjectPtr JKSellStockTableAdapter::getChildItem(BaseObjectConstRefPtr parent, int index)
// {
// 	BaseObjectConstRefPtr _baseObject = getValue(parent);
// 	StockCodeBLLConstRefPtr _stockCodeBll = dynamic_cast<JKStockCodeBLL*>(_baseObject.get());
// 	if (_stockCodeBll != nullptr)
// 	{
// 		vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecStockCodeTrade;
// 		_stockCodeBll->getTradesByType(tradeType, vecStockCodeTrade);
// 
// 		BaseObjectPtr item;
// 		int iterIndex = 0;
// 		for (auto var : vecStockCodeTrade)
// 		{
// 			if ((int)var->getType() & tradeType)
// 			{
// 				if (iterIndex == index)
// 				{
// 					item = dynamic_cast<JKBaseObject*>(var.get());
// 					break;
// 				}
// 				else
// 					iterIndex++;
// 			}
// 		}
// 
// 		return item;
// 	}
// 	return BaseObjectPtr();
// }
// 
// inline QVariant JKSellStockTableAdapter::data(BaseObjectConstRefPtr item, int role, const int & row, const int & column)
// {
// 	auto _baseObject = getValue(item);
// 	if (role == Qt::DisplayRole)
// 	{
// 		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
// 		JKStockTradeUtil tradeUtil;
// 		auto project = JKSingleton<JKUiContext>::GetInstance().getProjectBLL();
// 		double latestPrice = project->getCurStockCode()->getLatestPrice();
// 		if (_stockCodeTradeBll)
// 		{
// 			switch (column)
// 			{
// 			case 0:
// 				return QStringLiteral("卖出");
// 				break;
// 			case 1:
// 				return _stockCodeTradeBll->getSoldCount();
// 				break;
// 			case 2:
// 				return _stockCodeTradeBll->getBuyPrice();
// 				break;
// 			case 3:
// 				return (int)((tradeUtil.getTradeBuyCostPrice(_stockCodeTradeBll) + 0.005) * 100) / 100.0f;
// 				break;
// 			case 4:
// 				return QString::fromStdString(_stockCodeTradeBll->getDate());
// 				break;
// 			case 5:
// 				return _stockCodeTradeBll->getBuyAmount();
// 				break;
// 			case 6:
// 				return _stockCodeTradeBll->getSellPrice();
// 				break;
// 			case 7:
// 				return QString::fromStdString(_stockCodeTradeBll->getSoldDate());
// 				break;
// 			case 8:
// 				return _stockCodeTradeBll->getSoldAmount();
// 				break;
// 			case 9:
// 				return (int)((_stockCodeTradeBll->getRealEarning() + 0.005) * 100) / 100.0f;
// 				break;
// 			case 10:
// 				return QString("%1%").arg(QString::number(tradeUtil.getRealEarningPercent(_stockCodeTradeBll) * 100, 'f', 2));
// 				break;
// 			default:
// 				break;
// 			}
// 		}
// 	}
// 	else if (role == Qt::BackgroundColorRole)
// 	{
// 		StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = _baseObject->toTypeObject<JKStockCodeTradeBLL*>();
// 		if (_stockCodeTradeBll)
// 		{
// 			JKStockTradeUtil tradeUtil;
// 			auto project = JKSingleton<JKUiContext>::GetInstance().getProjectBLL();
// 			double latestPrice = project->getCurStockCode()->getLatestPrice();
// 			QVariant variant;
// 			switch (column)
// 			{
// 			case 9:
// 			{
// 				if (_stockCodeTradeBll->getRealEarning() >= 0)
// 					variant.setValue(UP_EARNING);
// 				else
// 					variant.setValue(DOWN_EARNING);
// 			}
// 			break;
// 			}
// 			return variant;
// 		}
// 	}
// 	return QVariant();
// }
// 
// QVariant JKSellStockTableAdapter::headerData(int section, Qt::Orientation orientation, int role)
// {
// 	if (orientation != Qt::Horizontal)
// 		return QVariant();
// 
// 	if (role == Qt::DisplayRole)
// 	{
// 		if (mapHeader.find(section) != mapHeader.end())
// 		{
// 			QVariant v = mapHeader[section];
// 			return mapHeader[section];
// 		}
// 	}
// 
// 	return QVariant();
// }
// 
// Qt::ItemFlags JKSellStockTableAdapter::flags(const QModelIndex & index) const
// {
// 	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
// }
// 
// BaseObjectPtr JKSellStockTableAdapter::getItemParent(BaseObjectConstRefPtr item)
// {
// 	if (item == root)
// 		return nullptr;
// 
// 	StockCodeTradeBLLConstRefPtr _stockCodeTradeBll = dynamic_cast<JKStockCodeTradeBLL*>(item.get());
// 	if (_stockCodeTradeBll != nullptr)
// 	{
// 		StockCodeBLLConstRefPtr _stockCode = LoadBLL(JKStockCodeBLL, JKStockCodeModel, _stockCodeTradeBll->getParentID(), -1);
// 		return _stockCode->toBaseObject();
// 	}
// 	return nullptr;
// 	//return item ? reinterpret_cast<Part*>(item)->parent : nullptr;
// }
// 
// int JKSellStockTableAdapter::getColumnCount()
// {
// 	return columnCount;
// }
// 
// BaseObjectPtr JKSellStockTableAdapter::getValue(BaseObjectConstRefPtr data)
// {
// 	return data ? data : root;
// }

StockBuyTableItem::StockBuyTableItem(StockCodeTradeBLLPtr data)
	: JKTreeModelCustomItem(data)
{
	addGetter(0, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		TradeType type = data->getType();
		if (type == TradeType::BUY)
			return QVariant(QStringLiteral("买入"));
		else if (type == TradeType::PART)
			return QVariant(QStringLiteral("买入"));

	});
	addGetter(1, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		return data->getCouldSellCount();
	});
	addGetter(2, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		return data->getBuyPrice(); });
	addGetter(3, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		JKStockTradeUtil tradeUtil;
		return (int)((tradeUtil.getTradeBuyCostPrice(data) + 0.005) * 100) / 100.0f;
	});
	addGetter(4, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		return QVariant(QString::fromStdString(data->getDate()));
	});
	addGetter(5, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		return data->getCouldSellCount()* data->getBuyPrice();
	});
	addGetter(6, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		JKStockTradeUtil tradeUtil;
		double latestPrice = JKSingleton<JKUiContext>::GetInstance().getProjectBLL()->getCurStockCode()->getLatestPrice();
		return (int)((tradeUtil.getExpactEarning(latestPrice, data, data->getCouldSellCount()) + 0.005) * 100) / 100.0;
	});
	addGetter(7, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		JKStockTradeUtil tradeUtil;
		double latestPrice = JKSingleton<JKUiContext>::GetInstance().getProjectBLL()->getCurStockCode()->getLatestPrice();
		return QVariant(QString("%1%").arg(QString::number(tradeUtil.getExpactEarningPercent(latestPrice, data) * 100, 'f', 2)));
	});
	addGetter(8, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		TradeType type = data->getType();
		if (type == TradeType::PART)
		{
			return (int)((data->getRealEarning() + 0.005) * 100) / 100.0f;
		}
	});
	addGetter(9, Qt::DisplayRole, [](StockCodeTradeBLLPtr data) {
		return QVariant(QString::fromStdString(data->getRemark()));
	});
}

StockBuyTableItem::~StockBuyTableItem()
{
}




