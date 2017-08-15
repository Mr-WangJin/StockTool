#pragma once

#include <JKFramework/SmartPtr/JKReferenced.h>
#include <JKFramework\JKJsonCPPHelper.h>
#include <json\value.h>

USING_JK_NAMESPACE

class JKBaseModel : public JKReferenced , public JKJsonCPPBase<Json::Value>
{
public:
	JKBaseModel();
	virtual ~JKBaseModel();




};