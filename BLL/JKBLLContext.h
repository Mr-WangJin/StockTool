#pragma once

#include "JKBaseBLL.h"

class JKProjectBLL;

class JKBLLContext : public JKBaseObject
{
	friend class JKSingleton<JKBLLContext>;
public:
	virtual ~JKBLLContext();

	void setProject(JKRef_Ptr<JKProjectBLL> _refProject);
	JKRef_Ptr<JKProjectBLL> getProject();

private:
	JKBLLContext();

private:
	JKRef_Ptr<JKProjectBLL> refProject;


};

