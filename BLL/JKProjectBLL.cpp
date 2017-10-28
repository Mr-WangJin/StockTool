#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "JKStockCodeBLL.h"
#include "JKStockCodeTradeBLL.h"
#include "Model/JKModelFactory.h"


JKProjectBLL::JKProjectBLL(ProjectInitStatus status)
{
	if (status == ProjectInitStatus::DEFAULT_FirstOrNew)
	{
		std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<JKProjectModel>();
		if (vecIds.size() > 0)
		{
			refJKProjectModel = SingleDB->loadBean<JKProjectModel>(vecIds[0]);
			if (refJKProjectModel->vecStockCode.size() > 0)
				refCurStockCode = new JKStockCodeBLL(refJKProjectModel->vecStockCode[0]);
		}
		else
		{
			refJKProjectModel = SingleDB->createBean<JKProjectModel>();
		}
	}
}

JKRef_Ptr<JKProjectBLL> JKProjectBLL::newProject(const JKString &path)
{
	if (JKSingleton<JKDatabase>::GetInstance().newDatabase(path))
	{
		JKRef_Ptr<JKProjectBLL> refProjectBLL = new JKProjectBLL(JKProjectBLL::ProjectInitStatus::DEFAULT_FirstOrNew);
		return refProjectBLL;
	}
	
	return nullptr;
}

JKRef_Ptr<JKProjectBLL> JKProjectBLL::openProject(const JKString & path)
{
	if (JKSingleton<JKDatabase>::GetInstance().openDatabase(path))
	{
		JKRef_Ptr<JKProjectBLL> refProjectBLL = new JKProjectBLL(JKProjectBLL::ProjectInitStatus::DEFAULT_FirstOrNew);
		return refProjectBLL;
	}
	return nullptr;
}

JKProjectBLL::~JKProjectBLL()
{
}

void JKProjectBLL::savePrject()
{
	
}
JKRef_Ptr<JKStockCodeBLL> JKProjectBLL::newStockCode()
{
	JKRef_Ptr<JKStockCodeBLL> _refStockCode = new JKStockCodeBLL();

	refJKProjectModel->addStockCode(_refStockCode->getModel());
	return _refStockCode;
}

void JKProjectBLL::setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode)
{
	refCurStockCode = stockCode;
}

JKRef_Ptr<JKStockCodeBLL> JKProjectBLL::getCurStockCode()
{
	return refCurStockCode;
}

vector<JKRef_Ptr<JKStockCodeBLL>> JKProjectBLL::getAllStockCode()
{
	vector<JKRef_Ptr<JKStockCodeBLL>> vecTrades;

	for (auto &var : refJKProjectModel->vecStockCode)
	{
		JKRef_Ptr<JKStockCodeBLL> _refStockCodeTradeBLL = new JKStockCodeBLL(var);
		vecTrades.push_back(_refStockCodeTradeBLL);
	}

	return vecTrades;
}

float JKProjectBLL::getStampTax()
{
	return refJKProjectModel->stampTax;
}
float JKProjectBLL::getTransfer()
{
	return refJKProjectModel->transfer;
}
float JKProjectBLL::getCommission()
{
	return refJKProjectModel->commission;
}
void JKProjectBLL::setStampTax(float stampTax)
{
	refJKProjectModel->stampTax = stampTax;
}
void JKProjectBLL::setTransfer(float transfer)
{
	refJKProjectModel->transfer = transfer;
}
void JKProjectBLL::setCommission(float commission)
{
	refJKProjectModel->commission = commission;
}

double JKProjectBLL::getCostPrice(JKRef_Ptr<JKStockCodeTradeBLL> _refJKStockCodeTrade)
{
	double cost = _refJKStockCodeTrade->getBuyPrice();

	return cost + this->getBuyTaxes(_refJKStockCodeTrade) / _refJKStockCodeTrade->getCount();
}

double JKProjectBLL::getBuyTaxes(JKRef_Ptr<JKStockCodeTradeBLL> _refJKStockCodeTrade)
{
	JKUInt64 count = _refJKStockCodeTrade->getCount();
	double buyPrice = _refJKStockCodeTrade->getBuyPrice();
	double commission = 0;
	if (count*buyPrice <= 10000)
		commission = 10000 * refJKProjectModel->commission;
	else
		commission = count * buyPrice * refJKProjectModel->commission;

	double taxes = count * refJKProjectModel->transfer + commission;
	return taxes;
}
double JKProjectBLL::getSellTaxes(JKRef_Ptr<JKStockCodeTradeBLL> _refJKStockCodeTrade)
{
	JKUInt64 count = _refJKStockCodeTrade->getCount();
	double sellPrice = _refJKStockCodeTrade->getSellPrice();
	double commission = 0;
	if (count*sellPrice <= 10000)
		commission = 10000 * refJKProjectModel->commission;
	else
		commission = count * sellPrice * refJKProjectModel->commission;

	double taxes = count* refJKProjectModel->stampTax + count * refJKProjectModel->transfer + commission;
	return taxes;
}

double JKProjectBLL::getPredictSellTaxes(JKRef_Ptr<JKStockCodeTradeBLL> _refJKStockCodeTrade, double sellPrice)
{
	JKUInt64 count = _refJKStockCodeTrade->getCount();
	double commission = 0;
	if (count*sellPrice <= 10000)
		commission = 10000 * refJKProjectModel->commission;
	else
		commission = count * sellPrice * refJKProjectModel->commission;

	double taxes = count* refJKProjectModel->stampTax + count * refJKProjectModel->transfer + commission;
	return taxes;
}
