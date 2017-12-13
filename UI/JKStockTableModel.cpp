#include "stdafx.h"
#include "JKStockTableModel.h"
#include "BLL/JKProjectBLL.h"
#include "BLL/JKStockCodeTradeBLL.h"
#include "BLL/JKStockCodeBLL.h"
#include "BLL/JKStockTradeUtil.h"

Q_DECLARE_METATYPE(JKString);

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
	if (refProject.valid())
	{
		vector<JKRef_Ptr<JKStockCodeTradeBLL>> vecRefStockCodeTradeBLLTemp = refProject->getCurStockCode()->getAllTrades();

		for (auto &var : vecRefStockCodeTradeBLLTemp)
		{
			if (showType & Show_Buy_Only && var->getType() == TradeType::BUY)
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

int JKStockTableModel::rowCount(const QModelIndex & parent) const
{
	return vecRefStockCodeTradeBLL.size();
}

int JKStockTableModel::columnCount(const QModelIndex & parent) const
{
	return 8;
}

QVariant JKStockTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::BackgroundColorRole))
		return QVariant();

	double latestPrice = refProject->getCurStockCode()->getLatestPrice();

	QVariant variant;
	if (role == Qt::DisplayRole)
	{
		JKStockTradeUtil tradeUtil(refProject);
		JKRef_Ptr<JKStockCodeTradeBLL> var = vecRefStockCodeTradeBLL[index.row()];
		switch (index.column())
		{
		case 0:
		{
			if (var->getType() == TradeType::BUY)
				variant.setValue(QStringLiteral("买入"));
			else if (var->getType() == TradeType::SELL)
				variant.setValue(QStringLiteral("卖出"));
		}
		break;
		case 1:
		{
			variant.setValue(QString::fromStdString(var->getDate()));
		}
		break;
		case 2:
		{
			variant.setValue(var->getCount());
		}
		break;
		case 3:
		{
			variant.setValue(var->getBuyPrice());
		}
		break;
		case 4:
		{
			variant.setValue(tradeUtil.getTradeBuyCost(var));
		}
		break;
		case 5:
		{
			variant.setValue(var->getSellPrice());
		}
		break;
		case 6:
		{
			double preRealEarning = 0;
			if (var->getType() == TradeType::BUY)
				preRealEarning = tradeUtil.getExpactEarning(latestPrice, var);
// 			else if (var->getType() == TradeType::SELL)
// 				preRealEarning = tradeUtil.getRealEarning(var->getSellPrice(), var);

			variant.setValue(preRealEarning);
		}
		break;
		case 7:
		{
			variant.setValue(QString("%1%").arg(tradeUtil.getRealEarningPercent(var) * 100));
		}
		break;
		default:
			break;
		}
	}
	
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
			return QVariant(QStringLiteral("成本价"));
		}
		case 5:
		{
			return QVariant(QStringLiteral("卖出价"));
		}
		case 6:
		{
			return QVariant(QStringLiteral("预计收益"));
		}
		case 7:
		{
			return QVariant(QStringLiteral("收益百分比"));
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