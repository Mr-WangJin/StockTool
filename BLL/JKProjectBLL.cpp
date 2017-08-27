#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "BLL/JKStockCodeBLL.h"


JKProjectBLL::JKProjectBLL()
{
	std::vector<sqlid_t> vecIds = SingleDB.getBeanIds<JKProjectModel>();
	if (vecIds.size() > 0)
	{
		refProjectModel = SingleDB.loadBean<JKProjectModel>(vecIds[0]);
		if (refProjectModel->vecStockCode.size() > 0)
			refCurStockCode = new JKStockCodeBLL(refProjectModel->vecStockCode[0]);
	}
	else
	{
		refProjectModel = SingleDB.createBean<JKProjectModel>();
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
	JKRef_Ptr<JKStockCodeBLL> refStockCode = new JKStockCodeBLL();

	refProjectModel->addStockCode(refStockCode->getStockCodeModel());
	return refStockCode;
}

void JKProjectBLL::setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode)
{
	refCurStockCode = stockCode;
}

JKRef_Ptr<JKStockCodeBLL> JKProjectBLL::getCurStockCode()
{
	return refCurStockCode;
}