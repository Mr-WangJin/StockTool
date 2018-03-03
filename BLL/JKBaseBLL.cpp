#include "stdafx.h"
#include "JKBaseBLL.h"


JKBaseObject::JKBaseObject()
{

}

JKBaseObject::~JKBaseObject()
{

}

const char* JKBaseObject::getClassName()
{
	return typeid(this).name();
}


//////////////////////////////////////////////////////////////////////////