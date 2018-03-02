#pragma once

#include "JKBaseBLL.h"
#include "JKCommon/JKCommon.h"
#include <JKCommon\JKGuid.h>
#include <map>
#include <typeinfo>
#include <assert.h>

template<typename ModelType>
class JKBLLContainer
{
	friend class JKSingleton<JKBLLContainer>;
public:
	virtual ~JKBLLContainer()
	{
		/*std::map<JKInt, JKRef_Ptr<JKBaseBLL<ModelType>>>::iterator bb = mapBLLObject.begin();
		JKString ccc = typeid(ModelType).name();
		for (; bb != mapBLLObject.end(); ++bb)
		{
			bb->second.release();
		}*/
	}

	template<typename BLLType>
	inline JKRef_Ptr<BLLType> newBLL(JKInt _parentID = -1);

	template<typename BLLType>
	inline JKRef_Ptr<BLLType> load(JKInt id, JKInt _parentID = -1);

	template<typename BLLType>
	inline std::list<JKRef_Ptr<BLLType>> loadAll(JKInt _parentID = -1);

	template<typename BLLType>
	inline JKRef_Ptr<BLLType> find(JKInt id);

	template<typename BLLType>
	inline void add(JKRef_Ptr<BLLType> refBLLObject);

	template<typename BLLType>
	inline void save(JKRef_Ptr<BLLType> refBLLObject);

	template<typename BLLType>
	inline void save(bean_ptr<ModelType> ptrModel);

	template<typename BLLType>
	inline void destroy(JKRef_Ptr<BLLType> refBLLObject);

	template<typename BLLType>
	inline void destroy(bean_ptr<ModelType> ptrModel);

public:
	JKBLLContainer() {};

private:
	std::map<JKInt, JKRef_Ptr<JKBaseBLL<ModelType>>> mapBLLObject;
};


template<typename ModelType>
template<typename BLLType>
inline JKRef_Ptr<BLLType> JKBLLContainer<ModelType>::newBLL(JKInt _parentID = -1)
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
inline JKRef_Ptr<BLLType> JKBLLContainer<ModelType>::load(JKInt id, JKInt _parentID = -1)
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
inline std::list<JKRef_Ptr<BLLType>> JKBLLContainer<ModelType>::loadAll(JKInt _parentID = -1)
{
	std::vector<sqlid_t> vecIds = SingleDB->getBeanIds<ModelType>();
	std::list<JKRef_Ptr<BLLType>> listRefBll;
	for (auto id : vecIds)
	{
		listRefBll.emplace_back(this->load<BLLType>(id));
	}
	return listRefBll;
}

template<typename ModelType>
template<typename BLLType>
inline JKRef_Ptr<BLLType>  JKBLLContainer<ModelType>::find(JKInt id) 
{
	if (mapBLLObject.find(id) == mapBLLObject.end())
		return nullptr;
	
	return JKRef_Ptr<BLLType>(dynamic_cast<BLLType*>(mapBLLObject[id].get()));
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

template<typename ModelType>
template<typename BLLType>
inline void JKBLLContainer<ModelType>::save(JKRef_Ptr<BLLType> refBLLObject)
{
	assert(mapBLLObject.find(refBLLObject->getOriginID()) == mapBLLObject.end());
	if (mapBLLObject.find(refBLLObject->getOriginID()) == mapBLLObject.end());
	{
		refBLLObject->save();
	}
}

template<typename ModelType>
template<typename BLLType>
inline void JKBLLContainer<ModelType>::save(bean_ptr<ModelType> ptrModel)
{
	if (ptrModel.destroyed())
		return;
	ptrModel.save();

	JKRef_Ptr<BLLType> refBLL = FindBLL(BLLType, ModelType, ptrModel.get_id());
	if (refBLL)
	{
		refBLL->save();
	}
}

template<typename ModelType>
template<typename BLLType>
inline void JKBLLContainer<ModelType>::destroy(JKRef_Ptr<BLLType> refBLLObject)
{
	assert(mapBLLObject.find(refBLLObject->getOriginID()) == mapBLLObject.end());
	if (mapBLLObject.find(refBLLObject->getOriginID()) == mapBLLObject.end());
	{
		mapBLLObject.erase(refBLLObject->getOriginID());
		refBLLObject->destroy();
	}
}

template<typename ModelType>
template<typename BLLType>
inline void JKBLLContainer<ModelType>::destroy(bean_ptr<ModelType> ptrModel)
{
	if (ptrModel.destroyed())
		return;
	JKRef_Ptr<BLLType> refBLL = LoadBLL(BLLType, ModelType, ptrModel.get_id());
	DestroyBLL(BLLType, ModelType, refBLL);
}


