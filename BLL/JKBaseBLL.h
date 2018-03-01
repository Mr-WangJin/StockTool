#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>
#include "JKBLLContext.h"

template<typename T>
class JKBaseBLL : public JKReferenced
{
public:
	JKBaseBLL()	{}
	JKBaseBLL(JKRef_Ptr<JKBLLContext> _refContext){	this->setContext(_refContext);}
	virtual ~JKBaseBLL() {}

public:
	const JKString & getId() {return ptrModel->id; }

	/** Éý¼¶Êý¾Ý */
	virtual void upgradeDataVersion(int dataVersion) {}

	virtual void save();
	virtual void destory();
	virtual bool destoried();

protected:
	bean_ptr<T> getModel() { return ptrModel; };

	JKRef_Ptr<JKBLLContext> getContext() { return refContext; }
	void setContext(JKRef_Ptr<JKBLLContext> _refContext) { refContext = _refContext; }

protected:
	JKRef_Ptr<JKBLLContext> refContext;
	
	bean_ptr<T> ptrModel; 
};


template<typename T>
void JKBaseBLL<T>::save()
{
	ptrModel.save();
}

template<typename T>
void JKBaseBLL<T>::destory()
{
	ptrModel.destroy();
}

template<typename T>
bool JKBaseBLL<T>::destoried()
{
	return ptrModel.destroyed();
}