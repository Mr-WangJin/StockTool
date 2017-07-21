#pragma once
#include "JKBaseDAL.h"



class JKProjectDAL : public JKBaseDAL
{
public:
	JKProjectDAL();
	~JKProjectDAL();


	bool OpenProject(const JKString &path);


protected:


};

