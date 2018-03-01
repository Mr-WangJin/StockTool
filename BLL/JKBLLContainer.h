#pragma once

#include "JKBaseBLL.h"
#include "JKCommon/JKCommon.h"
#include <JKCommon\JKGuid.h>
#include <map>

template<typename ModelType>
class JKBLLContainer
{
	friend class JKSingleton<JKBLLContainer>;
public:
	~JKBLLContainer() {};

	template<typename BLLType>
	inline JKRef_Ptr<BLLType> newBLL();

	template<typename BLLType>
	inline JKRef_Ptr<BLLType> load(JKInt id);

private:
	JKBLLContainer() {};

	template<typename BLLType>
	inline void addBLL(JKRef_Ptr<BLLType> refBLLObject);

private:
	std::map<JKInt, JKRef_Ptr<JKBaseBLL<ModelType>>> mapBLLObject;

	JKRef_Ptr<JKBLLContext> refContext;

};

template<typename ModelType>
template<typename BLLType>
inline JKRef_Ptr<BLLType> JKBLLContainer<ModelType>::newBLL()
{
	JKRef_Ptr<BLLType> refBLLObject = new BLLType(refContext);

	bean_ptr<ModelType> ptrModel = SingleDB->createBean<ModelType>();
	ptrModel->id = JKGuid::GuidToString(JKGuid::CreateGuid());
	refBLLObject->setModel(ptrModel);
	this->addBLL(refBLLObject);

	return refBLLObject;
}

template<typename ModelType>
template<typename BLLType>
inline JKRef_Ptr<BLLType> JKBLLContainer<ModelType>::load(JKInt id)
{
	if (mapBLLObject.find(id) == mapBLLObject.end());
	{
		ModelType ptrModel = SingleDB->loadBean<ModelType>(id);
		JKRef_Ptr<BLLType> refBLLObject = new BLLType(refContext);

	}
	else
	{
		return JKRef_Ptr<BLLType>(dynamic_cast<BLLType*>(mapBLLObject[id].get()));
	}
}

template<typename ModelType>
template<typename BLLType>
inline void JKBLLContainer<ModelType>::addBLL(JKRef_Ptr<BLLType> refBLLObject)
{
	if (mapBLLObject.find(refBLLObject->getOriginID()) == mapBLLObject.end());
	{
		mapBLLObject.insert(std::make_pair(refBLLObject->getOriginID(), refBLLObject));
	}
}