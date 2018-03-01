#include "stdafx.h"
#include "JKProjectSettingBLL.h"
#include "JKBLLContainer.h"


JKProjectSettingBLL::JKProjectSettingBLL()
{
	ptrModel = SingleDB->createBean<JKProjectSettingModel>();
	ptrModel->id = JKGuid::GuidToString(JKGuid::CreateGuid());
}

bool JKProjectSettingBLL::getIsStartCrawl()
{
	return ptrModel->isStartCrawl;
}

void JKProjectSettingBLL::setIsStartCrawl(bool _isStartCrawl)
{
	ptrModel->isStartCrawl = _isStartCrawl;
}

bool JKProjectSettingBLL::getIsAlert()
{
	return ptrModel->isAlert;
}

void JKProjectSettingBLL::setIsAlert(bool _isAlert)
{
	ptrModel->isAlert = _isAlert;
}

double JKProjectSettingBLL::getUpAlertPercent()
{
	return ptrModel->alertPercent;
}

void JKProjectSettingBLL::setUpAlertPercent(double _alertPercent)
{
	ptrModel->alertPercent = _alertPercent;
}

double JKProjectSettingBLL::getDownAlertPercent()
{
	return ptrModel->downAlertPercent;
}
void JKProjectSettingBLL::setDownAlertPercent(double _downAlertPercent)
{
	ptrModel->downAlertPercent = _downAlertPercent;
}

int JKProjectSettingBLL::getTableShowType()
{
	return ptrModel->tableShowType;
}

void JKProjectSettingBLL::setTableShowType(int type)
{
	ptrModel->tableShowType = type;
}
