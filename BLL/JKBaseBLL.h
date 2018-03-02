#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>
#include "JKBLLContext.h"

template<typename T>
class JKBLLContainer;

template<typename T>
class JKBaseBLL : public JKReferenced
{
	template<typename T>
	friend class JKBLLContainer;
public:
	JKBaseBLL(JKInt _parentID = -1) { parentID = _parentID; }
	virtual ~JKBaseBLL() {}

public:
	const JKString & getId() {return ptrModel->id; }
	JKInt getOriginID() { return ptrModel.get_id(); }

	JKInt getParentID() { return parentID; };
	void setParentID(JKInt pID) { parentID = pID; };

	/** Éý¼¶Êý¾Ý */
	virtual void upgradeDataVersion(int dataVersion) {}

	virtual void save();
	virtual bool destoried();

protected:
	bean_ptr<T> getModel() { return ptrModel; };
	void setModel(bean_ptr<T> model) { ptrModel = model; }

	virtual void destroy();

protected:
	JKInt parentID = -1;
	bean_ptr<T> ptrModel; 
};


template<typename T>
void JKBaseBLL<T>::save()
{
	ptrModel.save();
}

template<typename T>
void JKBaseBLL<T>::destroy()
{
	ptrModel.destroy();
}

template<typename T>
bool JKBaseBLL<T>::destoried()
{
	return ptrModel.destroyed();
}