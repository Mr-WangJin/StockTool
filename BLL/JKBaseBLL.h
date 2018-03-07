#pragma once

#include "JKBLLCommon.h"
#include <JKFramework/SmartPtr/JKReferenced.h>

/** 宏定义
*	类的属性
*/
#define PROPERTY_GETTER(valueType, name) \
	valueType get##name() {return ptrModel->##name;}

#define PROPERTY_SETTER(valueType, name) \
	void set##name(valueType value) { ptrModel->##name = value;}

#define PROPERTY(valueType, name) \
	PROPERTY_GETTER(valueType, name)\
	PROPERTY_SETTER(valueType, name)

class JKBaseObject : public JKReferenced
{
public: 
	JKBaseObject();
	virtual ~JKBaseObject();

	virtual const char* getClassName();

	JKRef_Ptr<JKBaseObject> toBaseObject() { return JKRef_Ptr<JKBaseObject>(this); };
	template<typename T>
	T toTypeObject() { return dynamic_cast<T>(this); };

};

template<typename T>
class JKBLLContainer;

template<typename T>
class JKBaseBLL : public JKBaseObject
{
	template<typename T>
	friend class JKBLLContainer;
public:
	JKBaseBLL(JKInt _parentID = -1) { parentID = _parentID; }
	virtual ~JKBaseBLL() {}

public:
	const JKString & getId() {return ptrModel->id; }
	inline JKInt getOriginID() { return ptrModel.get_id(); }

	JKInt getParentID() { return parentID; };
	void setParentID(JKInt pID) { parentID = pID; };

	//virtual const JKRef_Ptr<JKBaseObject> & getParent() { return nullptr; };
	//virtual BaseObjectConstRefPtr getParents() { return BaseObjectPtr(); };

	/** 升级数据 */
	virtual void upgradeDataVersion(int dataVersion) {}

	virtual void save();
	virtual bool destoried();

protected:
	bean_ptr<T> getModel() { return ptrModel; };
	void setModel(const bean_ptr<T> & model) { ptrModel = model; }

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