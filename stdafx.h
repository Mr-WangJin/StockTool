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