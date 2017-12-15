#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>
#include "JKBLLContext.h"

class JKBaseBLL : public JKReferenced
{
public:
	JKBaseBLL();
	JKBaseBLL(JKRef_Ptr<JKBLLContext> _refContext);
	virtual ~JKBaseBLL();

	/** Éý¼¶Êý¾Ý */
	virtual void upgradeDataVersion(int dataVersion) {}

	void setContext(JKRef_Ptr<JKBLLContext> _refContext);
	JKRef_Ptr<JKBLLContext> getContext();

protected:
	JKRef_Ptr<JKBLLContext> refContext;
	

};

