#include "stdafx.h"
#include "JKProjectData.h"
#include <File/JKPath.h>

USING_JK_NAMESPACE

JKProjectData::JKProjectData()
{
}


JKProjectData::~JKProjectData()
{
}

bool JKProjectData::OpenProject(const JKString & path)
{
	JKPath filePath(path);
	if (!filePath.is_file())
		return false;
	
	


}
