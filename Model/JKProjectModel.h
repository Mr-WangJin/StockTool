#pragma once
#include "JKBaseModel.h"

#include "JKUtil/JKDateUtil.h"


class JKProjectModel : public JKBaseModel
{
	friend class JKProjectDAL;
private:
	JKProjectModel();
	~JKProjectModel();


	JK_DISABLE_COPY(JKProjectModel)


private:
	JKString m_Name;
	JKRef_Ptr<JKDateUtil> m_CreateDate;

};

