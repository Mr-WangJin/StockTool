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

	JKRef_Ptr<JKStockCodeBLL> newStockCode();
	void setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode);
	JKRef_Ptr<JKStockCodeBLL> getCurStockCode();

	void savePrject();
	

protected:
	
private:
	bean_ptr<JKProjectModel> refProjectModel;

	JKRef_Ptr<JKStockCodeBLL> refCurStockCode;


};

