#pragma once

#include "JKBaseModel.h"


enum class CModelName { 
	C_JKProjectModel,
	C_JKStockCodeModel,
	C_JKStockCodeTrackModel,
	C_JKStockCodeSettingModel
};

#define CREATE_MODEL(T, TEnum) (T*)JKModelFactory::newObject(TEnum);


/** 
 * 枚举工厂：具体的实例化
 */
class JKModelFactory 
{

public:
	static JKBaseModel* newObject(CModelName eType);
};



