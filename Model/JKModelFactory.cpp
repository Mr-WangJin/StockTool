#include "stdafx.h"
#include "JKModelFactory.h"
#include "JKProjectModel.h"
#include "JKStockCodeModel.h"
#include "JKStockCodeTradeModel.h"
#include "JKStockCodeSettingModel.h"


JKBaseModel* JKModelFactory::newObject(CModelName eType)
{
	switch (eType) 
	{
	//case CModelName::C_JKProjectModel:				return new JKProjectModel();
	//case CModelName::C_JKStockCodeModel:			return new JKStockCodeModel();
	//case CModelName::C_JKStockCodeTrackModel:		return new JKStockCodeTradeModel();
	//case CModelName::C_JKStockCodeSettingModel:     return new JKStockCodeSettingModel();
	default: 
		return nullptr;
	}
}
