#pragma once


#include "BLL/JKBaseBLL.h"



class JKStockCodeBLL : public JKBaseBLL
{
public:
	JKStockCodeBLL();
	~JKStockCodeBLL();


	static JKRef_Ptr<JKStockCodeBLL> NewStockCodeBLL();



};


