#pragma once
#include "BLL/JKBaseBLL.h"



class JKProjectBLL : public JKBaseBLL
{
public:
	JKProjectBLL();
	~JKProjectBLL();


	bool OpenProject(const JKString &path);


protected:


};

