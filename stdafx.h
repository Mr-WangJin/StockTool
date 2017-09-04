#pragma once


#include <JKCommon/JKCommon.h>
#include <JKFramework/SmartPtr/JKRef_Ptr.h>
USING_JK_NAMESPACE

#include "bll\JKDatabase.h"


#include <hiberlite.h>

using namespace hiberlite;


#define SingleDB JKSingleton<JKDatabase>::GetInstance().db


enum class TradeType
{
	BUY = 1,
	SELL
};