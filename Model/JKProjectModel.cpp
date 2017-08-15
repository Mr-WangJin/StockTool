#include "stdafx.h"
#include "JKProjectModel.h"
#include <JKFile/JKPath.h>

USING_JK_NAMESPACE

JKProjectModel::JKProjectModel()
	: JKBaseModel()
{
}


JKProjectModel::~JKProjectModel()
{
}

void JKProjectModel::Serialization(Json::Value &vObj)
{
	//vObj["aaa"] = name;
	JsonCppWriteValue(vObj, name);
	JsonCppWriteKeyValue(vObj, "createDate", createDate->toString());
}

void JKProjectModel::Deserialization(const Json::Value &vObj)
{

}
