#include "stdafx.h"
#include "JKProjectModel.h"



HIBERLITE_EXPORT_CLASS(JKProjectModel)

void JKProjectModel::addStockCode(bean_ptr<JKStockCodeModel> stockCode)
{
	vecStockCode.push_back(stockCode);
}
