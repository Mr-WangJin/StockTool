#pragma once

#include <JKFramework/SmartPtr/JKReferenced.h>

class JKProjectBLL;

class JKBLLContext : public JKReferenced
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

