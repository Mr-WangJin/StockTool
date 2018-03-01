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

	template<typename BLLType>
	inline std::list<JKRef_Ptr<BLLType>> loadAll();

	template<typename BLLType>
	inline void add(JKRef_Ptr<BLLType> refBLLObject);

private:
	JKBLLContainer() {};



private:
	std::map<JKInt, JKRef_Ptr<JKBaseBLL<ModelType>>> mapBLLObject;
};

template<typename ModelType>
template<typename BLLType>
inline JKRef_Ptr<BLLType> JKBLLContainer<ModelType>::newBLL()
{
	JKRef_Ptr<BLLType> refBLLObject = new BLLType();

	bean_ptr<ModelType> ptrModel = SingleDB->createBean<ModelType>();
	ptrModel->id = JKGuid::GuidToString(JKGuid::CreateGuid());
	refBLLObject->setModel(ptrModel);
	this->add(refBLLObject);

	return refBLLObject;
}

template<typename ModelType>
template<typename BLLType>
inline JKRef_Ptr<BLLType> JKBLLContainer<ModelType>::load(JKInt id)
{
	if (mapBLLObject.find(id) == mapBLLObject.end())
	{
		bean_ptr<ModelType> ptrModel = SingleDB->loadBean<ModelType>(id);
		JKRef_Ptr<BLLType> refBLLObject = new BLLType();
		refBLLObject->setModel(ptrModel);
		this->add(refBLLObject);
		return refBLLObject;
	}
	else
	{
		return JKRef_Ptr<BLLType>(dynamic_cast<BLLType*>(mapBLLObject[id].get()));
	}
}

template<typename ModelType>
template<typename BLLType>
inline std::list<JKRef_Ptr<BLLType>> JKBLLContainer<ModelType>::loadAll()
{
	std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<JKProjectModel>();
	std::list<JKRef_Ptr<BLLType>> listRefBll;
	for (auto id : vecIds)
	{
		listRefBll.emplace_back(this->load<BLLType>(id));
	}
	return listRefBll;
}

template<typename ModelType>
template<typename BLLType>
inline void JKBLLContainer<ModelType>::add(JKRef_Ptr<BLLType> refBLLObject)
{
	if (mapBLLObject.find(refBLLObject->getOriginID()) == mapBLLObject.end());
	{
		mapBLLObject.insert(std::make_pair(refBLLObject->getOriginID(), refBLLObject));
	}
}