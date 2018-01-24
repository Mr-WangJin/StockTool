#include "stdafx.h"
#include "JKStockTableModel.h"
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeTradeBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKStockTradeUtil.h"

Q_DECLARE_METATYPE(JKString);

#define UP_EARNING QColor(255, 0, 0, 230)
#define Down_EARNING QColor(0, 255, 0, 230)

JKStockTableModel::JKStockTableModel(JKRef_Ptr<JKProjectBLL> _refProject, QObject* parent/* = nullptr*/)
	: QAbstractTableModel(parent)
	, showType(Show_All)
{
	this->setProject(_refProject);	
}

JKStockTableModel::~JKStockTableModel()
{
}

void JKStockTableModel::setProject(JKRef_Ptr<JKProjectBLL> _refProject)
{
	beginResetModel();

	vecRefStockCodeTradeBLL.clear();
	refProject = _refProject;
	if (refProject.valid() && refProject->getCurStockCode().valid())
	{
		vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLLTemp = refProject->getCurStockCode()->getAllTrades();

		for (auto &var : vecRefStockCodeTradeBLLTemp)
		{
			if (showType & Show_Buy_Only && (var->getType() == TradeType::BUY || var->getType() == TradeType::PART))
			{
				vecRefStockCodeTradeBLL.push_back(var);
			}
			if (showType & Show_Sell_Only && var->getType() == TradeType::SELL)
			{
				vecRefStockCodeTradeBLL.push_back(var);
			}
		}
	}

	endResetModel();
}

void JKStockTableModel::setShowType(TableShowType _showType)
{
	showType = _showType;


}

JKRef_Ptr<JKStockCodeTradeBLL> JKStockTableModel::getStockTradeByRow(int row)
{
	if (row >= vecRefStockCodeTradeBLL.size())
		return nullptr;

	return vecRefStockCodeTradeBLL[row];
}

int JKStockTableModel::rowCount(const QModelIndex & parent) const
{
	return vecRefStockCodeTradeBLL.size();
}

int JKStockTableModel::columnCount(const QModelIndex & parent) const
{
	return colCount;
}

QVariant JKStockTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	QVariant variant;
	if (role == Qt::DisplayRole)
	{
		double latestPrice = refProject->getCurStockCode()->getLatestPrice();
		JKStockTradeUtil tradeUtil(refProject);
		JKRef_Ptr<JKStockCodeTradeBLL> var = vecRefStockCodeTradeBLL[index.row()];
		TradeType type = var->getType();
		switch (index.column())
		{
		case 0:
		{
			if (type == TradeType::BUY)
				variant.setValue(QStringLiteral("买入"));
			else if (type == TradeType::SELL)
				variant.setValue(QStringLiteral("卖出"));
			else if (type == TradeType::PART)
				variant.setValue(QStringLiteral("部分"));

		}
		break;
		case 1:
		{
			variant.setValue(QString::fromStdString(var->getDate()));
		}
		break;
		case 2:
		{
			if (showType == Show_Sell_Only)
				variant.setValue(var->getSoldCount());
			else
				variant.setValue(var->getCouldSellCount());
		}
		break;
		case 3:
		{
			variant.setValue(var->getBuyPrice());
		}
		break;
		case 4:
		{
			if (showType == Show_Sell_Only)
				variant.setValue(var->getSellPrice());
			else
				variant.setValue(tradeUtil.getTradeBuyCostPrice(var));
		}
		break;
		case 5:
		{
			if (type == TradeType::SELL)
				break;
			double expactEarning = tradeUtil.getExpactEarning(latestPrice, var);
			variant.setValue(expactEarning);
		}
		break;
		case 6:
		{
			if (type == TradeType::SELL)
				break;
			variant.setValue(QString("%1%").arg(tradeUtil.getExpactEarningPercent(latestPrice, var) * 100));
		}
		break;
		case 7:
		{
			if (type == TradeType::BUY)
				break;
			variant.setValue(var->getSoldCount());
		}
		break;
		case 8:
		{
			if (type == TradeType::BUY)
				break;
			variant.setValue(var->getRealEarning());
		}
		break;
		default:
			break;
		}
	}
	else if (role == Qt::BackgroundColorRole)
	{
		JKRef_Ptr<JKStockCodeTradeBLL> var = vecRefStockCodeTradeBLL[index.row()];
		TradeType type = var->getType();
		JKStockTradeUtil tradeUtil(refProject);
		double latestPrice = refProject->getCurStockCode()->getLatestPrice();

		switch (index.column())
		{
		case 5:
		case 6:
		{
			if (type == TradeType::SELL)
				break;
			double expactEarning = tradeUtil.getExpactEarning(latestPrice, var, var->getCouldSellCount());

			if (expactEarning >= 0)
			{
				variant.setValue(UP_EARNING);
			}
			else
			{
				variant.setValue(Down_EARNING);
			}
		}
		break;
		case 8:
		{
			if (type == TradeType::BUY)
				break;
			if (var->getRealEarning() >= 0)
			{
				variant.setValue(UP_EARNING);
			}
			else
			{
				variant.setValue(Down_EARNING);
			}
		}
		break;
			
		}
	}
// 	else if (role == Qt::UserRole)
// 	{
// 		JKRef_Ptr<JKStockCodeTradeBLL> var = vecRefStockCodeTradeBLL[index.row()];
// 		variant.setValue(QString::fromStdString(var->getId()));
// 	}
	
	return variant;
}

QVariant JKStockTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
			if (showType == Show_Sell_Only)
				return QVariant(QStringLiteral("卖出价"));
			else
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

bool JKStockTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	return false;
}

Qt::ItemFlags JKStockTableModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return  QAbstractTableModel::flags(index) /*| Qt::ItemIsEditable*/;
}

void JKStockTableModel::sort(int column, Qt::SortOrder order /*= Qt::AscendingOrder*/)
{
	beginResetModel();

	int stockSortColum = column;
	int sortOrder = order;
	std::sort(vecRefStockCodeTradeBLL.begin(), vecRefStockCodeTradeBLL.end(),
		[&](JKRef_Ptr<JKStockCodeTradeBLL> first, JKRef_Ptr<JKStockCodeTradeBLL> second) ->bool{
		bool bSort = false;
		switch (stockSortColum)
		{
		case 0:
		{
			if (first->getType() == second->getType())
				return false;
			bSort = first->getType() > second->getType();
		}
		break;
		case 1:
		{
			if (first->getDate() == second->getDate())
				return false;
			bSort = first->getDate() > second->getDate();
		}
		break;
		case 2:
		{
			if (first->getCount() == second->getCount())
				return false;
			bSort = first->getCount() > second->getCount();
		}
		break;
		case 3:
		{
			if (first->getBuyPrice() == second->getBuyPrice())
				return false;
			bSort = first->getBuyPrice() > second->getBuyPrice();
		}
		break;
		case 4:
		{
			return false;
		}
		break;
		case 5:
		{
			if (first->getSellPrice() == second->getSellPrice())
				return false;
			bSort = first->getSellPrice() > second->getSellPrice();
		}
		break;
		case 6:
		{
			return false;
		}
		break;
		case 7:
		{
			return false;
		}
		break;
		default:
			return false;
			break;
		}
		if (sortOrder == 1)
			bSort = !bSort;
		return bSort;
	});

	endResetModel();
}