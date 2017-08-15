#pragma once
#include "JKBaseModel.h"

#include "JKUtil/JKDateUtil.h"


class JKProjectModel : public JKBaseModel
{
	friend class JKProjectDAL;
private:
	JKProjectModel();
	~JKProjectModel();

	virtual void Serialization(Json::Value &vObj) override;
	virtual void Deserialization(const Json::Value &vObj) override;


	JK_DISABLE_COPY(JKProjectModel)


private:
	JKString name;
	JKRef_Ptr<JKDateUtil> createDate;

};

