#include "stdafx.h"
#include "JKProjectBLL.h"
#include "Model/JKProjectModel.h"
#include "BLL/JKStockCodeBLL.h"


JKProjectBLL::JKProjectBLL()
{
	refProjectModel = SingleDB.createBean<JKProjectModel>();
}


JKProjectBLL::~JKProjectBLL()
{
}