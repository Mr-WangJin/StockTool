#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "bll/JKStockCodeBLL.h"
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

