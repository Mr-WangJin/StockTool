#pragma once
#include "JKBaseModel.h"

#include "JKUtil/JKDateUtil.h"
#include "JKStockCodeSetting.h"


class JKStockCode : public JKBaseModel
{
	friend class JKStockCodeDAL;
private:
	JKStockCode();
	~JKStockCode();


	JK_DISABLE_COPY(JKStockCode)


private:
	JKString m_Name;
	JKString m_Code;
	double m_LatestPrice;

	JKRef_Ptr<JKStockCodeSetting> m_Setting;

};

