#include "stdafx.h"
#include "JKProjectModel.h"



HIBERLITE_EXPORT_CLASS(JKProjectModel)

void JKProjectModel::addStockCode(bean_ptr<JKStockCode> stockCode)
{
	vecStockCode.push_back(stockCode);
}
