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
	return projectModel.valid();
}

bool JKProjectDAL::OpenProject(const JKString & path)
{
	JKPath filePath(path);
	if (!filePath.is_file())
		return false;
	
	JKString fileData;
	size_t dataLen = 0;
	if (!JKFileIO::ReadFile(path, JKFileIO::Read, fileData, dataLen))
		return false;

	try 
	{
		projectModel = new JKProjectModel;

		Json::Reader reader;
		Json::Value jsonValue;
		reader.parse(fileData, jsonValue);

		projectModel->Deserialization(jsonValue);
	}
	catch (...)
	{

		projectModel = nullptr;
		return false;
	}
}

void JKProjectDAL::saveProject(const JKString & path)
{
}

JKRef_Ptr<JKProjectModel> JKProjectDAL::getProjectModel()
{
	return JKRef_Ptr<JKProjectModel>();
}
