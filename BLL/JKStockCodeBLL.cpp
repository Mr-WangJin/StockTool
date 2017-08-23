#include "stdafx.h"
#include "JKStockCodeBLL.h"
#include "Model/JKStockCode.h"


JKStockCodeBLL::JKStockCodeBLL()
{

}

JKStockCodeBLL::~JKStockCodeBLL()
{
}

JKRef_Ptr<JKStockCodeBLL> JKStockCodeBLL::NewStockCodeBLL()
{
	bean_ptr<JKStockCode> stockCode = SingleDB.createBean<JKStockCode>();


	JKRef_Ptr<JKStockCodeBLL> stockCodeBll = new JKStockCodeBLL();
	return stockCodeBll;

}


//HIBERLITE_EXPORT_CLASS(JKStockCodeBLL)
