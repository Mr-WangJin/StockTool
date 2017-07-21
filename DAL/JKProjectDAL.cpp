#include "stdafx.h"
#include "JKProjectDAL.h"
#include <File/JKPath.h>

USING_JK_NAMESPACE

JKProjectDAL::JKProjectDAL()
	: JKBaseDAL()
{
}


JKProjectDAL::~JKProjectDAL()
{
}

bool JKProjectDAL::OpenProject(const JKString & path)
{
	JKPath filePath(path);
	if (!filePath.is_file())
		return false;
	
	


}
