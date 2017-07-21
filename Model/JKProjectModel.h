#pragma once
#include "JKBaseModel.h"



class JKProjectModel : public JKBaseModel
{
public:
	JKProjectModel();
	~JKProjectModel();


	bool OpenProject(const JKString &path);


protected:


private:
	JKString m_Name;


};

