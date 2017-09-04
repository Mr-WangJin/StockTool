#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "bll/JKStockCodeBLL.h"
#include "Model/JKModelFactory.h"


JKProjectBLL::JKProjectBLL(ProjectInitStatus status)
{
	if (status == ProjectInitStatus::DEFAULT_FirstOrNew)
	{
		std::vector<sqlid_t> vecIds = SingleDB.getBeanIds<JKProjectModel>();
		if (vecIds.size() > 0)
		{
			refJKProjectModel = SingleDB.loadBean<JKProjectModel>(vecIds[0]);
			if (refJKProjectModel->vecStockCode.size() > 0)
				refCurStockCode = new JKStockCodeBLL(refJKProjectModel->vecStockCode[0]);
		}
		else
		{
			refJKProjectModel = SingleDB.createBean<JKProjectModel>();
		}
	}
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