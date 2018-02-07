#include "stdafx.h"
#include "JKProjectSettingBLL.h"


JKProjectSettingBLL::JKProjectSettingBLL()
{
	refJKProjectSettingModel = SingleDB->createBean<JKProjectSettingModel>();
	refJKProjectSettingModel->id = JKGuid::GuidToString(JKGuid::CreateGuid());
}

bool JKProjectSettingBLL::getIsStartCrawl()
{
	return refJKProjectSettingModel->isStartCrawl;
}

void JKProjectSettingBLL::setIsStartCrawl(bool _isStartCrawl)
{
	refJKProjectSettingModel->isStartCrawl = _isStartCrawl;
}

bool JKProjectSettingBLL::getIsAlert()
{
	return refJKProjectSettingModel->isAlert;
}

void JKProjectSettingBLL::setIsAlert(bool _isAlert)
{
	refJKProjectSettingModel->isAlert = _isAlert;
}

double JKProjectSettingBLL::getUpAlertPercent()
{
	return refJKProjectSettingModel->alertPercent;
}

void JKProjectSettingBLL::setUpAlertPercent(double _alertPercent)
{
	refJKProjectSettingModel->alertPercent = _alertPercent;
}

double JKProjectSettingBLL::getDownAlertPercent()
{
	return refJKProjectSettingModel->downAlertPercent;
}
void JKProjectSettingBLL::setDownAlertPercent(double _downAlertPercent)
{
	refJKProjectSettingModel->downAlertPercent = _downAlertPercent;
}

int JKProjectSettingBLL::getTableShowType()
{
	return refJKProjectSettingModel->tableShowType;
}

void JKProjectSettingBLL::setTableShowType(int type)
{
	refJKProjectSettingModel->tableShowType = type;
}
