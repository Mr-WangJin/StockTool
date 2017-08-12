#include "stdafx.h"
#include "JKProjectBLL.h"
#include <JKFile/JKPath.h>

USING_JK_NAMESPACE

JKProjectBLL::JKProjectBLL()
	: JKBaseBLL()
{
}


JKProjectBLL::~JKProjectBLL()
{
}

bool JKProjectBLL::OpenProject(const JKString & path)
{
	JKPath filePath(path);
	if (!filePath.is_file())
		return false;
	
	


}
