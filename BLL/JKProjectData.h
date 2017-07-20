#pragma once
#include "BLL/JKBASEDATA.H"

class JKProjectData : public JKBaseData
{
public:
	JKProjectData();
	~JKProjectData();


	bool OpenProject(const JKString &path);




};

