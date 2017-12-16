#include "stdafx.h"
#include "JKStockCodeTradeBLL.h"
#include "JKStockCodeTradeItemBLL.h"
#include "Model/JKStockCodeTradeModel.h"
#include "JKStockTradeUtil.h"
#include "JKProjectBLL.h"
#include "JKStockCodeBLL.h"


void JKStockCodeTradeBLL::setParams(TradeType type, JKString date, JKUInt64 count, double buyPrice)
{
	refJKStockCodeTradeModel->type = (JKUInt)type;
	refJKStockCodeTradeModel->date = date;
	refJKStockCodeTradeModel->buyCount = count;
	refJKStockCodeTradeModel->buyPrice = buyPrice;
}

// void JKStockCodeTradeBLL::sell(double price)
// {
// 	refJKStockCodeTradeModel->type = (JKUInt)TradeType::SELL;
// 	refJKStockCodeTradeModel->sellPrice = price;
// }

void JKStockCodeTradeBLL::sell(double sellPrice, size_t sellCount, size_t sellSumCount, float stampTax, float transfer, float commission)
{
	if (this->getCouldSellCount() < sellCount || sellCount <= 0 || sellSumCount < sellCount)
		return;

	JKStockTradeUtil stockTrackUtil(stampTax, transfer, commission);
	double expactEarning = stockTrackUtil.getExpactEarning(sellPrice, JKRef_Ptr<JKStockCodeTradeBLL>(this));
	
	JKRef_Ptr<JKStockCodeTradeItemBLL> _refStockCodeTradeItem = new JKStockCodeTradeItemBLL(refContext);
	_refStockCodeTradeItem->setParams(sellPrice, sellCount, sellSumCount, stampTax, transfer, commission);
	_refStockCodeTradeItem->setRealEarning(expactEarning);
	refJKStockCodeTradeModel->addStockCodeTradeItem(_refStockCodeTradeItem->getModel());

	this->updateType();
}

TradeType JKStockCodeTradeBLL::getType()
{
	return (TradeType)refJKStockCodeTradeModel->type;
}
void JKStockCodeTradeBLL::updateType()
{
	int couldSellCount = this->getCouldSellCount();

	if (couldSellCount == 0)
	{
		refJKStockCodeTradeModel->type = (int)TradeType::SELL;
	}
	else if (couldSellCount == this->getCount())
	{
		refJKStockCodeTradeModel->type = (int)TradeType::BUY;
	}
	else
	{
		refJKStockCodeTradeModel->type = (int)TradeType::PART;
	}
}

JKString JKStockCodeTradeBLL::getDate()
{
	return refJKStockCodeTradeModel->date;
}
JKUInt64 JKStockCodeTradeBLL::getCount()
{
	return refJKStockCodeTradeModel->buyCount;
}
JKUInt64 JKStockCodeTradeBLL::getSoldCount()
{
	JKUInt64 count = 0;
	for (auto &var : refJKStockCodeTradeModel->vecSellItem)
	{
		count += var->sellCount;
	}
	return count;
}
JKUInt64 JKStockCodeTradeBLL::getCouldSellCount()
{
	JKUInt64 count = 0;
	for (auto &var : refJKStockCodeTradeModel->vecSellItem)
	{
		count += var->sellCount;
	}
	return refJKStockCodeTradeModel->buyCount - count;
}
double JKStockCodeTradeBLL::getBuyPrice()
{
	return refJKStockCodeTradeModel->buyPrice;
}

double JKStockCodeTradeBLL::getSellPrice()
{
	return refJKStockCodeTradeModel->sellPrice;
}

double JKStockCodeTradeBLL::getBuyPureCost()
{
	return refJKStockCodeTradeModel->buyPrice * refJKStockCodeTradeModel->buyCount;
}

double JKStockCodeTradeBLL::getRealEarning()
{
	double realEarning = 0;
	for (auto &var : refJKStockCodeTradeModel->vecSellItem)
	{
		realEarning += var->realEarning;
	}
	return realEarning;
}

void JKStockCodeTradeBLL::getTradeItems(std::vector<JKRef_Ptr<JKStockCodeTradeItemBLL>>& vecTradeItems)
{
	for (auto &var : refJKStockCodeTradeModel->vecSellItem)
	{
		JKRef_Ptr<JKStockCodeTradeItemBLL> _refStockCodeTradeItem = new JKStockCodeTradeItemBLL(var, refContext);
		vecTradeItems.push_back(_refStockCodeTradeItem);
	}
}

void JKStockCodeTradeBLL::upgradeDataVersion(int dataVersion)
{
	/** 升级说明
	 *	之前版本卖出没有创建JKStockCodeTradeItemModel，
	 *	后来添加上之后，需要对之前的工程创建JKStockCodeTradeItem
	 */
	if (dataVersion < 1)
	{
		if (this->getType() == TradeType::SELL)
		{
			JKRef_Ptr<JKProjectBLL> _refProject = refContext->getProject();
			float stampTax = _refProject->getStampTax();
			float transfer = _refProject->getTransfer();
			float commission = _refProject->getCommission();

			double sellPrice = this->getSellPrice();
			size_t count = this->getCount();

			this->sell(sellPrice, count, count, stampTax, transfer, commission);
		}
	}
}
