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

double JKProjectSettingBLL::getAlertPercent()
{
	return refJKProjectSettingModel->alertPercent;
}

void JKProjectSettingBLL::setAlertPercent(double _alertPercent)
{
	refJKProjectSettingModel->alertPercent = _alertPercent;
}

int JKProjectSettingBLL::getTableShowType()
{
	return refJKProjectSettingModel->tableShowType;
}

void JKProjectSettingBLL::setTableShowType(int type)
{
	refJKProjectSettingModel->tableShowType = type;
}
