#pragma once

#include <JKFramework/SmartPtr/JKReferenced.h>

class JKProjectBLL;

class JKBLLContext : public JKReferenced
{
public:
	JKBLLContext();
	virtual ~JKBLLContext();

	void setProject(JKRef_Ptr<JKProjectBLL> _refProject);
	JKRef_Ptr<JKProjectBLL> getProject();


private:
	JKRef_Ptr<JKProjectBLL> refProject;



};

