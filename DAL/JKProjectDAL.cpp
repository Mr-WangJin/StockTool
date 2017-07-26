#include "stdafx.h"
#include "JKProjectDAL.h"
#include <JKFile/JKPath.h>
#include <JKFile/JKFileIO.h>
#include "JKStockCodeDAL.h"
#include <json/reader.h>
#include <json/value.h>

USING_JK_NAMESPACE

JKProjectDAL::JKProjectDAL()
	: JKBaseDAL()
{
}


JKProjectDAL::~JKProjectDAL()
{
}

bool JKProjectDAL::isValid() const
{
	return m_ProjectModel.valid();
}

bool JKProjectDAL::OpenProject(const JKString & path)
{
	JKPath filePath(path);
	if (!filePath.is_file())
		return false;
	
	JKString data;
	size_t dataLen = 0;
	if (!JKFileIO::ReadFile(path, JKFileIO::Read, data, dataLen))
		return false;

	try 
	{
		m_ProjectModel = new JKProjectModel;

		Json::Reader reader;
		Json::Value result;
		reader.parse(data, result);

		Json::Value b5DFiles = result["B5DFiles"];
		for (int i = 0; i < b5DFiles.size(); ++i)
		{
			Json::Value b5dFile = b5DFiles[i];
			pB5DFile->setFileName(b5dFile["name"].asString());
			pB5DFile->setFullPath(b5dFile["path"].asString());
			pB5DFile->setVersionNum(b5dFile["versionNum"].asString());

		}
	}
	catch (...)
	{
		m_ProjectModel = nullptr;
		return;
	}
}

void JKProjectDAL::saveProject(const JKString & path)
{
}

JKRef_Ptr<JKProjectModel> JKProjectDAL::getProjectModel()
{
	return JKRef_Ptr<JKProjectModel>();
}
