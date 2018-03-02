#pragma once


#include <JKCommon/JKCommon.h>
#include <JKCommon/JKDef.h>
#include <JKFramework/SmartPtr/JKRef_Ptr.h>
USING_JK_NAMESPACE

#include "Model\JKDatabase.h"


#include <hiberlite.h>
#include <QMessageBox>


using namespace hiberlite;


#define SingleDB JKSingleton<JKDatabase>::GetInstance().db
#define BLLContainer(T) JKSingleton<JKBLLContainer<T>>::GetInstance()
#define BLLContext JKSingleton<JKBLLContext>::GetInstance()

#define NewBLL(BLLType, ModelType) BLLContainer(ModelType).newBLL<BLLType>()

#define LoadBLL(BLLType, ModelType, ID) BLLContainer(ModelType).load<BLLType>(ID)
#define LoadALLBLL(BLLType, ModelType) BLLContainer(ModelType).loadAll<BLLType>()

#define FindBLL(BLLType, ModelType, ID) BLLContainer(ModelType).find<BLLType>(ID)

#define SaveBLL(BLLType, ModelType, BLL) BLLContainer(ModelType).save<BLLType>(BLL)
#define SaveBean(BLLType, ModelType, Model) BLLContainer(ModelType).save<BLLType>(Model)

#define DestroyBLL(BLLType, ModelType, BLL) BLLContainer(ModelType).destroy<BLLType>(BLL)
#define DestroyBean(BLLType, ModelType, Model) BLLContainer(ModelType).destroy<BLLType>(Model)



enum class TradeType
{
	BUY = 1 << 0,
	SELL = 1 << 1,
	PART = 1 << 2,

};

enum class HoldStockType
{
	LONG_TIME,
	MID_TIME,
	SHORT_TIME
};