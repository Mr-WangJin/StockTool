#include "stdafx.h"
#include "JKBLLContext.h"
#include "JKProjectBLL.h"
#include "JKStockCodeBLL.h"

JKBLLContext::JKBLLContext()
{
}

JKBLLContext::~JKBLLContext()
{
}

void JKBLLContext::setProject(JKRef_Ptr<JKProjectBLL> _refProject)
{
	refProject = _refProject;
}

JKRef_Ptr<JKProjectBLL> JKBLLContext::getProject()
{
	return refProject;
}
