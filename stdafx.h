#pragma once


#include <JKCommon/JKCommon.h>
#include <JKCommon/JKDef.h>
#include <JKFramework/SmartPtr/JKRef_Ptr.h>
USING_JK_NAMESPACE

#include "Model\JKDatabase.h"
#include <hiberlite.h>
using namespace hiberlite;


#include "BLL\JKBaseBLL.h"
#include <QMessageBox>

/** 宏定义
 *	类变量
 */
#define DefTypePtr(obj) \
class JK##obj;\
using obj##Ptr = JKRef_Ptr<JK##obj>;\
using obj##ConstRefPtr = const JKRef_Ptr<JK##obj> &;
//using obj##ConstRefPtr = const JKRef_Ptr<JK##obj> &;
//using obj##RefPtr = JKRef_Ptr<JK##obj> &; \

DefTypePtr(BaseObject)
//using BaseObjectPtr = JKRef_Ptr<JKBaseObject>;
//using BaseObjectRefPtr = JKRef_Ptr<JKBaseObject> &;
//using BaseObjectConstRefPtr = const JKRef_Ptr<JKBaseObject> &;

DefTypePtr(ProjectBLL)
DefTypePtr(ProjectSettingBLL)
DefTypePtr(StockCodeBLL)
DefTypePtr(StockCodeTradeBLL)
DefTypePtr(StockCodeTradeItemBLL)


/** 宏定义
 *	新建 加载 保存 销毁 类 
 */
#define SingleDB JKSingleton<JKDatabase>::GetInstance().db
#define BLLContainer(T) JKSingleton<JKBLLContainer<T>>::GetInstance()
#define BLLContext JKSingleton<JKBLLContext>::GetInstance()

#define NewBLL(BLLType, ModelType, pID) BLLContainer(ModelType).newBLL<BLLType>(pID)

#define LoadBLL(BLLType, ModelType, ID, pID) BLLContainer(ModelType).load<BLLType>(ID, pID)
#define LoadALLBLL(BLLType, ModelType, pID) BLLContainer(ModelType).loadAll<BLLType>(pID)

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