#pragma once

#include "BLL/JKBaseBLL.h"
#include "Model/JKProjectModel.h"
//#include "BLL/JKStockCodeBLL.h"

class JKStockCodeBLL;

class JKProjectBLL : public JKBaseBLL
{
public:
	JKProjectBLL();
	~JKProjectBLL();

	JKRef_Ptr<JKStockCodeBLL> AddNewStockCode();

protected:
	
private:
	bean_ptr<JKProjectModel> refProjectModel;
};

