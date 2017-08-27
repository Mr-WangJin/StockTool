#include "stdafx.h"
#include "JKStockCodeBLL.h"
#include "Model/JKStockCode.h"


JKStockCodeBLL::JKStockCodeBLL()
{
	refStockCodeModel = SingleDB.createBean<JKStockCode>();
}

JKStockCodeBLL::JKStockCodeBLL(bean_ptr<JKStockCode> _refStockCodeModel)
{
	refStockCodeModel = _refStockCodeModel;
}

JKStockCodeBLL::~JKStockCodeBLL()
{
}

JKString JKStockCodeBLL::getName()
{
	return refStockCodeModel->name;
}

void JKStockCodeBLL::setParams(JKString name, JKString code, double latestPrice)
{
	refStockCodeModel->name = name;
	refStockCodeModel->code = code;
	refStockCodeModel->latestPrice = latestPrice;

}



//HIBERLITE_EXPORT_CLASS(JKStockCodeBLL)
