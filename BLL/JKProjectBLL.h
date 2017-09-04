#pragma once

#include "bll/JKBaseBLL.h"
#include "Model/JKProjectModel.h"
//#include "bll/JKStockCodeBLL.h"

class JKStockCodeBLL;

class JKProjectBLL : public JKBaseBLL
{
public:
	enum class ProjectInitStatus
	{
		DEFAULT_First,
		DEFAULT_New,
		DEFAULT_FirstOrNew
	};
public:
	JKProjectBLL(ProjectInitStatus status);
	~JKProjectBLL();
	
	
	void savePrject();

	JKRef_Ptr<JKStockCodeBLL> newStockCode();
	JKRef_Ptr<JKStockCodeBLL> getCurStockCode();

	void setCurStockCode(JKRef_Ptr<JKStockCodeBLL> stockCode);

	

	JK_BLL_INIT(JKProject);

private:
	JKRef_Ptr<JKStockCodeBLL> refCurStockCode;


};

