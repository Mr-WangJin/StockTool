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

	virtual void Serialization(Json::Value &vObj) override;
	virtual void Deserialization(const Json::Value &vObj) override;

	JK_DISABLE_COPY(JKStockCode)


private:
	JKString m_Name;
	JKString m_Code;
	uint64_t m_LatestPrice;

	JKRef_Ptr<JKStockCodeSetting> m_Setting;

};

