#pragma once
#include "JKBaseDAL.h"
#include "Model/JKProjectModel.h"

class JKProjectDAL : public JKBaseDAL
{
public:
	JKProjectDAL();
	~JKProjectDAL();

	bool isValid() const;
	bool OpenProject(const JKString &path);
	void saveProject(const JKString &path);

	JKRef_Ptr<JKProjectModel> getProjectModel();

private:
	JKRef_Ptr<JKProjectModel> m_ProjectModel;

};

