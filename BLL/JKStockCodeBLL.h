#pragma once


#include "BLL/JKBaseBLL.h"
#include "Model\JKStockCode.h"



class JKStockCodeBLL : public JKBaseBLL
{
	friend class JKProjectBLL;
public:
	~JKStockCodeBLL();

	JKString getName();

	void setParams(JKString name, JKString code, double latestPrice);

private:
	JKStockCodeBLL();

protected:
	JKStockCodeBLL(bean_ptr<JKStockCode>);
	bean_ptr<JKStockCode> getStockCodeModel() { return refStockCodeModel; };

private:
	bean_ptr<JKStockCode> refStockCodeModel;


};


