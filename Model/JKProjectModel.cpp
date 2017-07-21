#include "stdafx.h"
#include "JKProjectModel.h"
#include <File/JKPath.h>

USING_JK_NAMESPACE

JKProjectModel::JKProjectModel()
	: JKBaseModel()
{
}


JKProjectModel::~JKProjectModel()
{
}

bool JKProjectModel::OpenProject(const JKString & path)
{
	JKPath filePath(path);
	if (!filePath.is_file())
		return false;
	
	


}
