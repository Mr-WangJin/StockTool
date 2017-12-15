#include "stdafx.h"
#include "JKBaseBLL.h"

JKBaseBLL::JKBaseBLL()
	: JKReferenced()
{
}

JKBaseBLL::JKBaseBLL(JKRef_Ptr<JKBLLContext> _refContext)
{
	this->setContext(_refContext);
}

JKBaseBLL::~JKBaseBLL()
{
}

void JKBaseBLL::setContext(JKRef_Ptr<JKBLLContext> _refContext)
{
	refContext = _refContext;
}

JKRef_Ptr<JKBLLContext> JKBaseBLL::getContext()
{
	return refContext;
}
